"""Compare every GS readback on its original field, with no tolerance or field shift.

    python port/tools/probe_trace_diff.py before.bin after.bin
    python port/tools/probe_trace_diff.py --selftest

DHD_GS_READBACK_TRACE writes a 32-byte little-endian header (field, payload size,
BITBLTBUF, TRXPOS, TRXREG), then the bytes returned to the game. Unlike
zprobe_compare.py, this checks actual values and retains multiple reads per field.
It rejects empty and truncated transcripts. No game data is included here.
"""
import argparse
import struct
from pathlib import Path

HEADER = struct.Struct('<IIQQQ')


def records(data):
    out = []
    offset = 0
    while offset < len(data):
        if len(data) - offset < HEADER.size:
            raise ValueError(f'truncated header at byte {offset}')
        field, size, blt, pos, trx = HEADER.unpack_from(data, offset)
        offset += HEADER.size
        if size > len(data) - offset:
            raise ValueError(f'truncated payload at field {field}, byte {offset}')
        out.append((field, blt, pos, trx, data[offset:offset + size]))
        offset += size
    if not out:
        raise ValueError('empty transcript')
    return out


def compare(before, after):
    mismatches = [(i, a[0], b[0]) for i, (a, b) in enumerate(zip(before, after)) if a != b]
    return mismatches, abs(len(before) - len(after))


def selftest():
    def record(field, payload):
        return HEADER.pack(field, len(payload), 0x30000000, 0, 1) + payload
    data = record(3, b'abcd') + record(3, b'efgh') + record(4, b'ijkl')
    original = records(data)
    assert compare(original, records(data)) == ([], 0)
    # A second read in the same field must not overwrite the first.
    changed = records(record(3, b'abcd') + record(3, b'efgX') + record(4, b'ijkl'))
    assert compare(original, changed) == ([(1, 3, 3)], 0)
    assert compare(original, original[:-1]) == ([], 1)
    assert compare(original, records(record(4, b'abcd'))) == ([(0, 3, 4)], 2)
    for broken in (b'', data[:-1], data + b'x'):
        try:
            records(broken)
        except ValueError:
            pass
        else:
            raise AssertionError('accepted an empty or truncated transcript')
    print('PASS: identity, same-field reads, changed values, missing fields, truncation')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('before', nargs='?', type=Path)
    ap.add_argument('after', nargs='?', type=Path)
    ap.add_argument('--selftest', action='store_true')
    args = ap.parse_args()
    if args.selftest:
        selftest()
        return 0
    if not args.before or not args.after:
        ap.error('provide before and after transcripts')
    try:
        before, after = (records(p.read_bytes()) for p in (args.before, args.after))
    except (OSError, ValueError) as error:
        print(f'FAIL: {error}')
        return 1
    mismatches, missing = compare(before, after)
    print(f'{"FAIL" if mismatches or missing else "PASS"}: '
          f'{len(before)}/{len(after)} reads; {len(mismatches)} differing, {missing} unmatched')
    for index, a, b in mismatches[:10]:
        print(f'  read {index}: before field {a}, after field {b}')
    return int(bool(mismatches or missing))


if __name__ == '__main__':
    raise SystemExit(main())
