#!/usr/bin/env python3
"""Convert splat asm into something SN's ps2eeas assembles (for INCLUDE_ASM inside C units).

ps2eeas (1.9.25) quirks this works around:
  - only numeric GPR names ($29, not $sp);
  - segfaults on splat's .macro-based label macros -> expanded here to plain directives;
  - crashes on `c1 imm` (ops spimdisasm can't name) -> emitted as .word;
  - swaps vector source registers for `vadda` -> preserve the splat instruction word;
  - .LXXXXXXXX code labels -> global L_XXXXXXXX (jump tables in .rodata reference them);
  - has no %gp_rel() operator (and crashes on sym+off in its macro form) -> $gp-relative
    operands become plain numeric offsets from _gp. Retail layout is fixed, so the bytes
    are identical; the stub just carries no relocation for them.
Symbol addresses come from the name (D_XXXXXXXX / func_XXXXXXXX) or config/symbol_addrs.txt.

Usage: asm_sn.py IN.s OUT.s
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GP = 0x2A40F0
GPR = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5',
       't6', 't7', 's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1',
       'gp', 'sp', 'fp', 'ra']
REG = re.compile(r'\$(%s)\b' % '|'.join(sorted(GPR, key=len, reverse=True)))
LABEL = re.compile(r'^\s*(glabel|alabel|jlabel|dlabel|ehlabel|endlabel|enddlabel|nonmatching)'
                   r'\s+([^,\s]+)')
# spimdisasm writes ops it can't name (e.g. EE `sqrt.s`, whose source is in ft) as `c1 0x..`;
# ps2eeas crashes on that form, so use the raw word from the /* off vram BYTES */ comment.
RAWCOP = re.compile(r'^(\s*/\*\s*\w+\s+\w+\s+([0-9A-Fa-f]{8})\s*\*/\s*)c[0-3]\s+\S+\s*$')
# Only the non-broadcast vadda form has this operand-order discrepancy. These are
# existing INCLUDE_ASM instructions, never instructions generated from C.
RAWVADDA = re.compile(r'^(\s*/\*\s*\w+\s+\w+\s+([0-9A-Fa-f]{8})\s*\*/\s*)vadda(?:\.[xyzw]+)?\s+[^\n]+$')
LOCAL = re.compile(r'\.L([0-9A-Fa-f]{8})\b')  # see asmprep.py
GPREL = re.compile(r'%gp_rel\(([A-Za-z_.$][\w.$]*)\s*([+-]\s*(?:0x[0-9A-Fa-f]+|\d+))?\)')

_syms = None


def symbols():
    global _syms
    if _syms is None:
        _syms = {}
        p = os.path.join(ROOT, 'config', 'symbol_addrs.txt')
        if os.path.exists(p):
            for line in open(p):
                m = re.match(r'\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+)', line)
                if m:
                    _syms[m.group(1)] = int(m.group(2), 16)
    return _syms


def addr_of(name):
    m = re.match(r'(?:D|func|jtbl|L)_?([0-9A-Fa-f]{8})$', name)
    if m:
        return int(m.group(1), 16)
    if name in symbols():
        return symbols()[name]
    sys.exit('asm_sn: unknown address for gp-relative symbol %s' % name)


def gprel(m):
    off = int(m.group(2).replace(' ', ''), 0) if m.group(2) else 0
    v = addr_of(m.group(1)) + off - GP
    assert -0x8000 <= v < 0x8000, m.group(0)
    return ('-0x%X' % -v) if v < 0 else ('0x%X' % v)


def convert(text):
    out = []
    for line in text.split('\n'):
        m = LABEL.match(line)
        if m:
            kind, name = m.groups()
            if kind == 'nonmatching' and name.startswith('func_'):
                # objdiff convention: FUNC.NON_MATCHING marks an INCLUDE_ASM'd function as not
                # decompiled, so progress reports don't count its (identical) bytes.
                out += ['.globl %s.NON_MATCHING' % name, '%s.NON_MATCHING:' % name]
            elif kind in ('glabel', 'alabel', 'jlabel', 'ehlabel'):
                out += ['.globl %s' % name, '%s:' % name]   # jump/EH tables live in other objects
            elif kind == 'dlabel':
                out.append('%s:' % name)
            continue
        if line.lstrip().startswith('.include'):
            continue
        m = RAWCOP.match(line) or RAWVADDA.match(line)
        if m:
            b = m.group(2)
            out.append('%s.word 0x%s' % (m.group(1), b[6:8] + b[4:6] + b[2:4] + b[0:2]))
            continue
        line = GPREL.sub(gprel, line)
        out.append(REG.sub(lambda r: '$%d' % GPR.index(r.group(1)), line))
    return LOCAL.sub(r'L_\1', '\n'.join(out))


if __name__ == '__main__':
    src, dst = sys.argv[1:3]
    with open(src) as f:
        t = convert(f.read())
    with open(dst, 'w', newline='\n') as f:
        f.write(t + '\n')
