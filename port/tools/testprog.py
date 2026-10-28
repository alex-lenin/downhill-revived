"""Compile and run the port's test programs on the paths players run (finding 104).

    guest(name, sources)  C that runs inside the game core: compiled for wasm32 with the core's
                          flags (build_wasm.py's BASE), linked with wasi-libc, translated by wasm2c
                          and built for the 64-bit host with port/host64/test_guest.c. Its C library
                          reaches the host as the core's does (runtime/wasi_adapter.c and the
                          host_api.h services, which test_guest.c gives). A trap exits with 134.
    game_unit(rel, out)   one src/ unit as the core compiles it (patches, transform.py, the EE
                          float IR rewrites); returns the wasm32 object for guest().
    host(name, sources)   C that runs in the native host (port/host, port/gs, the host side of
                          port/hle): the host's clang for x86_64 Windows (or the machine's own
                          target elsewhere), the 64-bit build's float and aliasing flags.
    host_object(src, out) one host source compiled on its own (other defines, or C++20): an object
                          for host()'s sources.
    run(exe, *args)       runs a test program; returns its CompletedProcess, raises unless the exit
                          code is `expect` (default 0).

Products go to build/tests/<name>/ (or out=). Needs LLVM, wasi-sdk and WABT (build_host64.py's
paths); SDL3 (sdl3.py) only for host(..., sdl=True)."""
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
OUT = ROOT / 'build' / 'tests'
WINDOWS = sys.platform == 'win32'
EXE = '.exe' if WINDOWS else ''

from build_host64 import LLVM, WABT, WASI  # noqa: E402  (the toolchain the shipped build uses)
if WINDOWS and not shutil.which('clang') and LLVM.exists():
    os.environ['PATH'] = str(LLVM) + os.pathsep + os.environ['PATH']  # build_wasm.py's clang
CLANG = shutil.which('clang') or 'clang'
WASI_CLANG = WASI / 'bin' / ('clang' + EXE)
WASM2C = WABT / 'bin' / ('wasm2c' + EXE)
WASM2C_RT = WABT / 'share' / 'wabt' / 'wasm2c'

# The host build's code generation (build_host64.py): strict IEEE float, no contraction, zeroed
# locals, no strict aliasing, wrapping signed overflow.
HOST_FLAGS = ['-O2', '-ffp-model=strict', '-ffp-contract=off', '-ftrivial-auto-var-init=zero',
              '-fno-strict-aliasing', '-fwrapv', '-w']
HOST_TARGET = ['--target=x86_64-pc-windows-msvc', '-fuse-ld=lld', '-D_CRT_SECURE_NO_WARNINGS'] if WINDOWS else []
# host_fp_csr, host_fp_set_csr and host_cycles for host test programs that run VU or EE float code
HOST_FP = ROOT / 'port/host64/test_fp.c'
# The guest's stack, as the core's (build_wasm.STACK_SIZE).
GUEST_STACK = 1 << 20
# The host_api.h functions a guest test may import (port/host64/test_guest.c gives them), as the
# core's module imports them; its C library reaches them through runtime/wasi_adapter.c.
HOST_IMPORTS = ('host_console_write', 'host_exit', 'host_wall_seconds', 'host_putenv', 'host_putenv_s',
                'host_fp_csr', 'host_fp_set_csr', 'host_cycles', 'host_vu1_file', 'host_file_open',
                'host_file_close', 'host_file_read', 'host_file_write', 'host_file_flush', 'host_file_seek',
                'host_file_size', 'test_host_getenv')
# host_getenv, whose value the guest must hold in its own memory: the host copies it into one of
# a few buffers, used in turn (weak: a test may define its own).
GUEST_API = '''unsigned test_host_getenv(const char *name, char *buffer, unsigned size);
__attribute__((weak)) const char *host_getenv(const char *name)
{
    static char values[8][4096];
    static unsigned next;
    char *value = values[next++ % 8];
    return test_host_getenv(name, value, sizeof values[0]) ? value : 0;
}
'''


def _wasm():
    import build_wasm  # imported on first use: it reads the data image's names lazily
    return build_wasm


def out_dir(name, out=None):
    d = Path(out) if out else OUT / name
    d.mkdir(parents=True, exist_ok=True)
    return d


def _run(cmd, cwd=None, **kw):
    cmd = [str(c) for c in cmd]
    p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, encoding='utf-8', errors='replace', **kw)
    if p.returncode:
        raise RuntimeError('%s\n%s%s' % (' '.join(cmd), p.stdout, p.stderr))
    return p


def guest_flags(cpp=False):
    """The core's compile flags for port files (build_wasm.compile_c), without -c; x86 intrinsic
    headers resolve to SIMDe as in the core (build_wasm.shim_headers)."""
    w = _wasm()
    shim = OUT / 'shim'
    if not (shim / 'xmmintrin.h').exists():
        w.SHIM = str(shim)
        w.shim_headers()
    lang = ['-x', 'c++', '-std=gnu++98', '-fno-exceptions', '-fno-rtti'] if cpp else []
    return [f for f in w.BASE if f != '-c'] + ['-I', shim, '-I', w.SIMDE] + lang


def port_headers(out):
    """The game headers as the core sees them (transform.port_headers), under out/include."""
    import transform
    include = Path(out) / 'include'
    transform.port_headers(ROOT / 'include', include)
    return include


def game_unit(rel, out, text=None, includes=()):
    """src/<rel> (or `text`, its already patched source) compiled as build_wasm.py compiles game
    units: the ABI call fixes and port patches, transform.py with no label prefix and the data
    image's names extern, then the front end, the EE float IR rewrites and the back end. The data
    stays named (the test defines what it uses), not the core's numbered PS2 addresses. Returns
    the wasm32 object."""
    w = _wasm()
    import eefloat
    import transform
    from port_source import align_aggregate_allocas, apply_patches
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    guest_flags()  # the SIMDe headers
    shim = OUT / 'shim'
    if not (shim / 'ee_native_float_shim.h').exists():
        w.SHIM = str(shim)
        w.shim_ee_float()
    include = OUT / 'include'
    if not include.exists():
        transform.port_headers(ROOT / 'include', include)
    cpp = not rel.endswith('.c')
    if text is None:
        text = apply_patches(rel, transform.fix_abi_calls((ROOT / 'src' / rel).read_text(
            encoding='utf-8', errors='replace'), rel, None))
    if rel in w.WASM_VA_LIST:
        text = text.replace('typedef char *va_list;', 'typedef __builtin_va_list va_list;')
    text = transform.transform(w.cpp_linkage(text) if cpp else text, cpp, '', w.IMAGE)
    src = out / Path(rel).name
    src.write_text('#include "port.h"\n#line 1 "src/%s"\n%s' % (rel, w.fix_function_arrays(text)),
                   encoding='utf-8')
    # inline asm calls ps2_asm() (the test gives it, or links port/runtime/ps2asm.c), not the core's
    # generated functions (PS2_ASM_DIRECT, ps2asm_aot.py), which only a whole module has
    inc = ['-I', ROOT / 'port/include', '-I', include, '-I', ROOT / 'include', '-I', shim, '-I', w.SIMDE]
    inc += [x for i in includes for x in ('-I', i)]
    front = [CLANG] + [f for f in w.BASE if f != '-c'] + (w.OPT_LANG_CPP if cpp else w.OPT_LANG_C) + inc + [
        '-S', '-emit-llvm', '-Xclang', '-disable-llvm-passes', '-include', shim / 'ee_native_float_shim.h']
    ir = _run(front + [src, '-o', '-']).stdout
    ir = w.fmul_rewrite(eefloat.rewrite(align_aggregate_allocas(ir))[0])
    obj = src.with_suffix(src.suffix + '.o')
    _run([CLANG] + w.BASE + ['-x', 'ir', '-', '-o', obj], input=ir)
    if cpp:
        w.wasm_retail_names.rename(str(obj), thunks=w.INTRA_THUNKS)  # the C++ symbol policy
    return obj


def guest(name, sources, out=None, includes=(), defines=(), flags=(), cpp=False, sjlj=False, memory=None):
    """Build a guest test program from C sources (and ready objects): wasm32 -> wasm2c -> x64.
    The program's main() runs as in the core (wasm32: 32-bit pointers and long, the core's
    struct layout and float code). sjlj: setjmp/longjmp, as wasm exceptions (wasi-sdk's
    libsetjmp, wasm2c's exception runtime). memory: the guest's size in bytes, for tests that use
    fixed guest addresses (PS2 RAM at 0x08000000, the registers at 0x10000000) as the core does;
    the program's own data, stack and heap stay below 0x08000000. Returns the executable."""
    d = out_dir(name, out)
    eh = ['-mllvm', '-wasm-enable-sjlj', '-mllvm', '-wasm-use-legacy-eh=true'] if sjlj else []
    objects = []
    for s in map(Path, sources):
        if s.suffix in ('.o', '.obj'):
            objects.append(s)
            continue
        obj = d / (s.stem + '.wasm.o')
        _run([CLANG] + guest_flags(cpp or s.suffix == '.cpp') + ['-c', '-I', ROOT / 'port/runtime',
             '-I', ROOT / 'port/include'] + [x for i in includes for x in ('-I', i)] +
             ['-D' + x for x in defines] + list(flags) + eh + [s, '-o', obj], cwd=d)
        objects.append(obj)
    api = d / 'guest_api.c'
    api.write_text(GUEST_API)
    _run([CLANG] + guest_flags() + ['-c', api, '-o', d / 'guest_api.wasm.o'], cwd=d)
    _run([CLANG] + guest_flags() + ['-c', ROOT / 'port/runtime/wasi_adapter.c', '-o', d / 'wasi_adapter.wasm.o'], cwd=d)
    objects += [d / 'guest_api.wasm.o', d / 'wasi_adapter.wasm.o']
    wasm = d / (name + '.wasm')
    (d / 'host_api.syms').write_text('\n'.join(HOST_IMPORTS) + '\n')
    _run([WASI_CLANG, '--target=wasm32-wasip1', '--sysroot=' + str(WASI / 'share/wasi-sysroot'),
          '-Wl,-z,stack-size=%d' % GUEST_STACK, '-Wl,--stack-first', '-Wl,--allow-undefined-file=host_api.syms',
          *(['-Wl,--initial-memory=%d' % (-(-memory // 65536) * 65536)] if memory else []),
          *eh, *objects, '-o', wasm, '-lwasi-emulated-process-clocks'] + (['-lsetjmp'] if sjlj else []), cwd=d)
    _run([WASM2C, wasm, '-n', 'guest', '-o', d / 'guest.c'], cwd=d)
    # the import instances, in the order wasm2c's instantiate takes them
    proto = re.search(r'void wasm2c_guest_instantiate\(w2c_guest\*((?:, struct w2c_\w+\*)*)\);',
                      (d / 'guest.h').read_text())
    imports = ','.join('&' + {'wasi__snapshot__preview1': 'wasi', 'env': 'env'}[m]
                       for m in re.findall(r'struct w2c_(\w+)\*', proto.group(1)))
    return _host_link(name, d, [d / 'guest.c', ROOT / 'port/host64/test_guest.c',
                                WASM2C_RT / 'wasm-rt-impl.c', WASM2C_RT / 'wasm-rt-mem-impl.c',
                                WASM2C_RT / 'wasm-rt-exceptions-impl.c'],
                      ['-I', d, '-I', WABT / 'include', '-I', WASM2C_RT, '-I', _wasm().SIMDE,  # wasm SIMD
                       '-DGUEST_IMPORTS=' + imports, '-DWASM_RT_MEMCHECK_BOUNDS_CHECK=1',
                       '-DWASM_RT_STACK_DEPTH_COUNT=1', '-DWASM_RT_MAX_CALL_STACK_DEPTH=4096'])


def host(name, sources, out=None, includes=(), defines=(), flags=(), libs=(), sdl=False, low=False):
    """Build a host test program for the native 64-bit target. Returns the executable.

    low: the program mixes guest units (graphics.c, ...) with host ones and passes host pointers
    as guest addresses, so it runs below 2 GiB with guest_base 0 (port/host64/test_low.c): a
    pointer is its own guest address. Windows only (/LARGEADDRESSAWARE:NO)."""
    d = out_dir(name, out)
    extra = _host_extra(includes, defines, flags)
    link = list(libs)
    sources = [host_object(s, d, includes=includes, defines=defines, flags=flags) if str(s).endswith('.cpp') else s
               for s in sources]
    if low:
        if not WINDOWS:
            raise NotImplementedError('low host test programs need Windows')
        sources = list(sources) + [ROOT / 'port/host64/test_low.c']
        link += ['-Wl,/largeaddressaware:no']
    if sdl:
        import sdl3
        lib, include = sdl3.build()
        extra += ['-I', include]
        link += [lib] + (['-l' + x[:-4] for x in sdl3.SYSTEM_LIBS] if WINDOWS else list(sdl3.SYSTEM_LIBS))
    return _host_link(name, d, list(map(Path, sources)), extra, link)


def _host_extra(includes=(), defines=(), flags=()):
    extra = ['-I', ROOT / 'port/runtime', '-I', ROOT / 'port/include', '-I', ROOT / 'port/host']
    return extra + [x for i in includes for x in ('-I', i)] + ['-D' + x for x in defines] + list(flags)


def host_object(source, out, name=None, includes=(), defines=(), flags=()):
    """One source compiled for the host on its own (C++ as C++20); returns the object."""
    source = Path(source)
    obj = Path(out) / ((name or source.stem) + ('.obj' if WINDOWS else '.o'))
    lang = ['-std=c++20'] if source.suffix == '.cpp' else []
    _run([CLANG] + HOST_TARGET + HOST_FLAGS + lang + _host_extra(includes, defines, flags) +
         ['-c', source, '-o', obj], cwd=out)
    return obj


def _host_link(name, d, sources, extra, link=()):
    exe = d / (name + EXE)
    libs = [('-l' + l[:-4] if WINDOWS and str(l).endswith('.lib') and not os.path.isabs(str(l)) else l) for l in link]
    if WINDOWS:
        libs += ['-Wl,/NODEFAULTLIB:msvcrt'] if any(str(l).endswith('SDL3-static.lib') for l in link) else []
    else:
        libs += ['-lm', '-lpthread']
    _run([CLANG] + HOST_TARGET + HOST_FLAGS + extra + sources + ['-o', exe] + libs, cwd=d,
         env=dict(os.environ, TMP=str(d), TEMP=str(d)))
    return exe


def run(exe, *args, env=None, expect=0, timeout=300, quiet=False):
    """Run a test program; its output is printed unless quiet. Raises unless it exits with
    `expect` (an int, or None for any)."""
    p = subprocess.run([str(exe), *map(str, args)], capture_output=True, text=True, encoding='utf-8',
                       errors='replace', timeout=timeout, env=dict(os.environ, **(env or {})),
                       cwd=Path(exe).parent)
    if not quiet:
        sys.stdout.write(p.stdout)
        sys.stderr.write(p.stderr)
    if expect is not None and p.returncode != expect:
        raise RuntimeError('%s %s: exit %d, expected %d\n%s%s' % (Path(exe).name, ' '.join(map(str, args)),
                                                                 p.returncode, expect, p.stdout, p.stderr))
    return p
