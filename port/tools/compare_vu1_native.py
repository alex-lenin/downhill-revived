"""Compare exact and candidate VU1 replay snapshots; GPU/readback gates are separate."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', type=Path, required=True, help='bench_vu1_programs executable')
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('captures', nargs='+', type=Path)
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('output must stay under build/')
    caps = [p.resolve() for p in args.captures]
    if any(not p.is_file() for p in caps):
        ap.error('every capture must exist')
    for mode in (0, 1):
        dest = out / str(mode)
        dest.mkdir(parents=True, exist_ok=True)
        env = dict(os.environ, DHD_VU1_NATIVE=str(mode), DHD_VU1_SNAPSHOT=str(dest))
        with (dest / 'run.csv').open('w') as log, (dest / 'stderr.txt').open('w') as err:
            subprocess.run([str(args.exe.resolve()), '1', *map(str, caps)], env=env,
                           stdout=log, stderr=err, check=True, timeout=120)
    results = []
    for i, cap in enumerate(caps):
        for suffix in ('gif', 'state'):
            name = f'{i:03d}.{suffix}'
            a, b = [(out / str(mode) / name).read_bytes() for mode in (0, 1)]
            offset = next((j for j, (x, y) in enumerate(zip(a, b)) if x != y), None)
            if offset is None and len(a) != len(b):
                offset = min(len(a), len(b))
            results.append(dict(capture=cap.name, section=suffix, equal=a == b,
                                first_difference=offset, exact_bytes=len(a), candidate_bytes=len(b),
                                exact_sha256=hashlib.sha256(a).hexdigest(),
                                candidate_sha256=hashlib.sha256(b).hexdigest()))
            print(f'{cap.name} {suffix}: ' + ('MATCH' if a == b else f'DIFF at {offset}'))
    (out / 'results.json').write_text(json.dumps(results, indent=2) + '\n')
    failures = sum(not r['equal'] for r in results)
    print(f'{len(caps)} captures, {failures} differing sections; native coverage must be checked separately; no GPU backend/readback/image coverage')
    raise SystemExit(bool(failures))

if __name__ == '__main__':
    main()
