"""Summarize an ignored DHD_SND_WAV capture and DHD_SND_REPORT CSV.

Usage: python port/tools/audio_report.py build/port/audio-round.wav [commands.csv]
Only reads captures. Reports signal statistics, not speaker audibility or
PCSX2 fidelity. Uses the standard library; no audio backend is needed.
"""
import argparse
import array
import csv
import json
import math
import re
import sys
import wave


def summarize(path):
    with wave.open(str(path), 'rb') as wav:
        if wav.getnchannels() != 2 or wav.getsampwidth() != 2:
            raise ValueError('expected stereo 16-bit PCM')
        rate = wav.getframerate()
        channels = [dict(nonzero=0, clipped=0, peak=0, sum_squares=0) for _ in range(2)]
        frames = 0
        seconds = []
        while raw := wav.readframes(rate):
            pcm = array.array('h', raw)
            if sys.byteorder != 'little':
                pcm.byteswap()
            if len(pcm) % 2:
                raise ValueError('incomplete stereo frame')
            active = 0
            for i, sample in enumerate(pcm):
                channel = channels[i & 1]
                channel['nonzero'] += sample != 0
                channel['clipped'] += sample in (-32768, 32767)
                channel['peak'] = max(channel['peak'], abs(sample))
                channel['sum_squares'] += sample * sample
                active += sample != 0
            frames += len(pcm) // 2
            seconds.append(active)
        if frames != wav.getnframes():
            raise ValueError(f'truncated WAV: header says {wav.getnframes()}, read {frames}')
        for channel in channels:
            channel['rms'] = round(math.sqrt(channel.pop('sum_squares') / max(1, frames)), 3)
        return dict(path=str(path), frames=frames, rate=rate,
                    duration_seconds=round(frames / rate, 6), channels=channels,
                    nonzero_samples_per_second=seconds)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('wav')
    parser.add_argument('commands', nargs='?')
    args = parser.parse_args()
    result = summarize(args.wav)
    if args.commands:
        with open(args.commands, encoding='utf-8') as f:
            first = f.readline()
            stage = re.search(r'mix_clipped_left=(\d+); mix_clipped_right=(\d+)', first)
            if stage:
                result['mix_clipped_samples'] = [int(stage[1]), int(stage[2])]
                result['mix_clipped_percent'] = [round(int(stage[i]) * 100 / max(1, result['frames']), 6) for i in (1, 2)]
            f.seek(0)
            result['commands'] = [
                dict(command=row['command'], **{k: int(row[k]) for k in
                     ('calls', 'unimplemented', 'unsupported')})
                for row in csv.DictReader(line for line in f if not line.startswith('#'))
            ]
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
