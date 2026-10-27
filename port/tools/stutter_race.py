"""Stutter run: a race the game drives itself, in front on the real desktop, with a player's dhd.ini,
recorded by the game's pacing log and DHD_SPIKES lines and by PresentMon (how Windows showed each
picture: composed by DWM or flipped straight to the screen). PresentMon.exe is found on PATH or
at DHD_PRESENTMON.

    python port/tools/stutter_race.py --exe GAME/dhd.exe --ini GAME/dhd.ini --out OUT --seconds 180
        [--env VAR=VALUE ...] [--menu]

race_compare.py's screen-keyed route reaches the race; then Cross is held (the rider pedals the
whole way; steering does not matter for presentation). --menu stays in the main menu instead
(enough to see the present mode). After --seconds the window is closed (WM_CLOSE) so the logs
flush. The window is kept in front every half second: a game behind another window is composed
by DWM whatever it does. Writes OUT/dhd.log, OUT/pacing-*.csv, OUT/presentmon.csv and prints
the summary (stutter_report.py).
"""
from pathlib import Path
import argparse
import ctypes
import os
import subprocess
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parent))
import race_compare as race
from bench_race import focus_game, user32, WINDOW_CALLBACK, process_name
from ctypes import wintypes

PRESENTMON = os.environ.get('DHD_PRESENTMON') or 'PresentMon.exe'


def close_game(names):
    def each(hwnd, _):
        pid = wintypes.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
        if user32.IsWindowVisible(hwnd) and Path(process_name(pid.value)).name.lower() in names:
            user32.PostMessageW(hwnd, 0x0010, 0, 0)  # WM_CLOSE
        return True
    user32.EnumWindows(WINDOW_CALLBACK(each), 0)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--ini', type=Path, required=True, help="the player's dhd.ini (copied; the pacing log goes to OUT)")
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--seconds', type=float, default=180, help='seconds of running before the window is closed')
    ap.add_argument('--env', action='append', default=[], help='VAR=VALUE for the game')
    ap.add_argument('--menu', action='store_true', help='stay in the main menu')
    args = ap.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    # The player's settings, with the logs pointed here.
    lines = [l for l in args.ini.read_text(encoding='utf-8', errors='replace').splitlines()
             if not l.strip().upper().startswith(('DHD_PACING_LOG', 'DHD_SPIKES', 'DHD_PACING_STATS'))]
    lines += [f'DHD_PACING_LOG = {out / "pacing"}', 'DHD_SPIKES = 1', 'DHD_PACING_STATS = 1']
    ini = out / 'dhd.ini'
    ini.write_text('\n'.join(lines) + '\n', encoding='utf-8')
    env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    env.update(DHD_SETTINGS=str(ini), DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_LAUNCHER_MODE='off',
               DHD_AUTO_UPDATE='off', DHD_NAV='0:triangle|cross' if args.menu else race.NAV)
    if not args.menu:
        env.update(DHD_RACE_SYNC='1', DHD_RACE_PAD='0:600:cross')
    env.update(dict(item.split('=', 1) for item in args.env))
    for old in out.glob('pacing-*.csv'):
        old.unlink()
    names = {args.exe.name.lower(), 'dhd.exe', 'dhd64.exe'}
    with open(out / 'dhd.log', 'wb') as log:
        game = subprocess.Popen([str(args.exe)], cwd=args.exe.parent, env=env, stdout=log, stderr=subprocess.STDOUT)
        (out / 'game.pid').write_text(str(game.pid))
        # Every process (without elevation PresentMon cannot name a process that started after it;
        # stutter_report.py keeps the game's rows by game.pid). Stopped through its ETW session
        # below: a killed PresentMon leaves the session running, and a few of those starve new ones.
        session = f'dhd_stutter_{os.getpid()}'
        monitor = subprocess.Popen([str(PRESENTMON), '--output_file', str(out / 'presentmon.csv'), '--no_console_stats',
                                    '--session_name', session],
                                   stdout=open(out / 'presentmon.out', 'wb'), stderr=subprocess.STDOUT)
        start = time.monotonic()
        samples = focused = 0
        while game.poll() is None:
            elapsed = time.monotonic() - start
            if elapsed > args.seconds:
                close_game(names)
                try:
                    game.wait(timeout=20)
                except subprocess.TimeoutExpired:
                    game.kill()
                break
            state = focus_game(names)
            if state is not None:
                samples += 1
                focused += state
            time.sleep(0.5)
    subprocess.run(['logman', 'stop', session, '-ets'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        monitor.wait(timeout=30)
    except subprocess.TimeoutExpired:
        monitor.kill()
    print(f'exit {game.returncode}, in front {focused}/{samples} samples')
    subprocess.run([sys.executable, str(Path(__file__).with_name('stutter_report.py')), str(out)])


if __name__ == '__main__':
    main()
