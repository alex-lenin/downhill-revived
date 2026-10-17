"""Explain every r46 gate difference using the historical inventory and retail RAM.

Uses the existing gate's pointer equivalence and single run-varying exclusion;
does not change the gate or accept unexplained differences. Writes only build/.
"""
import argparse
import csv
import json
from pathlib import Path
import numpy as np

from gate_gs_optimization import ROOT


def audit(inventory, reference, run, retail_ram, out):
    rows = list(csv.DictReader(inventory.open()))
    corrections = {int(row['address'], 16): row['decision'].split(':')[1]
                   for row in rows if row['decision'].startswith('wrong-relocation:')}
    # Four type-0x8003 slopes derived at startup from the corrected durations.
    corrections.update({address: 'particle-derived-slope' for address in
                        (0x27E158, 0x27E478, 0x27E4C8, 0x27E608)})
    retail = np.frombuffer(retail_ram.read_bytes(), dtype='<u4')
    result = {}
    for tick in (0, 2, 12, 16):
        old = np.frombuffer((reference / 'port-ee' / f'ee-g+{tick}.bin').read_bytes(), dtype='<u4')
        for arm in ('exact', 'native'):
            new = np.frombuffer((run / arm / 'port-ee' / f'ee-g+{tick}.bin').read_bytes(), dtype='<u4')
            assert old.shape == new.shape == retail.shape
            def pointer(value):
                return ((value >= 0x40000000) & (value < 0x42000000)) | \
                       ((value >= 0x01000000) & (value < 0x08000000))
            bad = (old != new) & ~(pointer(old) & pointer(new))
            bad[0x2B461C // 4] = False
            indices = np.flatnonzero(bad)
            addresses = {int(index) * 4 for index in indices}
            assert addresses == set(corrections), (tick, arm, addresses ^ set(corrections))
            assert np.array_equal(new[indices], retail[indices]), (tick, arm, 'retail mismatch')
            result[f'{arm}:{tick}'] = [dict(address=f'0x{index * 4:08X}',
                                          reference=f'0x{old[index]:08X}', corrected=f'0x{new[index]:08X}',
                                          retail=f'0x{retail[index]:08X}', reason=corrections[int(index) * 4])
                                      for index in indices]
    out.write_text(json.dumps(result, indent=2) + '\n')
    print(f'{len(corrections)} intended words at every checkpoint in both arms; all match retail RAM')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('inventory', 'reference', 'run', 'retail-ram', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if ROOT / 'build' not in args.out.resolve().parents:
        parser.error('--out must stay below build/')
    audit(args.inventory, args.reference, args.run, args.retail_ram, args.out)
