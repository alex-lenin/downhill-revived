"""Sequential retail/native/exact visual survey; game-derived output stays in build/.

Use --list first, then --case NAME (repeatable). Existing ISO/BIOS are referenced
in place. Menu captures are keyed to the final navigation step, NOT boot time.
Race captures use race_compare's phase/clock anchors and consecutive fields.
No image score is a correctness verdict, especially after physics diverges.
"""
from __future__ import annotations

import argparse
import ctypes
from ctypes import wintypes
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time
import io
from functools import lru_cache

from PIL import Image, ImageDraw

import compare_pcsx2 as compare
import pcsx2_retail as retail
import race_compare as race

ROOT = compare.ROOT
OUT = ROOT / 'build/port/r48sweep'
BOOT = ['0:triangle|cross']
SINGLE = BOOT + ['15:cross', '3:cross', '16:cross']
OPTIONS = BOOT + ['15:down', '15:down', '15:cross']


def cases():
    result = {}
    def menu(name, steps, target):
        result[name] = dict(kind='menu', steps=steps, target=target)
    menu('main', BOOT, 15)
    result['boot-transition'] = dict(kind='fields', nav=','.join(BOOT), first=130, last=170,
        retail_first=930, retail_last=970,
        screen='Independent loading windows: port 130-170, retail 930-970; boot clocks are not equivalent')
    result['boot-transition-trace'] = dict(result['boot-transition'], trace=True, gsdump=True,
        screen='Loading diagnostics with field trace and GS stream; check capture perturbation against baseline')
    result['boot-transition-history'] = dict(result['boot-transition-trace'], gs_first=60, gs_fields=97,
        screen='Loading GS history before corruption; exact initial VRAM and packets can isolate native ownership')
    menu('single-player', BOOT + ['15:cross'], 3)
    menu('rider', BOOT + ['15:cross', '3:cross'], 16)
    menu('single-modes', SINGLE, 1)
    menu('track', SINGLE + ['1:cross'], 11)
    menu('bike', SINGLE + ['1:cross', '11:cross'], 18)
    menu('career', SINGLE + ['1:down', '1:cross'], 12)
    menu('career-super', SINGLE + ['1:down', '1:cross', '12:cross'], 18)
    menu('career-specialized', SINGLE + ['1:down', '1:cross', '12:down', '12:cross'], 17)
    menu('career-freeride', SINGLE + ['1:down', '1:cross', '12:down', '12:cross', '17:cross'], 18)
    menu('track-select', SINGLE + ['1:cross', '11:cross', '18@4C7F38=0:start'], 18)
    result['bike']['settle'] = result['track-select']['settle'] = 600
    menu('rider-info', BOOT + ['15:cross', '3:square'], 3)
    menu('multiplayer', BOOT + ['15:down', '15:cross'], 10)
    menu('options', OPTIONS, 2)
    menu('controls', OPTIONS + ['2:cross'], 13)
    menu('controls-alt', OPTIONS + ['2:cross', '13:right'], 13)
    # Each scheme has vibration OFF/ON entries; one arrow changes only vibration.
    menu('controls-scheme', OPTIONS + ['2:cross', '13:right', '13:right'], 13)
    menu('difficulty', OPTIONS + ['2:down', '2:cross'], 19)
    # These pages run the common modal UI with menu ID 13 while it is open.
    for name, row in [('audio-other', 2), ('video-clips', 3),
                      ('load', 4), ('save', 5), ('replay-load', 6)]:
        # Disabled POSTERS / PICKUP EDITOR are skipped by the game's own cursor.
        menu(name, OPTIONS + ['2:down'] * row + ['2:cross'], 13)
    result['posters'] = dict(kind='unavailable', reason='Locked on a fresh save; no forced unlock or data mutation')
    result['pickup-editor'] = dict(kind='unavailable', reason='Locked on a fresh save')
    result['alpine'] = dict(kind='race', nav=race.NAV)
    result['alpine-actions'] = dict(kind='race', nav=race.NAV, track_id=0,
        pad=('0:60:cross,0.5:2:circle,3:1:square,4:1:r2,5.1:1:l1,'
             '7:1:r1,12:1:r2,14:2:l1+r1'),
        moments=[('combat', 'g', 60), ('jump', 'g', 330), ('trick', 'g', 420),
                 ('air', 'g', 840), ('crash', 'g', 1200), ('later', 'g', 1800)],
        screen='Scripted combat/bunny-hop/trick attempt; actual effects require image review')
    result['alpine-occluder'] = dict(kind='race', nav=race.NAV, track_id=0,
        pad='0:60:cross',
        moments=[('before-plane', 'g', 500), ('plane', 'g', 506), ('after-plane', 'g', 512)],
        screen='Isolate the close dark plane seen at full-pedal GO+507 in both renderers')
    result['alpine-finish'] = dict(kind='race', nav=race.NAV, finish_watch=True,
        until_finish=True, finish_limit=72000, track_id=0, pad='0:1200:cross', live_pad=True,
        moments=[('start', 'g', 30), ('mid', 'g', 3000), ('late', 'g', 9000)])
    # Fresh-save track mask 0x010020C3: six unlocked entries in the retail
    # selector's order (func_001F8440 / func_00193F68). Arrows skip locked rows.
    for index, name in enumerate(('alpine-fr', 'alpine-mx', 'alpine-td',
                                   'utah-fr', 'utah-mx', 'utah-td')):
        prefix, suffix = race.NAV.split('18@4C7F38=1:cross', 1)
        arrows = '18@4C7F38=1+240:down,' * index
        result[name] = dict(kind='race', nav=prefix + arrows + '18@4C7F38=1+240:cross' + suffix,
                            track_id=(0, 6, 24, 1, 7, 13)[index])
    # Use field captures and NAV instead of race_sync_pad: the latter overrides
    # NAV after countdown and cannot drive a paused (stopped-clock) modal.
    menu('pause', race.NAV.split(',') + ['9/2+240:start'], 9)
    result['pause'].update(hold_state='9/2', settle=180)
    for name, row in [('pause-options', 1), ('pause-tricks', 2), ('pause-playlist', 5)]:
        steps = race.NAV.split(',') + ['9/2+240:start', '9/2+240:down'] + ['9/2+0:down'] * (row-1) + ['9/2+0:cross']
        menu(name, steps, 9)
        result[name].update(hold_state='9/2', target_menus=[9, 13], settle=180)
    unlock_buttons = ('up triangle down cross left circle right square '
                      'down up up down down up up').split()
    # The retail recognizer clears a partial code after 20 neutral ticks
    # (001E6F58). Clock predicates advance each press on the next tick, avoiding
    # NAV's normal 30-field retry gap. The second code starts after a quiet gap.
    # Repeat adjacent predicates to hold two fields: a one-field pulse can fall
    # entirely between the retail game's 30 Hz pad samples.
    unlock_steps = [f'9/2@43EEC8={((60 + 6*i) if i < 8 else (180 + 6*(i-8))) + j:X}+0:{b}'
                    for i, b in enumerate(unlock_buttons) for j in range(2)]
    # 001F0780 changes the shell menu word to 2 when this succeeds.
    menu('unlock-probe', race.NAV.split(',') + unlock_steps, 2)
    result['unlock-probe'].update(hold_state='2', settle=180,
                                  unlock_mask=0x035FFFFF,
                                  screen='Exploratory built-in unlock input; verify mask before using')
    menu('unlock-pause', race.NAV.split(',') + unlock_steps + ['2/2+240:start'], 2)
    result['unlock-pause'].update(hold_state='2/2', settle=180, unlock_mask=0x035FFFFF)
    unlock_quit = race.NAV.split(',') + unlock_steps + ['2/2+240:start'] + ['2/2+240:down'] * 8 + ['2/2+240:cross']
    menu('unlock-quit', unlock_quit, 2)
    result['unlock-quit'].update(hold_state='2/2', settle=180, unlock_mask=0x035FFFFF)
    unlocked_levels = (race.NAV.split(',') + unlock_steps + ['2/2+240:start', '2/2+240:down',
                         '2/2+0:down', '2/2+0:down', '2/2+0:cross'])
    menu('unlocked-level-list', unlocked_levels, 2)
    result['unlocked-level-list'].update(hold_state='2/2', settle=180, unlock_mask=0x035FFFFF)
    japan_steps = unlocked_levels + ['2/2+240:right'] + ['2/2+0:right'] * 17 + ['2/2+0:cross', '2/5+240:cross']
    menu('unlocked-japan', japan_steps, 9)
    result['unlocked-japan'].update(hold_state='2/2@4B2840=C', settle=600,
        target_menus=[2, 9], target_phase=[2, 3], target_track=12, unlock_mask=0x035FFFFF)
    for name, row in [('unlocked-fog-editor', 5), ('unlocked-poster-maker', 6), ('unlocked-playlist', 7)]:
        steps = race.NAV.split(',') + unlock_steps + ['2/2+240:start', '2/2+240:down'] + ['2/2+0:down'] * (row-1) + ['2/2+0:cross']
        menu(name, steps, 2)
        result[name].update(hold_state='2/2', settle=180, unlock_mask=0x035FFFFF)
    # Always Stoked (rider +7BB2) survives a level reload. Toggle it off in
    # the bootstrap race; the other ordinary flags reset in 0012C450. Guard
    # later UI steps on that actual flag word so NAV cannot skip the code.
    stoked_off = [f'2/2@43EEC8={300 + 6*i + j:X}+0:{b}'
                  for i, b in enumerate('down square square left circle'.split()) for j in range(2)]
    # The bootstrap can advance from phase 2 to 3 while these delayed steps
    # settle. Both are ordinary racing; the verified flag word is the guard.
    guard = '2@2B6E70=1000001'
    clean_levels = (race.NAV.split(',') + unlock_steps + stoked_off +
                    [guard + '+240:start', guard + '+240:down', guard + '+0:down',
                     guard + '+0:down', guard + '+0:cross'])
    tracks = [(0, 'italy-fr'), (6, 'italy-mx'), (24, 'italy-td'), (1, 'utah-fr'),
              (7, 'utah-mx'), (13, 'utah-td'), (2, 'hawaii-fr'), (8, 'hawaii-mx'),
              (10, 'hawaii-td'), (3, 'peru-fr'), (9, 'peru-mx'), (4, 'peru-td'),
              (5, 'russia-fr'), (14, 'russia-mx'), (22, 'russia-td'), (11, 'scotland-fr'),
              (15, 'scotland-mx'), (16, 'scotland-td'), (12, 'japan-fr'), (20, 'city-fr'),
              (17, 'city-mx'), (18, 'city-td'), (19, 'bc-fr'), (25, 'bc-mx')]
    for index, (track_id, name) in enumerate(tracks):
        arrows = ([guard + '+240:right'] + [guard + '+0:right'] * (index-1)) if index else []
        steps = clean_levels + arrows + [guard + '+240:cross', '2/5+240:cross']
        result['tour-' + name] = dict(kind='race', nav=','.join(steps), unlocked=True,
            track_id=track_id, pad='0:600:cross', finish_watch=True,
            moments=[('countdown', 'c', 120), ('start', 'g', 30), ('mid-clock', 'g', 1800), ('late-clock', 'g', 9000)],
            screen=name + ': clock landmarks; near-finish/results require image and state review')
    menu('loading', race.NAV.split(',')[:-1], 9)
    result['loading'].update(hold_state='9/5', settle=120)
    result['results'] = dict(result['alpine-finish'],
        screen='Full-pedal finish/results probe; native verified, retail finish still requires proof')
    result['replay'] = dict(kind='unavailable',
        reason='Native results-menu live-input playback captured; automated retail finish/replay route remains unverified')
    # The multiplayer path has a combined rider page (34), then loads the
    # selected event directly; the single-player shop flag is inapplicable.
    multi = BOOT + ['15:down', '15:cross', '10:cross', '16:cross', '24:cross', '11:cross',
                    '34+240:cross', '9/5:cross']
    result['split-screen'] = dict(kind='race', nav=','.join(multi), players=2)
    punches = ','.join(f'{0.1 + 0.5*i:.1f}:0.1:{"circle" if i % 2 == 0 else "square"}' for i in range(6))
    result['combat-pair'] = dict(kind='race', nav=','.join(multi), players=2, race_players=1,
        port_env={'DHD_RACE_PAD1': ''}, track_id=0, pad='3:7:cross,' + punches,
        moments=[('hit-' + str(t), 'g', t) for t in (12, 36, 60, 90, 120, 150, 180, 300)],
        screen='Two nearby stationary human riders; alternate punches before pedaling; verify actual hits')
    result['other-tracks'] = dict(kind='unavailable', reason='Track selection route and fresh-save unlock coverage not yet verified')
    menu('arcade', SINGLE + ['1:down', '1:down', '1:cross'], 18)
    menu('training', SINGLE + ['1:up', '1:cross'], 18)
    for name, steps in [('shop-bikes', ['18:cross']),
                        ('shop-jerseys', ['18:down', '18:cross']),
                        ('shop-color', ['18:down', '18:down', '18:cross'])]:
        menu(name, SINGLE + ['1:cross', '11:cross'] + steps, 18)
        result[name]['settle'] = 600
    for name, title in {'single-player': 'Rider selection', 'rider': 'Start new / continue game',
                        'single-modes': 'Single-player mode selection', 'track': 'Single-event type selection',
                        'bike': 'Bike shop', 'multiplayer': 'Multiplayer player-count setup'}.items():
        result[name]['screen'] = title
    return result


def quiet_popen(*args, **kwargs):
    """Ask Windows to show new process windows without activation."""
    startup = subprocess.STARTUPINFO()
    startup.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    startup.wShowWindow = 4  # SW_SHOWNOACTIVATE; game may explicitly show its window
    kwargs.setdefault('startupinfo', startup)
    return ORIGINAL_POPEN(*args, **kwargs)


ORIGINAL_POPEN = subprocess.Popen


def small_windows(pid):
    """Only this run's top-level windows; no foreground/focus calls."""
    if os.name != 'nt':
        return
    user = ctypes.windll.user32
    callback = ctypes.WINFUNCTYPE(ctypes.c_bool, ctypes.c_void_p, ctypes.c_void_p)
    def visit(hwnd, _):
        owner = ctypes.c_ulong()
        user.GetWindowThreadProcessId(ctypes.c_void_p(hwnd), ctypes.byref(owner))
        if owner.value == pid and user.IsWindowVisible(ctypes.c_void_p(hwnd)):
            user.SetWindowPos(ctypes.c_void_p(hwnd), None, 10, 30, 640, 480, 0x14)
        return True
    user.EnumWindows(callback(visit), 0)


def process_inventory():
    class Entry(ctypes.Structure):
        _fields_ = [('size', wintypes.DWORD), ('usage', wintypes.DWORD),
                    ('pid', wintypes.DWORD), ('heap', ctypes.c_size_t),
                    ('module', wintypes.DWORD), ('threads', wintypes.DWORD),
                    ('parent', wintypes.DWORD), ('priority', wintypes.LONG),
                    ('flags', wintypes.DWORD), ('exe', wintypes.WCHAR * 260)]
    kernel = ctypes.windll.kernel32
    kernel.CreateToolhelp32Snapshot.restype = wintypes.HANDLE
    handle = kernel.CreateToolhelp32Snapshot(2, 0)
    entries = []
    entry = Entry()
    entry.size = ctypes.sizeof(entry)
    if kernel.Process32FirstW(wintypes.HANDLE(handle), ctypes.byref(entry)):
        while True:
            entries.append((entry.pid, entry.parent, entry.exe))
            if not kernel.Process32NextW(wintypes.HANDLE(handle), ctypes.byref(entry)):
                break
    kernel.CloseHandle(wintypes.HANDLE(handle))
    return entries


def process_family(pid, entries=None):
    """The fixed-address launcher can start a child which owns EE RAM."""
    entries = process_inventory() if entries is None else entries
    family = {pid}
    for _ in range(4):
        family.update(child for child, parent, _ in entries if parent in family)
    return family


def process_executable(pid):
    kernel = ctypes.windll.kernel32
    kernel.OpenProcess.restype = wintypes.HANDLE
    handle = kernel.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
    if not handle:
        return None
    try:
        path = ctypes.create_unicode_buffer(32768)
        length = wintypes.DWORD(len(path))
        if kernel.QueryFullProcessImageNameW(wintypes.HANDLE(handle), 0, path, ctypes.byref(length)):
            return Path(path.value).resolve()
        return None
    finally:
        kernel.CloseHandle(wintypes.HANDLE(handle))


def require_idle(owned_pid=None):
    # One snapshot: a reservation child born between two snapshots must not be
    # classified as foreign merely because it was absent from the first one.
    entries = process_inventory()
    owned = process_family(owned_pid, entries) if owned_pid else set()
    active = []
    for pid, _, name in entries:
        if pid in owned:
            continue
        if name.lower().startswith('pcsx2'):
            active.append((pid, name))
        elif name.lower() == 'dhd.exe':
            path = process_executable(pid)
            active.append((pid, name, str(path) if path else 'path unavailable'))
    if active:
        raise RuntimeError(f'live capture deferred: another game/emulator is running: {active}')


def wait_idle(seconds=180, require_quiet=False):
    """Give a foreign game several minutes to exit; never stop its PID."""
    deadline = time.monotonic() + seconds
    saw_foreign, quiet_since = require_quiet, None
    while True:
        try:
            require_idle()
            if not saw_foreign:
                return
            if quiet_since is None:
                quiet_since = time.monotonic()
            if time.monotonic() - quiet_since >= 30:
                return
            if time.monotonic() >= deadline:
                raise RuntimeError('another game/emulator is running intermittently; no 30-second quiet window within idle wait')
            time.sleep(1)
        except RuntimeError:
            if time.monotonic() >= deadline:
                raise
            saw_foreign, quiet_since = True, None
            print('Waiting for the other game/emulator to exit (up to 180s)', flush=True)
            time.sleep(min(15, max(0, deadline - time.monotonic())))


def validate_disc(path):
    """The state addresses in this tool are only valid for the USA retail disc."""
    import pycdlib
    disc = pycdlib.PyCdlib()
    try:
        disc.open(str(path))
        cnf = io.BytesIO()
        disc.get_file_from_iso_fp(cnf, iso_path='/SYSTEM.CNF;1')
        if b'SCUS_971.77' not in cnf.getvalue().upper():
            raise ValueError('visual sweep requires USA SCUS_971.77; disc SYSTEM.CNF differs')
    finally:
        disc.close()


def preserve_directory(path, label):
    """Windows can retain emulator file handles briefly after PID termination."""
    target = path.parent / f'{path.name}-{label}-{time.time_ns()}'
    deadline = time.monotonic() + 30
    while True:
        try:
            path.rename(target)
            return
        except PermissionError:
            if time.monotonic() >= deadline:
                raise
            time.sleep(.5)


def bmp_complete(path):
    try:
        with path.open('rb') as f:
            header = f.read(6)
        return len(header) == 6 and header[:2] == b'BM' and int.from_bytes(header[2:6], 'little') == path.stat().st_size
    except OSError:
        return False


def port_words(pid, addresses):
    kernel = ctypes.windll.kernel32
    kernel.OpenProcess.restype = wintypes.HANDLE
    family = process_family(pid)
    for child in sorted(family - {pid}) + [pid]:
        handle = kernel.OpenProcess(0x10, False, child)
        if not handle:
            continue
        values = []
        for address in addresses:
            value = ctypes.c_uint()
            read = ctypes.c_size_t()
            ok = kernel.ReadProcessMemory(wintypes.HANDLE(handle), ctypes.c_void_p(0x08000000 + address),
                                          ctypes.byref(value), 4, ctypes.byref(read))
            if not ok or read.value != 4:
                break
            values.append(value.value)
        kernel.CloseHandle(wintypes.HANDLE(handle))
        if len(values) == len(addresses):
            return values
    return None


def port_menu(pid):
    values = port_words(pid, [0x29DED8])
    return values[0] if values and values[0] <= 34 else None


@lru_cache(maxsize=4)
def retail_export_rva(path):
    """PCSX2 exports EEmem specifically for debuggers (Memory.cpp)."""
    kernel = ctypes.windll.kernel32
    kernel.LoadLibraryExW.argtypes = [wintypes.LPCWSTR, wintypes.HANDLE, wintypes.DWORD]
    kernel.LoadLibraryExW.restype = wintypes.HMODULE
    kernel.GetProcAddress.argtypes = [wintypes.HMODULE, ctypes.c_char_p]
    kernel.GetProcAddress.restype = ctypes.c_void_p
    # Map the PE without resolving imports or executing an entry point.
    module = kernel.LoadLibraryExW(path, None, 1)  # DONT_RESOLVE_DLL_REFERENCES
    if not module:
        raise RuntimeError('cannot map the comparison adapter export table')
    try:
        address = kernel.GetProcAddress(module, b'EEmem')
        if not address:
            raise RuntimeError('comparison adapter lacks the PCSX2 EEmem debug export')
        return address - module
    finally:
        kernel.FreeLibrary(wintypes.HMODULE(module))


def retail_words(pid, addresses):
    """Read the owned emulator's EE words using its public debugger export."""
    class Module(ctypes.Structure):
        _fields_ = [('size', wintypes.DWORD), ('id', wintypes.DWORD), ('pid', wintypes.DWORD),
                    ('global_refs', wintypes.DWORD), ('refs', wintypes.DWORD),
                    ('base', ctypes.c_void_p), ('bytes', wintypes.DWORD), ('module', wintypes.HMODULE),
                    ('name', wintypes.WCHAR * 256), ('path', wintypes.WCHAR * 260)]
    kernel = ctypes.windll.kernel32
    kernel.CreateToolhelp32Snapshot.restype = wintypes.HANDLE
    snapshot = kernel.CreateToolhelp32Snapshot(0x18, pid)
    module = Module()
    module.size = ctypes.sizeof(module)
    ok = kernel.Module32FirstW(wintypes.HANDLE(snapshot), ctypes.byref(module))
    kernel.CloseHandle(wintypes.HANDLE(snapshot))
    if not ok:
        return None
    rva = retail_export_rva(module.path)
    kernel.OpenProcess.restype = wintypes.HANDLE
    handle = kernel.OpenProcess(0x10, False, pid)  # PROCESS_VM_READ only
    if not handle:
        return None
    try:
        base = ctypes.c_uint64()
        read = ctypes.c_size_t()
        if not kernel.ReadProcessMemory(wintypes.HANDLE(handle), ctypes.c_void_p(module.base + rva),
                                        ctypes.byref(base), 8, ctypes.byref(read)) or read.value != 8:
            return None
        values = []
        for address in addresses:
            value = ctypes.c_uint()
            if not kernel.ReadProcessMemory(wintypes.HANDLE(handle), ctypes.c_void_p(base.value + address),
                                            ctypes.byref(value), 4, ctypes.byref(read)) or read.value != 4:
                return None
            values.append(value.value)
        return values
    finally:
        kernel.CloseHandle(wintypes.HANDLE(handle))


def clean_env(iso):
    env = {k: v for k, v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
    env.update(DHD_ISO=iso, DHD_SETTINGS='none', DHD_WINDOW='windowed',
               DHD_WINDOW_WIDTH='640', DHD_WINDOW_HEIGHT='448', DHD_SCALE='2',
               DHD_WIDESCREEN='16:9', DHD_ASPECT='wide', DHD_FONT='sharp',
               DHD_FRAME_CLOCK='1', DHD_LOG='1', DHD_SKIP_MOVIES='1',
               DHD_MEMCARD='0', DHD_SND_OFF='1')
    return env


def wait_capture(process, directory, arm, target_step, target_menu, timeout, settle=120,
                 target_menus=None, target_phase=None, target_track=None):
    logpath = directory / ('retail-stdout.log' if arm == 'retail' else 'port.log')
    deadline = time.monotonic() + timeout
    arrival = None
    accepted_menus = target_menus or [target_menu]
    try:
        while time.monotonic() < deadline:
            require_idle(process.pid)
            small_windows(process.pid)
            log = logpath.read_text(errors='replace')
            emulog = directory / 'pcsx2.log'
            if arm == 'retail' and emulog.exists() and 'ReportErrorAsync: Startup Error' in emulog.read_text(errors='replace'):
                raise RuntimeError('PCSX2 startup error; see pcsx2.log')
            pattern = (r'DHD_COMPARE_NAV step=(\d+) field=(\d+) menu=(-?\d+)' if arm == 'retail'
                       else r'compare: nav step (\d+) field=(\d+) menu=(-?\d+)')
            reached = [(int(f), int(m)) for s, f, m in re.findall(pattern, log) if int(s) == target_step]
            captures = (re.findall(r'DHD_COMPARE_CAPTURE completed=(\d+)', log) if arm == 'retail'
                        else re.findall(r'compare: capture .*? field=(\d+)', log))
            if arm == 'retail':
                states = re.findall(r'DHD_COMPARE_FRAME field=(\d+).*? menu=(-?\d+) phase=(-?\d+)', log)
                actual = int(states[-1][1]) if states else None
                phase = int(states[-1][2]) if states else None
                tracks = re.findall(r'DHD_SWEEP_STATE field=\d+ unlocks=[0-9A-F]+ track=(-?\d+)', log)
                track = int(tracks[-1]) if tracks else None
            else:
                words = port_words(process.pid, [0x29DED8, 0x43F348, 0x4B2840])
                actual, phase, track = words if words else (None, None, None)
            phases = target_phase if isinstance(target_phase, list) else [target_phase]
            at_target = (actual in accepted_menus and (target_phase is None or phase in phases)
                         and (target_track is None or track == target_track))
            read_words = retail_words if arm == 'retail' else port_words
            addresses = [0x4346B4, 0x4B2840, 0x43EEC8, 0x3F5420,
                         0x2B6E68, 0x2B6E6C, 0x2B6E70, 0x2B6E74]
            if reached and at_target and captures:
                if arrival is None:
                    arrival = (int(captures[-1]), actual)
                if int(captures[-1]) >= arrival[0] + settle:
                    result = dict(arrival_field=arrival[0], observed_menu=arrival[1], status='captured', settle=settle,
                                  observed_phase=phase, observed_track=track)
                    result['state_word_addresses'] = addresses
                    result['state_words'] = read_words(process.pid, result['state_word_addresses'])
                    return result
            elif not at_target:
                arrival = None
            if process.poll() is not None:
                raise RuntimeError(f'{arm} exited {process.returncode} before settled target')
            time.sleep(.25)
        raise TimeoutError(f'{arm} final step not settled within {timeout}s; last arrival={arrival}')
    finally:
        compare.stop(process)


def run_menu(args, scenario, directory, arm):
    wait_idle()
    directory.mkdir(parents=True)
    # Menus can advertise their ID before the fade accepts input. A quick
    # one-shot cursor press can be lost; unlike Cross it cannot safely repeat.
    steps = []
    for step in scenario['steps']:
        state, buttons = step.split(':', 1)
        if '+' not in state and not state.startswith('0'):
            state += '+240'
        steps.append(state + ':' + buttons)
    nav = ','.join(steps + [f"{scenario.get('hold_state', scenario['target'])}+100000:triangle"])
    env = clean_env(os.environ['DHD_ISO'])
    env['DHD_PADS'] = str(scenario.get('players', 1))
    interval = scenario.get('interval', 30)
    if 'seed' in scenario:
        env['DHD_RACE_SEED'] = str(scenario['seed'])
    if arm != 'retail':
        exe, digest = compare.snapshot_exe(args.exe.resolve(), directory)
        env.update(DHD_NAV=nav, DHD_RENDER=arm, DHD_SCREENSHOT=str(directory / 'shot'),
                   DHD_CAPTURE_FRAME='60', DHD_CAPTURE_FRAME_INTERVAL=str(interval))
        command, cwd, logname = [str(exe)], ROOT / 'build/port/link', 'port.log'
    else:
        bios = retail.bios_file(args.bios)
        data = retail.config(directory, bios, 13)
        (directory / 'retail').mkdir()
        compare.write_movie(directory / 'input.p2m2', [], 20000)
        env.update(DHD_COMPARE_MOVIE=str(directory / 'input.p2m2'), DHD_COMPARE_NAV=nav,
                   DHD_COMPARE_NAV_PLAYERS=str(scenario.get('players', 1)),
                   DHD_COMPARE_SWEEP_STATE='1',
                   DHD_COMPARE_FRAMELOG='1',
                   DHD_COMPARE_FIELDS=','.join(str(f) for f in range(60, 60001, interval)),
                   DHD_COMPARE_CAPTURE=str(directory / 'retail'))
        if 'seed' in scenario:
            env['DHD_COMPARE_RACE_SEED'] = str(scenario['seed'])
        command = [str(args.pcsx2.resolve()), '-datapath', str(data), '-batch', '-nogui',
                   '-fastboot', '-unlimited', '-logfile', str(directory / 'pcsx2.log'),
                   '--', str(Path(os.environ['DHD_ISO']).resolve())]
        cwd, logname = args.pcsx2.resolve().parent, 'retail-stdout.log'
        digest = compare.sha256(args.pcsx2)
    with (directory / logname).open('wb') as log:
        p = subprocess.Popen(command, cwd=cwd, env=env, stdout=log, stderr=subprocess.STDOUT)
        print(f'{directory.parent.name}/{arm}: PID {p.pid}', flush=True)
        result = wait_capture(p, directory, arm, len(scenario['steps']), scenario['target'], args.timeout,
                              scenario.get('settle', 120), scenario.get('target_menus'),
                              scenario.get('target_phase'), scenario.get('target_track'))
    if 'sample_offsets' in scenario:
        result['sample_offsets'] = scenario['sample_offsets']
    result.update(exe_sha256=digest, nav=nav, expected_menu=scenario['target'])
    if result['observed_menu'] not in scenario.get('target_menus', [scenario['target']]):
        result['status'] = 'wrong-screen'
    if 'unlock_mask' in scenario:
        if arm == 'retail':
            states = re.findall(r'DHD_SWEEP_STATE field=\d+ unlocks=([0-9A-F]+)',
                                (directory / logname).read_text(errors='replace'))
            mask = int(states[-1], 16) if states else None
        else:
            words = result.get('state_words')
            mask = words[0] if words else None
        result['observed_unlock_mask'] = mask
        if mask != scenario['unlock_mask']:
            result['status'] = 'input-failed'
    return result


def menu_images(directory, arm, result, latest=False):
    log = (directory / ('retail-stdout.log' if arm == 'retail' else 'port.log')).read_text(errors='replace')
    if arm == 'retail':
        captures = [(int(f), directory / 'retail' / f'field-{int(f):06d}.png')
                    for f in re.findall(r'DHD_COMPARE_CAPTURE completed=(\d+)', log)]
    else:
        captures = [(int(f), Path(p)) for p, f in re.findall(r'compare: capture (.*?) field=(\d+)', log)]
    if latest:
        return captures[-3:]
    # Settled samples at +60/+90/+120; no nearest-picture search across unrelated screens.
    settle = result.get('settle', 120)
    return [min(captures, key=lambda item: abs(item[0] - result['arrival_field'] - offset))
            for offset in result.get('sample_offsets', (settle-60, settle-30, settle))]


def run_fields(args, scenario, directory, arm):
    wait_idle()
    directory.mkdir(parents=True)
    first, last = scenario['first'], scenario['last']
    env = clean_env(os.environ['DHD_ISO'])
    if arm == 'retail':
        first, last = scenario.get('retail_first', first), scenario.get('retail_last', last)
        data = retail.config(directory, retail.bios_file(args.bios), 13)
        (directory / 'retail').mkdir()
        compare.write_movie(directory / 'input.p2m2', [], last + 120)
        env.update(DHD_COMPARE_MOVIE=str(directory / 'input.p2m2'), DHD_COMPARE_NAV=scenario['nav'],
                   DHD_COMPARE_CAPTURE=str(directory / 'retail'),
                   DHD_COMPARE_FIELDS=','.join(map(str, range(first, last + 1))))
        command = [str(args.pcsx2.resolve()), '-datapath', str(data), '-batch', '-nogui', '-fastboot',
                   '-unlimited', '-logfile', str(directory / 'pcsx2.log'), '--', os.environ['DHD_ISO']]
        cwd, logname, digest = args.pcsx2.resolve().parent, 'retail-stdout.log', compare.sha256(args.pcsx2)
        paths = [directory / 'retail' / f'field-{f:06d}.png' for f in range(first, last + 1)]
        def finished():
            text = (directory / logname).read_text(errors='replace')
            return all(path.exists() for path in paths) and bool(re.search(rf'completed={last}\b', text))
    else:
        exe, digest = compare.snapshot_exe(args.exe.resolve(), directory)
        env.update(DHD_NAV=scenario['nav'], DHD_RENDER=arm, DHD_SCREENSHOT=str(directory / 'shot'),
                   DHD_CAPTURE_FIELDS=f'{first}-{last}')
        if scenario.get('trace'):
            env.update(DHD_FIELD_TRACE=str(directory / 'trace'), DHD_FIELD_TRACE_FIELDS='125-170')
            env['DHD_NATIVE_TRACE'] = str(directory / 'native')
        if scenario.get('gsdump'):
            (directory / 'gs').mkdir()
            env.update(DHD_GS_CAPTURE=str(directory / 'gs'),
                       DHD_GS_CAPTURE_FRAME=str(scenario.get('gs_first', 125)),
                       DHD_GS_CAPTURE_FIELDS=str(scenario.get('gs_fields', 32)))
        command, cwd, logname = [str(exe)], ROOT / 'build/port/link', 'port.log'
        paths = [directory / f'shot-f{f:06d}.bmp' for f in range(first, last + 1)]
        def finished():
            return all(bmp_complete(path) for path in paths)
    with (directory / logname).open('wb') as log:
        p = subprocess.Popen(command, cwd=cwd, env=env, stdout=log, stderr=subprocess.STDOUT)
        print(f'{directory.parent.name}/{arm}: PID {p.pid}', flush=True)
        deadline = time.monotonic() + args.timeout
        try:
            while not finished():
                require_idle(p.pid)
                small_windows(p.pid)
                if p.poll() is not None or time.monotonic() >= deadline:
                    raise RuntimeError('consecutive-field probe did not finish; see port.log')
                time.sleep(.25)
        finally:
            compare.stop(p)
    return dict(status='captured', first=first, last=last, captures=len(paths), exe_sha256=digest)


def sheet(directory, images, filename='contact.png'):
    arms = list(images)
    canvas = Image.new('RGB', (800 * len(arms), 478 * 3), '#202020')
    draw = ImageDraw.Draw(canvas)
    for col, arm in enumerate(arms):
        for row, (field, path) in enumerate(images[arm]):
            im = Image.open(path).convert('RGB')
            # Raw GS buffers have non-square display pixels. Compare intended
            # presentation geometry: retail 4:3, port 16:9, common height.
            im = im.resize((597 if arm == 'retail' else 796, 448), Image.Resampling.BILINEAR)
            x, y = col * 800, row * 478
            draw.text((x + 5, y + 5), f'{arm} field {field} ({path.name})', fill='white')
            canvas.paste(im, (x + (800-im.width)//2, y+25))
    canvas.save(directory / filename)


def run_race(args, scenario, directory, arm):
    wait_idle()
    directory.mkdir(parents=True)
    if scenario.get('finish_watch') and arm != 'retail' and b'DHD_RACE_CAPTURE_INTERVAL' not in args.exe.read_bytes():
        raise RuntimeError('results-tail survey requires a clean executable with periodic race capture support')
    if scenario.get('unlocked'):
        marker = b'DHD_COMPARE_RACE_UNLOCKED' if arm == 'retail' else b'DHD_RACE_SYNC_UNLOCKED'
        binary = args.pcsx2 if arm == 'retail' else args.exe
        if marker not in binary.read_bytes():
            raise RuntimeError('unlocked race survey requires a rebuilt clean executable / comparison adapter')
    moments = scenario.get('moments', [('countdown', 'c', 120), ('start', 'g', 30), ('sun', 'g', 240),
               ('mid', 'g', 960), ('pickup', 'g', 1420), ('later', 'g', 3000)])
    pad = scenario.get('pad', race.RACE_PAD)
    a = argparse.Namespace(exe=args.exe.resolve(), pcsx2=args.pcsx2.resolve(), bios=args.bios,
        nav=scenario['nav'], race_seed=81257, race_pad=pad, retail_renderer=13,
        window=2, dump='track_id' in scenario, gsdump=False,
        port_env=dict(clean_env(os.environ['DHD_ISO']), DHD_RENDER=arm,
                      DHD_PADS=str(scenario.get('players', 1))),
        retail_env=dict(DHD_COMPARE_NAV_PLAYERS=str(scenario.get('players', 1))))
    a.race_players = scenario.get('race_players', scenario.get('players', 1))
    a.port_env.update(scenario.get('port_env', {}))
    if scenario.get('finish'):
        a.race_movie_fields = 36120
        a.port_env.update(DHD_CAPTURE_FRAME='3000', DHD_CAPTURE_FRAME_INTERVAL='120',
                          DHD_RACE_FRAMELOG='1')
        a.retail_env.update(DHD_COMPARE_RACE_STOP='36000', DHD_COMPARE_FRAMELOG='1',
            DHD_COMPARE_FIELDS=','.join(str(f) for f in range(3000, 40001, 120)))
    if scenario.get('unlocked'):
        a.port_env['DHD_RACE_SYNC_UNLOCKED'] = '1'
        a.retail_env['DHD_COMPARE_RACE_UNLOCKED'] = '1'
    live_pad = directory / 'live-pad.txt'
    if scenario.get('live_pad'):
        if arm == 'retail':
            if b'DHD_COMPARE_LIVE_PAD' not in args.pcsx2.read_bytes():
                raise RuntimeError('stopped-clock input requires the rebuilt comparison adapter')
            live_pad.write_text('4000\n')  # Pedal, active-high PS2 CROSS.
            a.retail_env['DHD_COMPARE_LIVE_PAD'] = str(live_pad)
        else:
            live_pad.write_text(pad)
            a.port_env['DHD_RACE_PAD'] = '@' + str(live_pad)
    if scenario.get('finish_watch'):
        limit = scenario.get('finish_limit', 12000)
        a.race_movie_fields = max(36120, limit + 120)
        a.port_env.update(DHD_RACE_CAPTURE_FROM='3000', DHD_RACE_CAPTURE_INTERVAL='120')
        a.retail_env.update(DHD_COMPARE_RACE_STOP=str(limit),
            DHD_COMPARE_FIELDS=','.join(str(f) for f in range(3000, max(60001, limit+20000), 120)))
    if arm == 'retail':
        p, _ = race.launch_retail(a, directory, race.clock_events(pad), moments)
    else:
        p, _ = race.launch_port(a, directory, race.clock_events(pad), moments)
    print(f'{directory.parent.name}/{arm}: PID {p.pid}', flush=True)
    deadline = time.monotonic() + args.timeout
    finish_since = None
    saw_go = False
    finish_snapshot = None
    last_state = None
    try:
        while time.monotonic() < deadline:
            require_idle(p.pid)
            small_windows(p.pid)
            if p.poll() is not None:
                break
            last = moments[-1]
            if not scenario.get('finish') and not scenario.get('finish_watch') and arm != 'retail' and bmp_complete(directory / f'port/race-{last[1]}{last[2]:+d}+4.bmp'):
                break
            if scenario.get('finish_watch'):
                logpath = directory / ('retail-stdout.log' if arm == 'retail' else 'port.log')
                logtext = logpath.read_text(errors='replace')
                fields = re.findall(r'(?:DHD_COMPARE_CAPTURE completed=|compare: capture .*? field=)(\d+)', logtext)
                field = int(fields[-1]) if fields else 0
                key = f'{last[1]}{last[2]:+d}'
                if arm == 'retail':
                    anchor = re.search(r'DHD_COMPARE_RACE moment=' + re.escape(key) + r' field=(\d+)', logtext)
                    last_done = bool(anchor and (directory / 'retail' / f'field-{int(anchor[1])+6:06d}.png').exists())
                else:
                    last_done = bmp_complete(directory / f'port/race-{key}+4.bmp')
                state = None
                if 'anchor=g ' in logtext:
                    reader = retail_words if arm == 'retail' else port_words
                    state = reader(p.pid, [0x4B2840, 0x43EF18, 0x43EEC8, 0x2B6C50, 0x43F344])
                    if state:
                        last_state = state
                    if state and state[0] == scenario['track_id'] and state[1] & 255 and finish_since is None:
                        finish_since, finish_snapshot = field + 120, state
                        if scenario.get('live_pad'):
                            # Release pedal so held CROSS cannot dismiss results.
                            replacement = live_pad.with_suffix('.tmp')
                            replacement.write_text('0\n' if arm == 'retail' else '')
                            replacement.replace(live_pad)
                if finish_since is not None and field >= finish_since + 600:
                    break
                if finish_since is None and last_done and not scenario.get('until_finish'):
                    break
                if scenario.get('until_finish') and state and state[2] >= scenario['finish_limit'] and finish_since is None:
                    break
            if scenario.get('finish'):
                logpath = directory / ('retail-stdout.log' if arm == 'retail' else 'port.log')
                states = re.findall(r'(?:DHD_COMPARE_FRAME|compare: frame) field=(\d+).*? menu=(-?\d+) phase=(-?\d+) clock=(-?\d+)',
                                    logpath.read_text(errors='replace'))
                if states:
                    field, menu, phase, clock = map(int, states[-1])
                    saw_go |= menu == 9 and phase in (2, 3) and clock > 60
                    if saw_go and clock >= 36000 and finish_since is None:
                        break  # same ten-game-minute bound as the retail movie/stop
                    if saw_go and (menu != 9 or phase not in (1, 2, 3, 5)):
                        if finish_since is None:
                            finish_since = field
                        if field >= finish_since + 600:
                            break
            time.sleep(.25)
        else:
            raise TimeoutError(f'{arm} race deadline exceeded')
    finally:
        compare.stop(p)
    files = list((directory / ('retail' if arm == 'retail' else 'port')).glob('*.png' if arm == 'retail' else '*.bmp'))
    logtext = (directory / ('retail-stdout.log' if arm == 'retail' else 'port.log')).read_text(errors='replace')
    counts = {}
    for name, anchor, tick in moments:
        key = f'{anchor}{tick:+d}'
        if arm == 'retail':
            found = re.search(r'DHD_COMPARE_RACE moment=' + re.escape(key) + r' field=(\d+)', logtext)
            counts[name] = sum((directory / 'retail' / f'field-{int(found[1])+2+d:06d}.png').exists() for d in range(5)) if found else 0
        else:
            counts[name] = sum(bmp_complete(directory / f'port/race-{key}+{d}.bmp') for d in range(5))
    result = dict(status='captured' if all(n == 5 for n in counts.values()) else 'incomplete',
                  moments=moments, pad=pad, captures=len(files))
    result['moment_counts'] = counts
    if scenario.get('finish_watch'):
        result.update(finish_state_field=finish_since, finish_state_words=finish_snapshot,
                      final_state_words=last_state,
                      final_state_addresses=[0x4B2840, 0x43EF18, 0x43EEC8, 0x2B6C50, 0x43F344])
        if finish_since is not None:
            result['status'] = 'finish-observed'
    if scenario.get('finish'):
        result.update(status='needs-review' if finish_since is not None else 'finish-not-observed',
                      finish_state_field=finish_since)
    if 'track_id' in scenario:
        import struct
        dumps = directory / ('retail-ee' if arm == 'retail' else 'port-ee')
        path = dumps / f'ee-{moments[0][1]}{moments[0][2]:+d}.bin'
        if path.exists():
            with path.open('rb') as f:
                f.seek(0x4B2840)
                result['observed_track'] = struct.unpack('<I', f.read(4))[0]
            result['expected_track'] = scenario['track_id']
            if result['observed_track'] != scenario['track_id']:
                result['status'] = 'wrong-track'
            if scenario.get('unlocked'):
                with path.open('rb') as f:
                    f.seek(0x2B6E68)
                    result['ordinary_cheat_words'] = list(struct.unpack('<3I', f.read(12)))
                if result['ordinary_cheat_words'] != [0, 0, 0]:
                    result['status'] = 'input-failed'
        else:
            result['status'] = 'incomplete'
    return result


def race_sheets(directory, manifest):
    images = {}
    for arm, result in manifest['arms'].items():
        if 'moments' not in result:
            continue
        base = directory / arm
        log = (base / ('retail-stdout.log' if arm == 'retail' else 'port.log')).read_text(errors='replace')
        for name, anchor, tick in result['moments']:
            key = f'{anchor}{tick:+d}'
            if arm == 'retail':
                found = re.search(r'DHD_COMPARE_RACE moment=' + re.escape(key) + r' field=(\d+)', log)
                paths = [(int(found[1]) + 2 + d, base / 'retail' / f'field-{int(found[1])+2+d:06d}.png')
                         for d in range(5)] if found else []
            else:
                paths = [(d, base / 'port' / f'race-{key}+{d}.bmp') for d in range(5)]
            paths = [(f, p) for f, p in paths if p.exists() and (arm == 'retail' or bmp_complete(p))]
            if paths:
                images.setdefault(name, {})[arm] = paths
    for name, arms in images.items():
        canvas = Image.new('RGB', (800 * len(arms), 478 * 5), '#202020')
        draw = ImageDraw.Draw(canvas)
        for col, (arm, paths) in enumerate(arms.items()):
            for row, (field, path) in enumerate(paths):
                im = Image.open(path).convert('RGB')
                im = im.resize((597 if arm == 'retail' else 796, 448), Image.Resampling.BILINEAR)
                x, y = col * 800, row * 478
                draw.text((x+5, y+5), f'{arm} {name} field/delta={field} {path.name}', fill='white')
                canvas.paste(im, (x+(800-im.width)//2, y+25))
        canvas.save(directory / f'contact-{name}.png')
    if manifest.get('scenario', {}).get('finish') or manifest.get('scenario', {}).get('finish_watch'):
        tail = {arm: menu_images(directory / arm, arm, result, latest=True)
                for arm, result in manifest['arms'].items()
                if result.get('status') in ('needs-review', 'finish-not-observed', 'captured', 'incomplete', 'finish-observed')}
        if tail:
            sheet(directory, tail, 'contact-finish-candidate.png')


def review_pages(directory, manifest):
    """Inventory every actual capture, including incidental periodic pictures."""
    output = directory / 'review-pages'
    output.mkdir(exist_ok=True)
    index = {}
    errors = {}
    for arm in manifest.get('arms', {}):
        folder = directory / arm / ('retail' if arm == 'retail' else 'port')
        files = sorted(folder.glob('*.png' if arm == 'retail' else '*.bmp'))
        # Menu port captures live one level above race captures.
        if arm != 'retail' and not files:
            files = sorted((directory / arm).glob('shot*.bmp'))
        for page, start in enumerate(range(0, len(files), 24)):
            selected = files[start:start + 24]
            canvas = Image.new('RGB', (1280, 6 * 256), '#202020')
            draw = ImageDraw.Draw(canvas)
            for i, path in enumerate(selected):
                x, y = (i % 4) * 320, (i // 4) * 256
                draw.text((x + 2, y + 2), path.name, fill='white')
                try:
                    with Image.open(path) as source:
                        im = source.convert('RGB').resize((299 if arm == 'retail' else 318, 224),
                                                          Image.Resampling.BILINEAR)
                    canvas.paste(im, (x + (320 - im.width) // 2, y + 22))
                except (OSError, ValueError) as error:
                    # PID termination can cut the final incidental BMP short.
                    # Inventory it visibly; never substitute a nearby picture.
                    errors[str(path.relative_to(directory))] = str(error)
                    draw.text((x + 5, y + 50), 'UNREADABLE CAPTURE', fill='#ff7070')
            name = f'{arm}-{page:03}.png'
            canvas.save(output / name)
            index[name] = [str(p.relative_to(directory)) for p in selected]
    (output / 'index.json').write_text(json.dumps(index, indent=2) + '\n')
    (output / 'errors.json').write_text(json.dumps(errors, indent=2) + '\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--report', action='store_true', help='rebuild sheets and report coverage without starting any game')
    ap.add_argument('--review-pages', action='store_true', help='also tile and inventory every actual screenshot for manual review')
    ap.add_argument('--case', action='append', choices=list(cases()))
    ap.add_argument('--arms', nargs='+', choices=['retail', 'native', 'exact'], default=['retail', 'native', 'exact'])
    ap.add_argument('--exe', type=Path, default=ROOT / 'build/port/clean/bde3cf92/dhd.exe')
    ap.add_argument('--pcsx2', type=Path, default=ROOT / 'build/port/compare-pcsx2-src/bin/pcsx2-dhd-compare.exe')
    ap.add_argument('--bios', default=os.environ.get('DHD_PCSX2_BIOS'))
    ap.add_argument('--timeout', type=float, default=180)
    ap.add_argument('--retry-blocked', action='store_true', help='preserve failed arm directories and retry them')
    ap.add_argument('--recapture', action='store_true', help='preserve previous arm directories and capture again')
    args = ap.parse_args()
    if args.list:
        print(json.dumps(cases(), indent=2))
        return
    if args.report:
        coverage = {}
        for name, scenario in cases().items():
            directory = OUT / name
            path = directory / 'manifest.json'
            manifest = json.loads(path.read_text()) if path.exists() else dict(scenario=scenario, arms={})
            coverage[name] = dict(screen=scenario.get('screen', name), kind=scenario['kind'],
                                  reason=scenario.get('reason'), arms=manifest.get('arms', {}))
            if scenario['kind'] == 'menu':
                images = {arm: menu_images(directory / arm, arm, result)
                          for arm, result in manifest.get('arms', {}).items() if result['status'] == 'captured'}
                if images:
                    sheet(directory, images)
                    tail = {arm: menu_images(directory / arm, arm, result, latest=True)
                            for arm, result in manifest.get('arms', {}).items() if result['status'] == 'captured'}
                    sheet(directory, tail, 'contact-latest.png')
            elif scenario['kind'] == 'race':
                race_sheets(directory, manifest)
            if args.review_pages and directory.exists():
                review_pages(directory, manifest)
        OUT.mkdir(parents=True, exist_ok=True)
        (OUT / 'coverage.json').write_text(json.dumps(coverage, indent=2) + '\n')
        print(json.dumps({name: {a: r['status'] for a, r in row['arms'].items()}
                          for name, row in coverage.items()}, indent=2))
        return
    if not args.case:
        ap.error('select --case; --list describes the navigation inventory')
    if not Path(os.environ.get('DHD_ISO', '')).is_file():
        ap.error('set DHD_ISO to your existing ISO')
    validate_disc(os.environ['DHD_ISO'])
    subprocess.Popen = quiet_popen
    OUT.mkdir(parents=True, exist_ok=True)
    for name in args.case:
        scenario = cases()[name]
        directory = OUT / name
        directory.mkdir(exist_ok=True)
        manifest_path = directory / 'manifest.json'
        manifest = json.loads(manifest_path.read_text()) if manifest_path.exists() else dict(scenario=scenario, arms={})
        manifest['scenario'] = scenario
        if scenario['kind'] != 'unavailable':
            manifest.pop('status', None)
            manifest.pop('reason', None)
        if scenario['kind'] == 'unavailable':
            manifest.update(status='open', reason=scenario['reason'])
            manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
            continue
        for arm in args.arms:
            previous = manifest['arms'].get(arm)
            if previous and not args.recapture and not (args.retry_blocked and previous['status'] != 'captured'):
                continue
            try:
                existing = directory / arm
                if existing.exists():
                    # Preserve evidence; an atomic rename also refuses a still-open run.
                    preserve_directory(existing, 'previous')
                run = {'menu': run_menu, 'fields': run_fields, 'race': run_race}[scenario['kind']]
                try:
                    manifest['arms'][arm] = run(args, scenario, directory / arm, arm)
                except RuntimeError as exc:
                    if 'another game/emulator is running' not in str(exc):
                        raise
                    # A foreign instance can appear during capture. The run's
                    # finally block has stopped only our PID family. Wait and
                    # retry this same arm once, preserving its interrupted data.
                    wait_idle(require_quiet=True)
                    if existing.exists():
                        preserve_directory(existing, 'interrupted')
                    manifest['arms'][arm] = run(args, scenario, directory / arm, arm)
            except Exception as exc:
                manifest['arms'][arm] = dict(status='blocked', error=str(exc))
                print(f'{name}/{arm}: {exc}', flush=True)
            manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
        if scenario['kind'] == 'menu':
            images = {arm: menu_images(directory / arm, arm, result)
                      for arm, result in manifest['arms'].items() if result['status'] == 'captured'}
            if images:
                sheet(directory, images)
        elif scenario['kind'] == 'race':
            race_sheets(directory, manifest)
        if args.review_pages:
            review_pages(directory, manifest)


if __name__ == '__main__':
    main()
