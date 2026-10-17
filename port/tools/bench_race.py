"""Fair race benchmark: one fixed, deterministic race, uncapped, no diagnostics.

Every candidate runs the same race as race_compare.py (screen-keyed navigation, the same race pad
script and seed, DHD_RACE_SYNC) on the unpaced virtual frame clock (DHD_FRAME_CLOCK=1). The port
times the virtual-field window DHD_BENCH (port/gs/present.c) and exits at its last field. Runs
are interleaved (A B C, A B C, ...) so drift on the machine hits every candidate alike.

    python port/tools/bench_race.py --candidate base=build/port/a/dhd.exe \
        --candidate native=build/port/b/dhd.exe:DHD_RENDER=native --runs 3

A candidate is LABEL=EXE[:VAR=VALUE[,VAR=VALUE...]]. --sound plays audio and reports its gaps (use
DHD_FRAME_CLOCK=0 in the candidate for real-time pacing). Reports the median and spread of fields/s
and the median p99 and worst-1% frame times; writes OUT/summary.json and one log per run.
"""
from ctypes import wintypes
from pathlib import Path
import argparse
import ctypes
import json
import os
import re
import statistics
import subprocess
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parent))
import compare_pcsx2 as compare
import race_compare as race

LINE = re.compile(rb'bench: fields=(\d+)-(\d+) fields_per_s=([\d.]+) p50_ms=([\d.]+) p99_ms=([\d.]+) '
                  rb'worst1pct_ms=([\d.]+) max_ms=([\d.]+)')
# The native renderer's pictures over the same window (DHD_RENDER=native; with DHD_INTERP more
# pictures than fields): displayed rate and time between presents.
SHOWN = re.compile(rb'bench interp: displayed_fps=([\d.]+) present_ms p50=([\d.]+) p99=([\d.]+) '
                   rb'worst1pct=([\d.]+) max=([\d.]+)')
# Audio output over the window (--sound; port/hle/sound.c): gaps, silence written in them, queue.
SOUND = re.compile(rb'bench snd: underruns=(\d+) silence_ms=([\d.]+) dropped_ms=([\d.]+) skipped_ms=([\d.]+) '
                   rb'played_s=([\d.]+) queue_ms p1=(\d+) p50=(\d+) p99=(\d+) rate_ppm=([-+\d.]+) '
                   rb'tick_ms p50=([\d.]+) p99=([\d.]+) max=([\d.]+)')
# CPU time of each named port thread over the window (port/gs/present.c), ms per field.
THREADS = re.compile(rb'bench threads: cpu_ms_per_field((?: \w+=[\d.]+)+) process=([\d.]+) busy_cores=([\d.]+)')
SOUND_KEYS = ('snd_underruns', 'snd_silence_ms', 'snd_dropped_ms', 'snd_skipped_ms', 'snd_played_s', 'snd_queue_p1_ms',
              'snd_queue_p50_ms', 'snd_queue_p99_ms', 'snd_rate_ppm', 'tick_p50_ms', 'tick_p99_ms', 'tick_max_ms')


def parse_candidate(text):
    match = re.fullmatch(r'([^=]+)=(.+?)(?::([A-Za-z_]\w*=.*))?', text)
    if not match:
        raise SystemExit(f'bad candidate {text!r}')
    label, exe, env = match.groups()
    extra = dict(item.split('=', 1) for item in env.split(',')) if env else {}
    return label, Path(exe).resolve(), extra


user32 = ctypes.WinDLL('user32')
kernel32 = ctypes.WinDLL('kernel32')
user32.GetForegroundWindow.restype = wintypes.HWND
kernel32.OpenProcess.restype = wintypes.HANDLE
WINDOW_CALLBACK = ctypes.WINFUNCTYPE(wintypes.BOOL, wintypes.HWND, wintypes.LPARAM)


def process_name(pid):
    handle = kernel32.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
    if not handle:
        return ''
    try:
        buffer = ctypes.create_unicode_buffer(1024)
        size = wintypes.DWORD(len(buffer))
        return buffer.value if kernel32.QueryFullProcessImageNameW(handle, 0, buffer, ctypes.byref(size)) else ''
    finally:
        kernel32.CloseHandle(handle)


def focus_game():
    """Brings the game's window to the front. Returns whether it has focus, or None before it exists.
    Only one game runs during a benchmark, so any visible dhd.exe window is it."""
    found = []

    def each(hwnd, _):
        pid = wintypes.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
        if user32.IsWindowVisible(hwnd) and Path(process_name(pid.value)).name.lower() == 'dhd.exe':
            found.append(hwnd)
        return True
    user32.EnumWindows(WINDOW_CALLBACK(each), 0)
    if not found:
        return None
    foreground = user32.GetForegroundWindow()
    if foreground in found:
        return True
    hwnd = found[0]
    user32.ShowWindow(hwnd, 9)  # SW_RESTORE
    # A key event lifts Windows' foreground lock for this process; Alt alone does nothing in the game.
    user32.keybd_event(0x12, 0, 0, 0)
    user32.keybd_event(0x12, 0, 2, 0)
    user32.SetForegroundWindow(hwnd)
    user32.BringWindowToTop(hwnd)
    return user32.GetForegroundWindow() == hwnd


def run_once(exe, extra, window, timeout, log_path, sound=False):
    env = {k: v for k, v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
    env.update(DHD_ISO=os.environ['DHD_ISO'], DHD_FRAME_CLOCK='1', DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_SND_OFF='1',
               DHD_SETTINGS='none',  # no dhd.ini beside the exe is read (finding 056)
               DHD_AUTOPAD_FRAMES=compare.frame_script(compare.parse_pad(race.NAVIGATION)),
               DHD_NAV=race.NAV, DHD_RACE_SYNC='1', DHD_RACE_PAD=race.RACE_PAD, DHD_RACE_SEED='81257',
               DHD_BENCH=window, DHD_BENCH_EXIT='1')
    if sound:  # real audio output, to count its gaps
        del env['DHD_SND_OFF']
    env.update(extra)
    start = time.monotonic()
    samples = focused = 0
    with open(log_path, 'wb') as log:
        process = subprocess.Popen([str(exe)], cwd=exe.parent, env=env, stdout=log, stderr=subprocess.STDOUT,
                                   creationflags=getattr(subprocess, 'ABOVE_NORMAL_PRIORITY_CLASS', 0))
        # Windows slows a game whose window is in the background, so keep it in front.
        while process.poll() is None:
            if time.monotonic() - start > timeout:
                process.kill()
                process.wait(timeout=30)
                raise RuntimeError(f'{exe} timed out after {timeout} s; see {log_path}')
            state = focus_game()
            if state is not None:
                samples += 1
                focused += state
            time.sleep(0.5)
    match = LINE.search(Path(log_path).read_bytes())
    if not match:
        raise RuntimeError(f'{exe} printed no bench line (exit {process.returncode}); see {log_path}')
    fps, p50, p99, worst, peak = (float(match.group(i)) for i in range(3, 8))
    result = dict(fields_per_s=fps, p50_ms=p50, p99_ms=p99, worst1pct_ms=worst, max_ms=peak,
                  wall_s=round(time.monotonic() - start, 1), focused=round(focused / samples, 3) if samples else 0.0)
    shown = SHOWN.search(Path(log_path).read_bytes())
    if shown:
        result.update(zip(('displayed_fps', 'present_p50_ms', 'present_p99_ms', 'present_worst1pct_ms', 'present_max_ms'),
                          (float(shown.group(i)) for i in range(1, 6))))
    threads = THREADS.search(Path(log_path).read_bytes())
    if threads:
        cpu = {k.decode(): float(v) for k, v in (item.split(b'=') for item in threads.group(1).split())}
        cpu.update(process=float(threads.group(2)), busy_cores=float(threads.group(3)))
        result['cpu_ms_per_field'] = cpu
    heard = SOUND.search(Path(log_path).read_bytes())
    if heard:
        result.update(zip(SOUND_KEYS, (float(heard.group(i)) for i in range(1, 13))))
    return result


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--candidate', action='append', required=True, help='LABEL=EXE[:VAR=VALUE,...]')
    ap.add_argument('--runs', type=int, default=3)
    ap.add_argument('--window', default='6000-7000', help='virtual GS fields first-last')
    ap.add_argument('--timeout', type=int, default=1800, help='seconds per run')
    ap.add_argument('--out', type=Path, default=compare.ROOT / 'build/port/bench/latest')
    ap.add_argument('--sound', action='store_true', help='play audio and report its gaps (normally off)')
    args = ap.parse_args()
    if 'DHD_ISO' not in os.environ:
        sys.exit('DHD_ISO must name the local game image')
    candidates = [parse_candidate(c) for c in args.candidate]
    args.out.mkdir(parents=True, exist_ok=True)
    results = {label: [] for label, _, _ in candidates}
    for run in range(args.runs):
        for label, exe, extra in candidates:
            result = run_once(exe, extra, args.window, args.timeout, args.out / f'{label}-{run}.log', args.sound)
            results[label].append(result)
            print(f'{label} run {run}: {result["fields_per_s"]:.2f} fields/s, p99 {result["p99_ms"]:.2f} ms, '
                  f'worst 1% {result["worst1pct_ms"]:.2f} ms, focused {result["focused"] * 100:.0f}%' +
                  (f', displayed {result["displayed_fps"]:.2f} fps, present p99 {result["present_p99_ms"]:.2f} ms'
                   if 'displayed_fps' in result else '') +
                  (f', audio gaps {result["snd_underruns"]:.0f} ({result["snd_silence_ms"]:.0f} ms silence), '
                   f'queue p1/p50 {result["snd_queue_p1_ms"]:.0f}/{result["snd_queue_p50_ms"]:.0f} ms, '
                   f'game tick p99 {result["tick_p99_ms"]:.1f} max {result["tick_max_ms"]:.1f} ms'
                   if 'snd_underruns' in result else ''), flush=True)
    summary = {}
    for label, exe, extra in candidates:
        runs = results[label]
        fps = [r['fields_per_s'] for r in runs]
        summary[label] = dict(exe=str(exe), env=extra, window=args.window, runs=runs,
                              median_fields_per_s=statistics.median(fps), spread=max(fps) - min(fps),
                              median_p99_ms=statistics.median(r['p99_ms'] for r in runs),
                              median_worst1pct_ms=statistics.median(r['worst1pct_ms'] for r in runs))
        if all('cpu_ms_per_field' in r for r in runs):
            names = sorted(set().union(*(r['cpu_ms_per_field'] for r in runs)))
            summary[label]['median_cpu_ms_per_field'] = {
                n: statistics.median(r['cpu_ms_per_field'].get(n, 0.0) for r in runs) for n in names}
        if all('displayed_fps' in r for r in runs):
            for key in ('displayed_fps', 'present_p50_ms', 'present_p99_ms', 'present_worst1pct_ms', 'present_max_ms'):
                summary[label]['median_' + key] = statistics.median(r[key] for r in runs)
        if all('snd_underruns' in r for r in runs):
            for key in SOUND_KEYS:
                summary[label]['median_' + key] = statistics.median(r[key] for r in runs)
            summary[label]['total_snd_underruns'] = sum(r['snd_underruns'] for r in runs)
    base = summary[candidates[0][0]]['median_fields_per_s']
    for label, s in summary.items():
        s['gain_vs_first'] = s['median_fields_per_s'] / base - 1
        print(f'{label}: median {s["median_fields_per_s"]:.2f} fields/s (spread {s["spread"]:.2f}), '
              f'p99 {s["median_p99_ms"]:.2f} ms, worst 1% {s["median_worst1pct_ms"]:.2f} ms, '
              f'{s["gain_vs_first"] * 100:+.1f}% vs {candidates[0][0]}' +
              (f'; displayed {s["median_displayed_fps"]:.2f} fps, present p50 {s["median_present_p50_ms"]:.2f} / '
               f'p99 {s["median_present_p99_ms"]:.2f} / worst 1% {s["median_present_worst1pct_ms"]:.2f} ms'
               if 'median_displayed_fps' in s else '') +
              (f'; audio gaps {s["total_snd_underruns"]:.0f} in {len(s["runs"])} runs, '
               f'median silence {s["median_snd_silence_ms"]:.0f} ms, game tick p99 {s["median_tick_p99_ms"]:.1f} ms'
               if 'total_snd_underruns' in s else ''))
        if 'median_cpu_ms_per_field' in s:
            print('    CPU ms per field: ' + ', '.join('%s %.2f' % kv for kv in s['median_cpu_ms_per_field'].items()))
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')


if __name__ == '__main__':
    main()
