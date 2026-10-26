"""Generated code for the game's inline asm templates (port/runtime/ps2asm.c, PS2_ASM_AOT).

    python port/tools/ps2asm_aot.py OUT.inc DIR...

ps2asm.c parses each inline asm template (ps2_asm) and each recompiled COP2 instruction (ps2_exec)
once into an Ins sequence and then interprets it. In the wasm module that interpreter was over
half of the race's game thread on x64: every instruction re-decodes its operands through linear
memory. This reads every template literal in the transformed sources (DIR...: the game units, the
game headers, the recompiled functions), parses it the way ps2asm.c's compile() does, and writes
one function per distinct instruction sequence that calls ps2asm.c's own run_ins() on each
constant Ins in turn, so the compiler folds the decoding away and nothing else changes.

The parser here is only a generator: ps2asm.c still parses every template at run time and uses
a generated function only when the parsed sequence is byte-for-byte the one it was generated
from (select_aot_program). A template this parser reads differently is not generated, or not
matched, and stays on the interpreter. The opcode, kind and mnemonic tables are read from
ps2asm.c itself.
"""
import os
import re
import sys
from pathlib import Path

RUNTIME = Path(__file__).resolve().parents[1] / 'runtime' / 'ps2asm.c'
NO_COMP = 0xFF
OPK_GPR, OPK_FPR, OPK_MEM, OPK_LO, OPK_HI = 1, 2, 3, 4, 5
STRING = r'"(?:\\.|[^"\\\n])*"'
ASM_CALL = re.compile(r'\bps2_asm\(\s*((?:%s\s*)+),\s*((?:%s\s*)+),\s*(\d+)\s*,' % (STRING, STRING))
# ps2_exec_kept: a recompiled COP2 move done in C (recomp.h). Its program is still generated, so
# the function table and the templates' order stay as they were (finding 090 SP11).
EXEC_CALL = re.compile(r'\bps2_exec(?:_kept)?\(\s*((?:%s\s*)+)\)' % STRING)
DIRECT_CALL = re.compile(r'\bPS2X\(\s*(ps2x_\w+)\s*,\s*((?:%s\s*)+)\)' % STRING)  # recomp.py, wasm build
ASM_DIRECT = re.compile(r'\bPS2A\(\s*(ps2a_\w+)\s*,\s*((?:%s\s*)+),\s*((?:%s\s*)+),\s*(\d+)\s*,' % (STRING, STRING))  # transform.py


def tables(source):
    text = source.read_text(encoding='utf-8')
    kinds = re.search(r'enum \{ (K_NONE[^}]*)\}', text).group(1)
    k = {name.strip(): i for i, name in enumerate(kinds.split(',')) if name.strip()}

    def names(array):
        body = re.search(r'static const Name %s\[\] = \{(.*?)\n\};' % array, text, re.S).group(1)
        return re.findall(r'\{"(\w+)", (\w+)\}', body)
    gprs = re.search(r'GPR_NAMES\[32\] = \{(.*?)\};', text, re.S).group(1)
    return k, names('SIMPLE'), names('VFAMILY'), re.findall(r'"(\w+)"', gprs)


K, SIMPLE, VFAMILY, GPR_NAMES = tables(RUNTIME)
SIMPLE_OPS = dict(SIMPLE)


def c_literals(text):
    """The bytes of adjacent C string literals."""
    out = []
    for lit in re.findall(STRING, text):
        s, i = lit[1:-1], 0
        while i < len(s):
            c = s[i]
            if c != '\\':
                out.append(c); i += 1; continue
            e = s[i + 1]
            if e in '01234567':
                m = re.match(r'[0-7]{1,3}', s[i + 1:]).group(0)
                out.append(chr(int(m, 8))); i += 1 + len(m)
            elif e == 'x':
                m = re.match(r'[0-9A-Fa-f]+', s[i + 2:]).group(0)
                out.append(chr(int(m, 16) & 0xFF)); i += 2 + len(m)
            else:
                out.append({'n': '\n', 't': '\t', 'r': '\r', 'a': '\a', 'b': '\b', 'f': '\f',
                            'v': '\v'}.get(e, e)); i += 2
    return ''.join(out)


def atoi(s):
    m = re.match(r'[ \t\n\v\f\r]*([+-]?\d+)', s)
    return int(m.group(1)) if m else 0


def strtol(s, base=0):
    """C strtol with a 32-bit long (wasm32, i686): the value and the rest of the string."""
    m = re.match(r'[ \t\n\v\f\r]*([+-]?)', s)
    sign, i = m.group(1), m.end()
    if base == 0:
        if re.match(r'0[xX][0-9A-Fa-f]', s[i:]):
            base, i = 16, i + 2
        elif s[i:i + 1] == '0':
            base = 8
        else:
            base = 10
    digits = {8: '[0-7]+', 10: '[0-9]+', 16: '[0-9A-Fa-f]+'}[base]
    d = re.match(digits, s[i:])
    if not d:
        return 0, s
    v = int(d.group(0), base) * (-1 if sign == '-' else 1)
    v = max(-2 ** 31, min(2 ** 31 - 1, v))
    return v, s[i + d.end():]


def s32(v):
    v &= 0xFFFFFFFF
    return v - (1 << 32) if v >= 1 << 31 else v


def gpr_number(s):
    if s[:1].isdigit():
        return atoi(s) & 31
    if s == 's8':
        return 30
    return GPR_NAMES.index(s) if s in GPR_NAMES else -1


def comp_of(c):
    return 'xyzw'.index(c) if c and c in 'xyzw' else -1


class Prog:
    def __init__(self, cons, n):
        self.n_ops = n
        self.kind, self.alias = [0] * 32, [-1] * 32
        self.inp, self.out = [0] * 32, [0] * 32
        c = 0
        for k in range(min(n, 32)):
            self.alias[k] = -1
            out = inout = False
            while c < len(cons) and cons[c] != '|':
                ch = cons[c]
                if ch == '=': out = True
                elif ch == '+': inout = True
                elif ch in 'rd': self.kind[k] = OPK_GPR
                elif ch == 'f': self.kind[k] = OPK_FPR
                elif ch == 'm': self.kind[k] = OPK_MEM
                elif ch == 'l': self.kind[k] = OPK_LO
                elif ch == 'h': self.kind[k] = OPK_HI
                elif ch.isdigit() and ch in '0123456789':
                    self.alias[k] = int(ch)
                    self.kind[k] = self.kind[int(ch)]
                c += 1
            if c < len(cons) and cons[c] == '|':
                c += 1
            self.out[k] = int(out or inout)
            self.inp[k] = int(not out or inout)


def operand(p, t):
    """(kind, reg, comp, step, imm), or None (ps2asm.c: parse_operand)."""
    t = t.lstrip(' \t').rstrip(' \t')
    if t in ('ACC', '$ACC'): return (K['K_ACC'], 0, NO_COMP, 0, 0)
    if t == 'Q': return (K['K_Q'], 0, NO_COMP, 0, 0)
    if t == 'R': return (K['K_R'], 0, NO_COMP, 0, 0)
    if t == 'I': return (K['K_I'], 0, NO_COMP, 0, 0)
    paren = t.find('(')
    if paren >= 0:
        base = t[paren + 1:]
        end = base.find(')')
        if end < 0:
            return None
        base = base[:end]
        if '$vi' in base:
            step = 1 if '++' in base else -1 if '--' in base else 0
            return (K['K_VIMEM'], atoi(base[base.find('$vi') + 3:]) & 15, NO_COMP, step, 0)
        head = t[:paren]
        imm = s32(strtol(head)[0]) if head else 0
        if base[:1] == '%':
            k = atoi(base[1:])
            if k >= p.n_ops or p.kind[k] != OPK_GPR:
                return None
            return (K['K_MEM'], 32 + (p.alias[k] if p.alias[k] >= 0 else k), NO_COMP, 0, imm)
        if base[:1] != '$' or gpr_number(base[1:]) < 0:
            return None
        return (K['K_MEM'], gpr_number(base[1:]), NO_COMP, 0, imm)
    if t[:1] == '%':
        k = atoi(t[1:])
        if k >= p.n_ops:
            return None
        r = p.alias[k] if p.alias[k] >= 0 else k
        kind = p.kind[r]
        if kind == OPK_GPR: return (K['K_GPR'], 32 + r, NO_COMP, 0, 0)
        if kind == OPK_FPR: return (K['K_FPR'], 32 + r, NO_COMP, 0, 0)
        if kind == OPK_MEM: return (K['K_OPMEM'], r, NO_COMP, 0, 0)
        return None
    if t.startswith('$vf'):
        v, rest = strtol(t[3:], 10)
        comp = NO_COMP
        if rest:
            c = comp_of(rest[0])
            if c < 0 or len(rest) > 1:
                return None
            comp = c
        return (K['K_VF'], v & 31, comp, 0, 0)
    if t.startswith('$vi'):
        return (K['K_VI'], atoi(t[3:]) & 31, NO_COMP, 0, 0)
    if t[:1] == '$' and t[1:2] == 'f' and t[2:3].isdigit() and t[2:3] in '0123456789':
        return (K['K_FPR'], atoi(t[2:]) & 31, NO_COMP, 0, 0)
    if t[:1] == '$':
        r = gpr_number(t[1:])
        return None if r < 0 else (K['K_GPR'], r, NO_COMP, 0, 0)
    if t[:1] in tuple('0123456789-'):
        return (K['K_IMM'], 0, NO_COMP, 0, s32(strtol(t)[0]))
    return None


def instruction(p, line):
    """(op, dest, bc, n, arg, operands), 0 for nothing, -1 for an error (ps2asm.c: parse_line)."""
    mn = line.lstrip(' \t')
    cut = min([i for i in (mn.find(' '), mn.find('\t')) if i >= 0], default=-1)
    args = mn[cut + 1:] if cut >= 0 else ''
    mn = mn[:cut] if cut >= 0 else mn
    if not mn or mn.endswith(':') or mn[0] == '.':
        return 0
    dot = None
    if '.' in mn:
        mn, dot = mn.split('.', 1)
    op, bc, arg = None, NO_COMP, 0
    if mn in SIMPLE_OPS:
        op = SIMPLE_OPS[mn]
    elif mn[:1] == 'v':
        if mn.startswith(('vftoi', 'vitof')):
            op, arg = ('V_FTOI' if mn[1] == 'f' else 'V_ITOF'), atoi(mn[5:])
        else:
            for name, family in VFAMILY:
                if not mn[1:].startswith(name):
                    continue
                rest = mn[1 + len(name):]
                if len(rest) > 1:
                    continue
                if rest == 'q': bc = 4
                elif rest == 'i': bc = 5
                elif rest and comp_of(rest) >= 0: bc = comp_of(rest)
                elif rest: continue
                op = family
                break
    if op is None:
        return -1
    dest = 0
    if dot is not None and mn[0] == 'v':
        for ch in dot:
            if comp_of(ch) >= 0:
                dest |= 1 << comp_of(ch)
    ops = []
    while args and len(ops) < 3:
        end, depth = 0, 0
        while end < len(args) and (depth or args[end] != ','):
            depth += args[end] == '('
            depth -= args[end] == ')'
            end += 1
        a, args = args[:end], args[end + 1:] if end < len(args) else None
        o = operand(p, a)
        if o is None:
            return -1
        ops.append(o)
    return (op, dest, bc, len(ops), arg, tuple(ops))


FIRST_IS_SOURCE = {'SQ', 'SQC2', 'SWC1', 'MTC1', 'QMTC2', 'MULTU', 'V_SQI'}


def compile_program(tmpl, cons, n, recompiled):
    """The Ins sequence of a template, or None when ps2asm.c marks it bad (ps2asm.c: compile)."""
    p = Prog(cons, n)
    ins = []
    for line in re.split(r'[\n;]', tmpl):
        r = instruction(p, line)
        if r == -1:
            return None
        if r:
            ins.append(r)
    if not recompiled:  # check_abi_reads
        written = 0
        for op, dest, bc, count, arg, ops in ins:
            first_is_dest = op not in FIRST_IS_SOURCE
            for kind, reg, comp, step, imm in ops[1 if first_is_dest and ops and ops[0][0] == K['K_GPR'] else 0:]:
                if kind in (K['K_GPR'], K['K_MEM']) and reg < 32 and (0xB00000F0 >> reg & 1) and not (written >> reg & 1):
                    return None
            if first_is_dest and ops and ops[0][0] == K['K_GPR'] and ops[0][1] < 32:
                written |= 1 << ops[0][1]
    return tuple(ins) or None


def templates(dirs, asm_direct=None):
    found, direct = set(), {}
    asm_direct = {} if asm_direct is None else asm_direct
    for d in dirs:
        for path in Path(d).rglob('*'):
            if path.suffix not in ('.c', '.cpp', '.h', '.inc') or not path.is_file():
                continue
            text = path.read_text(encoding='utf-8', errors='replace')
            if 'ps2_asm(' not in text and 'ps2_exec' not in text and 'PS2X(' not in text and 'PS2A(' not in text:
                continue
            for m in ASM_CALL.finditer(text):
                found.add((c_literals(m.group(1)), c_literals(m.group(2)), int(m.group(3)), False))
            for m in ASM_DIRECT.finditer(text):
                site = (c_literals(m.group(2)), c_literals(m.group(3)), int(m.group(4)), False)
                if asm_direct.setdefault(m.group(1), site) != site:
                    sys.exit('%s: %s names two templates' % (path, m.group(1)))
                found.add(site)
            for m in EXEC_CALL.finditer(text):
                found.add((c_literals(m.group(1)), '', 0, True))
            for m in DIRECT_CALL.finditer(text):
                insn = c_literals(m.group(2))
                if direct.setdefault(m.group(1), insn) != insn:
                    sys.exit('%s: %s names "%s" and "%s"' % (path, m.group(1), direct[m.group(1)], insn))
                found.add((insn, '', 0, True))
    return found, direct


def opd(o):
    kind, reg, comp, step, imm = o
    return '{%d, %d, 0x%X, %d, %d}' % (kind, reg, comp, step, imm)


def asm_direct_function(fn, tmpl, cons, n, programs):
    """transform.py's PS2A(fn, ...): ps2_asm with this site's (template, constraints, count) folded
    in. The first call runs ps2_asm and asm_first checks that the run-time parse (instructions,
    operand kinds, ties, directions) is the one this code was generated from; until it is, or with
    DHD_ASM_GENERIC=1, or for a template not generated, every call stays on ps2_asm. The binding
    and write-back are ps2_asm's own, operand by operand; the program runs as run_program would."""
    ins = compile_program(tmpl, cons, n, False)
    head = 'void %s(const char *t, const char *c, int n, ps2_op *ops)' % fn
    if ins is None or n > 32:
        return '%s { ps2_asm(t, c, n, ops); }' % head
    p = Prog(cons, n)
    desc = ', '.join('%d, %d, %d, %d' % (p.kind[k], p.alias[k], p.inp[k], p.out[k]) for k in range(n))
    body = []
    for k in range(n):  # bind inputs, in ps2_asm's order
        if not p.inp[k]:
            continue
        r = p.alias[k] if p.alias[k] >= 0 else k
        if p.kind[r] == OPK_GPR:
            body.append('asm_bind_gpr(&ps2.gpr[%d], ops[%d].p, ops[%d].size);' % (32 + r, k, k))
        elif p.kind[r] == OPK_FPR:
            body.append('memcpy(&ps2.fpr[%d], ops[%d].p, 4);' % (32 + r, k))
    i = programs[ins]
    body.append('asm_caller = PS2_CALLER_ADDRESS();')
    body.append('if (!p->fast_kind || !run_fast_program(p, ops)) aot_run_%d(p, ops);' % i)
    for k in range(n):  # write outputs back
        if not p.out[k]:
            continue
        src = {OPK_GPR: ('&ps2.gpr[%d]' % (32 + k), 16), OPK_FPR: ('&ps2.fpr[%d]' % (32 + k), 4),
               OPK_LO: ('&ps2.lo', 8), OPK_HI: ('&ps2.hi', 8)}.get(p.kind[k])
        if src:
            body.append('asm_put(ops[%d].p, %s, ops[%d].size, %d);' % (k, src[0], k, src[1]))
    statics = 'static Prog *p; static int generic;' + (' static const int8_t d[] = {%s};' % desc if n else '')
    return ('%s {\n    %s\n    if (p) { %s }\n'
            '    else if (generic) ps2_asm(t, c, n, ops);\n'
            '    else { ps2_asm(t, c, n, ops); p = asm_first(t, c, n, aot_ins_%d, %d, %s); generic = !p; }\n}'
            % (head, statics, ' '.join(body), i, len(ins), 'd' if n else '0'))


def generate(out, dirs):
    programs, parsed = {}, {}
    asm_direct = {}
    found, direct = templates(dirs, asm_direct)
    for tmpl, cons, n, recompiled in sorted(found):
        ins = compile_program(tmpl, cons, n, recompiled)
        if ins:
            programs.setdefault(ins, len(programs))
            if recompiled:
                parsed[tmpl] = ins
    lines = ['/* Generated by port/tools/ps2asm_aot.py from the transformed sources: %d templates, '
             '%d instruction sequences. Do not commit. */' % (len(found), len(programs))]
    for ins, i in programs.items():
        rows = []
        for op, dest, bc, count, arg, ops in ins:
            o = [opd(x) for x in ops] + ['{0, 0, 0, 0, 0}'] * (3 - len(ops))
            rows.append('    {%s, %d, 0x%X, %d, %d, {%s}},' % (op, dest, bc, count, arg, ', '.join(o)))
        lines.append('static const Ins aot_ins_%d[] = {\n%s\n};' % (i, '\n'.join(rows)))
        calls = ' '.join('run_ins(p, &aot_ins_%d[%d], ops);' % (i, k) for k in range(len(ins)))
        lines.append('static void aot_run_%d(Prog *p, ps2_op *ops) { %s }' % (i, calls))
    lines.append('static const struct { int n; const Ins *ins; void (*run)(struct Prog *, ps2_op *); } '
                 'aot_programs[] = {')
    lines += ['    {%d, aot_ins_%d, aot_run_%d},' % (len(ins), i, i) for ins, i in programs.items()]
    lines += ['};', '']
    # recomp.py's PS2X(fn, text): fn runs the generated code of its text once the first call
    # (exec_first, through ps2_exec) found the run-time parse identical; a text not generated
    # here stays on ps2_exec. The texts are recomp.py's instruction text: no quote or backslash.
    for fn, insn in sorted(direct.items()):
        ins = parsed.get(insn)
        if ins is None:
            lines.append('void %s(void) { ps2_exec("%s"); }' % (fn, insn))
        else:
            lines.append('void %s(void) { static Prog *p; if (p) aot_run_%d(p, 0); '
                         'else p = exec_first("%s", aot_ins_%d, %d); }' % (fn, programs[ins], insn,
                                                                          programs[ins], len(ins)))
    for fn, (tmpl, cons, n, _) in sorted(asm_direct.items()):
        lines.append(asm_direct_function(fn, tmpl, cons, n, programs))
    lines.append('')
    Path(out).write_text('\n'.join(lines), encoding='utf-8')
    return len(found), len(programs)


if __name__ == '__main__':
    print('%d templates, %d instruction sequences' % generate(sys.argv[1], sys.argv[2:]))
