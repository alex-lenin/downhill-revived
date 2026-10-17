#!/usr/bin/env python3
"""Fix up splat/spimdisasm asm for modern GNU as (binutils 2.45, -march=r5900).

- spimdisasm writes VU0 macro-mode special registers bare (`ACC`, `I`, `Q`, `R`); gas wants
  `$ACC`, `$I`, `$Q`, `$R`. Only operands of `v*` instructions are touched.
- Code labels `.LXXXXXXXX` become `L_XXXXXXXX`: `.L` names never reach the symbol table, but
  jump tables in .rodata (another object) point at them.
asm/ stays as splat wrote it; this runs at build time.

Usage: asmprep.py IN.s OUT.s   (or import prep())
"""
import re, sys

VU_LINE = re.compile(r'^(\s*(?:/\*.*?\*/\s*)?)(v[a-z0-9.]+)(\s+)(.*)$')
SPECIAL = re.compile(r'(?<![$\w])(ACC|I|Q|R)(?!\w)')
LOCAL = re.compile(r'\.L([0-9A-Fa-f]{8})\b')


def prep_line(line):
    m = VU_LINE.match(line)
    if not m:
        return line
    head, op, sp, args = m.groups()
    return head + op + sp + SPECIAL.sub(r'$\1', args)


def prep(text):
    return LOCAL.sub(r'L_\1', '\n'.join(prep_line(l) for l in text.split('\n')))


if __name__ == '__main__':
    src, dst = sys.argv[1:3]
    with open(src) as f:
        t = prep(f.read())
    with open(dst, 'w', newline='\n') as f:
        f.write(t)
