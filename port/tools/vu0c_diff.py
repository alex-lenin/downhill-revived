"""Differential check of the compiled VU0 microprograms (port/tools/vu0_aot.py) against the interpreter.

    python port/tools/vu0c_diff.py [--cases N] [--repeat N] [--negative]

Builds port/runtime/vu0c_diff.c on x64 with build_host64's toolchain and code generation flags,
twice-linked vu0_interp.c (the original interpreter as the referee, and the build with
-DPS2_VU0_COMPILED), generating the programs and a VU0 micro memory image from the local
assets/vutext.bin into OUT (under build/; game-derived, never committed). Every entry address
runs on random raw-bit states both ways; see vu0c_diff.c. --negative corrupts one generated
operation in a private copy: the comparison must then fail. Exit status 1 on any difference.
"""
import argparse
import os
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))


def code_image(data):
    """VU0 micro memory after the game's VIF0 MPG upload of its package."""
    from vu1_aot import program
    from vu0_aot import PACKAGE
    pairs, _ = program(data, PACKAGE)
    image = bytearray(4096)
    for pc, (lo, hi) in pairs.items():
        struct.pack_into('<II', image, pc * 8, lo, hi)
    return bytes(image)


def build(out, negative=False, with_natives=False):
    from vu0_aot import generate
    generate(out / 'vu0_aot.inc')
    if negative:
        inc = (out / 'vu0_aot.inc').read_text()
        needle = 'vu0c_madd('
        at = inc.index(needle, inc.index('static void vu0c_08D0'))
        inc = inc[:at] + 'vu0c_msub(' + inc[at + len(needle):]
        (out / 'vu0_aot.inc').write_text(inc)
    from build_host64 import LLVM, MSVC, SDK, VERSION
    base = [str(LLVM / 'clang-cl.exe'), '--target=x86_64-pc-windows-msvc', '/nologo', '/O2', '/clang:-O3',
            '/clang:-march=x86-64-v2', '/clang:-mtune=znver3', '/clang:-ffp-model=strict', '/clang:-ffp-contract=off',
            '/clang:-fno-strict-aliasing', '/clang:-fwrapv', '/MT', '/w',
            '/I' + str(out), '/I' + str(ROOT / 'port/runtime'), '/I' + str(ROOT / 'port/include'),
            '/I' + str(ROOT / 'port/host'), '/imsvc' + str(MSVC / 'include')]
    base += ['/imsvc' + str(SDK / 'Include' / VERSION / x) for x in ('ucrt', 'um', 'shared')]
    source = str(ROOT / 'port/runtime/vu0_interp.c')
    natives = []
    if with_natives:  # the native routines too (vu0_native.h), timed against the compiled programs
        from vu1_aot import generate as generate_vu1
        from vu0_native_words import derive
        generate_vu1(out / 'vu1_aot.inc')
        derive(out / 'vu1_aot.inc', out / 'vu0_native_words.inc')
        natives = ['-DPS2_VU0_NATIVE', '-DVU0C_DIFF_NATIVES']
    subprocess.run(base + natives + ['-DPS2_VU0_COMPILED', '/c', source, '/Fo' + str(out / 'compiled.obj')], cwd=out, check=True)
    subprocess.run(base + ['-DPS2_VU_REFERENCE', '-Dps2_vu0_run=ps2_vu0_reference', '-Dps2_vu1_run=ps2_vu1_reference',
                           '-Dps2_vu_aot_load=ps2_vu_aot_reference_load', '/c', source, '/Fo' + str(out / 'reference.obj')],
                   cwd=out, check=True)
    exe = out / 'vu0c_diff.exe'
    subprocess.run(base + natives + [str(out / 'compiled.obj'), str(out / 'reference.obj'), str(ROOT / 'port/runtime/vu0c_diff.c'),
                           '/Fe' + str(exe), '/link', '/libpath:' + str(MSVC / 'lib/x64'),
                           '/libpath:' + str(SDK / 'Lib' / VERSION / 'ucrt/x64'),
                           '/libpath:' + str(SDK / 'Lib' / VERSION / 'um/x64')], cwd=out, check=True)
    return exe


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/vu0c-diff')
    ap.add_argument('--cases', type=int, default=20000)
    ap.add_argument('--repeat', type=int, default=0)
    ap.add_argument('--negative', action='store_true')
    ap.add_argument('--natives', action='store_true', help='also build the native routines and time them (with --repeat)')
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('generated code holds game microcode: OUT must stay under build/')
    out.mkdir(parents=True, exist_ok=True)
    vutext = Path(os.environ.get('DHD_VUTEXT', ROOT / 'assets/vutext.bin'))
    (out / 'vu0_code.bin').write_bytes(code_image(vutext.read_bytes()))
    exe = build(out, args.negative, args.natives)
    p = subprocess.run([str(exe), str(vutext), str(out / 'vu0_code.bin'), str(args.cases), str(args.repeat)],
                       stderr=subprocess.PIPE, text=True)
    # The guards run the interpreter on changed words too; its complaints about them are expected.
    for line in p.stderr.splitlines():
        if not line.startswith(('vu0: failed lower', 'vu0: unknown upper', 'vu0: microprogram', 'last PCs', 'VI:')):
            print(line, file=sys.stderr)
    if args.negative:
        if p.returncode != 1:
            raise SystemExit('negative control: the corrupted program was not caught')
        print('negative control: caught')
        return
    raise SystemExit(p.returncode)


if __name__ == '__main__':
    main()
