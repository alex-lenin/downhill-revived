#!/usr/bin/env python3
"""Decompilation progress: .text functions and bytes built from C/C++ vs still asm.

A function counts as decompiled when it lies in a config/units.txt unit and its source does
not INCLUDE_ASM it. That only means something because the build's check_image passes, so
this refuses to report while build/check.ok is missing or older than the inputs.

Usage: progress.py [--list]
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import text_functions, units, TEXT_END, TEXT_ASM


def main():
    ok = os.path.join(ROOT, 'build', 'check.ok')
    elf = os.path.join(ROOT, 'build', 'dhd.elf')
    if not os.path.exists(ok) or os.path.getmtime(ok) < os.path.getmtime(elf):
        sys.exit('build/check.ok missing or stale: run `python -m ninja` first (the image must MATCH)')
    funcs = text_functions()
    bounds = [v for v, _ in funcs] + [TEXT_END]
    size = {v: bounds[i + 1] - v for i, (v, _) in enumerate(funcs)}
    us = units()
    done = []
    for s, e, src, _, _ in us:
        text = open(os.path.join(ROOT, src)).read()
        stubs = set(re.findall(r'INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)', text))
        for v, f in funcs:
            if s <= v < e and f[:-2] not in stubs:
                done.append((v, f[:-2]))
    total_b = TEXT_END - 0x10A000
    done_b = sum(size[v] for v, _ in done)
    print('functions: %d / %d (%.2f%%)' % (len(done), len(funcs), 100.0 * len(done) / len(funcs)))
    print('.text bytes: %#x / %#x (%.3f%%)' % (done_b, total_b, 100.0 * done_b / total_b))
    print('units: %d' % len(us))
    if '--list' in sys.argv:
        for v, name in done:
            print('  %08x %s' % (v, name))


if __name__ == '__main__':
    main()
