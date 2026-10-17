"""Kick dependency analysis of a captured VU1 run sequence (port/runtime/vu1_seq_dep.c).

Capture first (game data, under build/):
    DHD_VU_SEQ=build/port/vu-seq DHD_VU_SEQ_AT=<game seconds> DHD_VU_SEQ_KICKS=8000 dhd.exe ...
then
    python port/tools/vu1_seq_dep.py build/port/vu-seq
builds the analyzer with the VU interpreter (the PCSX2-checked reference), replays every run from
its real state and from the state a parallel dispatcher would have 1-3 runs earlier, and writes
DIR/dep.csv plus a summary per microprogram entry.
"""
import argparse
import collections
import csv
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('dir', type=Path)
    ap.add_argument('--max', type=int, default=0)
    args = ap.parse_args()
    out = args.dir.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('the capture must stay under build/')
    exe = out / 'vu1_seq_dep.exe'
    subprocess.run(['clang-cl', '-m32', '/nologo', '/O2', '-Wno-deprecated-declarations', '-msse2', '-mno-fma',
                    '-mfpmath=sse', '/clang:-ffp-contract=off', '/I' + str(ROOT / 'port/runtime'),
                    str(ROOT / 'port/runtime/vu0_interp.c'), str(ROOT / 'port/runtime/vu1_seq_dep.c'),
                    '/Fe' + str(exe)], cwd=out, check=True, timeout=600)
    with (out / 'dep.csv').open('w') as log:
        subprocess.run([str(exe), str(out)] + ([str(args.max)] if args.max else []), stdout=log, check=True,
                       timeout=3600)
    rows = list(csv.DictReader((out / 'dep.csv').open()))
    by = collections.defaultdict(lambda: [0, 0, 0, 0, collections.Counter()])
    for r in rows:
        b = by[r['entry']]
        b[0] += 1
        for d in (1, 2, 3):
            b[d] += r['dep%d' % d] == '0'
        if r['dep1'] == '1':
            b[4][r['regs1'] + ('m' if r['mem1'] != '0' else '')] += 1
    print('%d runs over fields %s-%s' % (len(rows), rows[0]['field'], rows[-1]['field']))
    print('entry   runs  indep@1  indep@2  indep@3  dependent: register groups (1 vf, 2 acc, 4 vi, 8 q/p/i/r, 16 ctrl; m memory)')
    for e, b in sorted(by.items(), key=lambda kv: -kv[1][0]):
        print('%s %6d  %6.1f%%  %6.1f%%  %6.1f%%  %s' % (e, b[0], 100 * b[1] / b[0], 100 * b[2] / b[0], 100 * b[3] / b[0],
                                                     dict(b[4].most_common(4))))


if __name__ == '__main__':
    main()
