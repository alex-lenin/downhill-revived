"""Compile every src/ unit with clang for 32-bit x86 (syntax only) and group the errors.
usage: python port/tools/native_census.py [-j N] [--show KIND]"""
import argparse, collections, glob, json, os, re, subprocess
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, 'build', 'port-census.json')
ap = argparse.ArgumentParser()
ap.add_argument('-j', type=int, default=8)
ap.add_argument('--show')
a = ap.parse_args()

BASE = ['clang', '--target=i686-pc-linux-gnu', '-fsyntax-only', '-w', '-ferror-limit=0',
        '-fno-builtin', '-I', os.path.join(ROOT, 'include'), '-DNATIVE=1']


def run(path):
    cpp = not path.endswith('.c')
    cmd = BASE + (['-x', 'c++', '-std=gnu++98', '-fno-exceptions'] if cpp else ['-x', 'c', '-std=gnu89'])
    p = subprocess.run(cmd + [path], capture_output=True, text=True, encoding='utf-8', errors='replace')
    errs = re.findall(r'^[^\n]*?:(\d+):\d+: error: (.*)$', p.stderr, re.M)
    return os.path.relpath(path, ROOT).replace(os.sep, '/'), [(int(l), m) for l, m in errs]


def kind(msg):
    """Collapse names/numbers so similar errors group together."""
    m = re.sub(r"'[^']*'", "'X'", msg)
    return re.sub(r'\d+', 'N', m)


if a.show is None:
    files = sorted(glob.glob(os.path.join(ROOT, 'src', '**', '*.c*'), recursive=True))
    with ThreadPoolExecutor(a.j) as ex:
        res = dict(ex.map(run, files))
    json.dump(res, open(OUT, 'w'), indent=0)
res = json.load(open(OUT))
kinds, files_by = collections.Counter(), collections.defaultdict(set)
for f, errs in res.items():
    for _, m in errs:
        kinds[kind(m)] += 1
        files_by[kind(m)].add(f)
clean = sum(1 for e in res.values() if not e)
print('%d files, %d compile cleanly, %d with errors, %d errors' % (
    len(res), clean, len(res) - clean, sum(len(e) for e in res.values())))
for k, n in kinds.most_common(40):
    print('%6d %5d files  %s' % (n, len(files_by[k]), k[:140]))
if a.show:
    for f, errs in res.items():
        for l, m in errs:
            if a.show in kind(m):
                print('%s:%d: %s' % (f, l, m))
