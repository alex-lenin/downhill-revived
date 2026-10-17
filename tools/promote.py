#!/usr/bin/env python3
"""Promote a matched candidate into src/: write src/unsorted/<ADDR>.<ext> with a UNIT marker,
rebuild the real image and keep the file only if the whole image still matches.

Usage: promote.py FUNC CANDIDATE [--data SECTION=A:B ...] [--flag -fX ...] [--commit]
The candidate's own UNIT marker (if any) supplies end/data/flags. With --commit, a matching
promotion is committed (one commit per function).
"""
import argparse, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from diff_func import lookup
from configure import unit_marker, text_functions, TEXT_END, UNIT_MARK


def run(cmd, **kw):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **kw)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func')
    ap.add_argument('cand')
    ap.add_argument('--data', action='append', default=[])
    ap.add_argument('--flag', action='append', default=[])
    ap.add_argument('--commit', action='store_true')
    a = ap.parse_args()
    name, addr, size = lookup(a.func)
    cand = os.path.relpath(os.path.abspath(a.cand), ROOT).replace('\\', '/')
    body = open(os.path.join(ROOT, cand)).read()
    from source_rules import violation
    why = violation(body, name)
    if why:
        sys.exit('NOT PROMOTED: %s' % why)
    m = unit_marker(cand)
    if m:
        addr = m[0]  # a multi-function unit may start before FUNC
        end, data, flags = m[1], ['%s=0x%X:0x%X' % (k, lo, hi) for k, (lo, hi) in m[3].items()], m[4]
        body = body.split('\n', 1)[1] if UNIT_MARK.match(body) else body
    else:
        end = next((v for v, _ in text_functions() if v > addr), TEXT_END)
        data, flags = a.data, a.flag
    # A unit that covers existing units (a TU merge) replaces them; kept only if the image matches.
    from configure import units
    replaced = [u[2] for u in units() if u[0] < end and u[1] > addr]
    for u in units():
        if u[2] in replaced and (u[0] < addr or u[1] > end):
            sys.exit('NOT PROMOTED: %s only partly overlaps the new unit; merge by hand' % u[2])
    backup = {p: open(os.path.join(ROOT, p), newline='').read() for p in replaced}
    # A merge must not turn decompiled C back into asm: every function that a replaced unit
    # had in C must be in C in the new unit too (the image check can't see this; asm matches).
    asm = lambda text: set(re.findall(r'INCLUDE_ASM\([^,]+,\s*(\w+)\)', text))
    starts = [(v, os.path.splitext(n)[0]) for v, n in text_functions()]
    lost = []
    for u in units():
        if u[2] in replaced:
            in_c = {n for v, n in starts if u[0] <= v < u[1]} - asm(backup[u[2]])
            lost += sorted(in_c & asm(body))
    if lost:
        sys.exit('NOT PROMOTED: the new unit has INCLUDE_ASM for functions already decompiled in '
                 'the units it replaces: %s. Copy their C into the candidate.' % ' '.join(lost))
    for p in replaced:
        os.remove(os.path.join(ROOT, p))
    ext = os.path.splitext(cand)[1]
    dst = 'src/unsorted/%08X%s' % (addr, ext)
    if os.path.exists(os.path.join(ROOT, dst)):
        sys.exit('%s exists: merge by hand' % dst)
    marker = ' '.join(['0x%08X 0x%08X' % (addr, end)] + data + flags)
    with open(os.path.join(ROOT, dst), 'w', newline='\n') as f:
        f.write('/* UNIT %s */\n%s' % (marker, body.lstrip('\n')))
    r = run([sys.executable, 'tools/configure.py'])
    ok = r.returncode == 0
    if ok:
        r = run([sys.executable, '-m', 'ninja'])
        ok = r.returncode == 0  # the default target is check.ok: ninja fails unless MATCH
    if not ok:
        os.remove(os.path.join(ROOT, dst))
        for p, text in backup.items():
            with open(os.path.join(ROOT, p), 'w', newline='') as f:
                f.write(text)
        run([sys.executable, 'tools/configure.py'])
        out = '\n'.join(l for l in (r.stdout + r.stderr).split('\n')
                        if 'Warning: .end' not in l and 'macro invoked' not in l)
        print(out[-3000:])
        sys.exit('NOT PROMOTED: the image does not match with %s' % dst)
    print('promoted %s -> %s (image MATCH)' % (name, dst))
    if replaced:
        print('  replaced (TU merge): %s' % ' '.join(replaced))
    if a.commit:
        run(['git', 'add', '-A', dst] + replaced)
        msg = 'Decompile %s%s' % (
            name, ('\n\nMerged into one unit with ' + ', '.join(replaced) + ' (shared TU).') if replaced else '')
        r = run(['git', 'commit', '-q', '-m', msg])
        print(r.stdout + r.stderr or 'committed')


if __name__ == '__main__':
    import srclock
    with srclock.exclusive():  # trial builds (try_func) wait meanwhile
        main()
