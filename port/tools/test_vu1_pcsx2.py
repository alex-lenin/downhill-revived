"""Compare VU1 with a pinned, independently compiled PCSX2 interpreter.

Requires clang-cl/x86 libraries. Downloads public GPL reference sources into build/;
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


def build(aot=False, isa="sse2", thinlto=False, pgo_use=None):
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
    flags = ['clang-cl', '-m32', '/nologo', '/O2', '-Wno-deprecated-declarations',
             '-m' + isa, '-mno-fma', '-mfpmath=sse', '/clang:-ffp-contract=off']
    if thinlto: flags += ['-flto=thin', '-fuse-ld=lld']
    reference = []
    if aot:
        subprocess.run(flags + ['/DPS2_VU_REFERENCE', '/Dps2_vu0_run=ps2_vu0_reference', '/Dps2_vu1_run=ps2_vu1_reference',
                                '/Dps2_vu_aot_load=ps2_vu_aot_reference_load',
                                '/c', str(ROOT / 'port/runtime/vu0_interp.c'), '/Foreference.obj'],
                       cwd=OUT, check=True)
        reference = ['reference.obj']
        from vu1_aot import generate
        print('VU1 AOT:', generate(OUT / 'vu1_aot.inc'), 'straight-line blocks', flush=True)
        flags += ['/DPS2_VU_AOT', '/DPS2_VU_AOT_EMBED', '/DPS2_VU_AOT_TEST', '/I' + str(OUT)]
    native_flags = ['-fprofile-instr-use=' + str(pgo_use.resolve()), '-Werror=profile-instr-out-of-date'] if pgo_use else []
    # Match the trained runtime's trace_pair body for PGO. The oracle compares
    # complete final state; optional textual pair tracing is not part of that check.
    if pgo_use and os.environ.get('DHD_VU_TRACE'):
        raise ValueError('PGO oracle requires the normal, non-tracing runtime')
    trace_flags = [] if pgo_use else ['/DPS2_VU_TRACE']
    subprocess.run(flags + native_flags + trace_flags + ['/c', str(ROOT / 'port/runtime/vu0_interp.c'),
                            '/Fonative.obj'], cwd=OUT, check=True)
    subprocess.run(flags + ['/std:c++20', '/I' + str(OUT), '/c',
                            str(ROOT / 'port/runtime/pcsx2_vu_oracle.cpp'), '/Fooracle.obj'], cwd=OUT, check=True)
    subprocess.run(['clang-cl', '-m32', '/nologo', '-fuse-ld=lld', 'oracle.obj', 'native.obj', *reference, '/Feoracle.exe', '/link', '/threads:6', '/opt:lldltojobs=6'],
                   cwd=OUT, check=True)
    if aot:
        # Synthetic programs also exercise the fallback for pairs absent from
        # the retail overlays, mixed with matching pairs in the same run.
        subprocess.run(flags + ['native.obj', str(ROOT / 'port/runtime/test_vu1.c'),
                                str(ROOT / 'port/gs/memory.c'), str(ROOT / 'port/gs/gs.c'),
                                '/Feaot-instructions.exe', '/link', '/threads:6', '/opt:lldltojobs=6'], cwd=OUT, check=True)
        subprocess.run([str(OUT / 'aot-instructions.exe')], cwd=OUT, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seeds', type=int, default=32, help='seeded float fixtures per entry')
    parser.add_argument('--thinlto', action='store_true')
    parser.add_argument('--pgo-use', type=Path)
    parser.add_argument('--isa', choices=('sse2', 'sse4.1', 'avx2'), default='sse2')
    parser.add_argument('--aot', action='store_true', help='compare generated instruction pairs with PCSX2')
    parser.add_argument('captures', nargs='*')
    args = parser.parse_args()
    build(args.aot, args.isa, args.thinlto, args.pgo_use)
    data = Path(os.environ.get('DHD_VUTEXT', ROOT / 'assets/vutext.bin')).read_bytes()
    results = []

    def check(label, command):
        run = subprocess.run([str(OUT / 'oracle.exe'), *command], capture_output=True, text=True, timeout=30)
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
