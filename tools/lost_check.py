#!/usr/bin/env python3
"""List functions that are INCLUDE_ASM in src/ now but were matching C in some committed
version of a src/ file (lost to a TU merge). The image check can't see this: asm matches too.

Usage: lost_check.py        (exit 1 if any are found)
"""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import units, text_functions

ASM = re.compile(r'INCLUDE_ASM\([^,]+,\s*(\w+)\)')
MARK = re.compile(r'/\*\s*UNIT\s+0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)')


def git(*a):
    return subprocess.run(['git', *a], cwd=ROOT, capture_output=True, text=True).stdout


def main():
    names = [(v, os.path.splitext(n)[0]) for v, n in text_functions()]
    asm_now = {}
    for u in units():
        for f in ASM.findall(open(os.path.join(ROOT, u[2])).read()):
            asm_now[f] = u[2]
    ever_c = {}  # function -> (commit, path) of the oldest version that had it in C
    log = git('log', '--reverse', '--format=@@%H', '--name-only', '--diff-filter=AM', '--', 'src/unsorted')
    for block in log.split('@@')[1:]:
        lines = block.split()
        commit, paths = lines[0], lines[1:]
        for p in paths:
            text = git('show', '%s:%s' % (commit, p))
            m = MARK.match(text)
            if not m:
                continue
            lo, hi = int(m.group(1), 16), int(m.group(2), 16)
            in_asm = set(ASM.findall(text))
            for v, n in names:
                if lo <= v < hi and n not in in_asm:
                    ever_c.setdefault(n, (commit[:7], p))
    lost = sorted(n for n in ever_c if n in asm_now)
    for n in lost:
        print('%s  was C in %s:%s  now asm in %s' % (n, ever_c[n][0], ever_c[n][1], asm_now[n]))
    print('%d lost' % len(lost))
    sys.exit(1 if lost else 0)


if __name__ == '__main__':
    main()
