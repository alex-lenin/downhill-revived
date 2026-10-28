"""Full-state fixed-template oracle, including a required failing corruption control.

Synthetic inputs only (port/runtime/test_ps2asm_fast.c), as a guest program: wasm32 -> wasm2c.
"""
import argparse
import json
import re
from pathlib import Path

import testprog

ROOT = Path(__file__).resolve().parents[2]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out', type=Path, default=ROOT / 'build/port/asm-fast-test')
    args = ap.parse_args()
    out = args.out.resolve()
    if not out.is_relative_to(ROOT / 'build'):
        ap.error('output must be under this checkout\'s build/')
    out.mkdir(parents=True, exist_ok=True)
    exe = testprog.guest('asm-fast', [ROOT / 'port/runtime/test_ps2asm_fast.c'], out=out, memory=0x12010000)
    results = []
    controls = [('check', 0, [])]
    for mode, expected, extra in controls:
        run = testprog.run(exe, mode, *extra, expect=None, quiet=True)
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
