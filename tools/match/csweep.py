#!/usr/bin/env python3
"""Sweep compilers x flag combinations over all cases; print per-flag-set match counts.

Usage: csweep.py [--cc A,B] [--base "-O2 -G0"] [--pairs]
"""
import os, sys, glob, itertools, argparse
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import grade as G
from grade import grade
from disfn import functions

HERE = os.path.dirname(os.path.abspath(__file__))
SUBS = ['-fno-schedule-insns', '-fno-schedule-insns2', '-fno-gcse', '-fno-regmove',
        '-fno-expensive-optimizations', '-fno-strength-reduce', '-fno-cse-follow-jumps',
        '-fno-rerun-cse-after-loop', '-fno-force-mem', '-fno-caller-saves', '-fno-peephole',
        '-fomit-frame-pointer', '-fno-strict-aliasing', '-fno-thread-jumps', '-fno-defer-pop',
        '-fno-optimize-sibling-calls', '-fno-delayed-branch']


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--cc', default='sn')
    ap.add_argument('--base', default='-O2 -G8 -fopt-stack')
    ap.add_argument('--pairs', action='store_true')
    ap.add_argument('--only', default='')
    ap.add_argument('--dist', action='store_true', help='rank by total edit distance')
    a = ap.parse_args()
    files = sorted(glob.glob(os.path.join(HERE, 'cases', 'func_*.c*')))
    if a.only:
        files = [f for f in files if any(x in f for x in a.only.split(','))]
    sizes = {s: e - s for s, e, _ in functions()}
    combos = [()] + [(x,) for x in SUBS]
    if a.pairs:
        combos += list(itertools.combinations(SUBS, 2))
    jobs = [(cc, c, f) for cc in a.cc.split(',') for c in combos for f in files]

    def run(job):
        cc, c, f = job
        func = os.path.basename(f).split('.')[0]
        addr = int(func[5:], 16)
        lang = 'c' if f.endswith('.c') else 'c++'
        r = grade(cc, lang, f, func, addr, a.base.split() + list(c), False, sizes.get(addr))
        return job, (r, getattr(G.LAST, 'dist', 999))

    res = {}
    with ThreadPoolExecutor(8) as ex:
        for (cc, c, f), r in ex.map(run, jobs):
            res.setdefault((cc, c), []).append((os.path.basename(f).split('.')[0][5:], r))
    key = (lambda kv: sum(d for _, (r, d) in kv[1])) if a.dist else (lambda kv: -sum(bool(r) for _, (r, d) in kv[1]))
    for (cc, c), rs in sorted(res.items(), key=key)[:40]:
        hits = [n for n, (r, d) in rs if r]
        dist = sum(d for _, (r, d) in rs)
        if hits or a.dist:
            print('%2d d=%-4d %-24s %-55s %s' % (len(hits), dist, cc, a.base + ' ' + ' '.join(c),
                  ' '.join('%s:%d' % (n, d) for n, (r, d) in rs)))


if __name__ == '__main__':
    main()
