"""Compile/run production clock invariance checks (Windows, LLVM)."""
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[2]
out=ROOT/'build/port/test-fps'
out.mkdir(parents=True,exist_ok=True)
subprocess.run(['clang','--target=i686-pc-windows-msvc','-O2','-w','-msse2','-mfpmath=sse',
    '-ffp-contract=off','-I',str(ROOT/'port/runtime'),str(ROOT/'port/hle/fps.c'),
    str(ROOT/'port/hle/test_fps.c'),'-o',str(out/'test.exe'),'-luser32'],check=True)
for rate in (60,61,75,90,100,120,144):
    subprocess.run([str(out/'test.exe'),str(rate)],check=True)
    if rate>60:
        subprocess.run([str(out/'test.exe'),str(rate),'wall'],check=True)
