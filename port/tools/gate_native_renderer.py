"""Game-state gate for the native renderer (DHD_RENDER=native).

One frozen executable runs the recorded race twice: the exact GS path (the reference renderer) and
the native renderer. Pictures may differ; game state may not. The gate requires:
  - the EE dumps at GO+0/+2/+12/+16 of both arms to match the canonical reference;
  - the EE dumps at every moment (through GO+4500) to match between the arms;
  - every GS readback the EE consumes (flare Z) to match between the arms, record for record;
  - the native arm to have really used the native renderer.
The comparison functions and their existing exclusions are imported unchanged. Captures stay under
ignored build/.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import time
from gate_gs_optimization import (ROOT, rc, TICKS, MOMENTS, sha, wait_race, ee_compare, compare_readbacks)

ARMS = {'exact': {}, 'native': {'DHD_RENDER': 'native'}}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--external-out', action='store_true',
                    help='allow private artifacts outside the checkout; comparisons and reference are unchanged')
    # r46-plain: a plain build_clean.py executable after the data-fixup audit.
    # All changes from r40-plain are enumerated and checked against independent
    # retail RAM in finding 062 (including round 45's pickup timing correction).
    # r40-plain: the exact arm of this gate on 22e70d54 (release 0.2.0), a plain build_clean.py
    # executable. It differs from r39-plain (b7904561, 0.1.0) only in 2 words, 0x4F4324 and
    # 0x4F4454: EXIT GAME text left in the font packet buffer (finding 056; finding 053 section 6).
    # r39-plain differed from r38-reviewed (1499a7c4, the frozen object basis) only in 54 words of
    # dead main-thread stack; r38-reviewed came after the 65 words of finding 050 were traced.
    ap.add_argument('--reference', type=Path, default=ROOT / 'build/port/compare/r46-plain')
    ap.add_argument('--env', action='append', default=[], help='additional NAME=VALUE for the native arm')
    # A contributor's own reference: run the gate once with --record on a plain build_clean.py
    # executable of a revision known to be good (a release commit). Its exact arm becomes
    # --reference (a new folder); later runs compare against it. References are game-derived
    # captures and stay local, under build/.
    ap.add_argument('--record', action='store_true',
                    help='write --reference (a new folder under build/) from this run\'s exact arm instead of '
                         'comparing with it')
    args = ap.parse_args()
    if args.record:
        reference = args.reference.resolve()
        if ROOT / 'build' not in reference.parents or reference.exists():
            ap.error('--record writes a new --reference folder under build/')
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents and (not args.external_out or out == ROOT or ROOT in out.parents):
        ap.error('output must stay below build/, or outside the checkout with --external-out')
    if not os.environ.get('DHD_ISO'):
        ap.error('DHD_ISO must reference the local game image')
    out.mkdir(parents=True, exist_ok=False)
    frozen = out / 'frozen.exe'
    shutil.copy2(args.exe.resolve(), frozen)
    digest = sha(frozen)
    arms = {name: dict(env) for name, env in ARMS.items()}
    arms['native'].update(dict(item.split('=', 1) for item in args.env))
    metadata = dict(scope='game state with the native renderer; pictures not compared', exe_sha256=digest,
                    arms=arms, moments_ticks=list(TICKS), nav=rc.NAV, race_pad=rc.RACE_PAD, seed=rc.RETAIL_RACE_SEED,
                    reference=str(args.reference.resolve()), race_timeout_seconds=600,
                    exclusions='unchanged gate_vu1_native.ee_compare and compare_readbacks')
    (out / 'method.json').write_text(json.dumps(metadata, indent=2) + '\n')
    fields = []
    for name, extra in arms.items():
        directory = out / name
        directory.mkdir()
        env = dict(DHD_GS_EARLY_DEPTH='1', DHD_GS_ADDRESS_SHIFT='1', DHD_GS_EXCLUSIVE_WORD_STORE='1',
                   DHD_GS_HARDWARE='1', DHD_PROFILE='1', DHD_GS_READBACK_TRACE=str(directory / 'readbacks.bin'))
        env.update(extra)
        run = argparse.Namespace(nav=rc.NAV, exe=frozen, race_pad=rc.RACE_PAD,
                                 window=0, race_seed=rc.RETAIL_RACE_SEED, dump=True, port_env=env)
        process, _ = rc.launch_port(run, directory, rc.clock_events(rc.RACE_PAD), MOMENTS)
        fields.append(wait_race(process, directory, time.monotonic() + 600))
        if sha(frozen) != digest:
            raise AssertionError('frozen executable changed')
    failures = []

    def checked(function, *values):
        try:
            function(*values)
        except (AssertionError, FileNotFoundError) as error:
            failures.append(f'{function.__name__}: {error}')
            print('GATE FAIL:', failures[-1], flush=True)
    for tick in TICKS[:4]:
        for name in arms:
            if not args.record:
                checked(ee_compare, args.reference / 'port-ee' / f'ee-g+{tick}.bin', out / name / 'port-ee' / f'ee-g+{tick}.bin')
    for tick in TICKS:
        checked(ee_compare, out / 'exact/port-ee' / f'ee-g+{tick}.bin', out / 'native/port-ee' / f'ee-g+{tick}.bin')
    checked(compare_readbacks, out / 'exact/readbacks.bin', out / 'native/readbacks.bin', max(fields))
    records = re.findall(r'profile native: (.*)', (out / 'native/port.log').read_text(errors='replace'))
    stats = dict((k, int(v)) for k, v in re.findall(r'(\w+)=(-?\d+)', records[-1])) if records else None
    if not stats or stats.get('presented', 0) <= 0:
        failures.append('native renderer not actually used (no presented native frames)')
    metadata.update(native_stats=stats, result='FAIL' if failures else 'PASS', final_capture_fields=fields,
                    failures=failures)
    (out / 'result.json').write_text(json.dumps(metadata, indent=2) + '\n')
    if failures:
        raise AssertionError('; '.join(failures))
    if args.record:
        shutil.copytree(out / 'exact', reference)
        (reference / 'reference-manifest.json').write_text(json.dumps(dict(
            scope='native renderer EE reference, recorded with gate_native_renderer.py --record',
            source_run=str(out), frozen_exe_sha256=digest), indent=2) + '\n')
        print('reference recorded:', reference, '(the arms matched each other; nothing to compare it with yet)', flush=True)
        return
    print('NATIVE RENDERER GAME-STATE GATE MATCH', flush=True)


if __name__ == '__main__':
    main()
