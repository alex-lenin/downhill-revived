"""Synthetic GS packets for native/PCSX2 software renderer parity.

python port/tools/gs_fixture.py [--check output-image]
Creates build/port/gs-fixture/mipmap.gs and expected pixel values. No game data.
Replay with replay_gs.py or PCSX2's software renderer with SaveFrame enabled.
--check accepts its framebuffer PNG or the native replay PPM (requires Pillow).
"""
import argparse
import json
from pathlib import Path
import struct

OUT = Path(__file__).resolve().parents[2] / 'build/port/gs-fixture'


def make_dump(env, context, writes, memory=None, height=64):
    freeze = bytearray(struct.pack('<I15Q24Q', 8, *env, *(context * 2)))
    freeze += bytes(32 + 8 + 8)  # GSVertex fields, obsolete register, transfer X/Y.
    if memory is None:
        memory = bytes(4 * 1024 * 1024)
    freeze += memory + bytes(4 * 20) + struct.pack('<f', 1)
    assert len(freeze) == 4194752
    priv = bytearray(8192)
    for reg, value in [(0, 2), (0x10, 2 << 13), (0x90, 1 << 9), (0xA0, 63 << 32 | (height-1) << 44)]:
        struct.pack_into('<Q', priv, reg, value)
    data = bytearray(struct.pack('<II', 0, len(freeze))) + freeze + priv
    packet = struct.pack('<4I', 0x8000 | len(writes), 1 << 28, 14, 0)
    packet += b''.join(struct.pack('<QQ', value, reg) for reg, value in writes)
    data += bytes([0, 3]) + struct.pack('<I', len(packet)) + packet
    for field in range(4):
        data += bytes([3]) + priv + bytes([1, field & 1])
    return data


def mipmap():
    tex = 1024 | 2 << 14 | 6 << 26 | 6 << 30 | 1 << 34 | 1 << 35
    mip = [0, 0]
    for level in range(1, 7):
        mip[(level - 1) // 3] |= (1024 + level * 256 | 2 << 14) << ((level - 1) % 3 * 20)
    env = [0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0]
    context = [0, tex, 0, 5, *mip, 63 << 16 | 63 << 48, 0, 0, 0, 1 << 16, 48 << 24 | 1 << 32]
    memory = bytearray(4 * 1024 * 1024)
    for level in range(7):
        offset = (1024 + level * 256) * 256
        memory[offset:offset + 8192] = struct.pack('<I', 0x80000000 | level * 32) * 2048
    writes, expected = [], []

    def point(x, y, tex1, q, color):
        qb = struct.unpack('<I', struct.pack('<f', q))[0]
        st = struct.unpack('<I', struct.pack('<f', q / 64))[0]
        writes.extend([(20, tex1), (0, 16), (1, 0x80808080 | qb << 32),
                       (2, st | st << 32), (5, x * 16 | y * 16 << 16)])
        expected.append([x, y, [color, 0, 0]])

    for mode in range(6):
        for level in range(7):
            point(level, mode, 1 | 6 << 2 | mode << 6 | level * 16 << 32, 1,
                  0 if mode < 2 else level * 32)
    for mode in range(2, 6):
        point(mode, 6, 1 | 6 << 2 | mode << 6 | 40 << 32, 1, 80 if mode & 1 else 96)
    for l in range(3):
        point(l, 7, 6 << 2 | 2 << 6 | l << 19 | 16 << 32, .5, ((1 << l) + 1) * 32)
    return make_dump(env, context, writes, memory), expected


def depth():
    env = [0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0]
    context = [0, 0, 0, 5, 0, 0, 63 << 16 | 127 << 48, 0, 0, 0, 1 << 16, 48 << 24]
    writes, expected = [], []
    for fi, psm in enumerate((48, 49, 50, 58)):
        for bi, base in enumerate((0xFFFFFF00, 0xFFFFFFF0, 0x7FFFFF00, 0x00FFFFF0)):
            mask = 0xFFFFFFFF if psm == 48 else 0xFFFFFF if psm == 49 else 0xFFFF;
            if bi < 3:
                base &= mask;
            writes.extend([(76, 16 | 1 << 16), (78, 32 | (psm & 15) << 24),
                           (71, 1 << 16 | 1 << 17), (0, 6), (5, 0), (5, 16*16 | 16*16 << 16)])
            # GEQUAL also asks PCSX2's trace to recheck integer Z equality;
            # its ALWAYS fast path can mistake large, close Z values for equal.
            writes.extend([(76, 16 | 1 << 16), (78, 32 | (psm & 15) << 24), (71, 1 << 16 | 2 << 17), (0, 3), (1, 0x80123456)])
            for x, y, z in ((0, 0, base), (16, 0, base+8), (0, 16, base+12)):
                writes.append((5, x*16 | y*16 << 16 | z << 32))
            # Sample the actual Z surface as a depth-format texture. RGB exposes
            # the low 24 depth bits without depending on a screenshot alpha byte.
            writes.extend([(76, 1 << 16), (78, 32 | (psm & 15) << 24 | 1 << 32), (71, 0),
                           (6, 1024 | 1 << 14 | psm << 20 | 4 << 26 | 4 << 30 | 1 << 34 | 1 << 35),
                           (0, 6 | 16 | 256), (3, 0), (5, bi*16*16 | fi*16*16 << 16),
                           (3, 16*16 | 16*16 << 16), (5, (bi+1)*16*16 | (fi+1)*16*16 << 16)])
            for y in range(1, 7):
                for x in range(1, 7):
                    ox, oy = bi*16+x, fi*16+y
                    z = min(base + (2*x+3*y)//4, mask)
                    if psm in (50, 58):
                        color = ((z & 31) << 3) | ((z & 0x3E0) << 6) | ((z & 0x7C00) << 9)
                    else:
                        color = z & 0xFFFFFF
                    expected.append([ox, oy, [color & 255, color >> 8 & 255, color >> 16 & 255]])
    return make_dump(env, context, writes), expected


def fog():
    fc = [40, 80, 160]
    env = [0, 1, 0, 0, 0, fc[0] | fc[1] << 8 | fc[2] << 16, 0, 0, 1, 0, 0, 3, 0, 0, 0]
    context = [0, 0, 0, 0, 0, 0, 63 << 16 | 63 << 48, 0, 0, 0, 1 << 16, 48 << 24 | 1 << 32]
    writes, expected = [], []
    for group, color in enumerate(([200, 120, 20], fc)):
        writes.extend([(0, 32), (1, color[0] | color[1] << 8 | color[2] << 16 | 128 << 24)])
        for f in range(256):
            x, y = f % 64, group*4 + f//64
            writes.extend([(10, f << 56), (5, x*16 | y*16 << 16)])
            expected.append([x, y, [fc[i] + ((color[i]-fc[i])*f >> 8) for i in range(3)]])
    return make_dump(env, context, writes), expected


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', type=Path)
    parser.add_argument('--kind', choices=('mipmap', 'depth', 'fog'), default='mipmap')
    args = parser.parse_args()
    data, expected = globals()[args.kind]()
    if args.check:
        from PIL import Image
        image = Image.open(args.check).convert('RGB')
        errors = [(x, y, image.getpixel((x, y)), rgb) for x, y, rgb in expected
                  if image.getpixel((x, y)) != tuple(rgb)]
        if errors:
            raise SystemExit('%d differences; first 8: %s' % (len(errors), errors[:8]))
        print('%d GS %s cases: zero differences' % (len(expected), args.kind))
    else:
        OUT.mkdir(parents=True, exist_ok=True)
        (OUT / (args.kind + '.gs')).write_bytes(data)
        (OUT / (args.kind + '.json')).write_text(json.dumps(expected))
        print(OUT / (args.kind + '.gs'))


if __name__ == '__main__':
    main()
