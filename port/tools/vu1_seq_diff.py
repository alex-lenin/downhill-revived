"""Differential check of the native VU1 kernels on captured run sequences (port/runtime/vu1_seq_diff.c).

Capture first (game data, under build/):
    DHD_VU_SEQ=build/port/vu-seq DHD_VU_SEQ_AT=<game seconds> DHD_VU_SEQ_KICKS=8000 dhd.exe ...
then
    python port/tools/vu1_seq_diff.py build/port/vu-seq [more capture dirs] [--repeat 20]
builds the harness with the game's AOT blocks and native kernels (generated from the local
microcode into OUT, never into tracked files), replays every captured run from its real state with
the native kernels off (the exact path) and on, and requires every register, the whole data memory
and every kicked GIF packet to be byte-identical. With --repeat it also times both modes per
microprogram key (OUT/<capture>.csv). Exit status 1 on any difference.
"""
import argparse
import csv
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))


def build(out, negative=False):
    from vu1_aot import generate
    generate(out / 'vu1_aot.inc')  # also writes vu1_native_clip.inc beside it
    runtime = ROOT / 'port/runtime'
    if negative:
        # Compile a private corrupted implementation; the comparator is unchanged.
        runtime = out / 'negative-runtime'
        runtime.mkdir(exist_ok=True)
        (runtime / 'vu0_interp.c').write_bytes((ROOT / 'port/runtime/vu0_interp.c').read_bytes())
        header = (ROOT / 'port/runtime/vu1_native_geometry.h').read_text()
        needle = 'return native_result(f, _mm_add_ps(acc, _mm_mul_ps(operand4(a), operand4(b))), d);'
        assert header.count(needle) == 1
        header = header.replace(needle, 'return native_result(f, _mm_xor_ps(_mm_add_ps(acc, _mm_mul_ps(operand4(a), operand4(b))), _mm_castsi128_ps(_mm_set1_epi32(1))), d);')
        (runtime / 'vu1_native_geometry.h').write_text(header)
    exe = out / 'vu1_seq_diff.exe'
    subprocess.run(['clang-cl', '-m32', '/nologo', '/O2', '-Wno-deprecated-declarations', '-msse2', '-mno-fma',
                    '-mfpmath=sse', '/clang:-ffp-contract=off', '/clang:-w', '-DPS2_VU_AOT', '-DPS2_VU_AOT_EMBED', '/I' + str(out),
                    '/I' + str(ROOT / 'port/runtime'), str(runtime / 'vu0_interp.c'),
                    str(ROOT / 'port/runtime/vu1_seq_diff.c'), '/Fe' + str(exe)], cwd=out, check=True, timeout=1200)
    return exe


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('captures', nargs='+', type=Path)
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/vu1-seq-diff')
    ap.add_argument('--repeat', type=int, default=0, help='timed replays of each run per mode')
    ap.add_argument('--max', type=int, default=0, help='runs per capture (default all)')
    ap.add_argument('--negative', action='store_true', help='private clamp-result corruption; comparison must fail')
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('generated code holds game microcode: OUT must stay under build/')
    out.mkdir(parents=True, exist_ok=True)
    exe = build(out, args.negative)
    failed = 0
    for capture in args.captures:
        capture = capture.resolve()
        table = out / (capture.name + '.csv')
        with table.open('w') as log:
            p = subprocess.run([str(exe), str(capture), str(args.repeat)] + ([str(args.max)] if args.max else []),
                               stdout=log, stderr=subprocess.PIPE, text=True, timeout=7200)
        print('%s: %s' % (capture.name, p.stderr.strip()))
        failed |= p.returncode != 0
        if args.repeat:
            rows = sorted(csv.DictReader(table.open()), key=lambda r: -float(r['exact_us']) * int(r['runs']))
            for r in rows[:12]:
                print('  %s/%s vi13 %s: %5d runs, exact %7.2f us, native %7.2f us, %.2fx' % (
                    r['program'], r['entry'], r['dispatch'], int(r['runs']), float(r['exact_us']),
                    float(r['native_us']), float(r['speedup'])))
    print('VU1 NATIVE DIFF ' + ('FAILED' if failed else 'MATCH'))
    sys.exit(1 if failed else 0)


if __name__ == '__main__':
    main()
