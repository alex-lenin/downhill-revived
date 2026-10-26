"""Measure field bob (the picture moving up and down every other field) on consecutive captures.

    python port/tools/field_bob.py FOLDER FIRST LAST [--region X,Y,W,H] [--prefix shot]

FOLDER holds PREFIX-fNNNNNN.bmp (or .png) captures of every field from FIRST to LAST, as
DHD_SCREENSHOT=FOLDER/shot DHD_CAPTURE_FIELDS=FIRST-LAST writes them (port/gs/present.c). For the
region (the whole picture by default) it prints:
  - adjacent: the mean RGB absolute error (0..255) of each field against the next one;
  - same-parity: the same against the field after next (f, f+2);
  - shifts: for each adjacent pair, the vertical shift in -5..5 output lines that matches best.

A still element (a HUD gauge) that bobs has a large adjacent error and a small same-parity one,
with shifts alternating +n/-n. Without bob both errors are small and every shift is 0. Finding 060
(the native renderer's half-line OFY correction) and finding 086 (W3, the browser's wobble) use
this measurement. Captures hold game pictures: keep them under build/, never commit them."""
import argparse
import os
import sys

import numpy as np
from PIL import Image


def load(folder, prefix, field):
    for ext in ('bmp', 'png'):
        path = os.path.join(folder, f'{prefix}-f{field:06d}.{ext}')
        if os.path.exists(path):
            return np.asarray(Image.open(path).convert('RGB'), dtype=np.float32)
    sys.exit(f'field_bob: no capture of field {field} in {folder}')


def crop(image, region):
    if not region:
        return image
    x, y, w, h = region
    return image[y:y + h, x:x + w]


def best_shift(a, b, margin=6):
    h = a.shape[0]
    if h <= 2 * margin:
        return 0
    scores = [(float(np.abs(a[margin:h - margin] - b[margin + s:h - margin + s]).mean()), abs(s), s)
              for s in range(-5, 6)]
    return min(scores)[2]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('folder')
    ap.add_argument('first', type=int)
    ap.add_argument('last', type=int)
    ap.add_argument('--region', help='X,Y,W,H in the capture\'s own pixels (default: whole picture)')
    ap.add_argument('--prefix', default='shot')
    args = ap.parse_args()
    if args.last - args.first < 2:
        ap.error('need at least three fields')
    region = [int(v) for v in args.region.split(',')] if args.region else None
    frames = [crop(load(args.folder, args.prefix, f), region) for f in range(args.first, args.last + 1)]
    mae = lambda a, b: float(np.abs(a - b).mean())
    adjacent = [mae(frames[i], frames[i + 1]) for i in range(len(frames) - 1)]
    same = [mae(frames[i], frames[i + 2]) for i in range(len(frames) - 2)]
    shifts = [best_shift(frames[i], frames[i + 1]) for i in range(len(frames) - 1)]
    print(f'{args.folder} fields {args.first}-{args.last} region {region or "all"}: '
          f'adjacent {np.mean(adjacent):.3f}, same-parity {np.mean(same):.3f}, shifts {shifts}')


if __name__ == '__main__':
    main()
