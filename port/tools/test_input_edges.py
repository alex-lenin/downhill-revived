"""Compile the production input cache and reader; require one edge per press."""
import subprocess, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
out=ROOT/'build/port/test-input-edges';out.mkdir(parents=True,exist_ok=True)
subprocess.run([sys.executable,str(ROOT/'port/tools/build.py'),'--out',str(out),
    'src/unsorted/001909A8.c','src/unsorted/00220640.c','src/unsorted/0017EB00.c'],cwd=ROOT,check=True)
subprocess.run(['clang','--target=i686-pc-windows-msvc','-O2','-w','-ffunction-sections',
    '-I',str(ROOT/'port/runtime'),str(ROOT/'port/hle/pad.c'),str(ROOT/'port/hle/test_input_edges.c'),
    str(out/'obj/unsorted/001909A8.c.o'),str(out/'obj/unsorted/00220640.c.o'),str(out/'obj/unsorted/0017EB00.c.o'),
    '-Wl,/opt:ref','-o',str(out/'test.exe')],check=True)
for rate in (60,61,75,90,100,120,144):
    subprocess.run([str(out/'test.exe'),str(rate)],check=True)
    if rate>60:
        subprocess.run([str(out/'test.exe'),str(rate),'variable'],check=True)
