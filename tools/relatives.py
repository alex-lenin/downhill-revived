#!/usr/bin/env python3
"""Nearest matched relative of every unfinished function, for worker prompts.

For each unfinished game/engine function, the already-matched functions are ranked by how similar
their instruction sequences are (mnemonics plus register-free operand shapes; addresses, offsets and
immediates ignored). The best few are written to build-try/relatives.tsv:
    FUNC  RELATIVE  SCORE  SOURCE
A matched relative with the same shape often shows the C form (and the inline-asm form for VU0/MMI
code) that this compiler needs.

usage: python tools/relatives.py [--top 3] [--min 0.55]
"""
import argparse, collections, difflib, importlib.util, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import TEXT_ASM, units  # noqa: E402

INS = re.compile(r'^\s*/\*\s*\w+\s+\w+\s+\w+\s*\*/\s+([a-z0-9.]+)\s*(.*)$')


def shape(path):
    """Instruction tokens: mnemonic + operand kinds (reg/fpr/vf/imm/mem/label), no values."""
    out = []
    for line in open(path, errors='replace'):
        m = INS.match(line)
        if not m:
            continue
        op, args = m.groups()
        kinds = []
        for a in [x.strip() for x in args.split(',') if x.strip()]:
            if '(' in a and a.endswith(')'):
                kinds.append('m')
            elif a.startswith('$vf') or a.startswith('$vi'):
                kinds.append('v')
            elif a.startswith('$f'):
                kinds.append('f')
            elif a.startswith('$'):
                kinds.append('r')
            elif a.startswith('.'):
                kinds.append('l')
            else:
                kinds.append('i')
        out.append(op + ':' + ''.join(kinds))
    return out


def grams(seq, n=3):
    return collections.Counter(tuple(seq[i:i + n]) for i in range(max(0, len(seq) - n + 1)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--top', type=int, default=3)
    ap.add_argument('--min', type=float, default=0.55)
    a = ap.parse_args()
    s = importlib.util.spec_from_file_location('queue_mod', os.path.join(ROOT, 'tools', 'queue.py'))
    q = importlib.util.module_from_spec(s)
    s.loader.exec_module(q)
    rows = [r for r in q.table() if r['region'] in ('game', 'engine') and r['status'] not in ('remnant', 'handwritten')]
    us = units()
    src_of = {}
    for r in rows:
        if r['status'] == 'done':
            src_of[r['name']] = next((u[2] for u in us if u[0] <= r['addr'] < u[1]), '')
    shapes = {r['name']: shape(os.path.join(ROOT, TEXT_ASM, r['name'] + '.s')) for r in rows}
    done = [r['name'] for r in rows if r['status'] == 'done' and len(shapes[r['name']]) >= 4]
    dgrams = {f: grams(shapes[f]) for f in done}
    out = []
    for r in rows:
        if r['status'] == 'done':
            continue
        f, sq = r['name'], shapes[r['name']]
        g = grams(sq)
        gt = sum(g.values()) or 1
        pre = []
        for d in done:
            dl = len(shapes[d])
            if dl < len(sq) * 0.5 or dl > len(sq) * 2:
                continue
            common = sum((g & dgrams[d]).values())
            pre.append((common / max(gt, sum(dgrams[d].values()) or 1), d))
        pre.sort(reverse=True)
        best = []
        for _, d in pre[:25]:
            best.append((difflib.SequenceMatcher(None, sq, shapes[d], autojunk=False).ratio(), d))
        best.sort(reverse=True)
        for score, d in best[:a.top]:
            if score >= a.min:
                out.append((f, d, score, src_of.get(d, '')))
    path = os.path.join(ROOT, 'build-try', 'relatives.tsv')
    with open(path, 'w', newline='\n') as fh:
        for f, d, score, src in out:
            fh.write('%s\t%s\t%.3f\t%s\n' % (f, d, score, src))
    have = len({f for f, _, _, _ in out})
    print('%d unfinished functions, %d with a matched relative >= %.2f -> %s' % (
        sum(1 for r in rows if r['status'] != 'done'), have, a.min, path))


if __name__ == '__main__':
    main()
