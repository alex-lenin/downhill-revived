#!/usr/bin/env python3
"""Quick pre-check of one function from a compiled object, without linking the image.

Compares the function's bytes in an unlinked .o with retail, ignoring only the fields the
linker fills in (the bits covered by a relocation: jal targets, %hi/%lo, $gp offsets).
Every other bit is final in the object, so a difference there is a difference in the linked
image too: the quick check can prove a DIFF, never a MATCH. tools/try_func.py uses it to skip
the full build while the function still differs; MATCH always comes from the full build and
tools/check_image.py.

Usage: quick_check.py FUNC OBJ.o     (exit 0 = no difference outside relocated fields,
                                      1 = differs, 3 = can't tell, e.g. FUNC not in OBJ)
"""
import os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from check_image import read_elf, RETAIL

# MIPS relocation types -> bits of the word the linker writes
MASK = {4: 0x03FFFFFF,   # R_MIPS_26: jal/j target
        5: 0x0000FFFF,   # R_MIPS_HI16
        6: 0x0000FFFF,   # R_MIPS_LO16
        7: 0x0000FFFF}   # R_MIPS_GPREL16
# anything else: the whole word (more lenient, never stricter)


def function_in_object(path, func):
    """-> (words, masks, reloc symbol per word, compiled size) for func, or None"""
    d = open(path, 'rb').read()
    if d[:4] != b'\x7fELF' or d[4] != 1:
        return None
    shoff, = struct.unpack_from('<I', d, 32)
    shentsize, shnum, _ = struct.unpack_from('<HHH', d, 46)
    S = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    symtab = next((i for i, s in enumerate(S) if s[1] == 2), None)
    if symtab is None:
        return None
    st, strs = S[symtab], S[S[symtab][6]]
    name = lambda o: d[strs[4] + o:d.index(b'\0', strs[4] + o)].decode()
    syms = []
    for k in range(st[5] // 16):
        nm, value, size, info, _, shndx = struct.unpack_from('<IIIBBH', d, st[4] + k * 16)
        syms.append((name(nm), value, size, info & 0xF, shndx))
    # C++ methods are mangled: func_0011D148__1A
    hits = [s for s in syms if s[3] == 2 and (s[0] == func or s[0].startswith(func + '__'))]
    if len(hits) != 1:
        return None
    _, value, size, _, shndx = hits[0]
    sec = S[shndx]
    data = d[sec[4]:sec[4] + sec[5]]
    relocs = {}
    for s in S:
        if s[1] in (9, 4) and s[7] == shndx:  # SHT_REL / SHT_RELA for this section
            ent = 8 if s[1] == 9 else 12
            for k in range(s[5] // ent):
                off, info = struct.unpack_from('<II', d, s[4] + k * ent)
                relocs[off] = (MASK.get(info & 0xFF, 0xFFFFFFFF) | relocs.get(off, (0,))[0],
                               syms[info >> 8][0] if (info >> 8) < len(syms) else '?')
    return data, value, size, relocs


def compare(func, addr, size, obj):
    """-> (retail words, built words, differs per word, note per word, compiled size) or None"""
    got = function_in_object(obj, func)
    if got is None:
        return None
    data, value, csize, relocs = got
    n = size // 4
    base, _, text = read_elf(RETAIL)[1]['.text']
    r = list(struct.unpack_from('<%dI' % n, text, addr - base))
    raw = data[value:value + size].ljust(size, b'\0')  # past the section: ld's zero fill
    b = list(struct.unpack_from('<%dI' % n, raw))
    differs, notes = [], []
    for i in range(n):
        m, sym = relocs.get(value + i * 4, (0, None))
        differs.append((r[i] & ~m) != (b[i] & ~m))
        notes.append(sym)
    return r, b, differs, notes, csize


def main():
    func, obj = sys.argv[1:3]
    from diff_func import lookup
    name, addr, size = lookup(func)
    c = compare(name, addr, size, obj)
    if c is None:
        print('UNKNOWN %s: no single function symbol for it in %s' % (name, obj))
        sys.exit(3)
    r, b, differs, _, csize = c
    bad = sum(differs) + (csize > size)
    print('%s %s: %d of %d words differ outside relocated fields; compiled %#x bytes, retail %#x'
          % ('PASS' if not bad else 'DIFF', name, sum(differs), len(r), csize, size))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
