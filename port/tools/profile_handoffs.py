"""Summarize DHD_TIMELINE queue work/wait attribution (milliseconds per field).

Busy means wall time outside instrumented queue waits, including descheduling and
GPU/API waits. CPU includes spinning. Spin and park are subsets of queue wait;
neither is an additional deduction from busy. A park includes OS wakeup latency.
"""
import argparse
import csv
from pathlib import Path
import statistics


def summarize(path):
    with path.open(newline='') as stream:
        rows = list(csv.DictReader(stream))
    if not rows:
        raise ValueError(f'{path}: no fields')
    def mean(key):
        return statistics.fmean(float(row[key]) for row in rows)
    wall = sorted(float(row['wall_ms']) for row in rows)
    print(f'{path}: {len(rows)} fields, {1000 / mean("wall_ms"):.3f} fields/s, '
          f'p50={wall[len(wall)//2]:.3f}, p99={wall[min(len(wall)-1, len(wall)*99//100)]:.3f} ms')
    print('thread  outside_queue  queue_wait  spin_wall  park_wall  total_cpu')
    for name, waits in [('ee', ('ee_wait_vu', 'ee_wait_gs', 'ee_ring_full')),
                        ('vu', ('vu_idle', 'vu_gs_full')), ('gs', ('gs_idle',))]:
        wait = sum(mean(key) for key in waits)
        values = (mean('wall_ms') - wait, wait, mean(name + '_spin'),
                  mean(name + '_park'), mean(name + '_cpu_ms'))
        print(f'{name:6s}' + ''.join(f'{value:12.4f}' for value in values))
    print('wait detail: ' + ', '.join(f'{key}={mean(key):.4f}' for key in
          ('ee_wait_vu', 'ee_wait_gs', 'ee_ring_full', 'vu_idle', 'vu_gs_full', 'gs_idle')))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('timeline', type=Path, nargs='+')
    args = parser.parse_args()
    for path in args.timeline:
        summarize(path)


if __name__ == '__main__':
    main()
