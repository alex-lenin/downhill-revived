"""Frozen same-executable hardware-off/on six-moment native validation.

Both arms use accepted round27 shader optimizations111; only DHD_GS_HARDWARE
changes0->1. Existing EE exclusions, image bytes and depth-readback comparison
functions are imported unchanged. Requires nonzero actual hardware submissions,
not merely correctness through fallback. This is native validation, not retail
full-game equivalence. Captures remain under ignored build/.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time
from gate_gs_optimization import (ROOT, rc, TICKS, MOMENTS, RACE_IMAGES, MENU_IMAGES,
                                sha, wait_race, wait_menu, compare_images,
                                ee_compare, compare_readbacks)

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--reference', type=Path, default=ROOT / 'build/port/compare/r27-defined-native')
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT / 'build' not in out.parents:
        ap.error('output must stay below build/')
    if not os.environ.get('DHD_ISO'):
        ap.error('DHD_ISO must reference the local game image')
    out.mkdir(parents=True, exist_ok=False)
    frozen = out / 'frozen.exe'
    shutil.copy2(args.exe.resolve(), frozen)
    digest = sha(frozen)
    metadata = dict(scope='native-vs-native; not retail full-game parity', exe_sha256=digest,
                    original=dict(hardware='0', early='1', shift='1', store='1'), candidate=dict(hardware='1', early='1', shift='1', store='1'),
                    moments_ticks=list(TICKS), late_seconds=[60, 75], nav=rc.NAV,
                    race_pad=rc.RACE_PAD, seed=rc.RETAIL_RACE_SEED,
                    reference=str(args.reference.resolve()), race_timeout_seconds=600,
                    exclusions='unchanged gate_vu1_native.ee_compare and compare_readbacks')
    (out / 'method.json').write_text(json.dumps(metadata, indent=2) + '\n')
    fields = []
    for mode, early, shift, store in [('0', '1', '1', '1'), ('1', '1', '1', '1')]:
        directory = out / mode
        directory.mkdir()
        toggles = dict(DHD_GS_EARLY_DEPTH=early, DHD_GS_ADDRESS_SHIFT=shift,
                       DHD_GS_EXCLUSIVE_WORD_STORE=store, DHD_GS_HARDWARE=mode)
        env = dict(toggles, DHD_VU1_NATIVE_STATS='1', DHD_PROFILE='1',
                   DHD_GS_READBACK_TRACE=str(directory / 'readbacks.bin'))
        run = argparse.Namespace(nav=rc.NAV, exe=frozen, race_pad=rc.RACE_PAD,
                                 window=0, race_seed=rc.RETAIL_RACE_SEED, dump=True, port_env=env)
        deadline = time.monotonic() + 600
        process, _ = rc.launch_port(run, directory, rc.clock_events(rc.RACE_PAD), MOMENTS)
        fields.append(wait_race(process, directory, deadline))
        if sha(frozen) != digest:
            raise AssertionError('frozen executable changed')
        menu = directory / 'menu'
        menu.mkdir()
        menu_env = {k:v for k,v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
        menu_env.update(toggles, DHD_PROFILE='1', DHD_ISO=os.environ['DHD_ISO'], DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_FRAME_CLOCK='1',
                        DHD_SND_OFF='1', DHD_NAV='0:triangle|cross', DHD_RACE_SEED=str(rc.RETAIL_RACE_SEED),
                        DHD_SCREENSHOT=str(menu / 'frame'), DHD_CAPTURE_FRAME='600', DHD_CAPTURE_FRAME_INTERVAL='60')
        with (menu / 'run.log').open('wb') as log:
            process = subprocess.Popen([str(frozen)], cwd=menu, env=menu_env, stdout=log, stderr=subprocess.STDOUT)
            wait_menu(process, menu, time.monotonic() + 180)
    failures = []
    def checked(function, *values, **options):
        try:
            function(*values, **options)
        except (AssertionError, FileNotFoundError) as error:
            failures.append(f'{function.__name__}: {error}')
            print('GATE FAIL:', failures[-1], flush=True)
    # Preserve every early state gate, using the existing canonical producer.
    for tick in TICKS[:4]:
        for mode in ('0', '1'):
            checked(ee_compare, args.reference / 'port-ee' / f'ee-g+{tick}.bin', out / mode / 'port-ee' / f'ee-g+{tick}.bin')
    for name in RACE_IMAGES[:4]:
        for mode in ('0', '1'):
            checked(compare_images, args.reference / 'port', out / mode / 'port', [name])
    for tick in TICKS:
        checked(ee_compare, out / '0/port-ee' / f'ee-g+{tick}.bin', out / '1/port-ee' / f'ee-g+{tick}.bin')
    for name in RACE_IMAGES:
        checked(compare_images, out / '0/port', out / '1/port', [name])
    for mode in ('0', '1'):
        actual = {p.name for p in (out / mode / 'port').glob('*.bmp')}
        if actual != set(RACE_IMAGES):
            failures.append(f'unexpected race BMP set mode={mode}: {sorted(actual)}')
    for name in MENU_IMAGES:
        checked(compare_images, out / '0/menu', out / '1/menu', [name])
    # Canonical menu comparisons add coverage; additional generated reference
    # frames are irrelevant to the explicitly selected capture moments.
    for mode in ('0', '1'):
        for name in MENU_IMAGES:
            checked(compare_images, args.reference / 'menu', out / mode / 'menu', [name])
    checked(compare_readbacks, out / '0/readbacks.bin', out / '1/readbacks.bin', max(fields))
    hits = {}
    pattern = re.compile(r'profile GS hardware: enabled=(\d+) available=(\d+) batches=(\d+) primitives=(\d+) reference=(\d+) rejects=([0-9,]+)')
    for kind, log_path in [('race', out / '1/port.log'), ('menu', out / '1/menu/run.log')]:
        records = pattern.findall(log_path.read_text(errors='replace'))
        if records:
            enabled, available, batches, primitives, reference, rejected = records[-1]
            hits[kind] = dict(enabled=int(enabled), available=int(available), batches=int(batches),
                              primitives=int(primitives), reference_batches=int(reference),
                              rejected=[int(v) for v in rejected.split(',')])
        else:
            hits[kind] = None
    if not hits['race'] or not hits['race']['available'] or hits['race']['primitives'] <= 0:
        failures.append('hardware race path not actually exercised; fallback does not establish a hardware PASS')
    metadata.update(hardware_path_stats=hits)
    metadata.update(result='FAIL' if failures else 'PASS', final_capture_fields=fields, failures=failures)
    (out / 'result.json').write_text(json.dumps(metadata, indent=2) + '\n')
    if failures:
        raise AssertionError('; '.join(failures))
    print('HARDWARE-OFF/ON SIX-MOMENT LATE GATE MATCH WITH NONZERO HARDWARE HITS', flush=True)


if __name__ == '__main__':
    main()
