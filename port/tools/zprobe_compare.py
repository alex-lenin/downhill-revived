"""Compares the lens flare's Z probe answers of two runs (DHD_GS_READBACK_TRACE files): the exact
probe (DHD_EXACT_PROBE=1) against native depth (finding 097).

    python port/tools/zprobe_compare.py BEFORE.bin AFTER.bin

Each Z read (PSMZ*) is a small patch the game compares against the flare's depth: pixels nearer
than the flare hide it. Here each pixel is "open" (the patch's farthest value class: the far
plane, nothing drawn) or "covered" (scenery wrote a nearer Z), and the runs are compared field
by field on the number of covered pixels, the native answer allowed to be up to two fields late.
Prints how many probes agree, and the fields where the two disagree on whether the flare is
hidden (all covered) or shown (none covered), for screenshots.
"""
import collections
import struct
import sys


def reads(path):
    """{field: [values]} of the Z reads (the last read in a field wins)."""
    out = {}
    data = open(path, 'rb').read()
    at = 0
    while at + 32 <= len(data):
        field, size = struct.unpack_from('<II', data, at)
        blt, pos, trx = struct.unpack_from('<QQQ', data, at + 8)
        body = data[at + 32:at + 32 + size]
        at += 32 + size
        psm = blt >> 24 & 63
        if psm & 0x30 != 0x30:
            continue
        bits = {0x30: 32, 0x31: 24, 0x32: 16, 0x3A: 16}.get(psm, 32)
        step = bits // 8
        values = [int.from_bytes(body[i:i + step], 'little') for i in range(0, len(body) - step + 1, step)]
        out[field] = values
    return out


def main():
    before, after = reads(sys.argv[1]), reads(sys.argv[2])
    every = [v for vs in before.values() for v in vs]
    far = collections.Counter(every).most_common(1)[0][0] if every else 0
    covered = lambda vs: sum(v != far for v in vs)
    agree = near = total = 0
    flips = []
    for field, vs in sorted(after.items()):
        options = [before[f] for f in (field, field - 1, field - 2) if f in before]
        if not options:
            continue
        total += 1
        mine = covered(vs)
        best = min(abs(covered(o) - mine) for o in options)
        agree += best == 0
        near += best <= 2
        exact = covered(options[0])
        if (mine == 0) != (exact == 0) and best > 2:
            flips.append((field, exact, mine))
    print(f'Z reads: before {len(before)}, after {len(after)}, compared {total}; far value {far:#x}')
    if total:
        print(f'covered pixel count equal {agree} ({100 * agree / total:.1f}%), within 2 of 16: {near} ({100 * near / total:.1f}%)')
    print(f'fields where one shows the flare and the other hides it: {len(flips)}')
    for field, exact, mine in flips[:40]:
        print(f'  field {field}: exact covered {exact}, native covered {mine}')


if __name__ == '__main__':
    main()
