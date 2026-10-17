"""Measure unclipped B0078/B1040 loop feasibility, without speculation or copies."""
import argparse
from pathlib import Path
import subprocess
from vu1_aot import generate

ROOT = Path(__file__).resolve().parents[2]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('capture', type=Path)
    ap.add_argument('--repeats', type=int, default=100000)
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/geometry-ceiling')
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents or args.repeats <= 0:
        ap.error('positive repeats and output under build/ required')
    out.mkdir(parents=True, exist_ok=True)
    generate(out / 'vu1_aot.inc')
    subprocess.run(['clang-cl', '-m32', '/nologo', '/O2', '-Wno-deprecated-declarations',
                    '-msse2', '-mfpmath=sse', '/clang:-ffp-contract=off', '/DPS2_VU_AOT', '/DPS2_VU_AOT_EMBED',
                    '/I' + str(out), '/I' + str(ROOT / 'port/runtime'),
                    str(ROOT / 'port/runtime/bench_vu1_geometry.c'),
                    str(ROOT / 'port/gs/gs.c'), str(ROOT / 'port/gs/memory.c'),
                    '/Feceiling.exe'], cwd=out, check=True, timeout=600)
    result = subprocess.run([str(out / 'ceiling.exe'), str(args.capture.resolve()), str(args.repeats)],
                            capture_output=True, text=True, timeout=120)
    (out / 'result.txt').write_text(result.stdout + result.stderr)
    print(result.stdout + result.stderr)
    result.check_returncode()

if __name__ == '__main__':
    main()
