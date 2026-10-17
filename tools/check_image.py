#!/usr/bin/env python3
"""Compare a built ELF with retail SCUS_971.77: the only authority on "matching".

Checks the entry point and, for every retail section that has an address (.text, .vutext,
.data, .rodata, .gcc_except_table, .sdata, .sbss, .bss): address, size and (for PROGBITS)
every byte. Exit 0 only if all are identical. On a byte difference it names the retail
function (from asm/nonmatchings/text) or data symbol containing it.

Not checked (by design, see docs/findings/003): ELF/program headers, section order in the
file, .reginfo, DVP overlay sections, symbol tables — they don't affect the loaded program.

Usage: check_image.py [--stamp FILE] BUILT.elf [RETAIL.elf]   (--stamp: write FILE on match)
"""
import os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RETAIL = os.path.join(ROOT, 'iso', 'SCUS_971.77')
CHECKED = ['.text', '.vutext', '.data', '.rodata', '.gcc_except_table', '.sdata', '.sbss', '.bss']


def read_elf(path):
    d = open(path, 'rb').read()
    entry, = struct.unpack_from('<I', d, 24)
    shoff, = struct.unpack_from('<I', d, 32)
    shentsize, shnum, shstr = struct.unpack_from('<HHH', d, 46)
    S = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    so = S[shstr][4]
    secs = {}
    for s in S:
        name = d[so + s[0]:d.index(b'\0', so + s[0])].decode()
        data = None if s[1] == 8 else d[s[4]:s[4] + s[5]]  # SHT_NOBITS
        secs[name] = (s[3], s[5], data)
    return entry, secs


def owner(addr):
    """Name of the retail function containing addr (best effort)."""
    best = None
    d = os.path.join(ROOT, 'asm', 'nonmatchings', 'text')
    if os.path.isdir(d):
        for f in os.listdir(d):
            m = re.match(r'func_([0-9A-F]{8})\.s$', f)
            if m and int(m.group(1), 16) <= addr and (best is None or int(m.group(1), 16) > best[0]):
                best = (int(m.group(1), 16), f[:-2])
    return '%s+0x%X' % (best[1], addr - best[0]) if best else '?'


def main():
    args = sys.argv[1:]
    stamp = None
    if args[:1] == ['--stamp']:
        stamp, args = args[1], args[2:]
    built = args[0]
    retail = args[1] if len(args) > 1 else RETAIL
    re_, rs = read_elf(retail)
    be, bs = read_elf(built)
    bad = 0
    if re_ != be:
        print('entry point %#x != retail %#x' % (be, re_)); bad += 1
    for name in CHECKED:
        ra, rsz, rd = rs[name]
        if name not in bs:
            print('%-18s missing' % name); bad += 1; continue
        ba, bsz, bd = bs[name]
        msg = []
        if ba != ra:
            msg.append('address %#x != %#x' % (ba, ra))
        if bsz != rsz:
            msg.append('size %#x != %#x' % (bsz, rsz))
        if rd is not None:
            if bd is None:
                msg.append('is NOBITS')
            else:
                diffs = [i for i in range(0, min(len(rd), len(bd)), 4) if rd[i:i + 4] != bd[i:i + 4]]
                if diffs:
                    first = ra + diffs[0]
                    where = owner(first) if name == '.text' else '%s+0x%X' % (name, diffs[0])
                    msg.append('%d words differ, first at %#x (%s)' % (len(diffs), first, where))
        if msg:
            bad += 1
            print('%-18s DIFF: %s' % (name, '; '.join(msg)))
        else:
            print('%-18s ok   %#010x %#x' % (name, ra, rsz))
    print('MATCH' if not bad else 'NOT MATCHING (%d problems)' % bad)
    if stamp and not bad:
        open(stamp, 'w').write('ok\n')
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
