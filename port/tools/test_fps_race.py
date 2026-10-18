"""Deterministic high-rate race: prove a change leaves the 120/144 simulation unchanged.

    python port/tools/test_fps_race.py --exe OLD.exe --out build/fps-race/old
    python port/tools/test_fps_race.py --exe NEW.exe --out build/fps-race/new --reference build/fps-race/old

Runs one fixed-seed race (race_compare.py's menu route, full pedal) at DHD_FPS=144 by default,
unpaced (DHD_FRAME_CLOCK=1): the adaptive wall clock is off there, so the run repeats exactly
(finding 067). DHD_FPS_TRACE records every rider's state once per race tick and at every real
update of the first six seconds, and DHD_FPS_STOP ends the game at --ticks. With --reference,
every column the two traces share (except the host wall time) must be identical, row by row;
the verdict is MATCH or DIFF with the first difference. The renderer gate
(gate_native_renderer.py) covers the 60 path; this covers the high-rate one. Needs DHD_ISO. The
traces hold game-derived state: they stay below build/ (or outside the checkout, --external-out).
"""
import argparse
import csv
import json
import os
import shutil
import sys
import time
from itertools import zip_longest
from pathlib import Path

import race_compare as rc
from owned_process import stop_verified

ROOT = Path(__file__).resolve().parents[2]
TRACES = ('trace.csv', 'trace.csv.steps.csv')
IGNORED = {'wall'}  # host time, not game state


def run(exe, out, rate, ticks, timeout):
    settings = argparse.Namespace(exe=exe.resolve(), nav=rc.NAV, race_pad='0:900:cross', window=0,
                                  race_seed=rc.RETAIL_RACE_SEED, dump=False, no_gsdump=True,
                                  port_env={'DHD_FPS': str(rate), 'DHD_RENDER': 'native',
                                            'DHD_FPS_TRACE': str(out / 'trace.csv'), 'DHD_FPS_STOP': str(ticks)})
    start = time.monotonic()
    process, digest = rc.launch_port(settings, out, rc.clock_events(settings.race_pad), [])
    try:
        while process.poll() is None:
            if time.monotonic() - start > timeout:
                raise TimeoutError(f'race did not reach tick {ticks} in {timeout} s')
            time.sleep(1)
        if process.returncode:
            raise RuntimeError(f'game exited {process.returncode}; see {out / "port.log"}')
    finally:
        stop_verified(process, out / 'dhd.exe')
    with (out / 'trace.csv').open() as f:
        seen = {}
        for row in csv.DictReader(f):
            seen.setdefault(int(row['tick']), set()).add(int(row['slot']))
    if sorted(seen) != list(range(ticks + 1)) or any(slots != set(range(10)) for slots in seen.values()):
        raise RuntimeError('incomplete race trace: not every tick 0..%d has all ten riders' % ticks)
    (out / 'run.json').write_text(json.dumps(dict(rate=rate, ticks=ticks, seed=rc.RETAIL_RACE_SEED,
                                                  exe_sha256=digest, wall_seconds=time.monotonic() - start),
                                             indent=2) + '\n')


def compare(reference, actual):
    """Rows and shared columns of each trace must be identical; returns a report."""
    report = {}
    for name in TRACES:
        with (reference / name).open() as a, (actual / name).open() as b:
            left, right = csv.DictReader(a), csv.DictReader(b)
            columns = [c for c in left.fieldnames if c in right.fieldnames and c not in IGNORED]
            rows = 0
            for x, y in zip_longest(left, right):
                if x is None or y is None:
                    report[name] = dict(match=False, rows=rows, difference='different row count')
                    break
                changed = [c for c in columns if x[c] != y[c]]
                if changed:
                    report[name] = dict(match=False, rows=rows, difference=dict(
                        tick=x['tick'], slot=x['slot'], columns={c: [x[c], y[c]] for c in changed}))
                    break
                rows += 1
            else:
                report[name] = dict(match=True, rows=rows, columns=columns)
    return report


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--reference', type=Path, help='an earlier output folder of this tool to compare with')
    ap.add_argument('--rate', type=int, default=144)
    ap.add_argument('--ticks', type=int, default=12000, help='race ticks (1/60 s) to run')
    ap.add_argument('--timeout', type=int, default=3600)
    ap.add_argument('--external-out', action='store_true', help='allow an output folder outside the checkout')
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents and (not args.external_out or out == ROOT or ROOT in out.parents):
        ap.error('--out must be below build/, or outside the checkout with --external-out')
    if not 60 <= args.rate <= 144:
        ap.error('--rate must be 60..144')
    if not os.environ.get('DHD_ISO'):
        ap.error('DHD_ISO must reference the local game image')
    if out.exists():
        ap.error(f'{out} exists; every run gets a new folder')
    out.mkdir(parents=True)
    run(args.exe, out, args.rate, args.ticks, args.timeout)
    if not args.reference:
        print(f'{args.rate} Hz race through tick {args.ticks} recorded in {out}')
        return 0
    report = compare(args.reference.resolve(), out)
    verdict = 'MATCH' if all(r['match'] for r in report.values()) else 'DIFF'
    (out / 'comparison.json').write_text(json.dumps(dict(verdict=verdict, reference=str(args.reference),
                                                         traces=report), indent=2) + '\n')
    for name, r in report.items():
        print(f'{name}: {r["rows"]} rows', 'identical' if r['match'] else f'differ: {r["difference"]}')
    print(verdict)
    return verdict != 'MATCH'


if __name__ == '__main__':
    sys.exit(main())
