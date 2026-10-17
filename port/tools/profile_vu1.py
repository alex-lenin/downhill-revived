"""Summarize DHD_VU1_PROFILE windows without counting each key as another frame."""
import argparse
import collections
import csv
from pathlib import Path


def summarize(path):
    totals = collections.defaultdict(lambda: [0, 0.0, 0.0])
    windows = {}
    with Path(path).open(newline='') as f:
        for row in csv.DictReader(f):
            window = (float(row['start']), float(row['end']))
            frames = int(row['frames'])
            if window in windows and windows[window] != frames:
                raise ValueError('inconsistent frame count within profile window')
            windows[window] = frames
            key = (int(row['overlay']), int(row['entry'], 16), int(row['address'], 16), int(row['mscnt']))
            total = totals[key]
            total[0] += int(row['calls'])
            total[1] += float(row['vu_ms'])
            total[2] += float(row['gif_ms'])
    duration = sum(end - start for start, end in windows)
    frames = sum(windows.values())
    if duration <= 0 or frames <= 0:
        raise ValueError('profile has no complete frame windows')
    return totals, duration, frames


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('profile', type=Path)
    ap.add_argument('--limit', type=int, default=15)
    args = ap.parse_args()
    totals, duration, frames = summarize(args.profile)
    print(f'{duration:.3f} wall seconds, {frames} presented frames, {frames/duration:.2f} fps')
    print('overlay entry address MSCNT calls/frame VU-ms/frame frame-share GIF-ms/frame')
    for (overlay, entry, addr, cont), (calls, vu, gif) in sorted(totals.items(), key=lambda item: -item[1][1])[:args.limit]:
        label = {1: 'A', 2: 'B'}.get(overlay, '?')
        print(f'{label:>7} {entry:04X} {addr:04X} {cont:5} {calls/frames:11.2f} '
              f'{vu/frames:11.4f} {vu/duration/10:10.2f}% {gif/frames:12.4f}')

if __name__ == '__main__':
    main()
