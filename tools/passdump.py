#!/usr/bin/env python3
"""Which compiler pass made the difference? Trace a candidate's RTL through every SN cc1 pass.

For near misses where the instructions are right but their order, hoisting, delay-slot filling or
operand choice differs, this compiles the candidate with the project's flags plus `-da` (a dump
after every pass) and shows the function's instructions pass by pass in a compact one-line form.
Find the first pass where the build's shape departs from retail, then change the C that feeds
that pass (see docs/findings/012-sn-gcc-matching-levers.md).

GCC 2.95 pass order (dump suffix): rtl, jump, cse, addressof, gcse, loop, cse2, flow, combine,
regmove, sched (sched1), lreg (local alloc), greg (global alloc + reload), flow2, sched2,
jump2, mach, dbr (delay slots), ...

usage: python tools/passdump.py CANDIDATE.c [--func NAME] [--flag F ...]
         [--track REGEX ...]   per pass, the position and text of insns matching REGEX
         [--show PASS]         full compact listing of one pass (e.g. sched, greg, dbr)
         [--keep DIR]          keep the dumps (default: a temp dir that is removed)
Example: python tools/passdump.py build-try/cand/func_X.c --track 0x450 --track 0x240
"""
import argparse, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import toolchain  # noqa: E402
from regalloc import functions, hard  # noqa: E402

ORDER = ['rtl', 'jump', 'cse', 'addressof', 'gcse', 'loop', 'cse2', 'flow', 'combine', 'regmove',
         'sched', 'lreg', 'greg', 'flow2', 'peephole2', 'sched2', 'jump2', 'mach', 'dbr', 'stack']


def blocks(text):
    """Top-level RTL expressions of a dump, in order (insn, jump_insn, call_insn, code_label, note...)."""
    out, depth, start = [], 0, None
    # ';;' comment lines (sched's ready lists and visualization) have unbalanced parentheses.
    text = re.sub(r'^;;.*$', '', text, flags=re.M)
    for i, ch in enumerate(text):
        if ch == '(':
            if depth == 0:
                start = i
            depth += 1
        elif ch == ')' and depth:
            depth -= 1
            if depth == 0:
                out.append(text[start:i + 1])
    return out


def compact(rtl, hardregs):
    """One readable line for an insn: '23 set r120 <- mem(r116+0x450)'."""
    # '/i' (from an inlined function), '/s', '/u'... flags follow the kind: (insn/i 138 ...)
    m = re.match(r'\((\w+)(?:/\w+)?(?::\w+)? (\d+)', rtl)
    if not m:
        return None
    kind, uid = m.group(1), m.group(2)
    if kind == 'code_label':
        return 'L%s:' % uid
    if kind == 'note':
        n = re.search(r'NOTE_INSN_(\w+)', rtl)
        return '  -- %s' % n.group(1) if n and n.group(1).startswith(('LOOP', 'BLOCK_BEG')) and 'LOOP' in n.group(1) else None
    if kind == 'barrier':
        return None
    s = ' '.join(rtl.split())
    s = re.sub(r'\(const_int (-?\d+) \[(0x[0-9a-f]+)\]\)', r'\2', s)
    s = re.sub(r'\(const_int (-?\d+)\)', r'\1', s)
    s = re.sub(r':(SI|DI|SF|DF|QI|HI|TI|CC|CCFP|BLK)\b', '', s)
    s = re.sub(r'/[vsiuf]+\b', '', s)
    s = re.sub(r'\(reg (\d+)\)', lambda r: (hard(r.group(1)) if hardregs or int(r.group(1)) < 64
                                            else 'r' + r.group(1)), s)
    s = re.sub(r'\(reg (\d+) \$?(\w+)\)', lambda r: '$' + r.group(2), s)
    s = re.sub(r' \d+ \{[\w*]+\}', '', s)  # insn code and pattern name
    s = re.sub(r'\(nil\)', '', s)
    s = re.sub(r'\(expr_list:\w+ .*$', '', s)  # REG_NOTES
    s = re.sub(r'\(insn_list.*$', '', s)
    body = re.sub(r'^\(\w+(?::\w+)? \d+ -?\d+ -?\d+ ', '', s)  # flags were stripped above
    body = re.sub(r'\s+-1\s*\)*\s*$', '', body)
    body = re.sub(r'[\s)]+$', lambda q: ')' * max(0, body.count('(') - body.count(')') + q.group(0).count(')')), body).strip()
    tag = {'insn': '', 'jump_insn': 'JUMP ', 'call_insn': 'CALL '}.get(kind, kind + ' ')
    return '%s %s%s' % (uid, tag, body)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('src')
    ap.add_argument('--func')
    ap.add_argument('--flag', action='append', default=[])
    ap.add_argument('--track', action='append', default=[])
    ap.add_argument('--show')
    ap.add_argument('--keep')
    a = ap.parse_args()
    from configure import unit_marker
    m = unit_marker(a.src) if os.path.exists(a.src) else None
    flags = list(toolchain.CFLAGS if a.src.endswith('.c') else toolchain.CXXFLAGS)
    flags += (m[4] if m else []) + a.flag
    if '-sce29' in flags:
        sys.exit('passdump: -sce29 units use another compiler; not supported here')
    tmp = a.keep or tempfile.mkdtemp(prefix='passdump_')
    os.makedirs(tmp, exist_ok=True)
    try:
        base = 'v' + os.path.splitext(a.src)[1]
        shutil.copy(a.src, os.path.join(tmp, base))
        inc = [x for d in ('include', 'src') for x in ('-I', os.path.join(ROOT, d))]
        r = subprocess.run(toolchain.exe(toolchain.CC) + ['-S', base, '-o', 'v.s'] + inc + flags + ['-da'],
                           cwd=tmp, capture_output=True, text=True)
        if r.returncode:
            sys.exit((r.stdout + r.stderr)[-1500:])
        dbase = base if os.path.exists(os.path.join(tmp, base + '.rtl')) else 'v.cc'
        passes = []
        for p in ORDER:
            f = os.path.join(tmp, '%s.%s' % (dbase, p))
            if os.path.exists(f):
                fn = functions(open(f, errors='replace').read())
                name = next((n for n in fn if not a.func or n == a.func or a.func in n), None)
                if name:
                    hardregs = ORDER.index(p) >= ORDER.index('greg')
                    lines = [c for c in (compact(b, hardregs) for b in blocks(fn[name])) if c]
                    passes.append((p, lines))
        if not passes:
            sys.exit('no dumps for %s' % (a.func or 'the function'))
        if a.show:
            for p, lines in passes:
                if p == a.show:
                    print('== %s (%d lines)' % (p, len(lines)))
                    print('\n'.join(lines))
        for pat in a.track:
            rx = re.compile(pat)
            print('== track %r' % pat)
            for p, lines in passes:
                hits = [(i, l) for i, l in enumerate(lines) if rx.search(l)]
                print('  %-9s %s' % (p, '  |  '.join('@%d %s' % (i, l[:90]) for i, l in hits) or '(gone)'))
        if not a.show and not a.track:
            for p, lines in passes:
                print('%-9s %d lines' % (p, len(lines)))
        if a.keep:
            print('dumps kept in %s' % tmp)
    finally:
        if not a.keep:
            shutil.rmtree(tmp, ignore_errors=True)


if __name__ == '__main__':
    main()
