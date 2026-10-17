"""Profile local VU1 captures with the current AOT runtime (no embedded game data)."""
import argparse
import os
from pathlib import Path
import subprocess
import sys
from vu1_aot import generate

ROOT = Path(__file__).resolve().parents[2]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--native-geometry', action='store_true', help='compatibility option; native geometry is now compiled by default')
    ap.add_argument('--clip-pipeline', action='store_true', help='retain clipping flag queues for comparison')
    ap.add_argument('--isa', choices=('sse2', 'sse4.1', 'avx2'), default='sse2')
    ap.add_argument('--repeats', type=int, default=10000)
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/vu-program-bench')
    ap.add_argument('captures', nargs='+', type=Path)
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('output must stay under build/')
    out.mkdir(parents=True, exist_ok=True)
    captures = [str(p.resolve()) for p in args.captures]
    if any(not Path(p).is_file() for p in captures):
        ap.error('every capture must exist')
    generate(out / 'vu1_aot.inc')
    flags = ['clang-cl', '-m32', '/nologo', '/O2', '-Wno-deprecated-declarations',
             '-m' + args.isa, '-mno-fma', '-mfpmath=sse', '/clang:-ffp-contract=off', '/DPS2_VU_AOT', '/DPS2_VU_AOT_EMBED',
             '/I' + str(out), '/I' + str(ROOT / 'port/runtime')]
    if args.clip_pipeline:
        from vu1_native_clip import generate as generate_clip
        generate_clip(out / 'vu1_native_clip.inc', early_flags=not args.clip_pipeline)
    subprocess.run(flags + [str(ROOT / 'port' / p) for p in
                   ('runtime/vu0_interp.c', 'runtime/bench_vu1_programs.c', 'gs/memory.c', 'gs/gs.c')]
                   + ['/Febench.exe'], cwd=out, check=True, timeout=600)
    with (out / 'programs.csv').open('w') as log:
        subprocess.run([str(out / 'bench.exe'), str(args.repeats), *captures],
                       stdout=log, check=True, timeout=600)
    print(out / 'programs.csv')

if __name__ == '__main__':
    main()
