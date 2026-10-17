"""E4 of finding 040 (finding 041): two game instances on this PC run the same 4-player race
from the same scripted pads, in different configurations, and log the game state's hash every
60 fields (DHD_HASH_LOG). Lockstep needs the race-state hash (online.c game_hash) to match field for field.

    python port/tools/lockstep_local.py --exe build/port/link/dhd.exe --seconds 300 [--out DIR]

Arm A: native renderer + interpolation at 120 Hz; arm B: native renderer, plain. Needs DHD_ISO.
Output (game data: keep it under build/): DIR/A, DIR/B with hashes.txt and the logs."""
import argparse, os, subprocess, sys, time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
NAV4 = '0:triangle|cross,15:down,15:cross,10:down,10:down,10:cross,16:cross,24:cross,11:cross,34:cross'
# Each rider pedals (Cross) most of the time and steers in its own rhythm, by race-clock
# seconds after GO.
RACE_PADS = [
    ','.join([f'{t}:5:cross' for t in range(0, 600, 6)] + [f'{3 + 8 * k}:0.4:{"left" if k % 2 else "right"}' for k in range(70)]),
    ','.join([f'{t}:4:cross' for t in range(1, 600, 5)] + [f'{5 + 7 * k}:0.3:{"right" if k % 2 else "left"}' for k in range(80)]),
    ','.join([f'{t}:6:cross+r1' for t in range(0, 600, 7)] + [f'{2 + 9 * k}:0.5:{"left" if k % 3 else "right"}' for k in range(60)]),
    ','.join([f'{t}:3:cross' for t in range(0, 600, 4)] + [f'{4 + 6 * k}:0.2:{"right" if k % 2 else "left"}' for k in range(90)]),
]
ARMS = {'A': {'DHD_RENDER': 'native', 'DHD_INTERP': '1', 'DHD_INTERP_HZ': '120'},
        'B': {'DHD_RENDER': 'native'}}


def env_for(arm, out, extra):
    env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    env.update(DHD_ISO=os.environ['DHD_ISO'], DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_SND_OFF='1', DHD_PADS='4', DHD_NAV=NAV4,
               DHD_RACE_SYNC='1', DHD_RACE_SEED='81257', DHD_HASH_LOG=str(out / 'hashes.txt'))
    for i, script in enumerate(RACE_PADS):
        env['DHD_RACE_PAD' if i == 0 else f'DHD_RACE_PAD{i}'] = script
    env.update(ARMS[arm])
    env.update(extra)
    return {k: v for k, v in env.items() if v != ''}


def compare(a, b):
    def load(p):
        rows = {}
        for line in open(p):
            field, *rest = line.split()
            rows[int(field)] = dict(x.split('=') for x in rest)
        return rows
    ra, rb = load(a), load(b)
    common = sorted(set(ra) & set(rb))
    first = {k: next((f for f in common if ra[f][k] != rb[f][k]), None) for k in ('game', 'static', 'all')}
    race = [f for f in common if ra[f]['menu'] == '9' and ra[f]['phase'] in ('1', '2', '3')]
    return common, first, race


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', default=str(ROOT / 'build' / 'port' / 'link' / 'dhd.exe'))
    ap.add_argument('--seconds', type=int, default=300)
    ap.add_argument('--out', default=str(ROOT / 'build' / 'port' / 'e4'))
    ap.add_argument('--stop-if', help='stop both instances as soon as this file exists (a benchmark lock)')
    ap.add_argument('--arm-b', action='append', default=[], help='extra NAME=VALUE for arm B (e.g. DHD_VIEW=2)')
    ap.add_argument('--arm-a', action='append', default=[], help='extra NAME=VALUE for arm A; NAME= removes NAME')
    a = ap.parse_args()
    if not os.environ.get('DHD_ISO'):
        sys.exit('set DHD_ISO')
    if a.stop_if and os.path.exists(a.stop_if):
        sys.exit(f'{a.stop_if} exists: not starting')
    out = Path(a.out).resolve()
    procs = {}
    for arm in ARMS:
        d = out / arm
        d.mkdir(parents=True, exist_ok=True)
        extra = dict(x.split('=', 1) for x in (a.arm_b if arm == 'B' else a.arm_a))
        procs[arm] = subprocess.Popen([os.path.abspath(a.exe)], cwd=d, env=env_for(arm, d, extra),
                                      stdout=open(d / 'stdout.log', 'w'), stderr=open(d / 'stderr.log', 'w'))
    deadline = time.time() + a.seconds
    while time.time() < deadline and all(p.poll() is None for p in procs.values()):
        if a.stop_if and os.path.exists(a.stop_if):
            print('stopped early:', a.stop_if, 'exists')
            break
        time.sleep(1)
    for p in procs.values():
        if p.poll() is None:
            subprocess.run(['taskkill', '/T', '/F', '/PID', str(p.pid)], capture_output=True)
    common, first, race = compare(out / 'A' / 'hashes.txt', out / 'B' / 'hashes.txt')
    print(f'{len(common)} common checkpoints (fields {common[0] if common else "-"}-{common[-1] if common else "-"}), '
          f'{len(race)} in the race (countdown to finish)')
    for key, what in (('game', 'race state (the online hash)'), ('static', 'static area (info)'), ('all', 'all 32 MB (info)')):
        print(f'{what}: ' + ('MATCH' if first[key] is None else f'DIFFER from field {first[key]}'))
    return 0 if first['game'] is None and common else 1


if __name__ == '__main__':
    sys.exit(main())
