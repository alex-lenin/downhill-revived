#!/usr/bin/env python3
"""Show one function of a built ELF next to retail, instruction by instruction.

Both sides are fully linked images, so every word is compared exactly (no masking):
call targets, %hi/%lo, $gp offsets and jump tables are all real addresses.

Usage: diff_func.py FUNC [--elf build/dhd.elf] [--quiet | --compact]
FUNC is a splat function name (file asm/nonmatchings/text/FUNC.s) or a hex address.
--compact: only the differing words with 2 lines of context (at most 40 of them) and a count of
           kinds; no listing on MATCH. Display only: the comparison and the verdict are the same.
Exit 0 = identical, 1 = differs.
"""
import argparse, os, re, struct, sys
import rabbitizer as rz

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from check_image import read_elf, RETAIL

TEXT_ASM = os.path.join(ROOT, 'asm', 'nonmatchings', 'text')
VRAM = re.compile(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s')
rz.config.regNames_gprAbiNames = rz.Abi.O32


def lookup(func):
    """-> (name, vram, size) from splat's .s (`nonmatching NAME, SIZE` + first insn comment)."""
    if re.fullmatch(r'(0x)?[0-9A-Fa-f]{6,8}', func):
        addr = int(func, 16)
        for f in os.listdir(TEXT_ASM):
            t = open(os.path.join(TEXT_ASM, f)).read()
            m = VRAM.search(t)
            if m and int(m.group(1), 16) == addr:
                func = f[:-2]
                break
        else:
            sys.exit('no function starts at %#x' % addr)
    path = os.path.join(TEXT_ASM, func + '.s')
    if not os.path.exists(path):
        sys.exit('unknown function %s (no %s)' % (func, os.path.relpath(path, ROOT)))
    t = open(path).read()
    size = int(re.search(r'nonmatching\s+\S+,\s*(0x[0-9A-Fa-f]+)', t).group(1), 16)
    return func, int(VRAM.search(t).group(1), 16), size


def words(secs, addr, n):
    base, size, data = secs['.text']
    off = addr - base
    return list(struct.unpack_from('<%dI' % n, data, off))


def dis(w, a):
    return rz.Instruction(w, a, rz.InstrCategory.R5900).disassemble()


def kinds(pairs):
    """count differing words: other instruction / same instruction, other registers / other operands"""
    regs = re.compile(r'\$\w+')
    n = {'instruction': 0, 'registers': 0, 'operands': 0}
    for (x, y, at) in pairs:
        ix = rz.Instruction(x, at, rz.InstrCategory.R5900)
        iy = rz.Instruction(y, at, rz.InstrCategory.R5900)
        if ix.getOpcodeName() != iy.getOpcodeName():
            n['instruction'] += 1
        elif regs.sub('R', ix.disassemble()) == regs.sub('R', iy.disassemble()):
            n['registers'] += 1
        else:
            n['operands'] += 1
    return ', '.join('%s %d' % kv for kv in n.items() if kv[1])


COMPACT = 10  # differing words shown by --compact: workers run it every try, so keep it small


def show(name, addr, r, b, differs, mode, notes=None):
    """Print the listing (mode 'full' or 'compact'; 'quiet' prints nothing). differs[i]: word i
    differs; notes[i]: relocation symbol of an unlinked built word (quick check), or None."""
    n = len(r)
    built = lambda i: dis(b[i], addr + i * 4) + (' [reloc %s]' % notes[i] if notes and notes[i] else '')
    line = lambda i: '%s %08x  %-40s %s' % ('*' if differs[i] else ' ', addr + i * 4,
                                            dis(r[i], addr + i * 4), built(i) if differs[i] else '')
    diff = [i for i in range(n) if differs[i]]
    if mode == 'compact' and diff:
        rows = sorted({j for i in diff[:COMPACT] for j in range(i - 1, i + 2) if 0 <= j < n})
        print('%s @ %#x, %#x bytes   retail | built   (first %d differing words, 1 line of context; '
              '--full for the whole listing)' % (name, addr, n * 4, COMPACT))
        for k, i in enumerate(rows):
            if k and i != rows[k - 1] + 1:
                print('  ...')
            print(line(i))
        if len(diff) > COMPACT:
            print('  ... %d more differing words after %08x' % (len(diff) - COMPACT, addr + diff[COMPACT - 1] * 4))
        print('differing words: ' + kinds([(r[i], b[i], addr + i * 4) for i in diff]))
    elif mode == 'full':
        print('%s @ %#x, %#x bytes   retail | built' % (name, addr, n * 4))
        for i in range(n):
            print(line(i))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func')
    ap.add_argument('--elf', default=os.path.join(ROOT, 'build', 'dhd.elf'))
    ap.add_argument('--quiet', action='store_true')
    ap.add_argument('--compact', action='store_true')
    a = ap.parse_args()
    name, addr, size = lookup(a.func)
    n = size // 4
    r = words(read_elf(RETAIL)[1], addr, n)
    b = words(read_elf(a.elf)[1], addr, n)
    differs = [x != y for x, y in zip(r, b)]
    bad = sum(differs)
    show(name, addr, r, b, differs, 'quiet' if a.quiet else 'compact' if a.compact else 'full')
    print('%s %s: %d of %d words differ' % ('MATCH' if not bad else 'DIFF', name, bad, n))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
