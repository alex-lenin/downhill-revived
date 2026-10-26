"""Differential check of ps2float.h's single-precision VU macro arithmetic (ps2_vu_madd,
ps2_vu_arith4) against its double-precision reference (finding 090 SP14).

    python port/tools/vu_madd_diff.py [--cases N] [--seed S] [--negative] [--no-wasm]

Builds port/runtime/vu_madd_diff.c
  - on x64 with build_host64's code generation flags (strict FP model, the wasm2c build's), run
    under the EE's MXCSR (toward zero, FTZ, DAZ), round to nearest, and the two half-way modes;
  - on x64 at -O2 without the strict model (the optimizer of the wasm32 compile), round to nearest;
  - as wasm32 + SIMD128 with wasi-sdk (build_wasm.py's flags), run under node: the browser's
    plain IEEE arithmetic and the wasm lowering of the vector code.
--negative builds a vector MADD without its overflow fallback: the check must then fail.
Exit status 1 on any difference. Output under build/vu_madd_diff.
"""
import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

NODE_RUNNER = r"""
const fs = require('fs'); const { WASI } = require('wasi');
const wasi = new WASI({ version: 'preview1', args: process.argv.slice(2), env: {}, returnOnExit: true });
const wasmModule = new WebAssembly.Module(fs.readFileSync(process.argv[2]));
const instance = new WebAssembly.Instance(wasmModule, wasi.getImportObject());
process.exitCode = wasi.start(instance);
"""


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--cases', type=int, default=1000000, help='per MXCSR mode')
    ap.add_argument('--seed', default='0x5eed5eed')
    ap.add_argument('--negative', action='store_true')
    ap.add_argument('--no-wasm', action='store_true')
    a = ap.parse_args()
    from build_host64 import LLVM, MSVC, SDK, VERSION, WASI
    out = ROOT / 'build' / 'vu_madd_diff'
    out.mkdir(parents=True, exist_ok=True)
    source = ROOT / 'port' / 'runtime' / 'vu_madd_diff.c'
    defines = ['-DVU_MADD_DIFF_NEGATIVE'] if a.negative else []
    msvc = ['/imsvc' + str(MSVC / 'include')] + ['/imsvc' + str(SDK / 'Include' / VERSION / x) for x in ('ucrt', 'um', 'shared')]
    libs = ['/link', '/LIBPATH:' + str(MSVC / 'lib' / 'x64')] + \
           ['/LIBPATH:' + str(SDK / 'Lib' / VERSION / x / 'x64') for x in ('ucrt', 'um')]
    common = [str(LLVM / 'clang-cl.exe'), '--target=x86_64-pc-windows-msvc', '/nologo', '/MT', '/w',
              '/clang:-fno-strict-aliasing', '/clang:-fwrapv', '/I' + str(ROOT / 'port/runtime')] + msvc + defines
    runs = []
    strict = out / 'strict.exe'
    subprocess.run(common + ['/O2', '/clang:-O3', '/clang:-march=x86-64-v2', '/clang:-mtune=znver3',
                             '/clang:-ffp-model=strict', '/clang:-ffp-contract=off', str(source), '/Fe' + str(strict)]
                   + libs, cwd=out, check=True)
    # MXCSR: EE (toward zero, FTZ, DAZ), nearest, nearest + FTZ/DAZ, toward zero alone
    runs.append(('x64 strict', [str(strict), str(a.cases), a.seed, 'ffc0', '1f80', '9fc0', '7f80']))
    plain = out / 'plain.exe'
    subprocess.run(common + ['/O2', '/clang:-ffp-contract=off', str(source), '/Fe' + str(plain)] + libs,
                   cwd=out, check=True)
    runs.append(('x64 -O2', [str(plain), str(a.cases), a.seed, '1f80']))
    if not a.no_wasm:
        wasm = out / 'vu_madd_diff.wasm'
        sysroot = WASI / 'share' / 'wasi-sysroot'
        subprocess.run([str(WASI / 'bin' / 'clang.exe'), '--target=wasm32-wasip1', '--sysroot=' + str(sysroot),
                        '-O2', '-msimd128', '-ffp-contract=off', '-fwrapv', '-fno-strict-aliasing',
                        '-I' + str(ROOT / 'port/runtime')] + defines + [str(source), '-o', str(wasm)],
                       cwd=out, check=True)
        runner = out / 'run.cjs'
        runner.write_text(NODE_RUNNER)
        runs.append(('wasm32 node', ['node', str(runner), str(wasm), str(a.cases), a.seed]))
    failed = False
    for name, cmd in runs:
        r = subprocess.run(cmd, cwd=out, capture_output=True, text=True)
        print('== %s (exit %d)' % (name, r.returncode))
        print((r.stdout + r.stderr).strip())
        failed |= r.returncode != 0
    if a.negative:
        print('negative control: %s' % ('caught' if failed else 'NOT CAUGHT'))
        sys.exit(0 if failed else 1)
    print('vu_madd_diff: %s' % ('DIFF' if failed else 'MATCH'))
    sys.exit(1 if failed else 0)


if __name__ == '__main__':
    main()
