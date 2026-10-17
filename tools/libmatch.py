#!/usr/bin/env python3
"""Find library functions in retail .text by matching archive members' code.

For every function symbol in every member of the given .a files, its bytes are compared with
retail .text at every 8-aligned address, ignoring relocated fields (R_MIPS_26 target,
HI16/LO16/GPREL16 immediates). Unique, exact (masked) matches of >= MIN_WORDS words are
reported as `ADDR NAME ARCHIVE:MEMBER`. Short functions match by coincidence, so they are
only accepted when the whole member matches contiguously.

Usage: libmatch.py LIB.a [LIB.a ...] [--min 6] [--out config/libmatch.txt]
"""
import argparse, os, struct, sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from check_image import read_elf, RETAIL

R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_GPREL16, R_MIPS_LITERAL = 4, 5, 6, 7, 8
MASK = {R_MIPS_26: 0xFC000000, R_MIPS_HI16: 0xFFFF0000, R_MIPS_LO16: 0xFFFF0000,
        R_MIPS_GPREL16: 0xFFFF0000, R_MIPS_LITERAL: 0xFFFF0000}


def ar_members(path):
    d = open(path, 'rb').read()
    assert d[:8] == b'!<arch>\n', path
    i, names = 8, b''
    while i + 60 <= len(d):
        hdr = d[i:i + 60]
        name = hdr[:16].decode().strip()
        size = int(hdr[48:58].decode().strip())
        body = d[i + 60:i + 60 + size]
        if name == '//':
            names = body
        elif name.startswith('/') and name[1:].strip().isdigit():
            off = int(name[1:])
            yield names[off:names.index(b'/', off)].decode(), body
        elif name not in ('/', '/SYM64/'):
            yield name.rstrip('/'), body
        i += 60 + size + (size & 1)


def obj_functions(d):
    """[(name, words, masks)] for each function symbol in an ELF32 MIPS .o's .text."""
    if d[:4] != b'\x7fELF':
        return []
    shoff, = struct.unpack_from('<I', d, 32)
    shentsize, shnum, shstr = struct.unpack_from('<HHH', d, 46)
    S = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    so = S[shstr][4]
    nm = lambda o, base: d[base + o:d.index(b'\0', base + o)].decode('latin1')
    secname = [nm(s[0], so) for s in S]
    out = []
    for si, s in enumerate(S):
        if s[1] != 2:  # SHT_SYMTAB
            continue
        strtab = S[s[6]][4]
        syms = [struct.unpack_from('<IIIBBH', d, s[4] + k * 16) for k in range(s[5] // 16)]
        for k, (st_name, value, size, info, other, shndx) in enumerate(syms):
            if info & 0xF != 2 or not size or shndx >= len(S) or secname[shndx] != '.text':
                continue  # STT_FUNC in .text only
            text = S[shndx]
            w = list(struct.unpack_from('<%dI' % (size // 4), d, text[4] + value))
            m = [0xFFFFFFFF] * len(w)
            for rs in S:
                if rs[1] == 9 and rs[7] == shndx:  # SHT_REL for .text
                    for r in range(rs[5] // 8):
                        off, rinfo = struct.unpack_from('<II', d, rs[4] + r * 8)
                        j = (off - value) // 4
                        if 0 <= j < len(w):
                            m[j] &= ~MASK.get(rinfo & 0xFF, 0) & 0xFFFFFFFF if rinfo & 0xFF in MASK else 0
            out.append((nm(st_name, strtab), w, m))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('libs', nargs='+')
    ap.add_argument('--min', type=int, default=6)
    ap.add_argument('--out')
    a = ap.parse_args()
    _, secs = read_elf(RETAIL)
    base, size, data = secs['.text']
    text = list(struct.unpack_from('<%dI' % (size // 4), data, 0))
    index = defaultdict(list)  # first word (masked) -> positions, built lazily per mask
    found = []
    for lib in a.libs:
        for member, body in ar_members(lib):
            for name, w, m in obj_functions(body):
                if len(w) < a.min:
                    continue
                # anchor on the first word with a full mask
                k = next((i for i, x in enumerate(m) if x == 0xFFFFFFFF), None)
                if k is None:
                    continue
                hits = []
                key = w[k]
                if key not in index:
                    index[key] = [i for i, x in enumerate(text) if x == key]
                for p in index[key]:
                    s = p - k
                    if s < 0 or s % 2 or s + len(w) > len(text):
                        continue
                    if all((text[s + i] ^ w[i]) & m[i] == 0 for i in range(len(w))):
                        hits.append(base + s * 4)
                if len(hits) == 1:
                    found.append((hits[0], name, '%s:%s' % (os.path.basename(lib), member), len(w)))
    found.sort()
    lines = ['%08X %-40s %s %d' % f for f in found]
    print('\n'.join(lines))
    print('%d functions identified' % len(found), file=sys.stderr)
    if a.out:
        with open(a.out, 'w', newline='\n') as f:
            f.write('# ADDR NAME ARCHIVE:MEMBER WORDS   (tools/libmatch.py)\n' + '\n'.join(lines) + '\n')


if __name__ == '__main__':
    main()
