"""Adapt WABT 1.0.42 host addresses for guest RAM mirrors.

Run on every generated C translation unit, before compiling at any optimization
level. Refuse unknown templates rather than silently lose mirror correctness.
"""
from pathlib import Path
import argparse
import re
import shutil
import struct

# An f32 constant that WABT writes as a decimal double literal (no f suffix): `var_f2 = 6.10351562e-05;`.
F32_DOUBLE_LITERAL = re.compile(r'(\bvar_f\d+ = )(-?)(\d+\.\d*(?:e[-+]?\d+)?|\d+e[-+]?\d+)(?=;)')


def exact_f32_literals(text):
    """Write f32 constants as exact hex float literals.

    WABT 1.0.42 prints some f32 constants as 9-digit decimal literals without an f
    suffix: double constants whose conversion to float happens at run time under
    strict FP (the host build's -ffp-model=strict honours the dynamic rounding mode).
    In the EE's round-toward-zero mode, 6.10351562e-05 (meant: 2^-14) then becomes the
    float below 2^-14 (W7i race field 1360: func_00222B68). The 9 digits round-trip
    to the intended f32 in round-to-nearest, so that value is written exactly."""
    def exact(m):
        value = struct.unpack('<f', struct.pack('<f', float(m[3])))[0]
        return '%s%s%sf' % (m[1], m[2], value.hex())
    return F32_DOUBLE_LITERAL.sub(exact, text)


def specialize_dispatch(text, hot_functions=()):
    """Guarded direct calls for small signature groups; keep checked fallback.

    Native entry equality is tested AFTER WABT's bounds/type check. A changed
    table entry or a cross-instance funcref still receives its actual context.
    Large groups stay indirect to bound code growth; this is no closed-table
    assumption and does not remove a wasm trap.
    """
    groups = {}
    for ft, fn in re.findall(r'\{RefFunc, (\w+), \(wasm_rt_function_ptr_t\)(\w+),', text):
        groups.setdefault(ft, set()).add(fn)

    requested = set(hot_functions)
    known = set().union(*groups.values()) if groups else set()
    if requested - known:
        raise ValueError(f'unknown wasm2c dispatch targets: {sorted(requested - known)}')

    def rewrite(match):
        # wasm2c emits one call expression per line. Split only top-level commas
        # (the second field is a function-pointer type containing more commas).
        args, depth, start = [], 0, 0
        expr = match[1]
        for i, char in enumerate(expr):
            if char == '(':
                depth += 1
            elif char == ')':
                depth -= 1
            elif char == ',' and depth == 0:
                args.append(expr[start:i].strip()); start = i + 1
        args.append(expr[start:].strip())
        if len(args) < 5:
            raise ValueError('unsupported wasm2c indirect call')
        table, cast, ft, index, *actual = args
        candidates = sorted(groups.get(ft, ()))
        if requested:
            candidates = [fn for fn in candidates if fn in requested]
        if not 1 <= len(candidates) <= 4:
            return match[0]
        # Generated operands must be side-effect-free locals/field accesses.
        if not all(re.fullmatch(r'[\w.>\[\]-]+', x) for x in [table, ft, index, *actual]):
            raise ValueError('unsupported wasm2c indirect-call operand')
        call = 'DO_CALL_INDIRECT(' + ', '.join([table, cast, index, *actual]) + ')'
        for fn in reversed(candidates):
            call = (f'({table}.data[{index}].func == (wasm_rt_function_ptr_t){fn} ? '
                    f'{fn}({", ".join(actual)}) : {call})')
        return f'(CHECK_CALL_INDIRECT({table}, {ft}, {index}), {call})'

    return re.sub(r'(?<![\w])CALL_INDIRECT\(([^\n]+)\)(?=;)', rewrite, text)


def prepare(path, *, minimum_size=True, dispatch=True, hot_functions=()):
    text = path.read_text()
    if dispatch:
        text = specialize_dispatch(text, hot_functions)
    if minimum_size:
        text = re.sub(r'(\bwasm_rt_local_memory_size = )(?!host64_memory_size\()([^;\n]+);',
                      r'\1host64_memory_size(\2);', text)
    marker = '#include "memory_access.h"'
    copy_old = 'memmove(MEM_ADDR(dest, dest_addr, n), MEM_ADDR(src, src_addr, n), n);'
    copy_new = 'host64_memory_copy(dest->data, dest_addr, src->data, src_addr, n);'
    if copy_old in text:  # every memory.copy site in the unit uses this template
        text = text.replace(copy_old, copy_new)
    elif copy_new not in text and re.search(r'\bmemmove\(', text):
        # a memmove call that is not the memory.copy template: refuse rather than guess
        raise ValueError(f'{path}: unsupported wasm2c memory.copy template')
    # no memory.copy in this unit (clang lowers memmove to a module function, as the wasm32 build does)
    elif copy_new not in text:
        pass
    if marker in text:
        # Upgrade the previous barrier adapter instead of silently retaining it.
        legacy = {
            'host64_memory_address(&((mem)->data[addr]))':
            'host64_memory_address((mem)->data, (addr), (n))',
            'host64_memory_address(&wasm_rt_local_memory_base[(addr)])':
            'host64_memory_address(wasm_rt_local_memory_base, (addr), (n))',
        }
        for old, new in legacy.items():
            text = text.replace(old, new)
        if ('#define MEM_ADDR(mem, addr, n) host64_memory_address((mem)->data, (addr), (n))' not in text or
            '#define MEM_ADDR_MEMOP(mem, addr, n) host64_memory_address(wasm_rt_local_memory_base, (addr), (n))' not in text):
            raise ValueError(f'{path}: unsupported prepared wasm2c memory template')
        path.write_text(text)
        return
    replacements = {
        '#define MEM_ADDR(mem, addr, n) &((mem)->data[addr])':
        '#define MEM_ADDR(mem, addr, n) host64_memory_address((mem)->data, (addr), (n))',
        '#define MEM_ADDR_MEMOP(mem, addr, n) (&wasm_rt_local_memory_base[(addr)])':
        '#define MEM_ADDR_MEMOP(mem, addr, n) host64_memory_address(wasm_rt_local_memory_base, (addr), (n))',
    }
    for old, new in replacements.items():
        if text.count(old) == 1:
            text = text.replace(old, new)
        elif 'MEM_ADDR' not in text:
            pass  # a split part: its MEM_ADDR defines come from the prepared impl header
        else:
            raise ValueError(f'{path}: unsupported wasm2c memory template: {old}')
    text = marker + '\n' + text
    path.write_text(text)



# ---- the module's C for the native hosts (build_host64.py, build_host64_linux.py) ----

def module_units(out, run, wasm2c, wasm=None, sources=(), num_outputs=1):
    """The module's C units in out: wasm2c's output for a .wasm (module name core), else copies
    of WABT .c units and the headers beside them. run(cmd, timeout=...) runs a command."""
    if wasm:
        if num_outputs < 1:
            raise ValueError('--num-outputs must be positive')
        run([wasm2c, wasm, '-n', 'core', '--num-outputs=' + str(num_outputs), '-o', out / 'core.c'],
            timeout=900)
        return [out / 'core.c'] if num_outputs == 1 else [out / f'core_{i}.c' for i in range(num_outputs)]
    units = []
    for source in sources:
        source = source.resolve()
        if source.suffix != '.c':
            raise ValueError('module units must be .c files')
        for header in source.parent.glob('*.h'):
            if header.resolve() != (out / header.name).resolve():
                shutil.copyfile(header, out / header.name)
        target = out / source.name
        if source != target:
            shutil.copyfile(source, target)
        units.append(target)
    return units


def check_launcher_exports(exports):
    """The exports launcher.c needs (exports: gen_host64_imports.generate's)."""
    required = {'memory', '__stack_pointer'}
    if not required <= exports.keys():
        raise ValueError(f'missing launcher exports: {required - exports.keys()}')
    if not {'main', '__main_argc_argv'} & exports.keys():
        raise ValueError('missing launcher entry export: main or __main_argc_argv')


def prepare_units(units, out, exact_f32=False):
    """Every unit prepared before compilation. WABT's split helper templates live in the shared
    implementation header: prepare that once, and specialize the individual units only where
    complete templates/metadata are present. exact_f32: f32 constants exact, independent of the
    EE rounding mode at run time (exact_f32_literals)."""
    for source in units:
        if exact_f32:
            source.write_text(exact_f32_literals(source.read_text()))
        if '#define MEM_ADDR(mem' in source.read_text():
            prepare(source)
        else:
            source.write_text(specialize_dispatch(source.read_text()))
    for header in out.glob('*-impl.h'):
        if '#define MEM_ADDR(mem' in header.read_text():
            prepare(header)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('files', type=Path, nargs='+')
    parser.add_argument('--no-dispatch', action='store_true', help='disable guarded direct-call promotion for A/B measurement')
    parser.add_argument('--dispatch-target', action='append', default=[],
                        help='profile-selected wrapper symbol; at most four per signature promoted')
    args = parser.parse_args()
    for path in args.files:
        prepare(path, dispatch=not args.no_dispatch, hot_functions=args.dispatch_target)
