"""Inventory data fixups against the ELF and independent EE RAM (outputs stay in build/).

This is a triage tool, not a proof that an address-shaped word is a pointer. In
particular, a splat D_ label alone is not evidence of an object boundary.
"""
import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess

import data
from pctarget import TARGET


def word(image, address):
    return struct.unpack_from('<I', image, address)[0]


def inventory(out, link_map, port_ram, retail_ram, historical=False):
    out.mkdir(parents=True, exist_ok=True)
    data.OUT = str(out)
    numeric_schemas = dict(instructions=data.INSTRUCTION_WORDS, tlb=data.TLB_WORDS,
                           text=data.TEXT_WORDS, inflate_extra=data.INFLATE_EXTRA_WORDS,
                           particle_timing={0x27D300 + i * 0x50 + 0x18 for i in range(77)})
    if historical:
        # Reconstruct the 4,419-entry pre-round-45 policy without changing any
        # tracked file. Keep round 41's view-descriptor correction in force.
        data.NUMBERS = data.NUMBERS - set().union(*numeric_schemas.values())
    paths, symbols, fixups = data.convert_all()
    llvm = Path.home() / 'scoop/apps/llvm/current/bin'
    clang = shutil.which('clang') or str(llvm / 'clang.exe')
    nm = shutil.which('llvm-nm') or str(llvm / 'llvm-nm.exe')
    obj = out / 'image.o'
    subprocess.run([clang, '--target=' + TARGET, '-c', paths[0], '-o', str(obj)], check=True)
    names = subprocess.check_output([nm, str(obj)], text=True).splitlines()
    assert not data.check_image(obj, symbols, names)
    path, count = data.write_fixups(obj, fixups, names)
    host = {}
    for line in link_map.read_text().splitlines():
        match = re.match(r'\s+[0-9A-Fa-f]+:[0-9A-Fa-f]+\s+(\S+)\s+([0-9A-Fa-f]{8,16})\s', line)
        if match:
            host[match[1]] = int(match[2], 16)
    for line in Path(paths[1]).read_text().splitlines():
        match = re.fullmatch(r'\.set (\S+), (0x[0-9A-Fa-f]+)', line)
        if match:
            host[match[1]] = int(match[2], 16)
    elf = data.retail_image()
    port, retail = port_ram.read_bytes(), retail_ram.read_bytes()
    starts = set(symbols.values())
    owners = {}
    for filename in data.FILES:
        if filename in data.ZERO:
            continue
        label = ''
        for line in (Path(data.ROOT) / 'asm/data' / filename).read_text().splitlines():
            if line.startswith('dlabel '):
                label = line.split()[1]
            match = data.ADDR_COMMENT.search(line)
            if match:
                owners[int(match[1], 16)] = (filename, label)
    rows = []
    for line in Path(path).read_text().splitlines():
        match = re.fullmatch(r'\.long (0x[0-9A-Fa-f]+), (.+)', line)
        if not match:
            continue
        address, operand = int(match[1], 16), match[2]
        original = word(elf, address - data.VUTEXT[0])
        resolved = host.get(operand)
        if resolved is None:
            terms = operand.split(' + ')
            resolved = sum(host[t] if t in host else int(t, 0) for t in terms)
        flags = []
        if operand.startswith('_D_'):
            target = int(operand.split()[0][3:], 16)
            if target not in starts:
                flags.append('target-not-data-label')
            if target & 0xFFFF <= 0x100 and target >> 16 <= 0x200:
                flags.append('packed-halfwords')
            if target >= data.IMAGE_END:
                flags.append('target-outside-initialized-image')
            kind = 'data-address'
        elif operand.startswith('_func_'):
            kind = 'code-pointer'
        elif resolved == 0:
            kind = 'exception-marker'
        else:
            kind = 'absolute-address'
        pv, rv = word(port, address), word(retail, address)
        if rv == original and pv == resolved:
            runtime = 'unchanged-relocation'
        elif pv == rv:
            runtime = 'equal-at-capture'
        elif rv < data.RAM_END and pv == data.PS2_RAM_BASE + rv:
            runtime = 'runtime-RAM-translation'
        elif 0x40000000 <= pv < 0x50000000 and 0x100000 <= rv < data.VUTEXT[0]:
            runtime = 'runtime-code-address-candidate'
        else:
            runtime = 'runtime-review'
        schema = next((name for name, slots in numeric_schemas.items() if address in slots), '')
        if resolved == original:
            decision = 'unchanged-value'
        elif schema:
            decision = 'wrong-relocation:' + schema
        elif kind == 'exception-marker':
            decision = 'exception-range-marker'
        else:
            decision = 'pointer'
        source, owner = owners[address]
        rows.append(dict(address=f'0x{address:08X}', retail_image=f'0x{original:08X}',
                         operand=operand, relocated=f'0x{resolved:08X}', kind=kind,
                         source=source, owner=owner, decision=decision,
                         target_data_label=original in starts, flags=';'.join(flags),
                         port_ram=f'0x{pv:08X}', retail_ram=f'0x{rv:08X}', runtime=runtime))
    assert len(rows) == count
    with (out / 'fixups.csv').open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)
    summary = dict(count=count, kinds=dict(Counter(r['kind'] for r in rows)),
                   decisions=dict(Counter(r['decision'] for r in rows)),
                   runtime=dict(Counter(r['runtime'] for r in rows)),
                   inputs=dict(link_map=str(link_map), port_ram=str(port_ram), retail_ram=str(retail_ram)))
    (out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary, indent=2))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--map', type=Path, required=True)
    parser.add_argument('--port-ram', type=Path, required=True)
    parser.add_argument('--retail-ram', type=Path, required=True)
    parser.add_argument('--historical', action='store_true', help='reconstruct the original 4,419 fixups')
    args = parser.parse_args()
    root = Path(data.ROOT).resolve()
    if root / 'build' not in args.out.resolve().parents:
        parser.error('--out must stay under build/')
    inventory(args.out.resolve(), args.map, args.port_ram, args.retail_ram, args.historical)
