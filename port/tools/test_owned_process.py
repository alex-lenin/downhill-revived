"""owned_process: a started process and everything it starts end together, on stop, a timeout, an
interrupt, the tool's own end (even killed, on Windows) and after the root has exited on its own.

    python port/tools/test_owned_process.py
    DHD_OWNED_GAME=build/host64-exe/dhd64.exe python port/tools/test_owned_process.py   # + a real game

The process trees are small Python sleepers (TREE below): each writes its PID to a file, starts the
next one with plain subprocess.Popen and sleeps. With DHD_OWNED_GAME (and DHD_ISO, else
state_trace_run's) the game also starts minimised, gets to its first pictures and is stopped by
closing its window.
"""
from pathlib import Path
import os
import shutil
import signal
import subprocess
import sys
import tempfile
import textwrap
import time
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import owned_process

# argv: depth, PID file, 'exit' to end the root once the whole tree is up (its children run on).
# Each level starts the next one, then writes its PID.
TREE = textwrap.dedent('''
    import os, subprocess, sys, time
    depth, pids, mode = int(sys.argv[1]), sys.argv[2], sys.argv[3]
    if depth > 1:
        subprocess.Popen([sys.executable, __file__, str(depth - 1), pids, 'sleep'])
    with open(pids, 'a') as f:
        f.write('%d\\n' % os.getpid())
    if mode == 'exit':
        while len(open(pids).read().split()) < depth:
            time.sleep(0.02)
    else:
        time.sleep(120)
''')

# A tool: starts a three-level tree and polls, as the tools do; on KeyboardInterrupt it leaves the
# with-block (Windows: CTRL_BREAK_EVENT is made the Ctrl+C it stands for).
TOOL = textwrap.dedent('''
    import signal, subprocess, sys, time
    sys.path.insert(0, sys.argv[1])
    import owned_process
    if hasattr(signal, 'SIGBREAK') and sys.argv[4] == 'interrupt':
        signal.signal(signal.SIGBREAK, signal.default_int_handler)
    try:
        with owned_process.start([sys.executable, sys.argv[2], '3', sys.argv[3], 'sleep']):
            print('started', flush=True)
            for _ in range(1200):
                time.sleep(0.1)
    except KeyboardInterrupt:
        print('interrupted', flush=True)
''')


def alive(pid):
    if os.name == 'nt':
        import ctypes
        k32 = ctypes.WinDLL('kernel32')
        k32.OpenProcess.restype = ctypes.c_void_p
        handle = k32.OpenProcess(0x1000, False, pid)   # PROCESS_QUERY_LIMITED_INFORMATION
        if not handle:
            return False
        code = ctypes.c_ulong()
        try:
            return bool(k32.GetExitCodeProcess(ctypes.c_void_p(handle), ctypes.byref(code))) and code.value == 259
        finally:
            k32.CloseHandle(ctypes.c_void_p(handle))
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    try:   # a zombie of ours (the tree's root before Popen reaped it) is not alive
        return os.waitpid(pid, os.WNOHANG) == (0, 0)
    except ChildProcessError:
        return True


class OwnedProcess(unittest.TestCase):
    def setUp(self):
        self.dir = Path(tempfile.mkdtemp(prefix='owned-'))
        self.addCleanup(shutil.rmtree, self.dir, True)
        self.tree = self.dir / 'tree.py'
        self.tree.write_text(TREE)
        self.pidfile = self.dir / 'pids.txt'

    def pids(self, count, seconds=20):
        """The tree's PIDs once count of them are written."""
        until = time.monotonic() + seconds
        while time.monotonic() < until:
            if self.pidfile.exists():
                pids = [int(x) for x in self.pidfile.read_text().split()]
                if len(pids) >= count:
                    return pids
            time.sleep(0.05)
        self.fail(f'the tree did not start {count} processes')

    def assertGone(self, pids, seconds=10):
        until = time.monotonic() + seconds
        while any(alive(p) for p in pids) and time.monotonic() < until:
            time.sleep(0.05)
        self.assertEqual([p for p in pids if alive(p)], [], 'processes of the tree still alive')

    def start_tree(self, depth=3, mode='sleep', **kw):
        return owned_process.start([sys.executable, str(self.tree), str(depth), str(self.pidfile), mode], **kw)

    def test_stop_ends_nested_children(self):
        p = self.start_tree(4)
        pids = self.pids(4)
        self.assertTrue(all(alive(x) for x in pids))
        if os.name == 'nt':   # the job holds the whole tree
            self.assertEqual(sorted(p.pids()), sorted(pids))
        p.stop()
        self.assertIsNotNone(p.returncode)
        self.assertGone(pids)
        self.assertEqual(p.pids(), [])
        p.stop()   # a second stop is harmless

    def test_run_timeout(self):
        begun = time.monotonic()
        p = owned_process.run([sys.executable, str(self.tree), '3', str(self.pidfile), 'sleep'], timeout=2)
        self.assertTrue(p.timed_out)
        self.assertLess(time.monotonic() - begun, 15)
        self.assertGone(self.pids(3))

    def test_root_exits_children_still_ended(self):
        p = owned_process.run([sys.executable, str(self.tree), '3', str(self.pidfile), 'exit'], timeout=30)
        self.assertFalse(p.timed_out)
        self.assertEqual(p.returncode, 0)
        self.assertGone(self.pids(3))

    def test_interrupt_in_with_block(self):
        pids = []
        with self.assertRaises(KeyboardInterrupt):
            with self.start_tree(3):
                pids = self.pids(3)
                raise KeyboardInterrupt
        self.assertGone(pids)

    def test_close_then_grace(self):
        asked = []
        p = self.start_tree(2, close=lambda owned: asked.append(owned.pid), grace=0.5)
        pids = self.pids(2)
        begun = time.monotonic()
        p.stop()   # the sleepers ignore the request: ended after the grace time
        self.assertEqual(asked, [p.pid])
        self.assertGreaterEqual(time.monotonic() - begun, 0.4)
        self.assertGone(pids)

    def tool(self, how):
        """A separate tool owning a tree, ended by how ('interrupt' or 'kill'); the tree's PIDs."""
        script = self.dir / 'tool.py'
        script.write_text(TOOL)
        flags = subprocess.CREATE_NEW_PROCESS_GROUP if os.name == 'nt' else 0
        tool = subprocess.Popen([sys.executable, str(script), str(Path(__file__).resolve().parent), str(self.tree),
                                 str(self.pidfile), how], stdout=subprocess.PIPE, text=True, creationflags=flags,
                                start_new_session=os.name != 'nt')
        try:
            self.assertEqual(tool.stdout.readline().strip(), 'started')
            pids = self.pids(3)
            if how == 'interrupt':
                os.kill(tool.pid, signal.CTRL_BREAK_EVENT if os.name == 'nt' else signal.SIGINT)
                self.assertEqual(tool.stdout.readline().strip(), 'interrupted')
            else:
                tool.kill()
            tool.wait(30)
        finally:
            if tool.poll() is None:
                tool.kill()
            tool.stdout.close()
        return pids

    def test_tool_interrupted(self):
        self.assertGone(self.tool('interrupt'))

    @unittest.skipUnless(os.name == 'nt', 'POSIX: a killed tool cannot end its process group')
    def test_tool_killed(self):
        self.assertGone(self.tool('kill'))

    @unittest.skipUnless(os.environ.get('DHD_OWNED_GAME'), 'DHD_OWNED_GAME is not set')
    def test_game_start_and_close(self):
        """The game, minimised, up to its first pictures; stopped by closing its window."""
        from state_trace_run import DEFAULT_ISO
        exe = Path(os.environ['DHD_OWNED_GAME']).resolve()
        env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
        env.update(DHD_ISO=os.environ.get('DHD_ISO', DEFAULT_ISO), DHD_SETTINGS='none', DHD_SKIP_MOVIES='1',
                   DHD_MEMCARD='0', DHD_SND_OFF='1', DHD_RENDER='native',   # the players' renderer
                   DHD_NAV='0:triangle|cross')   # a tool's run: no launcher
        log_path = self.dir / 'game.log'
        with open(log_path, 'wb') as log, \
                owned_process.start([str(exe)], cwd=exe.parent, env=env, stdout=log, stderr=subprocess.STDOUT,
                                    minimized=True, close=owned_process.Owned.close_windows, grace=15) as game:
            until = time.monotonic() + 60
            while game.poll() is None and time.monotonic() < until and b'gs: native renderer' not in log_path.read_bytes():
                game.minimize()
                time.sleep(0.25)
            for _ in range(8):   # a couple of seconds of pictures, kept minimised
                game.minimize()
                time.sleep(0.25)
            self.assertIsNone(game.poll(), 'the game ended by itself; see ' + str(log_path))
            pids = game.pids()
            self.assertIn(game.pid, pids)
            begun = time.monotonic()
            code = game.stop()
            closed_in = time.monotonic() - begun
        self.assertGone(pids)
        print(f'\ngame: {len(pids)} process(es), exit {code} {closed_in:.1f} s after WM_CLOSE '
              f'({"closed itself" if closed_in < 15 else "ended by the job"})', file=sys.stderr)
        self.assertLess(closed_in, 15, 'the game did not close on WM_CLOSE')


if __name__ == '__main__':
    unittest.main()
