"""Readers/writer lock between trial builds and promotion, so several trials can run at once.

try_func (a reader) builds against the main build's sources; promote.py (the
writer) rewrites src/ and rebuilds. The writer creates build/src.lock and waits until no reader is
registered; readers wait while the lock exists. Both sides clear stale entries of killed runs.
"""
import contextlib
import os
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOCK = os.path.join(ROOT, 'build', 'src.lock')
READERS = os.path.join(ROOT, 'build', 'src.readers')
QUEUE = os.path.join(ROOT, 'build', 'src.queue')  # waiting writers, oldest first
STALE = 900  # seconds; longer than any single trial
LOCK_STALE = 3600  # longer than any promotion batch


def _stale(path, limit=STALE):
    try:
        return time.time() - os.path.getmtime(path) > limit
    except OSError:
        return False


def _alive(pid):
    if os.name != 'nt':
        try:
            os.kill(pid, 0)
            return True
        except OSError:
            return False
    import ctypes  # os.kill would terminate the process on Windows
    k = ctypes.windll.kernel32
    h = k.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
    if not h:
        return False
    code = ctypes.c_ulong()
    ok = k.GetExitCodeProcess(h, ctypes.byref(code))
    k.CloseHandle(h)
    return bool(ok) and code.value == 259  # STILL_ACTIVE


def _dead_reader(name):
    path = os.path.join(READERS, name)
    return _stale(path) or (name.isdigit() and not _alive(int(name)))


def _drop(path):
    with contextlib.suppress(OSError):
        os.remove(path)


@contextlib.contextmanager
def shared():
    os.makedirs(READERS, exist_ok=True)
    me = os.path.join(READERS, '%d' % os.getpid())
    while True:
        while os.path.exists(LOCK) and not _stale(LOCK, LOCK_STALE):
            time.sleep(1)
        open(me, 'w').close()
        if not os.path.exists(LOCK) or _stale(LOCK, LOCK_STALE):  # re-check: a writer may have just started
            break
        _drop(me)
    try:
        yield
    finally:
        _drop(me)


@contextlib.contextmanager
def exclusive():
    os.makedirs(READERS, exist_ok=True)
    if os.environ.get('DHD_SRC_LOCKED'):  # nested: a batch tool -> promote.py
        yield
        return
    # writers take the lock in arrival order (a ticket each), so a waiting writer isn't overtaken forever
    os.makedirs(QUEUE, exist_ok=True)
    ticket = os.path.join(QUEUE, '%020d-%d' % (time.time_ns(), os.getpid()))
    open(ticket, 'w').close()
    try:
        while True:
            first = None
            for n in sorted(os.listdir(QUEUE)):
                pid = n.rpartition('-')[2]
                if pid.isdigit() and _alive(int(pid)):
                    first = n
                    break
                _drop(os.path.join(QUEUE, n))
            if first == os.path.basename(ticket):
                try:
                    fd = os.open(LOCK, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
                    break
                except FileExistsError:
                    if _stale(LOCK, LOCK_STALE):
                        _drop(LOCK)
            time.sleep(1)
    finally:
        _drop(ticket)
    os.environ['DHD_SRC_LOCKED'] = '1'
    try:
        while True:
            live = []
            for n in os.listdir(READERS):
                if _dead_reader(n):
                    _drop(os.path.join(READERS, n))
                else:
                    live.append(n)
            if not live:
                break
            time.sleep(1)
        yield
    finally:
        os.environ.pop('DHD_SRC_LOCKED', None)
        os.close(fd)
        _drop(LOCK)
