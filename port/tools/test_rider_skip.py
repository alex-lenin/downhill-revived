"""High-rate RIDER SELECTED skip (from n0buholic, PR #13): Cross must leave the screen promptly at
60 and 240 FPS, pressed early (in the camera blend) and late (after it).

    python port/tools/test_rider_skip.py [--exe build/host64-exe/dhd64.exe] [--iso PATH]

The screen's skip gates (001EB1D8, and the shop flight's) count elapsed 60 Hz fields. Counted per
update, an early press at 240 FPS was undone by the blend's end and the screen stayed about 1100
fields (port/patches_fps.py). Each case boots the game with scripted pad input (DHD_NAV: the title,
the main menu, rider select, then Cross 10 fields or a second after RIDER SELECTED shows), no
settings, saves, sound or movies, minimised, and reads the fields from nav step 4 (RIDER SELECTED)
to step 5 (the next screen) in its log. Budget: half a second after the press or the blend's end,
whichever is later. Needs DHD_ISO or --iso; about 10 s per case. Logs: build/rider-skip/.
"""
import argparse
import os
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import owned_process  # noqa: E402
import state_trace_run  # noqa: E402

OUT = ROOT / 'build' / 'rider-skip'


def case(exe, iso, fps, settle, timeout):
    """(fields from RIDER SELECTED to the next screen or None, the log's path)."""
    out = OUT / f'{fps}-{settle}'
    out.mkdir(parents=True, exist_ok=True)
    env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    env.update(DHD_ISO=str(iso), DHD_SETTINGS='none', DHD_RENDER='native', DHD_DATA_DIR=str(out),
               DHD_WINDOW='windowed', DHD_WINDOW_WIDTH='960', DHD_WINDOW_HEIGHT='540',
               DHD_WIDESCREEN='1', DHD_ASPECT='wide', DHD_SKIP_MOVIES='1', DHD_MEMCARD='0',
               DHD_SND_OFF='1', DHD_FPS=str(fps), DHD_UNLOCK_ALL='1', DHD_AUTO_UPDATE='off',
               DHD_LAUNCHER='1', DHD_STOP_FIELD=str(fps * 7),
               DHD_NAV=f'0:triangle|cross,15:cross,32:cross,3:cross,33+{settle}:cross')
    log_path = out / 'game.log'
    with open(log_path, 'wb') as log:
        game = owned_process.run([str(exe)], timeout=timeout, cwd=str(out), env=env, stdout=log,
                                 stderr=subprocess.STDOUT, minimized=True)
    text = log_path.read_text(errors='replace')
    if game.timed_out or game.returncode:
        raise RuntimeError(f'{fps} FPS, Cross after {settle}: '
                           f'{"timed out" if game.timed_out else "exit %s" % game.returncode}; see {log_path}')
    steps = {int(s): int(f) for s, f in re.findall(r'compare: nav step (\d+) field=(\d+) menu=', text)}
    return (steps[5] - steps[4] if 4 in steps and 5 in steps else None), log_path


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('--exe', type=Path, default=ROOT / state_trace_run.WINDOWS_EXE)
    p.add_argument('--iso', type=Path, default=Path(os.environ.get('DHD_ISO', state_trace_run.DEFAULT_ISO)))
    p.add_argument('--timeout', type=float, default=120)
    a = p.parse_args()
    failed = 0
    for fps in (60, 240):
        for settle in (10, fps):
            start = time.monotonic()
            took, log = case(a.exe.resolve(), a.iso.resolve(), fps, settle, a.timeout)
            budget = max(settle, fps // 2) + fps // 2
            ok = took is not None and took <= budget
            failed += not ok
            print(f'{"PASS" if ok else "FAIL"} {fps} FPS, Cross after {settle} fields: RIDER SELECTED for '
                  f'{took if took is not None else "?"} fields (budget {budget}), '
                  f'{time.monotonic() - start:.0f} s' + ('' if ok else f'; see {log}'), flush=True)
    sys.exit(1 if failed else 0)


if __name__ == '__main__':
    main()
