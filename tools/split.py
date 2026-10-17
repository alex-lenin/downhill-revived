#!/usr/bin/env python3
"""Regenerate asm/ and assets/ from the retail ELF with splat, from scratch.

splat never deletes files, so renaming a symbol would leave the old .s behind; this wipes the
generated trees first. Run after editing config/splat.yaml or config/symbol_addrs.txt, then
`python tools/configure.py && python -m ninja`.
"""
import os, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
for d in ('asm', 'assets'):
    shutil.rmtree(os.path.join(ROOT, d), ignore_errors=True)
sys.exit(subprocess.call([sys.executable, '-m', 'splat', 'split', 'config/splat.yaml'], cwd=ROOT))
