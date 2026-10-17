"""Bounded native-on/off race, image and exact GPU readback gates."""
import argparse
import os
import re
from pathlib import Path
import struct
import subprocess
import time
import numpy as np
import race_compare as rc

ROOT = Path(__file__).resolve().parents[2]
MOMENTS = [(f'g+{n}', 'g', n) for n in (0, 2, 12, 16)]

def wait_for(process, path, timeout):
    start = time.monotonic()
    try:
        while not path.exists():
            if process.poll() is not None:
                raise RuntimeError(f'game exited {process.returncode}')
            if time.monotonic() - start > timeout:
                raise TimeoutError(str(path))
            time.sleep(.5)
        time.sleep(2)
    finally:
        rc.compare.stop(process)

def ee_compare(a, b):
    x, y = [np.frombuffer(p.read_bytes(), dtype='<u4') for p in (a, b)]
    if x.shape != y.shape:
        raise AssertionError('EE dump sizes differ')
    def pointer(v):
        return ((v >= 0x40000000) & (v < 0x42000000)) | ((v >= 0x01000000) & (v < 0x08000000))
    bad = (x != y) & ~(pointer(x) & pointer(y))
    bad[0x2b461c // 4] = False  # Existing ref22 run-varying word exclusion.
    offsets = np.flatnonzero(bad)
    print(b.name, 'EE MATCH' if not len(offsets) else f'EE DIFF {len(offsets)} words, first {offsets[:4]*4}', flush=True)
    if len(offsets): raise AssertionError('EE mismatch under existing ref22 exclusions')

def images(a, b):
    files = sorted(a.glob('*.bmp'))[:4]
    if len(files) != 4: raise AssertionError('expected four frames')
    for p in files:
        if p.read_bytes() != (b / p.name).read_bytes():
            raise AssertionError(f'BMP byte mismatch: {p}')
        print(p.name, 'BMP BYTE MATCH', flush=True)

def readbacks(path):
    data = path.read_bytes(); pos = 0; records = []
    while pos + 32 <= len(data):
        field, count, blt, xy, size = struct.unpack_from('<IIQQQ', data, pos)
        end = pos + 32 + count
        if end > len(data): break  # Driver may stop during the trailing record.
        records.append((field, blt, xy, size, data[pos+32:end])); pos = end
    if not records: raise AssertionError('readback trace is empty')
    return records

def compare_readbacks(a, b, required_field):
    x, y = readbacks(a), readbacks(b)
    last = min(x[-1][0], y[-1][0]) - 1  # Exclude the last possibly incomplete field.
    if last < required_field: raise AssertionError("readbacks do not cover the final race capture")
    x, y = [[r for r in records if r[0] <= last] for records in (x, y)]
    if x != y:
        first = next((i for i, pair in enumerate(zip(x, y)) if pair[0] != pair[1]), min(len(x), len(y)))
        raise AssertionError(f'readback mismatch record {first}; counts {len(x)}/{len(y)}')
    z = sum((r[1] >> 24 & 63) in (48, 49, 50, 58) for r in x)
    if not z: raise AssertionError('no Z readbacks captured')
    print(f'READBACK MATCH: {len(x)} records, {z} Z reads, through virtual GS field {last}', flush=True)

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', required=True, type=Path)
    ap.add_argument('--control-run', type=Path, help='reuse an already gated control run with port-ee, port, menu and readbacks.bin')
    ap.add_argument('--off-exe', type=Path, help='optional separate control executable for compiler comparisons')
    ap.add_argument('--switch-values', nargs=2, default=['0', '1'], help='control and candidate values; use 1 1 to compare builds')
    ap.add_argument('--out', required=True, type=Path)
    ap.add_argument('--reference', type=Path, default=ROOT/'build/port/compare/ref22')
    ap.add_argument('--env', action='append', default=[], help='additional NAME=VALUE for both modes')
    ap.add_argument('--switch', default='DHD_VU1_NATIVE', help='environment option compared as 0 versus 1')
    args = ap.parse_args()
    out = args.out.resolve()
    if ROOT/'build' not in out.parents: ap.error('output must stay under build/')
    extra = dict(v.split('=', 1) for v in args.env)
    failures = []
    def checked(function, *arguments):
        try:
            function(*arguments)
        except AssertionError as error:
            failures.append(f'{function.__name__}: {error}')
            print('GATE FAIL:', failures[-1], flush=True)
    out.mkdir(parents=True, exist_ok=False)
    control = args.control_run.resolve() if args.control_run else out / "0"
    for mode in ((1,) if args.control_run else (0, 1)):
        d = out / str(mode); d.mkdir()
        exe = (args.off_exe if mode == 0 and args.off_exe else args.exe).resolve()
        env = dict(extra, DHD_VU1_NATIVE_STATS='1',
                   DHD_GS_READBACK_TRACE=str(d/'readbacks.bin'))
        env[args.switch] = args.switch_values[mode]
        run = argparse.Namespace(nav=rc.NAV, exe=exe, race_pad=rc.RACE_PAD,
                                 window=0, race_seed=rc.RETAIL_RACE_SEED, dump=True, port_env=env)
        p = rc.launch_port(run, d, rc.clock_events(rc.RACE_PAD), MOMENTS)[0]
        wait_for(p, d/'port-ee/ee-g+16.bin', 600)
        for name, _, _ in MOMENTS:
            checked(ee_compare, args.reference/'port-ee'/f'ee-{name}.bin', d/'port-ee'/f'ee-{name}.bin')
        md = d/'menu'; md.mkdir()
        env = {k:v for k,v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
        env.update(extra, DHD_ISO=os.environ['DHD_ISO'], DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_FRAME_CLOCK='1',
                   DHD_SND_OFF='1', DHD_NAV='0:triangle|cross', DHD_RACE_SEED=str(rc.RETAIL_RACE_SEED),
                   DHD_SCREENSHOT=str(md/'frame'), DHD_CAPTURE_FRAME='600', DHD_CAPTURE_FRAME_INTERVAL='60')
        env[args.switch] = args.switch_values[mode]
        with (md/'run.log').open('wb') as log:
            p = subprocess.Popen([str(exe)], cwd=md, env=env, stdout=log, stderr=subprocess.STDOUT)
            wait_for(p, md/'frame-003.bmp', 180)
    for name, _, _ in MOMENTS:
        checked(ee_compare, control/'port-ee'/f'ee-{name}.bin', out/'1/port-ee'/f'ee-{name}.bin')
    checked(images, control/'port', out/'1/port'); checked(images, control/'menu', out/'1/menu')
    fields = [int(re.search(r'moment=g\+16 delta=0 field=(\d+)', (directory/'port.log').read_text(errors='replace')).group(1)) for directory in (control, out/'1')]
    checked(compare_readbacks, control/'readbacks.bin', out/'1/readbacks.bin', max(fields))
    if failures:
        raise AssertionError('; '.join(failures))
    print('NATIVE LIVE GATES MATCH', flush=True)

if __name__ == '__main__':
    main()
