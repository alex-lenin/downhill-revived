"""Check guarded native VU0 against the original interpreter and pinned PCSX2.

Requires local DHD_VU0_CAPTURE inputs and clang-cl/x86 libraries. Generated words,
objects and captured input variants remain in ignored build/. No game data is
downloaded. --negative corrupts the native result in a private copy; must fail.
"""
from pathlib import Path
import argparse
import os
import subprocess

import test_vu0_pcsx2 as oracle
from vu1_aot import generate

ROOT = Path(__file__).resolve().parents[2]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--captures', type=Path, default=ROOT / 'build/port/vu0-captures')
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/vu0-native-test')
    ap.add_argument('--negative', action='store_true')
    ap.add_argument('--entry', choices=('0000', '02E0', '05E8', '07B8', '0808'))
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('output must stay under build/')
    entries = (args.entry,) if args.entry else ('0000', '02E0', '05E8', '07B8', '0808')
    captures = [p for entry in entries for p in sorted(args.captures.glob(f'vu0-{entry}-*.bin'))]
    if not captures:
        ap.error('local VU0 0000 captures are required')
    out.mkdir(parents=True, exist_ok=True)
    flags = ['clang-cl', '-m32', '/nologo', '/O2', '-w', '-msse2', '-mno-fma', '-mfpmath=sse',
             '/clang:-ffp-contract=off', '/I' + str(ROOT / 'port/runtime')]
    generate(out / 'vu1_aot.inc')
    source = ROOT / 'port/runtime/vu0_interp.c'
    if args.negative:
        (out / 'vu0_native.h').write_text((ROOT / 'port/runtime/vu0_native.h').read_text().replace(
            'cpu->i=1.0f;', 'cpu->vi[1] ^= 1; cpu->i=1.0f;').replace(
            'cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x760;',
            'cpu->vi[1]^=1;cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x760;').replace(
            'cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x4f0;',
            'cpu->vi[1]^=1;cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x4f0;').replace(
            'cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x808;',
            'cpu->vi[1]^=1;cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x808;').replace(
            'cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x830;',
            'cpu->vi[1]^=1;cpu->vu0_ctrl[1]=mac;cpu->vu0_ctrl[10]=0x830;'))
        (out / 'negative.c').write_text(source.read_text())
        source = out / 'negative.c'
    subprocess.run(flags + ['/DPS2_VU_AOT', '/DPS2_VU_AOT_EMBED', '/I' + str(out), '/c', str(source),
                            '/Fo' + str(out / 'native.obj')], check=True)
    subprocess.run(flags + ['/DPS2_VU_REFERENCE', '/Dps2_vu0_run=ps2_vu0_reference',
                            '/Dps2_vu1_run=ps2_vu1_reference', '/Dps2_vu_aot_load=ps2_vu_aot_reference_load',
                            '/c', str(ROOT / 'port/runtime/vu0_interp.c'), '/Fo' + str(out / 'reference.obj')], check=True)
    subprocess.run(flags + [str(out / 'native.obj'), str(out / 'reference.obj'),
                            str(ROOT / 'port/runtime/vu0_native_diff.c'), '/Fe' + str(out / 'diff.exe')], check=True)
    for entry in entries:
        capture = next(p for p in captures if p.stem.split('-')[1] == entry)
        run = subprocess.run([str(out / 'diff.exe'), str(capture)], check=not args.negative)
        if args.negative and run.returncode != 1:
            raise AssertionError(f'negative control did not fail comparison for {entry}: {run.returncode}')
    if args.negative:
        raise SystemExit(1)  # Every kernel's corrupted result was rejected.
    # Keep PCSX2's instruction/pipeline implementation and complete comparator.
    oracle.OUT = out / 'pcsx2'
    oracle.OUT.mkdir(exist_ok=True)
    oracle.build()
    # Require a positive native hit in each independent-oracle process too.
    adapter = (ROOT / 'port/runtime/pcsx2_vu0_oracle.cpp').read_text().replace(
        'int result = ps2_vu0_run(&native, address);',
        'ps2_vu0_native_mode = 1; int result = ps2_vu0_run(&native, address);\n'
        '    if (ps2_vu0_native_runs != 1) return 3;')
    (out / 'oracle.cpp').write_text(adapter)
    subprocess.run(flags + ['/std:c++20', '/I' + str(oracle.OUT), str(out / 'oracle.cpp'), str(out / 'native.obj'),
                            '/Fe' + str(out / 'oracle.exe')], check=True)
    count = 0
    for capture in captures:
        for seed in range(33):
            path = capture if not seed else oracle.seeded(capture, seed)
            run = subprocess.run([str(out / 'oracle.exe'), str(path)], capture_output=True, text=True, check=True)
            if '0 differences' not in run.stdout:
                raise AssertionError(run.stdout)
            count += 1
    print(f'Native VU0 PCSX2: {count} cases MATCH; raw-bit differential positive coverage checked.', flush=True)


if __name__ == '__main__':
    main()
