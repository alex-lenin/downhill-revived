"""Compare VU0 micro mode with a pinned, independently compiled PCSX2 interpreter.

Requires clang-cl/x86 libraries. Downloads public GPL reference sources into build/ (the same
pin as test_vu1_pcsx2.py, plus VU0microInterp.cpp). Inputs are local DHD_VU0_CAPTURE files
(registers, VU0 code/data and the VU1 register file); they hold game microcode and stay under
build/. Each capture is also re-run with seeded data memory to cover more paths.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import urllib.request

import test_vu1_pcsx2 as vu1

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/vu0-oracle'
CAPTURES = ROOT / 'build/port/vu0-captures'
HASHES = dict(vu1.HASHES, **{'VU0microInterp.cpp': 'b68d9d4e09bddd5cbb28e3da1d8cb8c0710ae6d4d40dd58e4a983e5a606a4f96'})
HASHES.pop('VU1microInterp.cpp')
# Entries started by func_0021B7E8/func_0021B8A8 (finding 018) and func_00263B48 (0x4F0).
ENTRIES = (0x0, 0x180, 0x238, 0x2E0, 0x4F0, 0x5E8, 0x7B8, 0x808, 0x8D0, 0xB40, 0xBD0, 0xC50, 0xCD0)


def build():
    upstream = OUT / 'upstream'
    upstream.mkdir(parents=True, exist_ok=True)
    for name, digest in HASHES.items():
        path = upstream / name
        if not path.exists():
            url = f'https://raw.githubusercontent.com/PCSX2/pcsx2/{vu1.PIN}/pcsx2/{name}'
            path.write_bytes(urllib.request.urlopen(url, timeout=30).read())
        data = path.read_bytes()
        if hashlib.sha256(data).hexdigest() != digest:
            raise RuntimeError(f'PCSX2 source hash mismatch: {path}')
        source = re.sub(r'^#include "[^"]+".*$', '', data.decode(), flags=re.M)
        if name == 'VUops.cpp':
            source = source[:source.index('//  VU0macro (COP2)')]
        if name == 'VU0microInterp.cpp':
            source = source[:source.index('InterpVU0 CpuIntVU0;')]
        (OUT / name).write_text(source, encoding='utf-8')
    flags = ['clang-cl', '-m32', '/nologo', '/O2', '-Wno-deprecated-declarations',
             '-msse2', '-mfpmath=sse', '/clang:-ffp-contract=off']
    subprocess.run(flags + ['/c', str(ROOT / 'port/runtime/vu0_interp.c'), '/Fonative.obj'], cwd=OUT, check=True)
    subprocess.run(flags + ['/std:c++20', '/I' + str(OUT), '/I' + str(ROOT / 'port/runtime'), '/c',
                            str(ROOT / 'port/runtime/pcsx2_vu0_oracle.cpp'), '/Fooracle.obj'], cwd=OUT, check=True)
    subprocess.run(['clang-cl', '-m32', '/nologo', 'oracle.obj', 'native.obj', '/Feoracle.exe'], cwd=OUT, check=True)


def seeded(capture, seed):
    """The capture with data memory floats varied (low 16 bits kept: counts and indices)."""
    data = bytearray(capture.read_bytes())
    header = struct.unpack_from('<4I', data)
    base = 16 + header[1] + 4096
    random = seed
    for i in range(1024):
        random = (random * 1664525 + 1013904223) & 0xffffffff
        word, = struct.unpack_from('<I', data, base + i * 4)
        if (word >> 23 & 0xff) not in (0, 0xff):  # normal floats only; keep integers and zeros
            struct.pack_into('<I', data, base + i * 4, word ^ ((random >> 9 & 127) << 8))
    path = OUT / 'seeded' / f'{capture.stem}-{seed}.bin'
    path.parent.mkdir(exist_ok=True)
    path.write_bytes(data)
    return path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seeds', type=int, default=4)
    parser.add_argument('captures', nargs='*', type=Path)
    args = parser.parse_args()
    captures = args.captures or sorted(CAPTURES.glob('*.bin'))
    if not captures:
        print('PCSX2 VU0: no DHD_VU0_CAPTURE inputs under', CAPTURES, '- skipped.')
        return 0
    build()
    results, entries = [], {}
    for capture in captures:
        address = struct.unpack_from('<I', capture.read_bytes())[0]
        for path in [capture] + [seeded(capture, s) for s in range(1, args.seeds + 1)]:
            run = subprocess.run([str(OUT / 'oracle.exe'), str(path)], capture_output=True, text=True, timeout=30)
            results.append(dict(case=path.name, entry=address, result=run.returncode, output=run.stdout, error=run.stderr))
            entries.setdefault(address, [0, 0])[run.returncode != 0] += 1
            if run.returncode:
                print(path.name, run.stdout.strip(), run.stderr.strip())
    (OUT / 'results.json').write_text(json.dumps(dict(pcsx2=vu1.PIN, cases=results), indent=2))
    for address in sorted(entries):
        ok, bad = entries[address]
        print(f'  entry {address:04X}: {ok} match, {bad} differ')
    missing = [f'{e:04X}' for e in ENTRIES if e not in entries]
    failed = sum(case['result'] != 0 for case in results)
    print(f'PCSX2 VU0: {len(results)} cases, {len(entries)} entries, {failed} failures.'
          + (f' Not captured: {" ".join(missing)}.' if missing else ''))
    return bool(failed)


if __name__ == '__main__':
    raise SystemExit(main())
