"""Aligned diff of a candidate against retail, for when the sizes differ (try_func's listing
compares word N with word N, so one extra instruction shifts everything after it).

Compiles SRC alone (tools/sncc.py), aligns the two instruction sequences on mnemonics
(difflib) and prints only the differences with 2 lines of context:
  -  retail only      +  built only      ~  same mnemonic, other operands
Immediates under a relocation in the built object show as 0; those lines are not marked.
Display only: tools/try_func.py stays the verdict.

usage: python tools/adiff.py FUNC SRC [--flag F]... [--all]   (a UNIT marker's flags are added)
"""
import argparse, difflib, os, struct, subprocess, sys
sys.path = [p for p in sys.path if os.path.abspath(p or '.') != os.path.dirname(os.path.abspath(__file__))]
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
import rabbitizer as rz  # noqa: E402
from quick_check import function_in_object  # noqa: E402
from diff_func import lookup  # noqa: E402
from check_image import read_elf, RETAIL  # noqa: E402

rz.config.regNames_gprAbiNames = rz.Abi.O32
ap = argparse.ArgumentParser()
ap.add_argument('func')
ap.add_argument('src')
ap.add_argument('--flag', action='append', default=[])
ap.add_argument('--all', action='store_true', help='print every line, not just differences')
ap.add_argument('--side', choices=('retail', 'built'), help='print one listing alone, with word indices')
a = ap.parse_args()

name, addr, size = lookup(a.func)
obj = 'build-try/adiff/%s.o' % name
os.makedirs('build-try/adiff', exist_ok=True)
from configure import unit_marker  # noqa: E402
marker = unit_marker(a.src) if os.path.exists(a.src) else None
flags = (marker[4] if marker else []) + a.flag  # a UNIT marker's flags, as try_func uses them
r = subprocess.run([sys.executable, 'tools/sncc.py', a.src, obj] + flags, capture_output=True, text=True)
if r.returncode:
    sys.exit((r.stdout + r.stderr)[-3000:])
got = function_in_object(obj, name)
if not got:
    sys.exit('%s not found in the object' % name)
data, value, csize, relocs = got
base, _, text = read_elf(RETAIL)[1]['.text']
rw = struct.unpack_from('<%dI' % (size // 4), text, addr - base)
bw = struct.unpack_from('<%dI' % (csize // 4), data, value)
ins = lambda w, at: rz.Instruction(w, at, rz.InstrCategory.R5900)
R = [ins(w, addr + 4 * i) for i, w in enumerate(rw)]
B = [ins(w, addr + 4 * i) for i, w in enumerate(bw)]
rel = lambda j: (value + 4 * j) in relocs
if a.side:
    for i, x in enumerate(R if a.side == 'retail' else B):
        print('%4d %s%s' % (i, x.disassemble(), '  # reloc' if a.side == 'built' and rel(i) else ''))
    sys.exit()
rows = []  # (mark, retail text, built text)
sm = difflib.SequenceMatcher(None, [x.getOpcodeName() for x in R], [x.getOpcodeName() for x in B], autojunk=False)
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == 'equal':
        for i, j in zip(range(i1, i2), range(j1, j2)):
            same = rw[i] == bw[j] or rel(j)
            rows.append((' ' if same else '~', R[i].disassemble(), '' if same else B[j].disassemble()))
    else:
        for k in range(max(i2 - i1, j2 - j1)):
            i, j = i1 + k, j1 + k
            rows.append(('-' if j >= j2 else '+' if i >= i2 else '~',
                         R[i].disassemble() if i < i2 else '', B[j].disassemble() if j < j2 else ''))
show = set(range(len(rows))) if a.all else {k for n, row in enumerate(rows) if row[0] != ' '
                                             for k in range(n - 2, n + 3)}
last = -1
for n, (m, x, y) in enumerate(rows):
    if n in show:
        if last >= 0 and n != last + 1:
            print('  ...')
        print('%s %-40s %s' % (m, x, y))
        last = n
bad = sum(1 for row in rows if row[0] != ' ')
print('%s: %d aligned rows differ; retail %#x bytes, built %#x' % (name, bad, size, csize))
