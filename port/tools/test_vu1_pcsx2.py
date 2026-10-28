"""Compare VU1 with a pinned, independently compiled PCSX2 interpreter, both on the 64-bit host
(where VU1 runs: vu0_interp.c is a host unit).

Needs LLVM (testprog.py). Downloads public GPL reference sources into build/;
retail microcode comes only from the user's DHD_VUTEXT (or assets/vutext.bin).
Optional positional arguments are local DHD_VU_CAPTURE files. No game data is embedded.
Mesh fixtures exercise setup and an unclipped three/four-vertex MSCNT packet.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import urllib.request

import testprog
from vudis import walk

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/vu-oracle'
PIN = '0f71e668b8adc950db7de1b1b15c2f5080c102e9'
HASHES = {
    'VU.h': 'c77e856f2ebe46a42cb3f1856afdec9f3662ca51fc7950454060b3ca4b2edea9',
    'VUops.h': '624f9b0ec81847f1941826dd05400685fe82ded288de8dd6789122bd8fd07fb1',
    'VUflags.h': '36b407f06b83964cd15111c9fdc6bad061c0d2842f3b59717b426eab7d62767b',
    'VUops.cpp': '37d5098321e062956880637e90b6a975a8f28d88092a2f333f8f0d2e3750bd16',
    'VUflags.cpp': 'b839d688b6c0085644b7ab90957a9d3c766862e41a578260b7f47d9755aed0fe',
    'VU1microInterp.cpp': 'c08627212c76be223586bd7bac5587b92ff7e818c6acd1fe23e8e94d0b8f9d5d',
}
ENTRIES = (
    (0x265C00, '50 2d0 850 e10 1010 1240 17c8 1f50 22c8 26d8 29c0 2e48 30f8'),
    (0x269C50, '78 280 6f0 7f0 948 a90 c20 1050 12f0 1430 1638 17e0 2738 28d0 2b10 2c20 50 2dc8 30a0'),
)


def build(aot=False):
    upstream = OUT / 'upstream'
    upstream.mkdir(parents=True, exist_ok=True)
    for name, digest in HASHES.items():
        path = upstream / name
        if not path.exists():
            url = f'https://raw.githubusercontent.com/PCSX2/pcsx2/{PIN}/pcsx2/{name}'
            path.write_bytes(urllib.request.urlopen(url, timeout=30).read())
        data = path.read_bytes()
        if hashlib.sha256(data).hexdigest() != digest:
            raise RuntimeError(f'PCSX2 source hash mismatch: {path}')
        # Replace platform headers with the adapter; retain instruction/pipeline code.
        source = re.sub(r'^#include "[^"]+".*$', '', data.decode(), flags=re.M)
        if name == 'VUops.cpp':
            source = source[:source.index('//  VU0macro (COP2)')]
        if name == 'VU1microInterp.cpp':
            source = source[:source.index('InterpVU1 CpuIntVU1;')]
        (OUT / name).write_text(source, encoding='utf-8')
    interp = ROOT / 'port/runtime/vu0_interp.c'
    defines, reference = [], []
    if aot:
        reference = [testprog.host_object(interp, OUT, 'reference', defines=[
            'PS2_VU_REFERENCE', 'ps2_vu0_run=ps2_vu0_reference', 'ps2_vu1_run=ps2_vu1_reference',
            'ps2_vu_aot_load=ps2_vu_aot_reference_load'])]
        from vu1_aot import generate
        print('VU1 AOT:', generate(OUT / 'vu1_aot.inc'), 'straight-line blocks', flush=True)
        defines = ['PS2_VU_AOT', 'PS2_VU_AOT_EMBED', 'PS2_VU_AOT_TEST']
    # The oracle compares complete final state; the optional textual pair tracing is extra.
    native = testprog.host_object(interp, OUT, 'native', includes=[OUT], defines=defines + ['PS2_VU_TRACE'])
    oracle = testprog.host_object(ROOT / 'port/runtime/pcsx2_vu_oracle.cpp', OUT, 'oracle', includes=[OUT],
                                  defines=defines)
    testprog.host('oracle', [oracle, native, *reference, testprog.HOST_FP], out=OUT, low=aot)  # AOT: guest_base
    if aot:
        # Synthetic programs also exercise the fallback for pairs absent from
        # the retail overlays, mixed with matching pairs in the same run.
        exe = testprog.host('aot-instructions', [native, ROOT / 'port/runtime/test_vu1.c', ROOT / 'port/gs/memory.c',
                                                 ROOT / 'port/gs/gs.c', testprog.HOST_FP], out=OUT, includes=[OUT], defines=defines,
                            low=True)
        testprog.run(exe)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seeds', type=int, default=32, help='seeded float fixtures per entry')
    parser.add_argument('--aot', action='store_true', help='compare generated instruction pairs with PCSX2')
    parser.add_argument('captures', nargs='*')
    args = parser.parse_args()
    build(args.aot)
    data = Path(os.environ.get('DHD_VUTEXT', ROOT / 'assets/vutext.bin')).read_bytes()
    results = []

    def check(label, command):
        run = subprocess.run([str(OUT / ('oracle' + testprog.EXE)), *command], capture_output=True, text=True, timeout=30)
        results.append(dict(case=label, result=run.returncode, output=run.stdout, error=run.stderr))
        if run.returncode:
            print(label, run.stdout, run.stderr)

    for package, entries in ENTRIES:
        code = bytearray(16384)
        for address, chunk in walk(data, package)[1]:
            code[address:address + len(chunk)] = chunk
        path = OUT / f'overlay-{package:08X}.bin'
        path.write_bytes(code)
        for entry in entries.split():
            for seed in [0, *range(2, args.seeds + 2)]:
                check(f'{package:08X}:{entry}:{seed}', ['--entry', str(path), entry, str(seed)])
                if package == 0x269C50 and entry not in ('50', '2dc8', '30a0'):
                    check(f'{package:08X}:{entry}:draw:{seed}', ['--entry', str(path), entry, str(seed), '--draw'])
    for capture in args.captures:
        check(capture, [str(Path(capture).resolve())])
    (OUT / 'results.json').write_text(json.dumps(dict(pcsx2=PIN, cases=results), indent=2))
    failed = sum(case['result'] != 0 for case in results)
    print(f'PCSX2 VU1: {len(results)} cases, 32 entries, {failed} failures.')
    return bool(failed)


if __name__ == '__main__':
    raise SystemExit(main())
