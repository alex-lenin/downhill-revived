#!/usr/bin/env python3
"""Disassemble retail functions. Function bounds are a heuristic: a function ends after
`jr $ra` + delay slot (with no later branch back into it); alignment nops are skipped.

Usage: disfn.py ADDR            disassemble the function at ADDR
       disfn.py --list [N]      list the first N functions with size and a one-line summary
"""
import os, sys, struct
import rabbitizer as rz
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from grade import ELF, sections

rz.config.regNames_gprAbiNames = rz.Abi.O32


def text():
    d = open(ELF, 'rb').read()
    t = [s for s in sections(d)[0] if s[0] == '.text'][0]
    base, off, size = t[4], t[5], t[6]
    return base, list(struct.unpack_from('<%dI' % (size // 4), d, off))


def functions():
    base, w = text()
    i, n = 0, len(w)
    while i < n:
        while i < n and w[i] == 0:
            i += 1
        start, far = i, i
        while i < n:
            ins = rz.Instruction(w[i], base + i * 4, rz.InstrCategory.R5900)
            if ins.isBranch() and not ins.isJump():
                far = max(far, (ins.getBranchOffset() + base + i * 4 - base) // 4)
            if w[i] == 0x03e00008 and i >= far:
                i += 2
                break
            i += 1
        yield base + start * 4, base + i * 4, w[start:i]


def show(addr):
    for s, e, ws in functions():
        if s == addr:
            for k, x in enumerate(ws):
                print('%08x %08x  %s' % (s + k * 4, x, rz.Instruction(x, s + k * 4, rz.InstrCategory.R5900).disassemble()))
            print('size %#x' % (e - s))
            return
    sys.exit('no function starts at %#x' % addr)


if __name__ == '__main__':
    if sys.argv[1] == '--list':
        n = int(sys.argv[2]) if len(sys.argv) > 2 else 50
        for k, (s, e, ws) in enumerate(functions()):
            if k >= n:
                break
            ops = ' '.join(rz.Instruction(x, s, rz.InstrCategory.R5900).getOpcodeName() for x in ws[:12])
            print('%08x %4x  %s' % (s, e - s, ops))
    else:
        show(int(sys.argv[1], 16))
