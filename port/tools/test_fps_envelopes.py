"""Run the actual ported camera envelope units against synthetic timing keys."""
import subprocess, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
out=ROOT/'build/port/test-fps-envelopes'
subprocess.run([sys.executable,str(ROOT/'port/tools/build.py'),'--out',str(out),
    str(ROOT/'src/unsorted/00239778.c'),str(ROOT/'src/unsorted/002398C0.c')],check=True)
subprocess.run(['clang','--target=i686-pc-windows-msvc','-O2','-msse2','-mfpmath=sse',
    '-ffp-contract=off',str(ROOT/'port/hle/test_fps_envelopes.c'),
    str(out/'obj/unsorted/00239778.c.o'),str(out/'obj/unsorted/002398C0.c.o'),
    '-o',str(out/'test.exe')],check=True)
subprocess.run([str(out/'test.exe')],check=True)
