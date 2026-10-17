"""One-screen brief for a function: queue status/note, scores of every saved candidate
(build-try/cand-*/FUNC*.c/.cpp, via tools/sweep.py) and the retail asm without comments.

usage: python tools/brief.py FUNC [--no-asm] [--no-cands]
"""
import glob, json, os, re, subprocess, sys

func = sys.argv[1]
q = subprocess.run([sys.executable, 'tools/queue.py', 'show', func], capture_output=True, text=True).stdout
try:
    info = json.loads(q[q.index('{'):])
    print('%s  status=%s worker=%s\nnote: %s' % (func, info.get('status'), info.get('worker'), info.get('note')))
except ValueError:
    print(q.strip())
if '--no-cands' not in sys.argv:
    cands = sorted(set(glob.glob('build-try/cand-*/%s*.c' % func) + glob.glob('build-try/cand-*/%s*.cpp' % func)))
    if cands:
        out = subprocess.run([sys.executable, 'tools/sweep.py', func] + cands + ['-j', '8'],
                             capture_output=True, text=True).stdout
        print('candidates:\n' + '\n'.join(out.splitlines()[:8]))
if '--no-asm' not in sys.argv:
    path = 'asm/nonmatchings/text/%s.s' % func
    for line in open(path):
        line = re.sub(r'^\s*/\*.*?\*/\s*', '', line).rstrip()
        if not line or line.startswith(('.align', 'nonmatching', 'endlabel', 'glabel')):
            continue
        print(re.sub(r'\s+', ' ', line).replace('( 0x', '(0x'))
