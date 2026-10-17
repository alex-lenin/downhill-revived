"""Compare original and optimized native GS paths at six race moments.

One frozen executable runs original switches 0/0/0 against the requested
--early/--shift/--store values. The default reference defines native producer
bytes audited in finding034. Existing EE exclusions and the exact readback
checker are imported unchanged. Early moments are clock ticks; late captures
are GO+60/+75seconds. This is native-vs-native validation, not full-game retail
parity. Output must be a fresh directory below build/.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/tools'))
import race_compare as rc
from gate_vu1_native import ee_compare, compare_readbacks, readbacks

TICKS = (0, 2, 12, 16, 3600, 4500)
MOMENTS = [(f'g+{tick}', 'g', tick) for tick in TICKS]
EE_SIZE = (32 << 20) + (16 << 10)
RACE_IMAGES = [f'race-g+{tick}+0.bmp' for tick in TICKS]
MENU_IMAGES = [f'frame-{i:03}.bmp' for i in range(4)]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def bmp_complete(path):
    if not path.exists():
        return False
    header = path.read_bytes()[:6]
    return len(header) == 6 and header[:2] == b'BM' and int.from_bytes(header[2:6], 'little') == path.stat().st_size


def capture_field(directory, tick):
    log = (directory / 'port.log').read_text(errors='replace')
    fields = re.findall(rf'moment=g\+{tick} delta=0 field=(\d+)\b', log)
    if len(fields) != 1:
        raise AssertionError(f'expected one g+{tick} capture field, got {fields}')
    return int(fields[0])


def wait_race(process, directory, deadline):
    """Stop only after complete artifacts and readbacks beyond the last capture."""
    try:
        while time.monotonic() < deadline:
            if process.poll() is not None:
                raise RuntimeError(f'game exited {process.returncode}: {directory}')
            dumps_done = all((directory / 'port-ee' / f'ee-g+{t}.bin').exists() and
                             (directory / 'port-ee' / f'ee-g+{t}.bin').stat().st_size == EE_SIZE for t in TICKS)
            if dumps_done and all(bmp_complete(directory / 'port' / name) for name in RACE_IMAGES):
                field = capture_field(directory, TICKS[-1])
                try:
                    records = readbacks(directory / 'readbacks.bin')
                except (AssertionError, FileNotFoundError):
                    records = []
                # The unchanged checker drops its final field; allow it to
                # prove coverage THROUGH the final captured field after stop.
                if records and records[-1][0] >= field + 2:
                    return field
            time.sleep(.5)
        raise TimeoutError(f'600-second race limit: {directory}')
    finally:
        rc.compare.stop(process)


def wait_menu(process, directory, deadline):
    try:
        while time.monotonic() < deadline:
            if process.poll() is not None:
                raise RuntimeError(f'menu exited {process.returncode}: {directory}')
            if all(bmp_complete(directory / name) for name in MENU_IMAGES):
                return
            time.sleep(.5)
        raise TimeoutError(f'180-second menu limit: {directory}')
    finally:
        rc.compare.stop(process)


def compare_images(a, b, names, exact_set=False):
    if exact_set:
        for directory in (a, b):
            actual = {p.name for p in directory.glob('*.bmp')}
            if actual != set(names):
                raise AssertionError(f'unexpected race BMP set {directory}: {sorted(actual)}')
    for name in names:  # Every selected image; no first-four slice.
        if (a / name).read_bytes() != (b / name).read_bytes():
            raise AssertionError(f'BMP byte mismatch: {name} ({a} vs {b})')
        print(name, 'BMP BYTE MATCH', flush=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--early', choices=('0', '1'), default='0')
    ap.add_argument('--shift', choices=('0', '1'), default='0')
    ap.add_argument('--store', choices=('0', '1'), default='0')
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
                    original=dict(early='0', shift='0', store='0'), candidate=dict(early=args.early, shift=args.shift, store=args.store),
                    moments_ticks=list(TICKS), late_seconds=[60, 75], nav=rc.NAV,
                    race_pad=rc.RACE_PAD, seed=rc.RETAIL_RACE_SEED,
                    reference=str(args.reference.resolve()), race_timeout_seconds=600,
                    exclusions='unchanged gate_vu1_native.ee_compare and compare_readbacks')
    (out / 'method.json').write_text(json.dumps(metadata, indent=2) + '\n')
    fields = []
    for mode, early, shift, store in [('0', '0', '0', '0'), ('1', args.early, args.shift, args.store)]:
        directory = out / mode
        directory.mkdir()
        toggles = dict(DHD_GS_EARLY_DEPTH=early, DHD_GS_ADDRESS_SHIFT=shift,
                       DHD_GS_EXCLUSIVE_WORD_STORE=store)
        env = dict(toggles, DHD_VU1_NATIVE_STATS='1',
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
        menu_env.update(toggles, DHD_ISO=os.environ['DHD_ISO'], DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_FRAME_CLOCK='1',
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
    metadata.update(result='FAIL' if failures else 'PASS', final_capture_fields=fields, failures=failures)
    (out / 'result.json').write_text(json.dumps(metadata, indent=2) + '\n')
    if failures:
        raise AssertionError('; '.join(failures))
    print('NATIVE-VS-NATIVE SIX-MOMENT LATE GATE MATCH', flush=True)


if __name__ == '__main__':
    main()
