"""End-to-end test of the online relay: four nettool processes race in one room, for each
scenario (clean, a player dropping at field 300, a player whose state differs from field 400).
Build first: python port/net/build_tests.py
usage: python port/net/run_e2e.py [--server URL] [--fields N] [SCENARIO...]
The server is --server or DHD_NET_SERVER (wrangler dev: ws://127.0.0.1:8787)."""
import json, os, subprocess, sys, tempfile, time

HERE = os.path.dirname(os.path.abspath(__file__))
TOOL = os.path.join(os.path.dirname(os.path.dirname(HERE)), 'build', 'port', 'net', 'nettool.exe')
SCENARIOS = {  # name: extra arguments per player (player 0 hosts)
    'clean': [[], [], [], []],
    'drop': [[], [], [], ['--drop-at', '300']],
    'desync': [[], [], ['--corrupt-from', '400'], []],
}


def run(server, name, extra, fields):
    code_file = os.path.join(tempfile.mkdtemp(), 'code')
    base = [TOOL, '--server', server, '--fields', str(fields)]
    procs = [subprocess.Popen(base + ['--name', 'P0'] + extra[0] + ['host', '--players', '4', '--code-file', code_file],
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)]
    until = time.time() + 20
    while not os.path.exists(code_file) or not open(code_file).read().strip():
        if time.time() > until or procs[0].poll() is not None:
            procs[0].kill()
            print(procs[0].communicate()[0])
            return False
        time.sleep(0.1)
    code = open(code_file).read().strip()
    for i in range(1, 4):
        procs.append(subprocess.Popen(base + ['--name', 'P%d' % i] + extra[i] + ['join', code],
                                      stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True))
        time.sleep(0.2)  # join order sets the slots
    results, outputs = {}, []
    for i, p in enumerate(procs):
        try:
            out = p.communicate(timeout=120)[0]
        except subprocess.TimeoutExpired:
            p.kill()
            out = p.communicate()[0] + '\n(timed out)'
        outputs.append(out)
        for line in out.splitlines():
            if line.startswith('RESULT '):
                results[i] = json.loads(line[7:])
    ok = True

    def check(cond, what):
        nonlocal ok
        if not cond:
            print('  FAILED: ' + what)
            ok = False

    survivors = [i for i in range(4) if not extra[i] or extra[i][0] != '--drop-at']
    check(sorted(results) == survivors, 'results from players %s' % sorted(results))
    for i, r in sorted(results.items()):
        print('  player %d: slot %d, %d fields, state %s, delay %d, ping %d ms, waited %d ms (worst %d), '
              '%d late fields, AI from %s, desync %s' % (i, r['slot'], r['fields'], r['state'], r['delay'], r['ping'],
                                                         r['waitedMs'], r['worstWaitMs'], r['lateFields'], r['aiFrom'],
                                                         r['desync']))
    if ok:
        states = {r['state'] for r in results.values()}
        check(len(states) == 1, 'states differ')
        check(all(r['fields'] == fields for r in results.values()), 'fields')
        if name == 'drop':
            dropped = 3  # joined last: slot 3
            starts = {r['aiFrom'][dropped] for r in results.values()}
            check(len(starts) == 1 and min(starts) >= 300, 'AI takeover fields %s' % starts)
            check(all(r['desync'] is None for r in results.values()), 'no desync')
        elif name == 'desync':
            check(all(r['desync'] and r['desync']['field'] == 419 for r in results.values()), 'desync at 419')
            bad = results[2]['slot']
            check(all(r['desync']['slots'] == (1 << bad if i != 2 else 15 & ~(1 << bad))
                      for i, r in results.items()), 'desync slots')
        else:
            check(all(r['desync'] is None and max(r['aiFrom']) < 0 for r in results.values()), 'clean race')
    if not ok:
        for i, out in enumerate(outputs):
            print('--- player %d\n%s' % (i, out))
    return ok


def main():
    args = sys.argv[1:]
    server = os.environ.get('DHD_NET_SERVER')
    fields = 600
    if '--server' in args:
        server = args[args.index('--server') + 1]
        del args[args.index('--server'):args.index('--server') + 2]
    if '--fields' in args:
        fields = int(args[args.index('--fields') + 1])
        del args[args.index('--fields'):args.index('--fields') + 2]
    if not server:
        sys.exit('no server: --server URL or DHD_NET_SERVER')
    failed = 0
    for name in args or list(SCENARIOS):
        print(name)
        if not run(server, name, SCENARIOS[name], fields):
            failed += 1
    print('run_e2e: %s' % ('ok' if not failed else '%d scenarios failed' % failed))
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
