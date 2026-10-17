"""Measure stereo PCM loudness in 100-ms bins, spectral bands and explicit excerpts.

Offsets/excerpts are arguments, never an implicit nearest-event state match.
Writes reusable JSON and CSV; captures remain ignored under build/port/audio-*.
"""
import argparse
import json
from pathlib import Path
import wave
import numpy as np


def read(path):
    with wave.open(str(path), 'rb') as file:
        if file.getsampwidth() != 2 or file.getnchannels() != 2:
            raise ValueError('expected stereo 16-bit PCM')
        rate = file.getframerate()
        pcm = np.frombuffer(file.readframes(file.getnframes()), dtype='<i2').reshape(-1, 2)
    return rate, pcm


def measure(pcm, rate):
    x = pcm.astype(np.float64)
    if not len(x):
        raise ValueError('empty excerpt')
    n = rate // 10
    bins = x[:len(x)//n*n].reshape(-1, n, 2)
    rms = np.sqrt(np.mean(bins*bins, axis=(1, 2)))
    # Hann-window averaged periodogram, separate channel powers avoid stereo
    # phase cancellation. This is spectral energy, not perceptual weighting.
    width = 4096
    windows = x[:len(x)//width*width].reshape(-1, width, 2)
    windows -= np.mean(windows, axis=1, keepdims=True)
    spectrum = np.mean(np.abs(np.fft.rfft(windows * np.hanning(width)[None, :, None], axis=1))**2, axis=(0, 2))
    frequencies = np.fft.rfftfreq(width, 1/rate)
    bands = (0, 100, 500, 2000, 8000, rate/2+1)
    energy = [float(spectrum[(frequencies >= lo) & (frequencies < hi)].sum()) for lo, hi in zip(bands, bands[1:])]
    total = sum(energy)
    return dict(seconds=len(x)/rate, rms=float(np.sqrt(np.mean(pcm.astype(np.float64)**2))),
                peak=int(np.abs(pcm.astype(np.int32)).max()), mean=np.mean(pcm, axis=0).tolist(),
                clipping_percent=float(np.mean((pcm == 32767) | (pcm == -32768))*100),
                bands_hz=list(bands), band_energy_percent=[e/total*100 if total else 0 for e in energy],
                rms_100ms=rms.tolist(),
                first_audible_100ms=next((i/10 for i, r in enumerate(rms) if r > 32), None))


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('native', type=Path)
    p.add_argument('reference', type=Path)
    p.add_argument('output', type=Path)
    p.add_argument('--native-at', type=float, default=0)
    p.add_argument('--reference-at', type=float, default=0)
    p.add_argument('--seconds', type=float)
    args = p.parse_args()
    result = dict(native=str(args.native), reference=str(args.reference),
                  native_at=args.native_at, reference_at=args.reference_at, requested_seconds=args.seconds)
    for side, path, start in (('native', args.native, args.native_at), ('reference', args.reference, args.reference_at)):
        rate, pcm = read(path)
        end = None if args.seconds is None else int((start+args.seconds)*rate)
        excerpt = pcm[int(start*rate):end]
        result[side+'_metrics'] = measure(excerpt, rate)
        with wave.open(str(args.output.with_name(args.output.stem+'-'+side+'.wav')), 'wb') as f:
            f.setparams((2, 2, rate, 0, 'NONE', 'not compressed')); f.writeframes(excerpt.tobytes())
    a, b = result['native_metrics'], result['reference_metrics']
    result['rms_delta_db'] = float(20*np.log10(a['rms']/b['rms'])) if b['rms'] else None
    n = min(len(a['rms_100ms']), len(b['rms_100ms']))
    result['rms_envelope_correlation'] = float(np.corrcoef(a['rms_100ms'][:n], b['rms_100ms'][:n])[0, 1]) if n > 1 else None
    args.output.write_text(json.dumps(result, indent=2), encoding='utf-8')
    with args.output.with_suffix('.csv').open('w', encoding='utf-8') as file:
        file.write('excerpt_seconds,native_rms,reference_rms\n')
        for i in range(n): file.write(f'{i/10:.1f},{a["rms_100ms"][i]:.6f},{b["rms_100ms"][i]:.6f}\n')
    print(json.dumps({k:v for k,v in result.items() if not k.endswith('_metrics')}))


if __name__ == '__main__':
    main()
