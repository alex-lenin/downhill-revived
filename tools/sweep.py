"""Score many candidate sources for one function in parallel with try_func (quick check).

usage:
  python tools/sweep.py FUNC FILE...                 score existing files
  python tools/sweep.py FUNC --base B.c --spec S.py  build variants of B.c, then score them

S.py defines VARIANTS = {name: [(old, new), ...]}: each variant is B.c with every `old`
replaced by `new` (each `old` must occur in B.c, or the variant is reported as 'bad spec').
An entry may also be a function taking and returning the source text.
A pair may also be ('@', text) to append text to the file. --retflip (with --base, no --spec)
builds one variant per `extern int|void NAME(` declaration with the return type flipped: the
callee's return type changes the $v0/$v1 choice for code after the call. Variants are written to
build-try/sweep/<FUNC>/<spec name>/<name>.c (or B's extension). Results are printed best first: words differing, 'size'
(compiled larger/smaller than retail), 'error' (did not compile) or MATCH, plus ~N, the edit
distance over mnemonics (see fuzzy(); sorted by it). A MATCH here is
only the quick object check: confirm it with tools/try_func.py and tools/promote.py.
"""
import os, sys
sys.path = [p for p in sys.path if os.path.abspath(p or '.') != os.path.dirname(os.path.abspath(__file__))]
import argparse, re, subprocess  # noqa: E402 (tools/queue.py shadows the stdlib queue module)
from concurrent.futures import ThreadPoolExecutor  # noqa: E402

ap = argparse.ArgumentParser()
ap.add_argument('func')
ap.add_argument('files', nargs='*')
ap.add_argument('--base')
ap.add_argument('--spec')
ap.add_argument('--retflip', action='store_true', help='variants flipping each extern int/void callee')
ap.add_argument('-j', type=int, default=6)
ap.add_argument('--show', action='store_true', help='print the try_func tail for the best one')
a = ap.parse_args()

jobs = [(f, f) for f in a.files]
if a.spec or a.retflip:
    base = open(a.base).read()
    env = {}
    if a.retflip:
        a.spec = 'retflip'
        env['VARIANTS'] = {}
        for m in re.finditer(r'^(extern\s+(?:static\s+)?)(int|void)(\s+\**\s*(\w+)\s*\()', base, re.M):
            flip = 'void' if m.group(2) == 'int' else 'int'
            env['VARIANTS'][m.group(4) + '_' + flip] = [(m.group(0), m.group(1) + flip + m.group(3))]
    else:
        exec(open(a.spec).read(), env)
    # one directory per spec, so variants of different specs with the same name don't
    # overwrite each other
    # one dir per (spec, base) so sweeping the same spec over another base keeps both
    stem = os.path.splitext(os.path.basename(a.base))[0]
    out = 'build-try/sweep/%s/%s' % (a.func, os.path.splitext(os.path.basename(a.spec))[0])
    if stem != a.func:
        out += '-' + stem
    ext = os.path.splitext(a.base)[1] or '.c'
    os.makedirs(out, exist_ok=True)
    for name, pairs in env['VARIANTS'].items():
        t = base
        ok = True
        for pair in pairs:
            if callable(pair):  # a function str -> str, for rewrites a pair can't express
                t = pair(t)
                continue
            old, new = pair
            if old == '@':
                t += new
            elif old in t:
                t = t.replace(old, new)
            else:
                ok = False
                miss = old
        p = '%s/%s%s' % (out, name, ext)
        if not ok:
            print('%-24s bad spec: not found: %r' % (name, miss[:70]))
            continue
        open(p, 'w', newline='\n').write(t)
        jobs.append((name, p))


sys.path.insert(0, 'tools')
_retail = None


def fuzzy(obj):
    """Edit distance between retail and compiled instruction sequences, comparing mnemonics only
    (registers and immediates ignored: it measures code shape, not allocation). Unlike
    the word-by-word diff it stays meaningful when the candidate is larger or smaller."""
    global _retail
    import difflib, struct
    import rabbitizer as rz
    from quick_check import function_in_object
    from diff_func import lookup
    from check_image import read_elf, RETAIL
    name, addr, size = lookup(a.func)
    key = lambda w: rz.Instruction(w, 0, rz.InstrCategory.R5900).getOpcodeName()
    if _retail is None:
        base, _, text = read_elf(RETAIL)[1]['.text']
        _retail = [key(w) for w in struct.unpack_from('<%dI' % (size // 4), text, addr - base)]
    got = function_in_object(obj, name) if os.path.exists(obj) else None
    if not got:
        return None
    data, value, csize, _ = got
    built = [key(w) for w in struct.unpack_from('<%dI' % (csize // 4), data, value)]
    sm = difflib.SequenceMatcher(None, _retail, built, autojunk=False)
    return sum(max(i2 - i1, j2 - j1) for op, i1, i2, j1, j2 in sm.get_opcodes() if op != 'equal')


def score(i_job):
    i, (name, path) = i_job
    tag = 'sw%d' % (i % a.j)
    r = subprocess.run([sys.executable, 'tools/try_func.py', a.func, path, '--tag', tag,
                        '--quiet'], capture_output=True, text=True)
    o = r.stdout + r.stderr
    try:
        f = fuzzy('build-try/%s/%s/quick/%s.o' % (tag, a.func, a.func))
    except Exception as e:  # scoring aid only
        f = None
    fz = ' ~%s' % f if f is not None else ''
    m = re.search(r'(\d+) of (\d+) words differ', o)
    if m:
        return (f if f is not None else 10**5, int(m.group(1))), '%s/%s%s' % (m.group(1), m.group(2), fz), name, path, o
    if re.search(r'^MATCH', o, re.M) or 'whole image: MATCH' in o:
        return (-1, -1), 'MATCH', name, path, o
    if 'larger' in o or 'smaller' in o:
        return (f if f is not None else 10**6, 10**6), 'size' + fz, name, path, o
    return (10**7, 0), 'error', name, path, o


# one worker per tag: chunk jobs so no two concurrent jobs share a build dir
res = []
with ThreadPoolExecutor(a.j) as ex:
    for k in range(0, len(jobs), a.j):
        res += list(ex.map(score, list(enumerate(jobs))[k:k + a.j]))
res.sort(key=lambda x: x[0])  # (fuzzy distance, differing words)
for s, label, name, path, _ in res:
    print('%-24s %-12s %s' % (name, label, path))
if a.show and res:
    print(res[0][4][-3000:])
