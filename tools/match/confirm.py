#!/usr/bin/env python3
"""Grade every tools/match/cases/func_XXXXXX.{c,cc} against the retail ELF with one flag set.

Usage: confirm.py [--cc NAME] [--lang c|c++] [--diff] [CASE...] -- FLAGS...
The target size of each function comes from disfn.py's boundary heuristic.
"""
import os, sys, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from grade import grade
from disfn import functions

HERE = os.path.dirname(os.path.abspath(__file__))


def main():
    argv = sys.argv[1:]
    flags = argv[argv.index('--') + 1:] if '--' in argv else []
    argv = argv[:argv.index('--')] if '--' in argv else argv
    cc = 'sn'
    if '--cc' in argv:
        k = argv.index('--cc'); cc = argv[k + 1]; del argv[k:k + 2]
    force_lang = None
    if '--lang' in argv:
        k = argv.index('--lang'); force_lang = argv[k + 1]; del argv[k:k + 2]
    diff = '--diff' in argv
    argv = [a for a in argv if a != '--diff']
    files = sorted(glob.glob(os.path.join(HERE, 'cases', 'func_*.c*')))
    if argv:
        files = [f for f in files if any(a in os.path.basename(f) for a in argv)]
    sizes = {s: e - s for s, e, _ in functions()}
    n = 0
    for f in files:
        func = os.path.basename(f).split('.')[0]
        addr = int(func[5:], 16)
        lang = force_lang or ('c' if f.endswith('.c') else 'c++')
        r = grade(cc, lang, f, func, addr, flags, diff, sizes.get(addr))
        n += bool(r)
        print('%-6s %s' % ({True: 'MATCH', False: 'DIFF', None: 'ERROR'}[r], os.path.basename(f)), flush=True)
    print('%d/%d match  [%s %s]' % (n, len(files), cc, ' '.join(flags)))


if __name__ == '__main__':
    main()
