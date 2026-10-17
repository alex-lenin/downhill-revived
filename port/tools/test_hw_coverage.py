"""Build/run the Windows hardware coverage and ROV feasibility test.

Requires a real FL11_1+ D3D11 device exposing rasterizer-ordered views and
conservative rasterization. Unsupported hardware/runtime returns3, never PASS.
This tests synthetic integer coverage and same-pixel order, not a game renderer.

Examples:
    python port/tools/test_hw_coverage.py --build-only
    python port/tools/test_hw_coverage.py

CLANG may name clang or clang-cl; otherwise clang is discovered on PATH.
Executables, compiler output, run output and summary stay under build/.
"""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[2]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--build-only', action='store_true',
                        help='Compile without creating a D3D device or running GPU work')
    parser.add_argument('--out', type=Path, default=ROOT / 'build/port/test-hw-coverage',
                        help='Output directory, required to remain inside repository build/')
    parser.add_argument('--timeout', type=float, default=60,
                        help='Maximum seconds for compilation and separately for execution')
    options = parser.parse_args()
    out = options.out.resolve()
    try:
        out.relative_to((ROOT / 'build').resolve())
    except ValueError:
        parser.error('--out must remain inside repository build/')
    if options.timeout <= 0:
        parser.error('--timeout must be positive')
    out.mkdir(parents=True, exist_ok=True)

    def report(status: str, exit_code: int, **details: object) -> int:
        result = {'status': status, 'exit_code': exit_code, **details}
        (out / 'summary.json').write_text(json.dumps(result, indent=2) + '\n',
                                        encoding='utf-8')
        print(json.dumps(result, indent=2))
        return exit_code

    if os.name != 'nt':
        return report('unsupported', 3, reason='Windows D3D11 hardware test')
    clang = shutil.which(os.environ.get('CLANG', 'clang'))
    if clang is None:
        return report('build_error', 2, reason='Set CLANG or put clang on PATH')
    source = ROOT / 'port/gs/test_hw_coverage.c'
    shader = ROOT / 'port/gs/test_hw_coverage.hlsl'
    executable = out / 'test_hw_coverage.exe'
    libraries = ['d3d11.lib', 'd3dcompiler.lib', 'dxgi.lib']
    if Path(clang).stem.lower() == 'clang-cl':
        command = [clang, '/nologo', '-m32', '/O2', str(source),
                   '/Fe:' + str(executable), '/Fo:' + str(out / 'test_hw_coverage.obj'),
                   '/link', *libraries]
    else:
        command = [clang, '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O2',
                   str(source), '-o', str(executable),
                   *['-Wl,' + library for library in libraries]]
    started = time.monotonic()
    try:
        with (out / 'build.log').open('wb') as log:
            built = subprocess.run(command, cwd=out, stdout=log,
                                   stderr=subprocess.STDOUT, timeout=options.timeout)
    except (OSError, subprocess.TimeoutExpired) as error:
        return report('build_error', 2, reason=str(error), compiler=clang)
    if built.returncode:
        return report('build_error', 2, compiler_returncode=built.returncode,
                      compiler=clang, build_log=str(out / 'build.log'))
    if options.build_only:
        return report('built_only', 0, executable=str(executable), shader=str(shader),
                      elapsed_s=time.monotonic() - started,
                      note='No device created and no GPU test executed')
    try:
        with (out / 'run.log').open('wb') as log:
            run = subprocess.run([str(executable), str(shader)], cwd=out,
                                 stdout=log, stderr=subprocess.STDOUT,
                                 timeout=options.timeout)
    except (OSError, subprocess.TimeoutExpired) as error:
        return report('execution_error', 2, reason=str(error), run_log=str(out / 'run.log'))
    output = (out / 'run.log').read_text(encoding='utf-8', errors='replace')
    print(output, end='')
    status = {0: 'pass', 1: 'fail', 2: 'execution_error', 3: 'unsupported'}.get(
        run.returncode, 'execution_error')
    # A zero exit from an unexpectedly wrong executable is not a test pass.
    if status == 'pass' and 'RESULT=PASS' not in output:
        status = 'execution_error'
    exit_code = {'pass': 0, 'fail': 1, 'execution_error': 2, 'unsupported': 3}[status]
    return report(status, exit_code, executable=str(executable), shader=str(shader),
                  test_returncode=run.returncode, run_log=str(out / 'run.log'),
                  elapsed_s=time.monotonic() - started)


if __name__ == '__main__':
    raise SystemExit(main())
