"""Strict 32-bit metadata and ordered full-GS-state differential, including corruption control."""
import subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def main():
    out=ROOT/'build/port/native-frontend-test';out.mkdir(parents=True,exist_ok=True)
    exe=out/'test.exe'
    subprocess.run(['clang-cl','-m32','/nologo','/O2','-msse2','-mno-fma','/clang:-ffp-contract=off',
        '-Wno-deprecated-declarations',str(ROOT/'port/gs/test_native_frontend.c'),
        str(ROOT/'port/gs/gs.c'),str(ROOT/'port/gs/memory.c'),'/Fe'+str(exe)],cwd=out,check=True)
    subprocess.run([str(exe)],check=True,timeout=60)
    if subprocess.run([str(exe),'negative'],timeout=60).returncode != 1:
        raise RuntimeError('corruption control did not fail')
    print('negative control failed as required')
if __name__=='__main__': main()
