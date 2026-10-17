"""An online race through the relay with state sync (findings 044, 047): two game instances on
this PC, the host and one that joins, each riding its own scripted line (DHD_NET_SCRIPTED), plus
optionally nettool bots that replay a recorded rider (--bots, port/net/nettool.c --replay). Each
game shows the others' riders as puppets; the host runs the AI riders. Both take screenshots at
the given race-clock moments (DHD_RACE_CAPTURE) and the run goes on until both games have the
host's results (or --seconds). The summary has the start (GO on the server's clock), the lag of
each puppet, hits, finishes and results.

    python port/tools/online_race.py [--seconds 420] [--out DIR] [--capture g+600,g+1200]
                                     [--bots N --replay FILE] [--kill-guest-at SECONDS]

Needs DHD_ISO, DHD_NET_SERVER (the official wss://dhd-online.downhill-revived.workers.dev or
your own; never write any other deployed address into a tracked file) and the
build (python port/tools/link.py). At most two dhd.exe of its own run; all are stopped by PID."""
import argparse, hashlib, os, re, subprocess, sys, time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
# Both pedal all the way from GO. The guest throws punches and kicks for the first 40 s, while
# the two are still side by side from the grid.
PEDAL = '0:400:cross'
PUNCHES = ','.join(f'{1 + k * 0.5:.1f}:0.15:{"square" if k % 2 == 0 else "circle"}' for k in range(80))
HOST_PAD = PEDAL
GUEST_PAD = PEDAL + ',' + PUNCHES
KEEP = ('went at', 'countdown starts', 'rides', 'hit kind', 'finished', 'result', 'left', 'AI now',
        'starts at grid', 'Online play stopped', 'crash kind', 'has loaded')


def iso_sha256(iso, cache):
    """The ISO's SHA-256, cached under build/ beside its size and time (it takes a while)."""
    st = os.stat(iso)
    key = f'{st.st_size} {int(st.st_mtime)}'
    if cache.exists():
        k, _, h = cache.read_text().strip().rpartition(' ')
        if k == key:
            return h
    d = hashlib.sha256()
    with open(iso, 'rb') as f:
        for block in iter(lambda: f.read(1 << 22), b''):
            d.update(block)
    cache.write_text(f'{key} {d.hexdigest()}\n')
    return d.hexdigest()


def game_env(iso_hash, pad, extra):
    env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    env.update(DHD_ISO=os.environ['DHD_ISO'], DHD_NET_SERVER=os.environ['DHD_NET_SERVER'], DHD_NET_ISO_SHA256=iso_hash,
               DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_SND_OFF='1', DHD_RENDER='native', DHD_RACE_SYNC='1', DHD_NET_SCRIPTED='1',
               DHD_RACE_PAD=pad, DHD_SCREENSHOT='shot', DHD_SCALE='1', DHD_SETTINGS='none')
    env.update(extra)
    return {k: v for k, v in env.items() if v != ''}


def wait_line(paths, pattern, seconds, procs, stop_if):
    """Until every file in paths has a line matching pattern; the matches (None where missing)."""
    until = time.time() + seconds
    found = [None] * len(paths)
    while time.time() < until:
        for i, path in enumerate(paths):
            if found[i] is None and path.exists():
                found[i] = re.search(pattern, path.read_text(errors='replace'))
        if all(found):
            break
        if any(p.poll() is not None for p in procs) or (stop_if and os.path.exists(stop_if)):
            break
        time.sleep(0.5)
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', default=str(ROOT / 'build' / 'port' / 'link' / 'dhd.exe'))
    ap.add_argument('--nettool', default=str(ROOT / 'build' / 'port' / 'net' / 'nettool.exe'))
    ap.add_argument('--seconds', type=int, default=420, help='at most this long once both race')
    ap.add_argument('--out', default=str(ROOT / 'build' / 'port' / 'online-race'))
    ap.add_argument('--track', default='0')
    ap.add_argument('--capture', default='g+300,g+900', help='DHD_RACE_CAPTURE moments for both')
    ap.add_argument('--bots', type=int, default=0, help='nettool players replaying --replay')
    ap.add_argument('--replay', help='a STATE/EVENT recording (DHD_NET_RECORD) for the bots')
    ap.add_argument('--kill-guest-at', type=float, help='stop the guest this many seconds after GO')
    ap.add_argument('--stop-if', default=os.path.join(ROOT, 'build', 'port', 'BENCH.lock'),
                    help='stop everything as soon as this file exists (the benchmark lock)')
    ap.add_argument('--a', action='append', default=[], help='extra NAME=VALUE for the host game')
    ap.add_argument('--b', action='append', default=[], help='extra NAME=VALUE for the joining game')
    a = ap.parse_args()
    for v in ('DHD_ISO', 'DHD_NET_SERVER'):
        if not os.environ.get(v):
            sys.exit('set ' + v)
    if a.stop_if and os.path.exists(a.stop_if):
        sys.exit(f'{a.stop_if} exists: not starting')
    out = Path(a.out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    iso_hash = iso_sha256(os.environ['DHD_ISO'], ROOT / 'build' / 'port' / 'iso-sha256.txt')
    procs, dirs = [], []

    def game(name, pad, extra):
        d = out / name
        d.mkdir(exist_ok=True)
        for f in d.glob('shot-*.bmp'):
            f.unlink()
        (d / 'stderr.log').unlink(missing_ok=True)
        env = game_env(iso_hash, pad, {'DHD_RACE_CAPTURE': a.capture, **extra})
        procs.append(subprocess.Popen([os.path.abspath(a.exe)], cwd=d, env=env, stdout=open(d / 'stdout.log', 'w'),
                                      stderr=open(d / 'stderr.log', 'w')))
        dirs.append(d)
        return d

    players = 2 + a.bots
    try:
        da = game('A', HOST_PAD, {'DHD_NET_CREATE': '1', 'DHD_NET_NAME': 'Host', 'DHD_NET_PLAYERS': str(players),
                                  'DHD_NET_TRACK': a.track, 'DHD_NET_RIDER': '4', **dict(x.split('=', 1) for x in a.a)})
        m = wait_line([da / 'stderr.log'], r'online: room (\w+) \(host\)', 120, procs, a.stop_if)[0]
        if not m:
            print('the host never opened a room; see', da / 'stderr.log')
            return 1
        code = m.group(1)
        print('room', code)
        db = game('B', GUEST_PAD, {'DHD_NET_JOIN': code, 'DHD_NET_NAME': 'Guest', 'DHD_NET_RIDER': '8',
                                   **dict(x.split('=', 1) for x in a.b)})
        version = wait_line([da / 'stderr.log'], r'version (dhd-port [\w.]+(?: \w+)?)', 30, procs, a.stop_if)[0].group(1)
        for k in range(a.bots):
            d = out / f'bot{k}'
            d.mkdir(exist_ok=True)
            procs.append(subprocess.Popen([a.nettool, '--bot', '--version', version, '--iso', iso_hash, '--name',
                                           f'Bot{k}', '--replay', a.replay, '--rider', str(k * 2 + 1), 'join', code],
                                          stdout=open(d / 'stdout.log', 'w'), stderr=subprocess.STDOUT))
        go = wait_line([da / 'stderr.log', db / 'stderr.log'], r'countdown starts', 400, procs, a.stop_if)
        if not all(go):
            print('a game did not reach the countdown')
        elif a.kill_guest_at:
            time.sleep(4 + a.kill_guest_at)  # the countdown, then the race
            subprocess.run(['taskkill', '/T', '/F', '/PID', str(procs[1].pid)], capture_output=True)
            print('guest stopped')
            procs.pop(1)
        logs = [da / 'stderr.log'] + ([] if a.kill_guest_at else [db / 'stderr.log'])
        done = wait_line(logs, r'online: result 1\.', a.seconds, procs, a.stop_if)
        time.sleep(6)
        if not all(done):
            print('not every game got the results')
    finally:
        for p in procs:
            if p.poll() is None:
                subprocess.run(['taskkill', '/T', '/F', '/PID', str(p.pid)], capture_output=True)
    ok = True
    for d in dirs:
        text = (d / 'stderr.log').read_text(errors='replace') if (d / 'stderr.log').exists() else ''
        lines = [l for l in text.splitlines() if l.startswith(('online:', 'puppet:'))]
        keep = [l for l in lines if any(k in l for k in KEEP)]
        clocks = [l for l in lines if 'race clock' in l]
        print(f'--- {d.name}: {len(keep)} lines')
        for l in keep[:80]:
            print(' ', l)
        for l in clocks[2:12:3] + clocks[-1:]:
            print('  ', l)
        if 'Online play stopped' in text or not clocks:
            ok = False
        print('  screenshots:', ' '.join(sorted(f.name for f in d.glob('shot-*.bmp'))))
    for k in range(a.bots):
        print(f'--- bot{k}:', (out / f'bot{k}' / 'stdout.log').read_text(errors='replace')[-600:])
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
