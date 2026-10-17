"""Observe the retail race state and drive proof inputs through @file autopad.

Reads EE memory only. Inputs stop at countdown; no late START can pause the race.
Requires Windows and psutil. Artifacts stay under build/port/race-proof/.
"""
from pathlib import Path
import argparse
import ctypes
import hashlib
import json
import os
import shutil
import struct
import subprocess
import time

import psutil

ROOT = Path(__file__).resolve().parents[2]
BOOTSTRAP = ('45:3:triangle,65:3:cross,125:2:cross,140:2:cross,155:2:cross,'
             '170:2:cross,185:2:cross,200:2:cross,215:2:cross,'
             '235:2:start,255:2:cross,275:2:cross')


def write_input(path, value):
    pending = path.with_suffix('.pending')
    pending.write_text(value, encoding='ascii')
    for attempt in range(100):
        try:
            pending.replace(path)
            return
        except PermissionError:
            if attempt == 99: raise
            time.sleep(.005)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('label')
    ap.add_argument('--exe', type=Path, required=True)
    ap.add_argument('--iso', type=Path, default=os.environ.get('DHD_ISO'))
    ap.add_argument('--seconds', type=float, default=350)
    ap.add_argument('--wait-for-race', action='store_true', help='allow up to 600 s of loading, then record at least 55 s after GO')
    ap.add_argument('--trap', action='store_true')
    ap.add_argument('--x87', action='store_true')
    ap.add_argument('--pedal', action='store_true', help='bounded Cross pulses in active racing; no late START')
    ap.add_argument('--steer', action='store_true', help='alternate brief left/right taps in active racing')
    ap.add_argument('--capture-at', type=float, default=250)
    ap.add_argument('--gs-dumps', action='store_true')
    ap.add_argument('--skip-vu', default='', help='comma-separated VU1 byte entries in hex; diagnostic raster suppression')
    ap.add_argument('--skip-overlay', choices=['A','B'], help='A particles/effects; B meshes/screen effects')
    ap.add_argument('--skip-program', default='', help='hex MSCAL entries; suppression follows their MSCNT continuations')
    ap.add_argument('--draw-trace', action='store_true', help='trace race draw provenance under this proof directory')
    ap.add_argument('--vu-capture', action='store_true', help='save first race input for each VU1 entry/overlay under ignored build/')
    ap.add_argument('--vu0-capture', action='store_true', help='save race VU0 microprogram inputs for test_vu0_pcsx2.py under ignored build/')
    ap.add_argument('--vu0-capture-count', type=int, default=8, help='VU0 inputs saved per microprogram entry')
    ap.add_argument('--quad-check', action='store_true', help='log inline asm quadword accesses that are not 16-byte aligned')
    args = ap.parse_args()
    if not args.label or args.label in ('.', '..') or Path(args.label).name != args.label:
        ap.error('label must be a directory name')
    if not args.iso or not args.iso.is_file(): ap.error('provide an existing ISO via --iso or DHD_ISO')
    out = ROOT / 'build/port/race-proof' / args.label
    out.mkdir(parents=True, exist_ok=False)
    exe = out / 'dhd.exe'; shutil.copyfile(args.exe, exe)
    shutil.copyfile(args.exe.with_suffix('.map'), exe.with_suffix('.map'))
    script = out / 'pad.txt'; write_input(script, BOOTSTRAP)
    env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    env.update(DHD_ISO=str(args.iso.resolve()), DHD_LOG='1', DHD_SKIP_MOVIES='1', DHD_MEMCARD='0',
               DHD_AUTOPAD='@' + str(script), DHD_SCREENSHOT=str(out / 'frame'),
               DHD_CAPTURE_AT=str(args.capture_at), DHD_CAPTURE_INTERVAL='3')
    if args.trap: env['DHD_FP_TRAP']='1'
    if args.x87: env['DHD_X87_CHECK']='1'
    if args.skip_vu: env['DHD_DRAW_SKIP_VU'] = args.skip_vu
    if args.skip_overlay: env['DHD_DRAW_SKIP_OVERLAY'] = args.skip_overlay
    if args.skip_program: env['DHD_DRAW_SKIP_PROGRAM'] = args.skip_program
    if args.draw_trace:
        (out / 'trace').mkdir()
        env.update(DHD_GS_TRACE=str(out / 'trace'), DHD_GS_TRACE_AT='280', DHD_GS_TRACE_COUNT='150000', DHD_GS_TRACE_LARGE='1')
    if args.vu_capture:
        (out / 'vu').mkdir()
        env.update(DHD_VU_CAPTURE=str(out / 'vu'), DHD_VU_CAPTURE_AT='285')
    if args.vu0_capture:
        (ROOT / 'build/port/vu0-captures').mkdir(parents=True, exist_ok=True)
        env.update(DHD_VU0_CAPTURE=str(ROOT / 'build/port/vu0-captures'), DHD_VU0_CAPTURE_AT='285',
                   DHD_VU0_CAPTURE_COUNT=str(args.vu0_capture_count))
    if args.quad_check: env['DHD_QUAD_CHECK'] = '1'
    if args.gs_dumps:
        (out / 'dumps').mkdir()
        env.update(DHD_GS_CAPTURE=str(out / 'dumps'), DHD_GS_CAPTURE_AT='285',
                   DHD_GS_CAPTURE_INTERVAL='3', DHD_GS_CAPTURE_FIELDS='4')
    kernel = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel.OpenProcess.restype = ctypes.c_void_p
    kernel.OpenProcess.argtypes = [ctypes.c_uint32, ctypes.c_int, ctypes.c_uint32]
    kernel.ReadProcessMemory.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p,
                                        ctypes.c_size_t, ctypes.c_void_p]
    kernel.CloseHandle.argtypes = [ctypes.c_void_p]
    handle = None; forced = False; go = None; race_seen = False; inputs_done = False
    last_input = 0.; next_record = 0.; active_until = 0.; current = BOOTSTRAP; active_menu = None
    start = time.monotonic(); deadline = max(args.seconds, 600) if args.wait_for_race else args.seconds
    manifest = dict(exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(), trap=args.trap, x87=args.x87,
                    bootstrap=BOOTSTRAP, inputs=[], read_only_ee=True, pedal=args.pedal, steer=args.steer,
                    skip_vu=args.skip_vu, skip_overlay=args.skip_overlay, draw_trace=args.draw_trace,
                    vu_capture=args.vu_capture, skip_program=args.skip_program)
    def read(address, count):
        buffer = ctypes.create_string_buffer(count)
        if not kernel.ReadProcessMemory(handle, address, buffer, count, None):
            raise RuntimeError(f'EE memory read failed at {address:08x}')
        return buffer.raw
    def ints(address, n=1): return struct.unpack('<' + 'i'*n, read(address, n*4))
    def floats(address, n=1): return struct.unpack('<' + 'f'*n, read(address, n*4))
    with (out / 'port.log').open('w') as log, (out / 'state.jsonl').open('w') as states:
        process = subprocess.Popen([str(exe)], cwd=out, env=env, stdout=log,
                                   stderr=log, creationflags=subprocess.CREATE_NO_WINDOW)
        try:
            until = time.monotonic() + 10
            while time.monotonic() < until:
                children = [child for child in psutil.Process(process.pid).children()
                            if child.name().lower() == exe.name.lower()]
                if children:
                    handle = kernel.OpenProcess(0x10, False, children[0].pid)
                    if handle:
                        manifest['game_pid'] = children[0].pid
                        break
                time.sleep(.05)
            if not handle: raise RuntimeError('could not attach to the reserved-RAM game child')
            while process.poll() is None and time.monotonic()-start < deadline:
                now = time.monotonic()-start
                try:
                    menu = ints(0x0829ded8)[0]; phase = ints(0x0843f348)[0]
                except RuntimeError:
                    if now > 15: raise
                    time.sleep(.1); continue # RAM initialization can still be in flight
                race_seen |= menu == 9
                if menu == 9 and phase in (1,2,3): inputs_done = True
                if phase == 2 and menu == 9 and go is None:
                    go = now; deadline = max(args.seconds, go+55)
                if now >= 280 or inputs_done:
                    button = None
                    if not inputs_done:
                        if menu == 9 and phase == 5: button = 'cross'
                        elif not race_seen and menu in (0,1,3,10,11,12,15,16,17,18,32,33,34): button = 'cross'
                    if inputs_done or (race_seen and menu != 9): desired = ''
                    elif now < active_until and menu == active_menu: desired = current
                    elif button and now-last_input >= 4:
                        # The host clock starts just after Popen. A bounded overlapping
                        # window tolerates startup latency; the next state clears it.
                        desired = f'{max(0,now-2):.3f}:4:{button}'
                        active_until = now+2; last_input = now; active_menu = menu
                        manifest['inputs'].append(dict(wall=now, menu=menu, phase=phase, button=button))
                    else: desired = ''
                    if args.pedal and go is not None and menu == 9 and phase in (2,3):
                        elapsed = now-go-3
                        if elapsed >= 0 and elapsed % 20 < 2:
                            desired = f'{go+3+20*int(elapsed//20):.3f}:2:cross'
                    if args.steer and go is not None and menu == 9 and phase in (2,3):
                        elapsed = now-go-6
                        if elapsed >= 0 and elapsed % 12 < .3:
                            tap = int(elapsed//12)
                            button = 'left' if tap % 2 == 0 else 'right'
                            desired += (',' if desired else '') + f'{go+6+12*tap:.3f}:.3:{button}'
                    if desired != current:
                        if go is not None and desired:
                            manifest['inputs'].append(dict(wall=now,menu=menu,phase=phase,script=desired,role='race'))
                        write_input(script, desired); current = desired
                if now >= next_record:
                    next_record = now+1
                    rec = dict(t=round(now,3), menu=menu, mode=ints(0x084b26ac)[0],
                               race=ints(0x0843eec0,6), race_state=phase,
                               ticks=ints(0x0829e3d0,4), riders=[])
                    for i in range(10):
                        rider = 0x082af2c0+i*0x7bd0
                        node = ints(rider+0x792c)[0] & 0xffffffff
                        rec['riders'].append(dict(index=i, node_address=node,
                            bone_cache_index=struct.unpack('<h',read(node+0xb6,2))[0] if node else -1,
                            position=floats(node+16,4) if node else [],
                            velocity=floats(rider+0x4180,4), speed=floats(rider+0x7ac0)[0],
                            crashes=ints(rider+0x7998)[0], physics_state=ints(rider+0x4220)[0],
                            animation=floats(rider+0x459c,6),
                            matrix=floats(node+64,16) if node else []))
                    if menu == 9:
                        # 0016BC30 selects this rider's 26-entry group through xB6.
                        bone_index = rec['riders'][0]['bone_cache_index']
                        rec['bone_rider'] = 0; rec['bone_cache_index'] = bone_index
                        if 0 <= bone_index < 128:
                            rec['bone_cache'] = floats(0x08769830 + bone_index*26*64, 26*16)
                            rec['bone_positions'] = floats(0x087684b0 + bone_index*26*16, 26*4)
                    states.write(json.dumps(rec)+'\n'); states.flush()
                time.sleep(.1)
        finally:
            if handle: kernel.CloseHandle(handle)
            if process.poll() is None:
                forced = True
                subprocess.run(['taskkill','/PID',str(process.pid),'/T','/F'],
                               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                process.wait()
            manifest.update(elapsed=time.monotonic()-start, exit=process.returncode,
                            deliberate_stop=forced, race_start_wall=go, countdown_inputs_stopped=inputs_done)
            (out / 'manifest.json').write_text(json.dumps(manifest,indent=2))
    print(out, manifest['exit'], manifest['race_start_wall'], flush=True)


if __name__ == '__main__': main()
