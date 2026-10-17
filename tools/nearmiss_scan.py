#!/usr/bin/env python3
"""Measure every saved attempt on unfinished functions; promote the ones that already match.

Workers leave candidates in build-try/cand*/ (FUNC.c / FUNC.cpp). For each function that isn't
decompiled yet, every candidate is checked with tools/try_func.py (quick check first, so a
DIFF costs ~2 s). A candidate that prints MATCH and whole image MATCH goes through
tools/promote.py (which builds and checks again). The rest are ranked in
build-try/nearmiss.tsv: func, best candidate, words differing, total words, fraction.

usage: python tools/nearmiss_scan.py [--no-promote] [--only FUNC,...]
"""
import argparse, glob, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, 'build-try', 'nearmiss.tsv')
PART = OUT + '.partial'  # one line per checked function, so an interrupted scan resumes


def unfinished():
    sys.path.insert(0, os.path.join(ROOT, 'tools'))
    import importlib.util as u
    s = u.spec_from_file_location('queue_mod', os.path.join(ROOT, 'tools', 'queue.py'))
    q = u.module_from_spec(s)
    s.loader.exec_module(q)
    return {r['name'] for r in q.table() if r['region'] in ('game', 'engine')
            and r['status'] not in ('done', 'remnant', 'handwritten') and not r['note'].startswith('handwritten')}


def measure(func, cand):
    r = subprocess.run([sys.executable, 'tools/try_func.py', func, cand, '--tag', 'nearmiss', '--quiet'],
                       cwd=ROOT, capture_output=True, text=True, timeout=900)
    out = r.stdout + r.stderr
    m = re.search(r'(MATCH|DIFF) %s: (\d+) of (\d+)' % func, out)
    if not m:
        return None
    return int(m.group(2)), int(m.group(3)), 'whole image: MATCH' in out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--no-promote', action='store_true')
    ap.add_argument('--only', help='comma-separated functions')
    a = ap.parse_args()
    todo = unfinished()
    if a.only:
        todo &= set(a.only.split(','))
    cands = {}
    for p in glob.glob(os.path.join(ROOT, 'build-try', 'cand*', 'func_*.c*')):
        name = os.path.basename(p)
        m = re.fullmatch(r'(func_[0-9A-F]{8})\.(c|cpp)', name)
        if m and m.group(1) in todo:
            cands.setdefault(m.group(1), []).append(os.path.relpath(p, ROOT).replace('\\', '/'))
    print('%d unfinished functions, %d with saved attempts' % (len(todo), len(cands)), flush=True)
    rows, promoted, done = [], [], set()
    if os.path.exists(PART):
        for line in open(PART):
            p = line.rstrip('\n').split('\t')
            done.add(p[0])
            if len(p) == 4:
                rows.append((p[0], p[1], int(p[2]), int(p[3])))
    for i, func in enumerate(sorted(cands), 1):
        if func in done:
            continue
        best = None
        for c in cands[func]:
            try:
                res = measure(func, c)
            except subprocess.TimeoutExpired:
                res = None
            if res is None:
                continue
            diff, total, image = res
            if diff == 0 and image and not a.no_promote:
                p = subprocess.run([sys.executable, 'tools/promote.py', func, c, '--commit'],
                                   cwd=ROOT, capture_output=True, text=True)
                if 'promoted' in p.stdout:
                    promoted.append(func)
                    print('PROMOTED %s from %s' % (func, c), flush=True)
                    subprocess.run([sys.executable, 'tools/report_commit.py', 'nearmiss_scan', func], cwd=ROOT)
                    best = None
                    break
            if best is None or diff < best[1]:
                best = (c, diff, total)
        if best and func not in promoted:
            rows.append((func,) + best)
        with open(PART, 'a') as f:
            f.write('%s\t%s\n' % (func, '\t'.join(str(x) for x in best) if best and func not in promoted else 'none'))
        if i % 20 == 0:
            print('%d/%d checked' % (i, len(cands)), flush=True)
    rows.sort(key=lambda r: r[2] / max(r[3], 1))
    with open(OUT, 'w') as f:
        for func, c, diff, total in rows:
            f.write('%s\t%s\t%d\t%d\t%.3f\n' % (func, c, diff, total, diff / max(total, 1)))
    print('promoted %d: %s' % (len(promoted), ' '.join(promoted)))
    print('near misses written to %s (%d functions)' % (OUT, len(rows)))


if __name__ == '__main__':
    main()
