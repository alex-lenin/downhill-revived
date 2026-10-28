"""The test tools' one runner for the game and its helpers (PresentMon, nettool, ...): a process
started here and every process it starts are owned together and end together: at stop(), at the
end of its with-block, on a timeout, an error or Ctrl+C in the tool, and when the tool exits.

    with owned_process.start([exe], cwd=..., env=..., stdout=log, minimized=True) as game:
        while game.poll() is None:
            game.minimize()        # or game.focus(): the owned processes' own windows
            ...
    # here the game and everything it started are gone

    game = owned_process.run([exe], timeout=600, ...)   # waits; game.timed_out, game.returncode

close= asks a process to end on its own first (the game's WM_CLOSE, so its logs flush; PresentMon's
`logman stop`, so its ETW session does not outlive it): stop() calls it once, waits up to grace
seconds for the process to end, then ends the whole tree.

- Windows: each process is created suspended in a new process group, put into a job object of its
  own with KILL_ON_JOB_CLOSE, then resumed, so whatever it starts is in the job from its start.
  Ending the tree terminates the job and waits until it is empty; the job's handle closing (this
  tool ending in any way, even killed) ends it too. The window helpers find the tree's windows by
  the job's process ids.
- POSIX (Linux, macOS): each process leads a session of its own (its process group); ending the
  tree sends SIGTERM to the group, then SIGKILL after a second. SIGTERM to the tool ends it through
  its exit, which stops what is still owned. The window helpers do nothing there.

Only processes started here are touched: nothing is found or stopped by name or by a bare PID.
"""
import atexit
import os
import signal
import subprocess
import sys
import time

WINDOWS = os.name == 'nt'
_live = set()   # every Owned not yet stopped


class Owned:
    """A started process and its descendants, with Popen's poll, wait, pid, returncode and args."""

    def __init__(self, args, *, close=None, grace=10.0, minimized=False, **popen):
        self.args = args
        self.close = close
        self.grace = grace
        self.timed_out = False
        self._job = None
        if WINDOWS:
            flags = popen.pop('creationflags', 0) | 0x4 | 0x200   # CREATE_SUSPENDED | CREATE_NEW_PROCESS_GROUP
            if minimized:
                info = popen.pop('startupinfo', None) or subprocess.STARTUPINFO()
                info.dwFlags |= subprocess.STARTF_USESHOWWINDOW
                info.wShowWindow = 7   # SW_SHOWMINNOACTIVE (SDL may still show it: minimize() follows up)
                popen['startupinfo'] = info
            self.process = subprocess.Popen(args, creationflags=flags, **popen)
            try:
                self._job = _job_for(self.process)
            except BaseException:
                self.process.kill()
                self.process.wait()
                raise
        else:
            self.process = subprocess.Popen(args, start_new_session=True, **popen)
        _live.add(self)

    pid = property(lambda self: self.process.pid)
    returncode = property(lambda self: self.process.returncode)

    def poll(self):
        return self.process.poll()

    def wait(self, timeout=None):
        """The root's exit code; subprocess.TimeoutExpired after timeout seconds (the tree runs on)."""
        return self.process.wait(timeout)

    def pids(self):
        """The tree's live processes (Windows: the job's; POSIX: the root, while it runs)."""
        if self._job:
            return _job_pids(self._job)
        return [self.pid] if self.poll() is None else []

    def stop(self, grace=None):
        """Ends the tree: close (once, if given) and up to grace seconds for the root to end, then
        whatever is still alive in it. The root's exit code."""
        if self not in _live:
            return self.process.wait(30)
        try:
            if self.close:
                try:
                    self.close(self)
                except Exception as e:   # the tree still ends below
                    print(f'owned_process: closing {self.args[0]} failed: {e}', file=sys.stderr)
                try:
                    self.process.wait(self.grace if grace is None else grace)
                except subprocess.TimeoutExpired:
                    pass
        finally:
            _live.discard(self)
            if self._job:
                _end_job(self._job)
                self._job = None
            else:
                _end_group(self.process)
        try:
            return self.process.wait(30)
        except subprocess.TimeoutExpired:
            raise RuntimeError(f'{self.args[0]} (pid {self.pid}) did not end') from None

    def minimize(self):
        minimize_windows(self.pids())

    def focus(self):
        return focus_windows(self.pids())

    def close_windows(self):
        close_windows(self.pids())

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.stop()


def start(args, **kw):
    """An Owned process: args and keywords as Popen's, with close=, grace= and minimized=."""
    return Owned(list(args), **kw)


def run(args, timeout=None, **kw):
    """Starts, waits up to timeout seconds and ends the tree whatever happened: the Owned, with its
    returncode and timed_out."""
    with start(args, **kw) as p:
        try:
            p.wait(timeout)
        except subprocess.TimeoutExpired:
            p.timed_out = True
    return p


def stop_all():
    """Stops every owned tree still running (at the tool's exit)."""
    for p in list(_live):
        try:
            p.stop()
        except Exception as e:
            print(f'owned_process: {e}', file=sys.stderr)


atexit.register(stop_all)


# ---- POSIX: process groups ----

def _end_group(process):
    for sig, wait in ((signal.SIGTERM, 1), (signal.SIGKILL, 0)):
        try:
            os.killpg(process.pid, sig)
        except (ProcessLookupError, PermissionError):   # gone (macOS answers EPERM for a group of zombies)
            return
        if wait:
            try:
                process.wait(wait)
            except subprocess.TimeoutExpired:
                pass


if not WINDOWS and signal.getsignal(signal.SIGTERM) is signal.SIG_DFL:
    # The default action would end the tool without its exit handlers (stop_all).
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(128 + signal.SIGTERM))


# ---- Windows: job objects and the tree's windows ----

if WINDOWS:
    import ctypes
    from ctypes import wintypes

    class _Basic(ctypes.Structure):   # JOBOBJECT_BASIC_LIMIT_INFORMATION
        _fields_ = [('PerProcessUserTimeLimit', ctypes.c_int64), ('PerJobUserTimeLimit', ctypes.c_int64),
                    ('LimitFlags', wintypes.DWORD), ('MinimumWorkingSetSize', ctypes.c_size_t),
                    ('MaximumWorkingSetSize', ctypes.c_size_t), ('ActiveProcessLimit', wintypes.DWORD),
                    ('Affinity', ctypes.c_size_t), ('PriorityClass', wintypes.DWORD),
                    ('SchedulingClass', wintypes.DWORD)]

    class _Extended(ctypes.Structure):   # JOBOBJECT_EXTENDED_LIMIT_INFORMATION
        _fields_ = [('Basic', _Basic), ('IoInfo', ctypes.c_uint64 * 6), ('ProcessMemoryLimit', ctypes.c_size_t),
                    ('JobMemoryLimit', ctypes.c_size_t), ('PeakProcessMemoryUsed', ctypes.c_size_t),
                    ('PeakJobMemoryUsed', ctypes.c_size_t)]

    class _PidList(ctypes.Structure):   # JOBOBJECT_BASIC_PROCESS_ID_LIST with room for 1024
        _fields_ = [('Assigned', wintypes.DWORD), ('InList', wintypes.DWORD), ('Ids', ctypes.c_size_t * 1024)]

    _k32 = ctypes.WinDLL('kernel32', use_last_error=True)
    _ntdll = ctypes.WinDLL('ntdll')
    _user32 = ctypes.WinDLL('user32')
    _k32.CreateJobObjectW.restype = wintypes.HANDLE
    _k32.CreateJobObjectW.argtypes = [ctypes.c_void_p, wintypes.LPCWSTR]
    _k32.SetInformationJobObject.argtypes = [wintypes.HANDLE, ctypes.c_int, ctypes.c_void_p, wintypes.DWORD]
    _k32.QueryInformationJobObject.argtypes = [wintypes.HANDLE, ctypes.c_int, ctypes.c_void_p, wintypes.DWORD,
                                               ctypes.c_void_p]
    _k32.AssignProcessToJobObject.argtypes = [wintypes.HANDLE, wintypes.HANDLE]
    _k32.TerminateJobObject.argtypes = [wintypes.HANDLE, wintypes.UINT]
    _k32.CloseHandle.argtypes = [wintypes.HANDLE]
    _ntdll.NtResumeProcess.argtypes = [wintypes.HANDLE]
    _ntdll.NtResumeProcess.restype = ctypes.c_long
    _user32.GetForegroundWindow.restype = wintypes.HWND
    _user32.GetWindowThreadProcessId.argtypes = [wintypes.HWND, ctypes.POINTER(wintypes.DWORD)]
    _user32.PostMessageW.argtypes = [wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPARAM]
    _user32.ShowWindow.argtypes = [wintypes.HWND, ctypes.c_int]
    _user32.IsIconic.argtypes = [wintypes.HWND]
    _user32.IsWindowVisible.argtypes = [wintypes.HWND]
    _user32.SetForegroundWindow.argtypes = [wintypes.HWND]
    _user32.BringWindowToTop.argtypes = [wintypes.HWND]
    _WINDOW_CALLBACK = ctypes.WINFUNCTYPE(wintypes.BOOL, wintypes.HWND, wintypes.LPARAM)

    def _check(ok, what):
        if not ok:
            raise ctypes.WinError(ctypes.get_last_error(), what)

    def _job_for(process):
        """A job that ends with its handle, holding the suspended process; then the process runs."""
        job = _k32.CreateJobObjectW(None, None)
        _check(job, 'CreateJobObjectW')
        info = _Extended()
        info.Basic.LimitFlags = 0x2000   # JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        try:
            _check(_k32.SetInformationJobObject(job, 9, ctypes.byref(info), ctypes.sizeof(info)),
                   'SetInformationJobObject')   # JobObjectExtendedLimitInformation
            _check(_k32.AssignProcessToJobObject(job, int(process._handle)), 'AssignProcessToJobObject')
            status = _ntdll.NtResumeProcess(int(process._handle))
            if status:
                raise OSError(f'NtResumeProcess: NTSTATUS 0x{status & 0xFFFFFFFF:08X}')
        except BaseException:
            _k32.CloseHandle(job)
            raise
        return job

    def _job_pids(job):
        ids = _PidList()
        if not _k32.QueryInformationJobObject(job, 3, ctypes.byref(ids), ctypes.sizeof(ids), None):
            return []   # JobObjectBasicProcessIdList
        return [int(ids.Ids[i]) for i in range(min(ids.InList, len(ids.Ids)))]

    def _end_job(job):
        """Terminates every process in the job, waits until none is left, closes the job."""
        _k32.TerminateJobObject(job, 1)
        until = time.monotonic() + 30
        while _job_pids(job) and time.monotonic() < until:
            time.sleep(0.02)
        _k32.CloseHandle(job)

    def _windows(pids):
        """The visible top-level windows of these processes."""
        pids, found = set(pids), []

        def each(hwnd, _):
            owner = wintypes.DWORD()
            _user32.GetWindowThreadProcessId(hwnd, ctypes.byref(owner))
            if owner.value in pids and _user32.IsWindowVisible(hwnd):
                found.append(hwnd)
            return True
        if pids:
            _user32.EnumWindows(_WINDOW_CALLBACK(each), 0)
        return found

    def minimize_windows(pids):
        """Minimises (without activating) every visible top-level window of these processes."""
        for hwnd in _windows(pids):
            if not _user32.IsIconic(hwnd):
                _user32.ShowWindow(hwnd, 7)   # SW_SHOWMINNOACTIVE

    def focus_windows(pids):
        """Brings these processes' window to the front: whether it has the focus, None before one exists."""
        found = _windows(pids)
        if not found:
            return None
        if _user32.GetForegroundWindow() in found:
            return True
        hwnd = found[0]
        _user32.ShowWindow(hwnd, 9)   # SW_RESTORE
        # A key event lifts Windows' foreground lock for this process; Alt alone does nothing in the game.
        _user32.keybd_event(0x12, 0, 0, 0)
        _user32.keybd_event(0x12, 0, 2, 0)
        _user32.SetForegroundWindow(hwnd)
        _user32.BringWindowToTop(hwnd)
        return _user32.GetForegroundWindow() == hwnd

    def close_windows(pids):
        """Asks each of these processes' windows to close (WM_CLOSE)."""
        for hwnd in _windows(pids):
            _user32.PostMessageW(hwnd, 0x0010, 0, 0)
else:
    def minimize_windows(pids):
        pass

    def focus_windows(pids):
        return None

    def close_windows(pids):
        pass
