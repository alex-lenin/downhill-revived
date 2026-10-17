#!/usr/bin/env python3
"""Why did the compiler pick that register? Dump SN cc1's register-allocation decisions for a candidate.

For near misses where every instruction is right but values sit in other registers, guessing at C
rewrites is blind: GCC 2.95's global allocator decides from numbers it computes. This compiles the
candidate with the project's flags plus `-dlg` (local + global allocation dumps) and prints, for the
function, each pseudo-register the global allocator handled, in its allocation order: references,
live length (insns), approximate priority and the hard register it got.

How GCC 2.95 (global.c) decides, roughly:
  - pseudos are allocated in decreasing priority  ~ refs * floor(log2(refs)) / live_length
  - each takes the first suitable hard register not used by an already-allocated pseudo that is live
    at the same time; an argument prefers its incoming register ($a0..) but loses it to a
    higher-priority overlapping pseudo.
So if retail moves an argument to another register at entry, some value outranked it while both were
live: change that value's live range (compute it later, use it fewer times, give it its own variable
or drop one) instead of reordering unrelated statements.

Same idea as OpenRAC rac3's regalloc tool (same SN 1.36 cc1); this is our own implementation.

usage: python tools/regalloc.py CANDIDATE.c [--func NAME] [--flag F ...] [--keep DIR] [--passes]
  --flag F   extra flags (e.g. --flag=-fno-opt-stack); a UNIT marker's flags are used automatically
  --passes   also keep the scheduling and delay-slot dumps (-dS -dR -dd) for instruction-order diffs
             (.sched = first scheduling pass, .sched2 = second, .dbr = delay-slot filling)
  --keep DIR write the dumps there (default: a temp dir that is removed)
"""
import argparse, math, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import toolchain  # noqa: E402

REG = {0: 'zero', 1: 'at', 2: 'v0', 3: 'v1', 4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3', 8: 't0', 9: 't1', 10: 't2',
       11: 't3', 12: 't4', 13: 't5', 14: 't6', 15: 't7', 16: 's0', 17: 's1', 18: 's2', 19: 's3', 20: 's4',
       21: 's5', 22: 's6', 23: 's7', 24: 't8', 25: 't9', 28: 'gp', 29: 'sp', 30: 's8', 31: 'ra'}


def hard(n):
    n = int(n)
    if n in REG:
        return '$' + REG[n]
    if 32 <= n < 64:
        return '$f%d' % (n - 32)
    return '#%d' % n


def functions(dump):
    """Split a GCC dump into {function name: text} (';; Function NAME' headers)."""
    parts = re.split(r'^;; Function (.+?)\s*$', dump, flags=re.M)
    return {parts[i]: parts[i + 1] for i in range(1, len(parts) - 1, 2)}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('src')
    ap.add_argument('--func')
    ap.add_argument('--flag', action='append', default=[])
    ap.add_argument('--keep')
    ap.add_argument('--passes', action='store_true')
    a = ap.parse_args()
    from configure import unit_marker
    m = unit_marker(a.src) if os.path.exists(a.src) else None
    flags = list(toolchain.CFLAGS if a.src.endswith('.c') else toolchain.CXXFLAGS)
    flags += (m[4] if m else []) + a.flag
    if '-sce29' in flags:
        sys.exit('regalloc: -sce29 units use another compiler; not supported here')
    tmp = a.keep or tempfile.mkdtemp(prefix='regalloc_')
    os.makedirs(tmp, exist_ok=True)
    try:
        base = 'v' + os.path.splitext(a.src)[1]
        shutil.copy(a.src, os.path.join(tmp, base))
        dumps = ['-dlg'] + (['-dS', '-dR', '-dd'] if a.passes else [])
        inc = [x for d in ('include', 'src') for x in ('-I', os.path.join(ROOT, d))]
        r = subprocess.run(toolchain.exe(toolchain.CC) + ['-S', base, '-o', 'v.s'] + inc + flags + dumps,
                           cwd=tmp, capture_output=True, text=True)
        if r.returncode:
            sys.exit((r.stdout + r.stderr)[-1500:])
        # cc1plus names its dumps v.cc.* for a .cpp source
        dbase = base if os.path.exists(os.path.join(tmp, base + '.lreg')) else 'v.cc'
        lreg = functions(open(os.path.join(tmp, dbase + '.lreg'), errors='replace').read())
        greg = functions(open(os.path.join(tmp, dbase + '.greg'), errors='replace').read())
        names = [n for n in greg if not a.func or n == a.func or a.func in n]
        if not names:
            sys.exit('no function %s in the dumps (have: %s)' % (a.func, ' '.join(greg)))
        for name in names:
            info = {}
            for mm in re.finditer(r'^Register (\d+) used (\d+) times across (\d+) insns(.*?)\.$',
                                  lreg.get(name, ''), re.M):
                n, refs, live, rest = int(mm.group(1)), int(mm.group(2)), int(mm.group(3)), mm.group(4)
                pri = (int(math.log2(refs)) if refs else 0) * refs / live if live else 0.0
                info[n] = (refs, live, pri, 'user var' in rest, re.findall(r'in class (\w+)', rest))
            g = greg[name]
            order = [int(x) for x in (re.search(r'regs to allocate: ([\d ]+)', g) or re.search('()', '')).group(1).split()]
            disp = {}
            if ';; Register dispositions:' in g:
                d = g.split(';; Register dispositions:')[1].split(';; Hard regs')[0]
                disp = {int(p): int(h) for p, h in re.findall(r'(\d+) in (\d+)', d)}
            print('%s: global allocation order (highest priority first)' % name)
            print('  pseudo  ->  reg    refs  live  priority  ')
            for p in order:
                refs, live, pri, user, _ = info.get(p, (None, None, 0.0, False, []))
                print('  %-6d  ->  %-5s  %-4s  %-4s  %8.3f  %s' % (p, hard(disp[p]) if p in disp else 'stack?',
                      refs, live, pri, 'named variable' if user else ''))
            local = sorted(p for p in disp if p not in order)
            if local:
                print('  allocated locally (live in one basic block): ' +
                      ', '.join('%d->%s' % (p, hard(disp[p])) for p in local))
            print()
        if a.keep or a.passes:
            print('dumps kept in %s (%s)' % (tmp, ' '.join(sorted(f for f in os.listdir(tmp) if f.startswith(base + '.')))))
    finally:
        if not a.keep and not a.passes:
            shutil.rmtree(tmp, ignore_errors=True)


if __name__ == '__main__':
    main()
