"""VU1 microcode study tool: walk the DMA chains in .vutext (assets/vutext.bin, extracted by splat
from your own ELF and never committed) that upload microprograms with VIF MPG, and disassemble
each package at its VU address. Output: build/port/vu1/<label>.txt.

The instruction tables follow the VU encoding as documented by Sony's VU User's Manual and as
implemented in PCSX2 (pcsx2/DebugTools/DisVUmicro.h); this is an independent implementation.
usage: python port/tools/vudis.py [LABEL_HEX...]   (default: the three packages the game uploads)"""
import os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE = 0x263BC0  # vram of .vutext
PACKAGES = [0x265C00, 0x269C50, 0x26CF80]  # DMA chains referenced by the game (finding 019)
OUT = os.path.join(ROOT, 'build', 'port', 'vu1')

BC = 'xyzw'
UPPER = {}
for i, n in enumerate(['ADD', 'SUB', 'MADD', 'MSUB', 'MAX', 'MINI', 'MUL']):
    for j in range(4):
        UPPER[i * 4 + j] = (n + BC[j], 'bc')
UPPER.update({0x1C: ('MULq', 'q'), 0x1D: ('MAXi', 'i'), 0x1E: ('MULi', 'i'), 0x1F: ('MINIi', 'i'),
              0x20: ('ADDq', 'q'), 0x21: ('MADDq', 'q'), 0x22: ('ADDi', 'i'), 0x23: ('MADDi', 'i'),
              0x24: ('SUBq', 'q'), 0x25: ('MSUBq', 'q'), 0x26: ('SUBi', 'i'), 0x27: ('MSUBi', 'i'),
              0x28: ('ADD', '3'), 0x29: ('MADD', '3'), 0x2A: ('MUL', '3'), 0x2B: ('MAX', '3'),
              0x2C: ('SUB', '3'), 0x2D: ('MSUB', '3'), 0x2E: ('OPMSUB', '3'), 0x2F: ('MINI', '3')})
UPPER_FD = {}  # (bits 0-1, bits 6-10)
for j in range(4):
    for k, n in ((0, 'ADDA'), (1, 'SUBA'), (2, 'MADDA'), (3, 'MSUBA'), (6, 'MULA')):
        UPPER_FD[(j, k)] = (n + BC[j], 'abc')
    UPPER_FD[(j, 4)] = ('ITOF' + ['0', '4', '12', '15'][j], 'cvt')
    UPPER_FD[(j, 5)] = ('FTOI' + ['0', '4', '12', '15'][j], 'cvt')
UPPER_FD.update({(0, 7): ('MULAq', 'aq'), (1, 7): ('ABS', 'cvt'), (2, 7): ('MULAi', 'ai'), (3, 7): ('CLIP', 'clip'),
                 (0, 8): ('ADDAq', 'aq'), (1, 8): ('MADDAq', 'aq'), (2, 8): ('ADDAi', 'ai'), (3, 8): ('MADDAi', 'ai'),
                 (0, 9): ('SUBAq', 'aq'), (1, 9): ('MSUBAq', 'aq'), (2, 9): ('SUBAi', 'ai'), (3, 9): ('MSUBAi', 'ai'),
                 (0, 10): ('ADDA', 'a3'), (1, 10): ('MADDA', 'a3'), (2, 10): ('MULA', 'a3'),
                 (0, 11): ('SUBA', 'a3'), (1, 11): ('MSUBA', 'a3'), (2, 11): ('OPMULA', 'a3'), (3, 11): ('NOP', '')})
LOWER = {0x00: 'LQ', 0x01: 'SQ', 0x04: 'ILW', 0x05: 'ISW', 0x08: 'IADDIU', 0x09: 'ISUBIU',
         0x10: 'FCEQ', 0x11: 'FCSET', 0x12: 'FCAND', 0x13: 'FCOR', 0x14: 'FSEQ', 0x15: 'FSSET', 0x16: 'FSAND',
         0x17: 'FSOR', 0x18: 'FMEQ', 0x1A: 'FMAND', 0x1B: 'FMOR', 0x1C: 'FCGET', 0x20: 'B', 0x21: 'BAL',
         0x24: 'JR', 0x25: 'JALR', 0x28: 'IBEQ', 0x29: 'IBNE', 0x2C: 'IBLTZ', 0x2D: 'IBGTZ', 0x2E: 'IBLEZ',
         0x2F: 'IBGEZ'}
LOWER_OP = {0x30: 'IADD', 0x31: 'ISUB', 0x32: 'IADDI', 0x34: 'IAND', 0x35: 'IOR'}
LOWER_T3 = {(0, 0x0C): 'MOVE', (1, 0x0C): 'MR32', (0, 0x0D): 'LQI', (1, 0x0D): 'SQI', (2, 0x0D): 'LQD',
            (3, 0x0D): 'SQD', (0, 0x0E): 'DIV', (1, 0x0E): 'SQRT', (2, 0x0E): 'RSQRT', (3, 0x0E): 'WAITQ',
            (0, 0x0F): 'MTIR', (1, 0x0F): 'MFIR', (2, 0x0F): 'ILWR', (3, 0x0F): 'ISWR', (0, 0x10): 'RNEXT',
            (1, 0x10): 'RGET', (2, 0x10): 'RINIT', (3, 0x10): 'RXOR', (0, 0x19): 'MFP', (0, 0x1A): 'XTOP',
            (1, 0x1A): 'XITOP', (0, 0x1B): 'XGKICK', (0, 0x1C): 'ESADD', (1, 0x1C): 'ERSADD',
            (2, 0x1C): 'ELENG', (3, 0x1C): 'ERLENG', (0, 0x1D): 'EATANxy', (1, 0x1D): 'EATANxz',
            (2, 0x1D): 'ESUM', (0, 0x1E): 'ESQRT', (1, 0x1E): 'ERSQRT', (2, 0x1E): 'ERCPR', (3, 0x1E): 'WAITP',
            (0, 0x1F): 'ESIN', (1, 0x1F): 'EATAN', (2, 0x1F): 'EEXP'}
BRANCHES = {'B', 'BAL', 'IBEQ', 'IBNE', 'IBLTZ', 'IBGTZ', 'IBLEZ', 'IBGEZ'}


def dest(w):
    return ''.join(c for c, bit in zip('xyzw', (24, 23, 22, 21)) if w >> bit & 1)


def sext(v, bits):
    return v - (1 << bits) if v >> (bits - 1) & 1 else v


def upper(w):
    ft, fs, fd, d = w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, dest(w)
    flags = ''.join(f for f, bit in zip('IEMDT', (31, 30, 29, 28, 27)) if w >> bit & 1)
    op = w & 0x3F
    if op >= 0x3C:
        name, form = UPPER_FD.get((op & 3, fd), ('?u%08X' % w, ''))
    else:
        name, form = UPPER.get(op, ('?u%08X' % w, ''))
    args = {'bc': 'vf%d%s, vf%d%s, vf%d%s' % (fd, d, fs, d, ft, BC[op & 3]),
            'q': 'vf%d%s, vf%d%s, Q' % (fd, d, fs, d), 'i': 'vf%d%s, vf%d%s, I' % (fd, d, fs, d),
            '3': 'vf%d%s, vf%d%s, vf%d%s' % (fd, d, fs, d, ft, d),
            'abc': 'ACC%s, vf%d%s, vf%d%s' % (d, fs, d, ft, BC[op & 3]), 'cvt': 'vf%d%s, vf%d%s' % (ft, d, fs, d),
            'aq': 'ACC%s, vf%d%s, Q' % (d, fs, d), 'ai': 'ACC%s, vf%d%s, I' % (d, fs, d),
            'clip': 'vf%dxyz, vf%dw' % (fs, ft), 'a3': 'ACC%s, vf%d%s, vf%d%s' % (d, fs, d, ft, d), '': ''}[form]
    return ('%s%s %s' % (name, '[%s]' % flags if flags else '', args)).strip(), flags


def lower(w, pc):
    """(text, branch target or None)."""
    op = w >> 25
    it, is_, id_, d = w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, dest(w)
    imm11 = sext(w & 0x7FF, 11)
    fsf, ftf = BC[w >> 21 & 3], BC[w >> 23 & 3]
    if op == 0x40:
        sub = w & 0x3F
        if sub >= 0x3C:
            name = LOWER_T3.get((sub & 3, id_), '?l%08X' % w)
            args = {'MOVE': 'vf%d%s, vf%d%s' % (it, d, is_, d), 'MR32': 'vf%d%s, vf%d%s' % (it, d, is_, d),
                    'LQI': 'vf%d%s, (vi%d++)' % (it, d, is_), 'SQI': 'vf%d%s, (vi%d++)' % (is_, d, it),
                    'LQD': 'vf%d%s, (--vi%d)' % (it, d, is_), 'SQD': 'vf%d%s, (--vi%d)' % (is_, d, it),
                    'DIV': 'Q, vf%d%s, vf%d%s' % (is_, fsf, it, ftf), 'SQRT': 'Q, vf%d%s' % (it, ftf),
                    'RSQRT': 'Q, vf%d%s, vf%d%s' % (is_, fsf, it, ftf), 'MTIR': 'vi%d, vf%d%s' % (it, is_, fsf),
                    'MFIR': 'vf%d%s, vi%d' % (it, d, is_), 'ILWR': 'vi%d%s, (vi%d)' % (it, d, is_),
                    'ISWR': 'vi%d%s, (vi%d)' % (it, d, is_), 'XGKICK': 'vi%d' % is_, 'XTOP': 'vi%d' % it,
                    'XITOP': 'vi%d' % it, 'MFP': 'vf%d%s, P' % (it, d)}.get(name)
            if args is None:
                args = '' if name in ('WAITQ', 'WAITP', 'NOP') else 'P, vf%d%s' % (is_, d or fsf)
            return ('%s %s' % (name, args)).strip(), None
        name = LOWER_OP.get(sub, '?l%08X' % w)
        if name == 'IADDI':
            return 'IADDI vi%d, vi%d, %d' % (it, is_, sext(id_, 5)), None
        return '%s vi%d, vi%d, vi%d' % (name, id_, is_, it), None
    name = LOWER.get(op)
    if name is None:
        return '?l%08X' % w, None
    if name in BRANCHES:
        t = pc + 1 + imm11
        if name == 'B':
            return 'B L%04X' % (t * 8), t
        if name == 'BAL':
            return 'BAL vi%d, L%04X' % (it, t * 8), t
        if name in ('IBEQ', 'IBNE'):
            return '%s vi%d, vi%d, L%04X' % (name, it, is_, t * 8), t
        return '%s vi%d, L%04X' % (name, is_, t * 8), t
    if name == 'LQ':
        return 'LQ vf%d%s, %d(vi%d)' % (it, d, imm11, is_), None
    if name == 'SQ':
        return 'SQ vf%d%s, %d(vi%d)' % (is_, d, imm11, it), None
    if name in ('ILW', 'ISW'):
        return '%s vi%d%s, %d(vi%d)' % (name, it, d, imm11, is_), None
    if name in ('IADDIU', 'ISUBIU'):
        return '%s vi%d, vi%d, 0x%X' % (name, it, is_, (w >> 10 & 0x7800) | (w & 0x7FF)), None
    if name in ('JR', 'JALR'):
        return ('JR vi%d' % is_ if name == 'JR' else 'JALR vi%d, vi%d' % (it, is_)), None
    if name.startswith('FC') or name.startswith('FS') or name.startswith('FM'):
        if name in ('FCAND', 'FCEQ', 'FCOR', 'FCSET'):
            return '%s vi1, 0x%06X' % (name, w & 0xFFFFFF), None
        if name == 'FCGET':
            return 'FCGET vi%d' % it, None
        if name in ('FSAND', 'FSEQ', 'FSOR'):
            return '%s vi%d, 0x%03X' % (name, it, (w >> 10 & 0x800) | (w & 0x7FF)), None
        if name == 'FSSET':
            return 'FSSET 0x%03X' % ((w >> 10 & 0x800) | (w & 0x7FF)), None
        return '%s vi%d, vi%d' % (name, it, is_), None
    return '%s %08X' % (name, w), None


def disassemble(code, base_addr):
    """Lines for a block of microcode loaded at VU address base_addr (bytes)."""
    n = len(code) // 8
    pcs = [base_addr // 8 + i for i in range(n)]
    targets, rows = set(), []
    for k in range(n):
        lo, up = struct.unpack_from('<II', code, k * 8)
        u, flags = upper(up)
        if 'I' in flags:
            l, t = 'LOI %s' % struct.unpack('<f', struct.pack('<I', lo))[0], None
        else:
            l, t = lower(lo, pcs[k])
        if t is not None:
            targets.add(t)
        rows.append((pcs[k], u, l, flags))
    out = []
    for pc, u, l, flags in rows:
        if pc in targets:
            out.append('L%04X:' % (pc * 8))
        mark = '   ; <-- end' if 'E' in flags else ''
        out.append('  %04X  %-44s %s%s' % (pc * 8, u, l, mark))
    return out


VIF = {0x00: 'NOP', 0x01: 'STCYCL', 0x02: 'OFFSET', 0x03: 'BASE', 0x04: 'ITOP', 0x05: 'STMOD', 0x06: 'MSKPATH3',
       0x07: 'MARK', 0x10: 'FLUSHE', 0x11: 'FLUSH', 0x13: 'FLUSHA', 0x14: 'MSCAL', 0x15: 'MSCALF', 0x17: 'MSCNT',
       0x20: 'STMASK', 0x30: 'STROW', 0x31: 'STCOL', 0x4A: 'MPG', 0x50: 'DIRECT', 0x51: 'DIRECTHL'}


def walk(data, label, sources=False):
    """Follows one DMA chain from label; returns (log lines, [(vu addr, microcode bytes)]), with
    sources (vu addr, microcode bytes, PS2 address of those bytes in data)."""
    off, log, mpgs = label - BASE, [], []
    while True:
        tag_lo, tag_hi, v0, v1 = struct.unpack_from('<4I', data, off)
        qwc, tid, addr = tag_lo & 0xFFFF, tag_lo >> 28 & 7, tag_hi
        log.append('%08X dmatag id=%d qwc=0x%X addr=%08X' % (BASE + off, tid, qwc, addr))
        if tid not in (0, 1, 6, 7):  # refe, cnt, ret, end carry their data inline (0 = refe: ref)
            log.append('  (tag id %d not followed)' % tid)
            break
        # the two VIF codes in the tag's upper half, then the payload stream
        stream = data[off + 8:off + 16 + qwc * 16]
        p = 0
        while p + 4 <= len(stream):
            code = struct.unpack_from('<I', stream, p)[0]
            cmd, num, imm = code >> 24 & 0x7F, code >> 16 & 0xFF, code & 0xFFFF
            name = VIF.get(cmd, 'UNPACK' if cmd >= 0x60 else '?%02X' % cmd)
            p += 4
            if name == 'MPG':
                size = (num or 256) * 8
                log.append('  MPG %d instructions -> VU 0x%04X' % (num or 256, imm * 8))
                mpgs.append((imm * 8, stream[p:p + size]) + ((BASE + off + 8 + p,) if sources else ()))
                p += size
            elif name in ('DIRECT', 'DIRECTHL'):
                p = (p + 15) & ~15
                p += (imm or 0x10000) * 16
                log.append('  %s %d qwords' % (name, imm))
            elif name in ('STROW', 'STCOL'):
                p += 16
                log.append('  ' + name)
            elif name == 'STMASK':
                p += 4
                log.append('  STMASK')
            elif name == 'UNPACK':
                log.append('  UNPACK %08X (not followed)' % code)
                break
            elif name != 'NOP':
                log.append('  %s 0x%X' % (name, imm))
        off += 16 + qwc * 16
        if tid in (6, 7, 0):
            break
    return log, mpgs


def main(labels):
    data = open(os.path.join(ROOT, 'assets', 'vutext.bin'), 'rb').read()
    os.makedirs(OUT, exist_ok=True)
    for label in labels:
        log, mpgs = walk(data, label)
        out = ['; VU1 package uploaded by the DMA chain at %08X (.vutext)' % label] + ['; ' + l for l in log]
        for addr, code in mpgs:
            out += ['', '; ---- MPG at VU 0x%04X, %d instructions' % (addr, len(code) // 8)]
            out += disassemble(code, addr)
        path = os.path.join(OUT, '%08X.txt' % label)
        with open(path, 'w', newline='\n') as f:
            f.write('\n'.join(out) + '\n')
        kicks = sum(l.count('XGKICK') for l in out)
        ends = sum(1 for l in out if '<-- end' in l)
        print('%08X: %d MPG blocks, %d instructions, %d XGKICK, %d [E] ends -> %s' % (
            label, len(mpgs), sum(len(c) for _, c in mpgs) // 8, kicks, ends, path))


if __name__ == '__main__':
    main([int(a, 16) for a in sys.argv[1:]] or PACKAGES)
