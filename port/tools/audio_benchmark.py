"""Measure normal-build mixer CPU at exactly 24/48 active voices with reverb.

Run in the port build environment (LLVM/Windows SDK available). Output defaults
to ignored build/port/audio-perf. Samples, not the performance clock, drive DSP.
Optional --baseline points to an audio source/header snapshot with the active
voice test hook. All compiler flags match link.py's HLE build, including host
code; SND_NO_HOST is deliberately absent. No assets or audio device are used.
"""
import argparse
import csv
import io
import json
import os
from pathlib import Path
import statistics
import subprocess

ROOT = Path(__file__).resolve().parents[2]
FLAGS = ['--target=i686-pc-windows-msvc', '-O2', '-msse2', '-mfpmath=sse', '-ffp-contract=off']


def compile_mixer(source, out, label):
    obj, exe = out / (label + '.obj'), out / (label + '.exe')
    subprocess.run(['clang', *FLAGS, '-c', '-w', '-I'+str(ROOT / 'build/port/link'),
                    '-I'+str(ROOT / 'port/runtime'), '-I'+str(ROOT / 'port/hle'),
                    str(source / 'sound.c'), '-o', str(obj)], check=True)
    subprocess.run(['clang', *FLAGS, '-D_CRT_SECURE_NO_WARNINGS', '-I'+str(ROOT / 'port/hle'),
                    str(ROOT / 'port/hle/bench_sound.c'), str(obj), '-luser32', '-lole32',
                    '-o', str(exe)], check=True)
    return exe


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=ROOT / 'build/port/audio-perf')
    parser.add_argument('--baseline', type=Path)
    parser.add_argument('--seconds', type=int, default=120)
    parser.add_argument('--trials', type=int, default=5)
    parser.add_argument('--cpu', type=int, default=0)
    args = parser.parse_args()
    if args.seconds < 1 or args.trials < 1 or args.cpu < 0:
        parser.error('seconds/trials must be positive and cpu nonnegative')
    out = args.out.resolve(); out.mkdir(parents=True, exist_ok=True)
    images = {}
    if args.baseline:
        images['before'] = compile_mixer(args.baseline.resolve(), out, 'before')
    images['after'] = compile_mixer(ROOT / 'port/hle', out, 'after')
    rows = []
    for trial in range(args.trials):
        for voices in (24, 48):
            phases = list(images)
            if trial & 1:
                phases.reverse()
            for phase in phases:
                env = os.environ.copy()
                env.update(DHD_SND_BENCH_CPU=str(args.cpu),
                           DHD_SND_BENCH_CASE=f'{voices}_active_voices_reverb')
                result = subprocess.run([str(images[phase]), str(args.seconds), '1'],
                                        env=env, capture_output=True, text=True, check=True)
                row = list(csv.DictReader(io.StringIO(result.stdout)))[0]
                row.update(phase=phase, trial=trial); rows.append(row)
                with (out / 'trials.csv').open('w', newline='', encoding='utf-8') as file:
                    writer = csv.DictWriter(file, fieldnames=list(rows[0]))
                    writer.writeheader(); writer.writerows(rows)
                print(phase, voices, trial, row['cpu_ms_per_audio_second'], flush=True)
    summary = dict(flags=FLAGS, cpu=args.cpu, audio_seconds=args.seconds, trials=args.trials,
                   host_output=False, active_voices_checked_each_audio_second=True, loads={})
    for voices in (24, 48):
        summary['loads'][voices] = {}
        for phase in images:
            values = [float(r['cpu_ms_per_audio_second']) for r in rows if
                      r['phase'] == phase and r['case'] == f'{voices}_active_voices_reverb']
            median = statistics.median(values)
            summary['loads'][voices][phase] = dict(cpu_ms_per_audio_second=median,
                share_of_one_core_percent=median / 10, min=min(values), max=max(values))
    (out / 'summary.json').write_text(json.dumps(summary, indent=2), encoding='utf-8')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
