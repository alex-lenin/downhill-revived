#!/usr/bin/env python3
"""Work queue: every .text function with region, features and status; claims for workers.

  queue.py list [--region game] [--max-size 0x80] [--status open] [-n 30] [--feature fpu]
  queue.py next --worker NAME [-n 5] [--max-size 0x80]   claim the n easiest open functions
  queue.py claim FUNC --worker NAME | release FUNC | done FUNC | fail FUNC --note TEXT
  queue.py show FUNC

Status: done (built from source, from the UNIT markers), claimed, failed (a worker gave up;
note says why), remnant (a dead-stripped function's leftover last word: stays asm), open. Claims live in build/claims.json (not committed), updated under a lock
so parallel workers in one checkout never take the same function.

Regions: crt0, game, libgcc, engine (0x201000-0x243000: built like game code; maybe an
engine/middleware layer), sdk (Sony libs + libm/libc). `game` and `engine` are queued by
default (--region all|game|engine|...).
"""
import argparse, json, os, re, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import text_functions, units, TEXT_ASM, TEXT_END

CLAIMS = os.path.join(ROOT, 'build', 'claims.json')
LOCK = CLAIMS + '.lock'
REGIONS = [  # (start, end, name) from config/libmatch.txt clusters; see docs/findings/003
    (0x10A000, 0x10A208, 'crt0'),
    (0x10A208, 0x1FC000, 'game'),
    (0x1FC000, 0x201000, 'libgcc'),
    (0x201000, 0x243000, 'engine'),   # same toolchain+flags as game: 3/3 probes matched
    (0x243000, TEXT_END, 'sdk'),
]
INSN = re.compile(r'^\s*/\*\s*\w+\s+\w+\s+\w+\s*\*/\s+([a-z0-9.]+)\s*(.*)$')


HANDWRITTEN = {l.split()[0] for l in open(os.path.join(ROOT, 'config', 'handwritten.txt'))
               if l.strip() and not l.startswith('#')} if os.path.exists(os.path.join(ROOT, 'config', 'handwritten.txt')) else set()
RETRY_COOLDOWN = 6 * 3600  # seconds after a failure before --retry-failed hands the function out again
REMNANTS = {l.split()[0] for l in open(os.path.join(ROOT, 'config', 'remnants.txt'))
            if l.strip() and not l.startswith('#')} if os.path.exists(os.path.join(ROOT, 'config', 'remnants.txt')) else set()


def region(v):
    return next(n for a, b, n in REGIONS if a <= v < b)


def features(path):
    ins, f = [], set()
    for line in open(path):
        m = INSN.match(line)
        if m:
            ins.append((m.group(1), m.group(2)))
    calls = sum(op in ('jal', 'jalr') for op, _ in ins)
    for op, args in ins:
        if op == 'jr' and not args.startswith('$ra'):
            f.add('jtbl')
        if op.startswith(('lwc1', 'swc1', 'mtc1', 'mfc1', 'add.s', 'mul.s', 'c.', 'bc1', 'cvt.')):
            f.add('fpu')
        if op.startswith(('lqc2', 'sqc2', 'qmtc2', 'qmfc2', 'vadd', 'vmul', 'vsub', 'vmadd', 'vopm',
                          'vcall', 'ctc2', 'cfc2')):
            f.add('vu0')
        if op in ('lq', 'sq') or op.startswith(('pa', 'pc', 'pm', 'pp', 'pe', 'pl', 'qfsrv')):
            f.add('mmi')
        if op.startswith('b') and re.search(r'\.L([0-9A-F]{8})', args):
            pass
        if op in ('sd',) and re.match(r'\$s\d|\$fp', args):
            f.add('saves')
    labels = re.findall(r'^\s*\.L([0-9A-F]{8}):', open(path).read(), re.M)
    if any(op.startswith('b') for op, _ in ins) and labels:
        here = [int(x, 16) for x in re.findall(r'/\*\s*\w+\s+([0-9A-F]{8})', open(path).read())]
        for op, args in ins:
            m = re.search(r'\.L([0-9A-F]{8})', args)
            if op.startswith('b') and m and here and int(m.group(1), 16) <= here[0] + 0:
                pass
        # backward branch => loop
        txt = open(path).read()
        for m in re.finditer(r'/\*\s*\w+\s+([0-9A-F]{8})\s+\w+\s*\*/\s+b\w*\s+[^\n]*\.L([0-9A-F]{8})', txt):
            if int(m.group(2), 16) < int(m.group(1), 16):
                f.add('loop')
                break
    if calls:
        f.add('calls')
    return len(ins), sorted(f)


def score(size, feats):
    """Rough difficulty: bigger and more exotic is harder."""
    return size + 0x40 * len(set(feats) & {'jtbl', 'vu0', 'mmi', 'loop'}) + 0x10 * ('fpu' in feats)


def load_claims():
    return json.load(open(CLAIMS)) if os.path.exists(CLAIMS) else {}


class Locked:
    def __enter__(self):
        os.makedirs(os.path.dirname(LOCK), exist_ok=True)
        for _ in range(600):
            try:
                self.fd = os.open(LOCK, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
                return self
            except FileExistsError:
                time.sleep(0.05)
        sys.exit('claims lock stuck: remove %s if no worker is running' % LOCK)

    def __exit__(self, *a):
        os.close(self.fd)
        os.remove(LOCK)


def save_claims(c):
    tmp = CLAIMS + '.tmp'
    with open(tmp, 'w') as f:
        json.dump(c, f, indent=1, sort_keys=True)
    os.replace(tmp, CLAIMS)


def table():
    funcs = text_functions()
    bounds = [v for v, _ in funcs] + [TEXT_END]
    done = set()
    for s, e, src, _, _ in units():
        stubs = set(re.findall(r'INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)', open(os.path.join(ROOT, src)).read()))
        done |= {f[:-2] for v, f in funcs if s <= v < e and f[:-2] not in stubs}
    claims = load_claims()
    rows = []
    for i, (v, f) in enumerate(funcs):
        name = f[:-2]
        n, feats = features(os.path.join(ROOT, TEXT_ASM, f))
        size = bounds[i + 1] - v
        c = claims.get(name, {})
        st = 'done' if name in done else c.get('status', 'open')
        # SN's linker dead-strips unreferenced functions in 8-byte blocks, leaving the last
        # word of one whose size is 4 mod 8: splat sees a 4-byte "function".
        # Several leftovers in a row become one splat "function" with no jr/j in it.
        text = open(os.path.join(ROOT, TEXT_ASM, f)).read()
        m = re.search(r'nonmatching\s+\S+,\s*(0x[0-9A-Fa-f]+)', text)
        jumps = re.search(r'\*/\s+(jr|j)\s', text)
        if st != 'done' and name in HANDWRITTEN:  # config/handwritten.txt: stays asm, never handed out
            st = 'handwritten'
        elif st == 'open' and ((m and int(m.group(1), 16) <= 4) or not jumps or name in REMNANTS):
            st = 'remnant'
        rows.append(dict(name=name, addr=v, size=size, insns=n, feats=feats, region=region(v),
                         status=st, worker=c.get('worker', ''), note=c.get('note', ''),
                         score=score(size, feats)))
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('cmd', choices=['list', 'next', 'claim', 'release', 'done', 'fail', 'show'])
    ap.add_argument('func', nargs='?')
    ap.add_argument('--worker', default='')
    ap.add_argument('--note', default='')
    ap.add_argument('--region', default='default')
    ap.add_argument('--status', default=None)
    ap.add_argument('--feature', action='append', default=[])
    ap.add_argument('--without', action='append', default=[])
    ap.add_argument('--max-size', type=lambda x: int(x, 0), default=None)
    ap.add_argument('--min-size', type=lambda x: int(x, 0), default=0)
    ap.add_argument('-n', type=int, default=30)
    ap.add_argument('--biggest', action='store_true', help='next: largest functions first (for strong, slow models)')
    ap.add_argument('--retry-failed', action='store_true',
                    help='next: failed functions are eligible too (after open ones of the same order)')
    ap.add_argument('--skip-failed-by', default='',
                    help='next, with --retry-failed: not those failed by a worker starting with this')
    a = ap.parse_args()

    if a.cmd in ('claim', 'release', 'done', 'fail'):
        with Locked():
            c = load_claims()
            cur = c.get(a.func, {})
            if a.cmd == 'claim':
                if cur.get('status') == 'claimed' and cur.get('worker') != a.worker:
                    sys.exit('%s already claimed by %s' % (a.func, cur['worker']))
                c[a.func] = dict(status='claimed', worker=a.worker, t=time.time(), fails=cur.get('fails', 0))
            elif a.cmd == 'release':
                c.pop(a.func, None)
            else:
                c[a.func] = dict(status='failed' if a.cmd == 'fail' else 'matched',
                                 worker=a.worker or cur.get('worker', ''), note=a.note, t=time.time(),
                                 fails=cur.get('fails', 0) + (a.cmd == 'fail'))
            save_claims(c)
        print('%s %s' % (a.cmd, a.func))
        return

    rows = table()
    if a.cmd == 'show':
        r = next((r for r in rows if r['name'] == a.func), None)
        print(json.dumps(r, indent=1) if r else 'unknown %s' % a.func)
        return
    want = {'game', 'engine'} if a.region == 'default' else None
    sel = [r for r in rows if (a.region == 'all' or r['region'] == a.region
                               or (want and r['region'] in want))
           and (a.max_size is None or r['size'] <= a.max_size)
           and r['size'] >= a.min_size
           and all(f in r['feats'] for f in a.feature)
           and not any(f in r['feats'] for f in a.without)]
    if a.cmd == 'next':
        assert a.worker, '--worker required'
        with Locked():
            c = load_claims()
            picked = []
            # retries: least-failed first, and not again within RETRY_COOLDOWN of the last failure (else the
            # same hopeless function comes straight back to the next worker)
            fails = lambda r: c.get(r['name'], {}).get('fails', 0)
            order = ((lambda r: (r['status'] != 'open', fails(r), -r['size'])) if a.biggest
                     else (lambda r: (fails(r), r['score'])))
            for r in sorted(sel, key=order):
                cur = c.get(r['name'])
                retry = (a.retry_failed and r['status'] == 'failed' and cur and cur.get('status') == 'failed'
                         and time.time() - cur.get('t', 0) > RETRY_COOLDOWN
                         and not (a.skip_failed_by and cur.get('worker', '').startswith(a.skip_failed_by))
                         and not cur.get('note', '').startswith('handwritten asm'))  # stays .s, don't retry
                if (r['status'] == 'open' and r['name'] not in c) or retry:
                    c[r['name']] = dict(status='claimed', worker=a.worker, t=time.time(),
                                        fails=cur.get('fails', 0) if cur else 0)
                    if retry:  # keep what the last worker found, for the next one's hint
                        c[r['name']].update(prev_note=cur.get('note', ''), prev_worker=cur.get('worker', ''))
                    picked.append(r)
                    if len(picked) >= a.n:
                        break
            save_claims(c)
        for r in picked:
            print('%s %08x %#x %s' % (r['name'], r['addr'], r['size'], ','.join(r['feats'])))
        return
    if a.status:
        sel = [r for r in sel if r['status'] == a.status]
    total = len(sel)
    counts = {}
    for r in sel:
        counts[r['status']] = counts.get(r['status'], 0) + 1
    for r in sorted(sel, key=lambda r: r['score'])[:a.n]:
        print('%-26s %08x %5x %-7s %-8s %s %s' % (r['name'], r['addr'], r['size'], r['status'],
                                                r['worker'], ','.join(r['feats']), r['note']))
    print('-- %d functions (%s)' % (total, ', '.join('%s %d' % kv for kv in sorted(counts.items()))))


if __name__ == '__main__':
    main()
