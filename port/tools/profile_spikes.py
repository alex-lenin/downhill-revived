"""Tag each benchmark field with overlapping opt-in DHD_SPIKES events.

Usage: python port/tools/profile_spikes.py RUN_DIRECTORY
RUN_DIRECTORY contains game.log and timeline.csv (DHD_TIMELINE).
Writes causes.csv and causes.json. GPU events are aligned through the precise
UTC/host clock pair recorded at the beginning of DHD_BENCH. Tags describe
coincidence, not proof of critical-path causation. A picture runs asynchronously;
a long readback can include OS descheduling. Cycle-based scheduling tags are
inferences, not context-switch traces. Sub-millisecond GPU calls are omitted
except pipeline creation. Sound should be read from the bench snd line.
"""
from pathlib import Path
import csv
import re
import bisect
import statistics
import json
import argparse


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('directory', type=Path, help='game.log and DHD_TIMELINE timeline.csv directory')
    p = ap.parse_args().directory
    log = (p / 'game.log').read_text(errors='replace')
    rows = [{k: float(v) for k, v in r.items()} for r in csv.DictReader((p / 'timeline.csv').open())]
    clock = re.search('bench timeline: clock field=\\d+ host_s=([\\d.]+)', log)
    if not rows or clock is None:
        ap.error('a nonempty timeline and benchmark clock are required')
    start = float(clock[1])
    ends = []
    for r in rows:
        start += r['wall_ms'] / 1000
        ends.append(start)
    clock = re.search('bench timeline: clock unix_s=([\\d.]+) host_s=([\\d.]+)', log)
    if clock is None:
        ap.error('the precise UTC/host clock pair is required (Windows DHD_TIMELINE)')
    off = float(clock[1]) - float(clock[2])
    events = []
    for m in re.finditer('spike_queue: start=([\\d.]+) ms=([\\d.]+) moved=(\\d+)', log):
        t, ms = (float(m[1]), float(m[2]))
        events.append((t, t + ms / 1000, 'queue_timeout_' + ('moved' if m[3] == '1' else 'unchanged'), ms))
    for m in re.finditer('spike_gpu: end_unix=([\\d.]+) ms=([\\d.]+) tag=(\\w+)', log):
        end, ms = (float(m[1]) - off, float(m[2]))
        events.append((end - ms / 1000, end, m[3], ms))
    for m in re.finditer('spike_io: start=([\\d.]+) ms=([\\d.]+) bytes=(\\d+)', log):
        t, ms = (float(m[1]), float(m[2]))
        events.append((t, t + ms / 1000, 'io', ms))
    for m in re.finditer('spike_present: field=\\d+ start=([\\d.]+) acquire_ms=([\\d.]+) pace_present_ms=([\\d.]+)', log):
        t, ms = (float(m[1]), float(m[2]) + float(m[3]))
        events.append((t, t + ms / 1000, 'present', ms))
    tags = [set() for r in rows]
    counts = {}
    maxima = {}
    window_start = ends[0] - rows[0]['wall_ms'] / 1000
    for a, b, tag, ms in events:
        if b < window_start or a > ends[-1]:
            continue
        counts[tag] = counts.get(tag, 0) + 1
        maxima[tag] = max(maxima.get(tag, 0), ms)
        if ms < 1:
            continue
        for i in range(bisect.bisect_left(ends, a), min(len(rows), bisect.bisect_right(ends, b) + 1)):
            tags[i].add(tag)
    med = statistics.median((r['ee_mcycles'] for r in rows))
    for i, r in enumerate(rows):
        if r['ee_mcycles'] > 1.35 * med:
            tags[i].add('ee_high_cycles')
        busy = r['wall_ms'] - r['ee_wait_vu'] - r['ee_wait_gs'] - r['ee_ring_full']
        if busy > 15 and r['ee_mcycles'] < 1.35 * med:
            tags[i].add('ee_descheduled_or_uninstrumented')
        if not tags[i]:
            tags[i].add('unattributed')
    threshold = sorted((r['wall_ms'] for r in rows))[len(rows) * 99 // 100]
    top = [i for i, r in enumerate(rows) if r['wall_ms'] >= threshold]
    summary = {
        'fields': len(rows),
        'p50': statistics.median((r['wall_ms'] for r in rows)),
        'p99': threshold,
        'max': max((r['wall_ms'] for r in rows)),
        'events': counts,
        'event_max_ms': maxima,
        'top1pct_count': len(top),
        'top1pct_tags': {t: sum((t in tags[i] for i in top)) for t in sorted(set.union(*(tags[i] for i in top)))},
        'cycles_all_vs_top': {k: [statistics.mean((r[k] for r in rows)), statistics.mean((rows[i][k] for i in top))] for k in rows[0] if k != 'field'}}
    (p / 'causes.json').write_text(json.dumps(summary, indent=2))
    print(json.dumps(summary, indent=2))
    with (p / 'causes.csv').open('w', newline='') as f:
        w = csv.writer(f)
        w.writerow(['field', 'wall_ms', 'tags'])
        for r, t in zip(rows, tags):
            w.writerow([int(r['field']), r['wall_ms'], '|'.join(sorted(t))])


if __name__ == '__main__':
    main()
