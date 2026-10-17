"""Summarize DHD_PROFILE samples using the matching lld map (no symbols guessed).

usage: python port/tools/profile.py build/port/menu.profile --after 60 --before 90
EIP samples include driver/wait time. Stack words are possible return addresses,
not an unwound call tree. Menu ticks measure game progress independently of the
display refresh rate; the counter resets on menu transitions.
"""
import argparse
from bisect import bisect_right
from collections import Counter
from pathlib import Path
import re
import struct

ap = argparse.ArgumentParser(__doc__)
ap.add_argument('trace', type=Path)
ap.add_argument('--map', type=Path, default=Path('build/port/link/dhd.map'))
ap.add_argument('--after', type=float, default=0)
ap.add_argument('--before', type=float, default=float('inf'))
ap.add_argument('--fields', help='first-last virtual fields (DHP4 traces), e.g. the DHD_BENCH window')
ap.add_argument('--top', type=int, default=20)
ap.add_argument('--callers', help='regex: for samples whose IP is in a matching symbol, the first stack '
                'return address outside matching symbols (e.g. the game code calling the asm runtime)')
ap.add_argument('--addresses', help='regex: per-address IP histogram inside matching symbols (symbol+offset), '
                'to read against a disassembly of dhd.exe')
a = ap.parse_args()
symbols = []
for line in a.map.read_text().splitlines():
    m = re.match(r'\s+0001:[0-9a-f]+\s+(\S+)\s+([0-9a-f]{8,16})\s+(?:f\s+)?(.+)', line, re.I)
    if m: symbols.append((int(m[2], 16), m[1], m[3]))
symbols.sort()
addresses = [s[0] for s in symbols]
def name(ip):
    i = bisect_right(addresses, ip) - 1
    return symbols[i][1] + ' [' + symbols[i][2] + ']' if i >= 0 and ip < addresses[-1] + 4096 else 'Windows/driver/wait'

data = a.trace.read_bytes()
size = 528
counter = {b'DHP1': 'game-time ticks', b'DHP2': 'menu frames', b'DHP3': 'display-buffer changes',
           b'DHP4': 'virtual fields'}.get(data[:4], 'game-time ticks')
if data[:4] in (b'DHP1', b'DHP2', b'DHP3', b'DHP4'): data, size = data[4:], 80
samples = [s for s in struct.iter_unpack('<%dI' % (size//4), data[:len(data)//size*size]) if a.after*1000 <= s[0] < a.before*1000]
if a.fields:
    lo, hi = map(int, a.fields.split('-'))
    samples = [s for s in samples if lo <= s[2] < hi]
eip, caller = Counter(), Counter()
for s in samples:
    eip[name(s[1])] += 1
    if name(s[1]) == 'Windows/driver/wait':
        for ip in s[4:]:
            n = name(ip)
            if n != 'Windows/driver/wait':
                caller[n] += 1
                break
if a.callers:
    pattern = re.compile(a.callers)
    via = Counter()
    for s in samples:
        if pattern.search(name(s[1])):
            for ip in s[4:]:
                n = name(ip)
                if not pattern.search(n) and n != 'Windows/driver/wait':
                    via[n] += 1
                    break
            else:
                via['(none on the stack)'] += 1
print('%d samples' % len(samples))
if a.addresses:
    pattern = re.compile(a.addresses)
    at = Counter()
    for s in samples:
        n = name(s[1])
        if pattern.search(n):
            i = bisect_right(addresses, s[1]) - 1
            at['%08X %s+0x%X' % (s[1], symbols[i][1], s[1] - symbols[i][0])] += 1
    print('IP addresses in /%s/' % a.addresses)
    for n, count in at.most_common(a.top): print('%5.1f%% %s' % (100*count/max(1,len(samples)), n))
tables = [('Instruction pointer', eip), ('First possible game return address during external work', caller)]
if a.callers:
    tables.append(('First return address outside /%s/ (samples inside it)' % a.callers, via))
for title, counts in tables:
    print(title)
    for n, count in counts.most_common(a.top): print('%5.1f%% %s' % (100*count/max(1,len(samples)), n))
if samples:
    print('time, menu state, %s (first/last per second):' % counter)
    groups = {}
    for s in samples:
        key = (s[0]//1000, s[3])
        if key not in groups: groups[key] = [s[2],s[2]]
        groups[key][1] = s[2]
    for (second,state), ticks in groups.items(): print(second, state, *ticks)
