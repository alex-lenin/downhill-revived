"""Full-state fixed-template oracle, including a required failing corruption control.

Synthetic inputs only (port/runtime/test_ps2asm_fast.c).
"""
import argparse
import json
import os
import re
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/asm-fast-test')
    args = ap.parse_args()
    out = args.out.resolve()
    if not out.is_relative_to(ROOT / 'build'):
        ap.error('output must be under this checkout\'s build/')
    out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ)
    llvm = Path.home() / 'scoop/apps/llvm/current/bin'
    env['PATH'] = str(llvm) + os.pathsep + env.get('PATH', '')
    clang = shutil.which('clang', path=env['PATH']) or 'clang'
    exe = out / 'test.exe'
    subprocess.run([clang, '--target=i686-pc-windows-msvc', '-O2', '-msse2',
                    '-mno-fma', '-mfpmath=sse', '-ffp-contract=off',
                    str(ROOT / 'port/runtime/test_ps2asm_fast.c'), '-o', str(exe)],
                   env=env, check=True)
    results = []
    controls = [('check', 0, [])]
    for mode, expected, extra in controls:
        run = subprocess.run([str(exe), mode, *extra], capture_output=True, text=True, env=env)
        label = mode + ('-' + extra[0] if extra else '')
        (out / (label + '.log')).write_text(run.stdout + run.stderr)
        results.append(dict(mode=label, exit=run.returncode))
        if run.returncode != expected or (mode == 'negative' and 'DIFF trial=0' not in run.stdout):
            raise AssertionError(f'{mode}: {run.returncode}\n{run.stdout}\n{run.stderr}')
        if mode == 'check':
            count = len(re.findall(r'^case=', run.stdout, re.M))
            if count < 7: raise AssertionError('missing template coverage')
            controls.extend(('negative', 1, [str(i)]) for i in range(count))
        print(f'{label}: expected exit {expected}; PASS', flush=True)
    (out / 'results.json').write_text(json.dumps(results, indent=2))


if __name__ == '__main__':
    main()
