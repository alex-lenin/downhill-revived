#!/usr/bin/env python3
"""Generate C for trivial 8-byte functions (`jr $ra` + one delay-slot instruction).

Patterns: empty, return constant / argument / argument+offset, field or $gp-variable getter and
setter. Output goes to OUTDIR/<ADDR>.c with a UNIT marker; nothing is trusted until the
image check passes (verify with a private build, then promote).

Usage: trivial.py OUTDIR FUNC...      prints the functions it could not template
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import TEXT_ASM, text_functions, TEXT_END

INS = re.compile(r'^\s*/\*\s*\w+\s+([0-9A-Fa-f]{8})\s+\w+\s*\*/\s+(.*?)\s*$')
TYPES = {'lw': 'int', 'sw': 'int', 'lbu': 'unsigned char', 'sb': 'unsigned char',
         'lb': 'signed char', 'lhu': 'unsigned short', 'lh': 'short', 'sh': 'short',
         'lwc1': 'float', 'swc1': 'float'}
ARG = {'$a0': 0, '$a1': 1, '$a2': 2, '$a3': 3}


def params(n, types=None):
    types = types or {}
    return ', '.join('%s a%d' % (types.get(i, 'int'), i) for i in range(n)) or 'void'


def template(name):
    lines = open(os.path.join(ROOT, TEXT_ASM, name + '.s')).read().split('\n')
    ins = [(m.group(1), re.sub(r'\s+', ' ', m.group(2))) for m in map(INS.match, lines) if m]
    if len(ins) != 2 or ins[0][1] != 'jr $ra':
        return None
    op, _, rest = ins[1][1].partition(' ')
    a = [x.strip() for x in rest.split(',')] if rest else []
    if op == 'nop':
        return 'void %s(void) {\n}\n' % name
    if op == 'daddu' and a[1] == '$zero' and a[2] == '$zero':
        return 'int %s(void) {\n    return 0;\n}\n' % name
    if op == 'addiu' and a[0] == '$v0' and a[1] == '$zero':
        return 'int %s(void) {\n    return %d;\n}\n' % (name, int(a[2], 0))
    if op == 'daddu' and a[0] == '$v0' and a[1] in ARG and a[2] == '$zero':
        k = ARG[a[1]]
        return 'int %s(%s) {\n    return a%d;\n}\n' % (name, params(k + 1), k)
    if op == 'addiu' and a[0] == '$v0' and a[1] == '$a0':
        return 'char *%s(char *p) {\n    return p + %d;\n}\n' % (name, int(a[2], 0))
    if op not in TYPES:
        return None
    t = TYPES[op]
    load = op[0] == 'l'
    m = re.match(r'(%gp_rel\((D_[0-9A-F]{8})\)\(\$gp\)|(-?0x[0-9A-Fa-f]+|\d+)\((\$a[0-3])\))$', a[1])
    if not m:
        return None
    reg = a[0]
    if m.group(2):  # $gp variable (in a delay slot ps2eeas always uses $gp: extern is right)
        var = m.group(2)
        decl = 'extern %s %s;\n\n' % (t, var)
        if load:
            if reg not in ('$v0', '$f0'):
                return None
            return decl + '%s %s(void) {\n    return %s;\n}\n' % (t, name, var)
        if reg == '$zero':
            return decl + 'void %s(void) {\n    %s = 0;\n}\n' % (name, var)
        if reg == '$a0':
            return decl + 'void %s(%s v) {\n    %s = v;\n}\n' % (name, t, var)
        if reg == '$f12':
            return decl + 'void %s(float v) {\n    %s = v;\n}\n' % (name, var)
        return None
    off, base = int(m.group(3), 0), ARG[m.group(4)]
    if off < 0:
        return None
    sname = 'S_%s' % name[5:]
    struct = 'struct %s {\n%s    %s x%X;\n};\n\n' % (
        sname, '    char pad[0x%X];\n' % off if off else '', t, off)
    ptypes = {base: 'struct %s *' % sname}
    if load:
        if reg not in ('$v0', '$f0'):
            return None
        sig = ', '.join(('struct %s *s' % sname) if i == base else 'int a%d' % i for i in range(base + 1))
        return struct + '%s %s(%s) {\n    return s->x%X;\n}\n' % (t, name, sig, off)
    if reg == '$zero':
        sig = ', '.join(('struct %s *s' % sname) if i == base else 'int a%d' % i for i in range(base + 1))
        return struct + 'void %s(%s) {\n    s->x%X = 0;\n}\n' % (name, sig, off)
    if reg in ARG and ARG[reg] > base:
        k = ARG[reg]
        sig = ', '.join(('struct %s *s' % sname) if i == base else
                        ('%s v' % t if i == k else 'int a%d' % i) for i in range(k + 1))
        return struct + 'void %s(%s) {\n    s->x%X = v;\n}\n' % (name, sig, off)
    if reg == '$f12' and base == 0:
        return struct + 'void %s(struct %s *s, float v) {\n    s->x%X = v;\n}\n' % (name, sname, off)
    return None


def main():
    out = sys.argv[1]
    os.makedirs(os.path.join(ROOT, out), exist_ok=True)
    funcs = text_functions()
    starts = [v for v, _ in funcs] + [TEXT_END]
    addr = {f[:-2]: (v, starts[i + 1]) for i, (v, f) in enumerate(funcs)}
    skipped = []
    for name in sys.argv[2:]:
        body = template(name)
        if body is None:
            skipped.append(name)
            continue
        s, e = addr[name]
        with open(os.path.join(ROOT, out, '%08X.c' % s), 'w', newline='\n') as f:
            f.write('/* UNIT 0x%08X 0x%08X */\n%s' % (s, e, body))
    print('skipped:', ' '.join(skipped) if skipped else 'none')


if __name__ == '__main__':
    main()
