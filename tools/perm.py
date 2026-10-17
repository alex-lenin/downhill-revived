"""Random source permuter for one function: hill-climbs on the aligned-diff row count.

Mutates only the body of FUNC in SRC (statement swaps, declaration order, commutative operand
swaps, ++/+=1 forms, if-condition spellings), compiles each variant with tools/sncc.py and
scores it like tools/adiff.py (aligned rows that differ, +1 per word of size difference).
Variants that score at least as well as the current one are kept (ties with probability
--drift); every new best is written to OUT/best-NNN.c. A score of 0 is only the quick object
check: confirm it with tools/try_func.py, then read the source before promoting (a permuted
source can be correct but unreadable; tidy it and re-check).

usage: python tools/perm.py FUNC SRC [--seconds 600] [-j 6] [--out DIR] [--seed N]
"""
import argparse, difflib, os, random, re, struct, subprocess, sys, time
sys.path = [p for p in sys.path if os.path.abspath(p or '.') != os.path.dirname(os.path.abspath(__file__))]
from concurrent.futures import ThreadPoolExecutor  # noqa: E402 (before tools/queue.py shadows queue)
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
from quick_check import function_in_object  # noqa: E402
from diff_func import lookup  # noqa: E402
from check_image import read_elf, RETAIL  # noqa: E402

ap = argparse.ArgumentParser()
ap.add_argument('func')
ap.add_argument('src')
ap.add_argument('--seconds', type=int, default=600)
ap.add_argument('-j', type=int, default=6)
ap.add_argument('--out')
ap.add_argument('--seed', type=int)
ap.add_argument('--drift', type=float, default=0.3, help='chance to accept an equal score')
a = ap.parse_args()

name, addr, size = lookup(a.func)
base, _, text = read_elf(RETAIL)[1]['.text']
RW = struct.unpack_from('<%dI' % (size // 4), text, addr - base)
OP = lambda w: (w >> 26, w & 0x3F if w >> 26 == 0 else 0)  # opcode (+funct): coarse mnemonic
out_dir = a.out or 'build-try/perm/%s' % name
os.makedirs(out_dir, exist_ok=True)
ext = os.path.splitext(a.src)[1] or '.c'
rng = random.Random(a.seed)


def score(src_text, slot):
    p = '%s/w%d%s' % (out_dir, slot, ext)
    obj = '%s/w%d.o' % (out_dir, slot)
    open(p, 'w', newline='\n').write(src_text)
    if os.path.exists(obj):
        os.remove(obj)
    try:
        r = subprocess.run([sys.executable, 'tools/sncc.py', p, obj], capture_output=True, text=True,
                           timeout=120)
    except subprocess.TimeoutExpired:  # a hung compiler must not stall the whole run
        return None
    if r.returncode or not os.path.exists(obj):
        return None
    got = function_in_object(obj, name)
    if not got:
        return None
    data, value, csize, relocs = got
    bw = struct.unpack_from('<%dI' % (csize // 4), data, value)
    sm = difflib.SequenceMatcher(None, [OP(w) for w in RW], [OP(w) for w in bw], autojunk=False)
    bad = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == 'equal':
            bad += sum(1 for i, j in zip(range(i1, i2), range(j1, j2))
                       if RW[i] != bw[j] and (value + 4 * j) not in relocs)
        else:
            bad += max(i2 - i1, j2 - j1)
    return bad


# ---- locate the function body -------------------------------------------------------------
SRC = open(a.src).read()
m = re.search(r'^[^\n;]*\b%s\s*\([^;{]*\)\s*\{' % re.escape(name), SRC, re.M)
if not m:
    sys.exit('definition of %s not found in %s' % (name, a.src))
start = m.end()
depth, k = 1, start
while depth:
    c = SRC[k]
    depth += c == '{'
    depth -= c == '}'
    k += 1
HEAD, BODY, TAIL = SRC[:start], SRC[start:k - 1], SRC[k - 1:]

SIMPLE = re.compile(r'^(\s*)[^\s#{}][^{}]*;\s*$')  # one complete statement on the line
DECL = re.compile(r'^\s*(?:const\s+|unsigned\s+|signed\s+|struct\s+|volatile\s+)*\w+[\s*]+\w+(\[[^\]]*\])?'
                  r'(\s*__attribute__\(\([^)]*\)\))?\s*(=[^;]*)?;\s*$')
SKIP = re.compile(r'\basm\b|__asm__|\breturn\b|\bgoto\b|\bbreak\b|\bcontinue\b|^\s*\w+:')
COMM = re.compile(r'(?<![\w.>\]])([A-Za-z_][\w]*(?:(?:->|\.)\w+|\[[^\[\]]*\])*|\d+(?:\.\d+)?f?)'
                  r'\s*(==|!=|\+|\*|&|\||\^)\s*'
                  r'([A-Za-z_][\w]*(?:(?:->|\.)\w+|\[[^\[\]]*\])*|\d+(?:\.\d+)?f?)(?![\w(\[.]|->)')


ASSIGN = re.compile(r'^\s*([^=;]*?)\s*(?:[-+*/%&|^]|<<|>>)?=(?!=)')
IDENT = re.compile(r'[A-Za-z_]\w*')


def effects(stmt):
    """(variables written, identifiers read, writes memory/has side effects, reads memory)."""
    s = stmt.strip()
    m = ASSIGN.match(s)
    lhs = m.group(1) if m else ''
    inc = re.match(r'^\s*(\+\+|--)?\s*([\w>.\-\[\]]+)\s*(\+\+|--)?;', s)
    if not m and inc and (inc.group(1) or inc.group(3)):
        lhs = inc.group(2)
    plain = re.fullmatch(r'[A-Za-z_]\w*', lhs or '')
    defs = {lhs} if plain else set()
    side = '(' in s or (bool(lhs) and not plain) or 'volatile' in s
    reads = set(IDENT.findall(s))
    mem = side or bool(re.search(r'->|\[|\*\s*\(|\*\w', s))
    return defs, reads, side, mem


def independent(x, y):
    dx, rx, sx, mx = effects(x)
    dy, ry, sy, my = effects(y)
    return not (dx & ry or dy & rx or (sx and my) or (sy and mx))


def mutate(body):
    lines = body.split('\n')
    kind = rng.random()
    if kind < 0.45:  # swap two adjacent independent simple statements at the same indentation
        cand = [i for i in range(len(lines) - 1)
                if SIMPLE.match(lines[i]) and SIMPLE.match(lines[i + 1])
                and not SKIP.search(lines[i]) and not SKIP.search(lines[i + 1])
                and SIMPLE.match(lines[i]).group(1) == SIMPLE.match(lines[i + 1]).group(1)
                and lines[i].count(';') == 1 and lines[i + 1].count(';') == 1
                and independent(lines[i], lines[i + 1])]
        if not cand:
            return None
        i = rng.choice(cand)
        lines[i], lines[i + 1] = lines[i + 1], lines[i]
    elif kind < 0.6:  # move a declaration (without initializer) elsewhere among declarations
        cand = [i for i, l in enumerate(lines) if DECL.match(l) and '=' not in l]
        if len(cand) < 2:
            return None
        i, j = rng.sample(cand, 2)
        lines[i], lines[j] = lines[j], lines[i]
    elif kind < 0.85:  # swap operands of one commutative operator
        cand = [i for i, l in enumerate(lines) if COMM.search(l) and not SKIP.search(l)
                and not DECL.match(l)]
        if not cand:
            return None
        i = rng.choice(cand)
        l = lines[i]
        # only a whole sub-expression `x OP y` (bounded by ( = , ; or ), so precedence is kept
        ms = [mm for mm in COMM.finditer(l)
              if re.search(r'(^|[(=,?:]|\breturn)\s*$', l[:mm.start()])
              and re.match(r'\s*($|[);,?:])', l[mm.end():])]
        if not ms:
            return None
        mm = rng.choice(ms)
        lines[i] = l[:mm.start()] + '%s %s %s' % (mm.group(3), mm.group(2), mm.group(1)) + l[mm.end():]
    else:  # small spelling changes
        cand = [i for i, l in enumerate(lines) if re.search(r'\+\+|\+= 1;|if \((\w[\w>.\-]*)\)', l)
                and not SKIP.search(l)]
        if not cand:
            return None
        i = rng.choice(cand)
        l = lines[i]
        r = rng.random()
        if re.search(r'(\w[\w>.\-\]\[]*)\+\+;', l) and r < 0.5:
            l = re.sub(r'(\w[\w>.\-\]\[]*)\+\+;', r'\1 += 1;', l, count=1)
        elif '+= 1;' in l:
            l = re.sub(r'(\S+) \+= 1;', r'\1++;', l, count=1)
        elif re.search(r'if \((\w[\w>.\-]*)\)', l):
            l = re.sub(r'if \((\w[\w>.\-]*)\)', r'if (\1 != 0)', l, count=1)
        else:
            return None
        lines[i] = l
    return '\n'.join(lines)


best_body = BODY
best = score(HEAD + BODY + TAIL, 0)
if best is None:
    sys.exit('base does not compile')
print('base: %d rows' % best, flush=True)
cur_body, cur = best_body, best
t_end = time.time() + a.seconds
n = 0
saved = 0
with ThreadPoolExecutor(a.j) as ex:
    while time.time() < t_end and best > 0:
        bodies = []
        for _ in range(a.j):
            b = cur_body
            for _ in range(rng.choice((1, 1, 2, 3))):
                nb = mutate(b)
                if nb:
                    b = nb
            bodies.append(b)
        scores = list(ex.map(lambda ib: score(HEAD + ib[1] + TAIL, ib[0]), enumerate(bodies)))
        n += len(bodies)
        for b, s in sorted(zip(bodies, scores), key=lambda x: (x[1] is None, x[1] or 0)):
            if s is None or b == cur_body:
                continue
            if s < cur or (s == cur and rng.random() < a.drift):
                cur_body, cur = b, s
            if s < best:
                best, best_body = s, b
                saved += 1
                p = '%s/best-%03d%s' % (out_dir, saved, ext)
                open(p, 'w', newline='\n').write(HEAD + b + TAIL)
                print('%6d tries: %d rows -> %s' % (n, s, p), flush=True)
            break
print('done: %d tries, best %d rows' % (n, best))
