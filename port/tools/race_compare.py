"""Compare the race in retail PCSX2 and the native port at the same game moments.

Both runs are synchronised by the game's own state, not by boot or vsync time.
The game's race clock (60 per second, one per PS2 vsync) restarts at the countdown (menu 9, race
phase 1) and at GO (phase 2, still running in phase 3). A moment c+N / g+N is
the first vsync after that phase's clock reaches N; both sides capture it and the
next 2 * window vsyncs. From the countdown both pads are neutral; after GO both
read the same race pad script keyed by the race clock. Before that, each side
boots with the same frame-keyed navigation script (a p2m2 movie for PCSX2).

Vsync counts don't line up: the port's virtual clock spends more vsyncs per game
frame than the PS2 (the countdown took 560 port fields against 260). The port
frame at delta 0 is paired with the most similar PCSX2 frame among deltas
0..2*window; the chosen delta is reported, never widened.

    python port/tools/race_compare.py race-r10 --exe build/port/compare-build/dhd.exe

Needs DHD_ISO, the adapter from prepare_pcsx2_compare.py, and a port image with
DHD_RACE_SYNC (build_compare_port.py). Output: build/port/compare/LABEL/.
"""
from pathlib import Path
from decimal import Decimal
from math import ceil
import argparse
import html
import json
import os
import re
import shutil
import subprocess
import sys
import time

import numpy as np
from PIL import Image, ImageDraw

sys.path.insert(0, str(Path(__file__).resolve().parent))
import compare_pcsx2 as compare
import pcsx2_retail as retail

# Navigation: the standard script, with a Cross every 20 s up to 10 minutes so
# a slower load still leaves the pre-race screen. Race sync ignores it from the
# countdown on, so no late START can pause the race.
NAVIGATION = compare.STANDARD_PAD + ''.join(f',{s}:2:cross' for s in range(375, 600, 20))
# Navigation by the screen shown (port/hle/menu_nav.h, on both sides): each step waits for its
# menu (and race phase), presses, and retries until the screen moves on, so no run waits out a
# timed schedule. Same screens as NAVIGATION: the memory card dialog (Triangle cancels, Cross
# retries) and the title, both menu 0, then 15, 3, 16, 1, 11, the race setup (18: Start until its
# menu object, SharedMenu D_004C7A30, shows the confirmation at +0x508, then Cross) and the race
# load (9/5) that ends on a press. --timed-nav uses NAVIGATION.
NAV = ('0:triangle|cross,15:cross,3:cross,16:cross,1:cross,11:cross,'
       '18@4C7F38=0:start,18@4C7F38=1:cross,9/5:cross')
# After GO: r10's proof inputs, now keyed to the race clock on both sides. Cross pedals
# 2 s every 20 s from GO+3 s; left/right taps 0.3 s alternate every 12 s from GO+6 s.
RACE_PAD = ','.join([f'{3+20*k}:2:cross' for k in range(3)] +
                    [f'{6+12*k}:0.3:{"left" if k % 2 == 0 else "right"}' for k in range(5)])
# Race-clock values (60 per second): countdown 2 s in; GO + 0.5 s; +10/+30/+50 s.
# The race seeds its RNGs (start grid, AI) from EE timer-0 time, which follows load timing (port/hle/autopad.c hle_race_seed).
# PCSX2 is deterministic: with the timed NAVIGATION it seeds 81257 (EE 0x0043FA10 in its dumps) every
# run, the seed of the reference dumps; NAV reaches the race sooner (80728), so both sides pin it.
RETAIL_RACE_SEED = 81257
MOMENTS = [('countdown', 'c', 120), ('start', 'g', 30), ('+10 s', 'g', 600),
           ('+30 s', 'g', 1800), ('+50 s', 'g', 3000)]
CLOCK = 60


def clock_events(script):
    """compare.parse_pad, keyed by race-clock units instead of NTSC fields (as autopad_read)."""
    compare.parse_pad(script)  # validates the script
    events = []
    for event in script.split(','):
        start, duration, chord = event.split(':')
        mask = 0
        for name in chord.split('+'):
            mask |= compare.BUTTONS[name]
        start, duration = Decimal(start), Decimal(duration)
        events.append((ceil(start * CLOCK), ceil((start + duration) * CLOCK), mask, chord))
    return events


def spec(moments):
    return ','.join(f'{anchor}{offset:+d}' for _, anchor, offset in moments)


HEARTBEAT = re.compile(rb'compare: heartbeat field=(\d+) menu=(-?\d+) phase=(-?\d+) clock=(-?\d+) pad=(\d+) eip=([0-9A-F]+)')
MENUS = {9: 'race'}


def port_stall(log, beats=3):
    """'stuck at ...' once the port's virtual field stopped advancing for `beats` heartbeats
    (30 s of wall time apart, port/hle/autopad.c), else None."""
    try:
        found = HEARTBEAT.findall(Path(log).read_bytes())
    except OSError:
        return None
    if len(found) <= beats or len({f[0] for f in found[-beats - 1:]}) != 1:
        return None
    field, menu, phase, clock, pad = (int(v) for v in found[-1][:5])
    eip = found[-1][5].decode()
    screen = MENUS.get(menu, f'menu {menu}')
    return (f'port stuck at {screen} (field {field}, phase {phase}, race clock {clock}, '
            f'last pad read at field {pad}, EE thread at host 0x{eip}: map it with dhd.map); see {log}')


def launch_retail(args, directory, race_events, moments):
    iso = os.environ['DHD_ISO']
    bios = retail.bios_file(args.bios)
    emulator = args.pcsx2.resolve()
    if b'DHD_COMPARE_RACE_CAPTURE' not in emulator.read_bytes():
        raise RuntimeError('rebuild the adapter: python port/tools/prepare_pcsx2_compare.py')
    # Private runtime copy: an adapter rebuild cannot replace a running image.
    runtime = directory / 'pcsx2-runtime'
    runtime.mkdir()
    for item in emulator.parent.iterdir():
        if item.suffix in ('.exe', '.dll', '.conf') and (item.suffix != '.exe' or item == emulator):
            shutil.copyfile(item, runtime / item.name)
        elif item.is_dir() and item.name in ('resources', 'QtPlugins', 'D3D12', 'translations'):
            shutil.copytree(item, runtime / item.name)
    data = retail.config(directory, bios, args.retail_renderer)
    navigation = compare.parse_pad(NAVIGATION)
    compare.write_movie(directory / 'navigation.p2m2', navigation, compare.field_at(620))
    compare.write_movie(directory / 'race.p2m2', race_events,
                        max(max(o for _, _, o in moments) + 2 * CLOCK, getattr(args, 'race_movie_fields', 0)),
                        players=getattr(args, 'race_players', 1))
    (directory / 'retail').mkdir()
    env = {k: v for k, v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
    if getattr(args, 'nav', NAV):
        env['DHD_COMPARE_NAV'] = getattr(args, 'nav', NAV)
    if getattr(args, 'race_seed', RETAIL_RACE_SEED) is not None:
        env['DHD_COMPARE_RACE_SEED'] = str(getattr(args, 'race_seed', RETAIL_RACE_SEED))
    env.update(DHD_COMPARE_MOVIE=str(directory / 'navigation.p2m2'), DHD_COMPARE_RACE='1',
               DHD_COMPARE_RACE_MOVIE=str(directory / 'race.p2m2'),
               DHD_COMPARE_RACE_CAPTURE=spec(moments), DHD_COMPARE_RACE_WINDOW=str(args.window),
               DHD_COMPARE_RACE_STOP=str(max(o for _, a, o in moments if a == 'g') + CLOCK),
               DHD_COMPARE_CAPTURE=str(directory / 'retail'))
    if args.retail_renderer != 13:
        env['DHD_COMPARE_CAPTURE_GPU'] = str(directory / 'retail')
    if args.dump:
        (directory / 'retail-ee').mkdir()
        env['DHD_COMPARE_RACE_DUMP'] = str(directory / 'retail-ee')
    if getattr(args, 'gsdump', False):
        (directory / 'retail-gs').mkdir()
        env['DHD_COMPARE_RACE_GSDUMP'] = str(directory / 'retail-gs')
    env.update(getattr(args, 'retail_env', {}))
    command = [str(runtime / emulator.name), '-datapath', str(data), '-batch', '-nogui', '-fastboot',
               '-unlimited', '-logfile', str(directory / 'pcsx2.log'), '--', str(Path(iso).resolve())]
    log = (directory / 'retail-stdout.log').open('wb')
    return subprocess.Popen(command, cwd=runtime, env=env, stdout=log, stderr=subprocess.STDOUT), bios


def launch_port(args, directory, race_events, moments):
    executable, digest = compare.snapshot_exe(args.exe, directory)
    if b'DHD_RACE_SYNC' not in executable.read_bytes():
        raise RuntimeError('port image lacks DHD_RACE_SYNC; run build_compare_port.py')
    (directory / 'port').mkdir()
    env = {k: v for k, v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
    env.update(DHD_ISO=os.environ['DHD_ISO'], DHD_FRAME_CLOCK='1', DHD_LOG='1', DHD_SKIP_MOVIES='1', DHD_MEMCARD='0',
               DHD_SETTINGS='none',  # no dhd.ini: the tools' settings, never a player's defaults (finding 056)
               DHD_SND_OFF='1', DHD_AUTOPAD_FRAMES=compare.frame_script(compare.parse_pad(NAVIGATION)),
               DHD_RACE_SYNC='1', DHD_RACE_PAD=args.race_pad,
               DHD_RACE_CAPTURE=spec(moments), DHD_RACE_CAPTURE_WINDOW=str(args.window),
               DHD_SCREENSHOT=str(directory / 'port' / 'race'))
    if getattr(args, 'nav', NAV):
        env['DHD_NAV'] = getattr(args, 'nav', NAV)
    if args.race_seed is not None:
        env['DHD_RACE_SEED'] = str(args.race_seed)
    if args.dump:
        (directory / 'port-ee').mkdir()
        env['DHD_RACE_DUMP'] = str(directory / 'port-ee')
    (directory / 'port-gs').mkdir()  # the renderer check replays these
    if not getattr(args, 'no_gsdump', False):
        env['DHD_RACE_GSDUMP'] = str(directory / 'port-gs')
    env.update(getattr(args, 'port_env', {}))
    log = (directory / 'port.log').open('wb')
    # Above normal: a game or build sharing the machine otherwise starves the port's EE thread, and a
    # run that should take 25 minutes takes hours (port/hle/host.c passes the class to its child).
    process = subprocess.Popen([str(executable)], cwd=directory, env=env, stdout=log, stderr=subprocess.STDOUT,
                               creationflags=getattr(subprocess, 'ABOVE_NORMAL_PRIORITY_CLASS', 0))
    return process, digest


def thumb(image):
    return np.asarray(image.resize((64, 56), Image.Resampling.BOX), dtype=np.float32)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('label')
    ap.add_argument('--exe', type=Path, default=compare.ROOT / 'build/port/compare-build/dhd.exe')
    ap.add_argument('--pcsx2', type=Path, default=compare.ROOT / 'build/port/compare-pcsx2-src/bin/pcsx2-dhd-compare.exe')
    ap.add_argument('--bios', default=os.environ.get('DHD_PCSX2_BIOS'))
    ap.add_argument('--retail-renderer', type=int, default=13, help='13 software (default), 3 D3D11')
    ap.add_argument('--window', type=int, default=2, help='fields either side of each moment')
    ap.add_argument('--nav', default=NAV, help='menu navigation steps, menu[/phase]:button,... (menu_nav.h)')
    ap.add_argument('--timed-nav', dest='nav', action='store_const', const='', help='the timed NAVIGATION schedule')
    ap.add_argument('--race-pad', default=RACE_PAD, help='race-clock seconds after GO: start:duration:button,...')
    ap.add_argument('--threshold', type=int, default=16)
    ap.add_argument('--line-shift', type=int, default=1)
    ap.add_argument('--timeout', type=float, default=3600)
    ap.add_argument('--race-seed', type=lambda v: None if v == 'timer' else int(v, 0), default=RETAIL_RACE_SEED,
                    help='race seed for the port (default: the PCSX2 run seed); "timer" keeps its own')
    ap.add_argument('--dump', action='store_true',
                    help='EE RAM + scratchpad at each moment: retail-ee/, port-ee/ (game data, stays in build/)')
    ap.add_argument('--gsdump', action='store_true',
                    help="also retail's GS dumps of the 2 fields at each moment, in retail-gs/ (the port always writes "
                         'port-gs/gs-c+120.gs and its own render of them, for the renderer check)')
    ap.add_argument('--replay-pcsx2', type=Path, default=compare.ROOT / 'build/port/pcsx2/pcsx2-qt.exe',
                    help="PCSX2 (stock 2.8.2) replaying the port's GS dumps for the renderer check")
    ap.add_argument('--replay-timeout', type=float, default=90)
    ap.add_argument('--pair-only', action='store_true', help='rescore an existing output directory')
    args = ap.parse_args()
    if Path(args.label).name != args.label or args.label in ('.', '..'):
        ap.error('label must be a directory name')
    if not os.environ.get('DHD_ISO') or not Path(os.environ['DHD_ISO']).is_file():
        ap.error('set DHD_ISO to your existing ISO')
    directory = compare.OUT / args.label
    race_events = clock_events(args.race_pad)
    expected = len(MOMENTS) * (2 * args.window + 1)
    if not args.pair_only:
        directory.mkdir(parents=True, exist_ok=False)
        manifest = dict(label=args.label, mode='race-sync', navigation=args.nav or NAVIGATION, race_pad=args.race_pad,
                        race_pad_clock=compare.frame_script(race_events), moments=MOMENTS,
                        window=args.window, threshold=args.threshold, status='running')
        pcsx2, bios = launch_retail(args, directory, race_events, MOMENTS)
        port, digest = launch_port(args, directory, race_events, MOMENTS)
        manifest.update(port_exe_sha256=digest, bios_filename=bios.name, bios_referenced_in_place=True,
                        retail_renderer=args.retail_renderer)
        deadline = time.monotonic() + args.timeout
        try:
            while pcsx2.poll() is None or port.poll() is None:
                if len(list((directory / 'port').glob('*.bmp'))) >= expected:
                    compare.stop(port)
                stall = port_stall(directory / 'port.log')
                if stall:
                    raise RuntimeError(stall)
                if time.monotonic() > deadline:
                    raise RuntimeError('deadline exceeded; see pcsx2.log, retail-stdout.log, port.log')
                time.sleep(1)
        finally:
            compare.stop(pcsx2)
            compare.stop(port)
        (directory / 'manifest.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
    manifest = json.loads((directory / 'manifest.json').read_text(encoding='utf-8'))
    pair(args, directory, manifest)


def anchors(text, pattern):
    return {tag: int(field) for tag, field in re.findall(pattern, text)}


def pair(args, directory, manifest):
    retail_log = (directory / 'retail-stdout.log').read_text(errors='replace')
    port_log = (directory / 'port.log').read_text(errors='replace')
    retail_anchor = anchors(retail_log, r'DHD_COMPARE_RACE anchor=([cg]) field=(\d+)')
    port_anchor = anchors(port_log, r'compare: race anchor=([cg]) field=(\d+)')
    manifest.update(retail_anchors=retail_anchor, port_anchors=port_anchor)
    # Countdown length in fields is a free consistency check of the sync rule.
    if {'c', 'g'} <= retail_anchor.keys() and {'c', 'g'} <= port_anchor.keys():
        manifest['countdown_fields'] = dict(retail=retail_anchor['g'] - retail_anchor['c'],
                                            port=port_anchor['g'] - port_anchor['c'])
    retail_frames = {int(f): directory / 'retail' / f'field-{int(f):06d}.png'
                     for f in re.findall(r'DHD_COMPARE_CAPTURE completed=(\d+)', retail_log)}
    # Field and clock where each side first reached a moment.
    retail_moment = {m: (int(f), int(c)) for m, f, c in
                     re.findall(r'DHD_COMPARE_RACE moment=(\S+) field=(\d+) clock=(-?\d+)', retail_log)}
    port_moment = {m: (int(f), int(c)) for m, f, c in
                   re.findall(r'compare: race moment=(\S+) delta=0 field=(\d+) clock=(-?\d+)', port_log)}
    frames = []
    for i, (name, anchor, offset) in enumerate(MOMENTS):
        key = f'{anchor}{offset:+d}'
        renderer = renderer_check(args, directory, i, key)
        if renderer:
            print(f'{name}: renderer {"PASS" if renderer["pass"] else "FAIL"}, MAE {renderer["mae"]:.4f} '
                  f'against PCSX2 replaying the same stream', flush=True)
        port_file = directory / 'port' / f'race-{key}+0.bmp'
        if not port_file.exists() or key not in retail_moment:
            frames.append(dict(moment=name, anchor=anchor, clock=offset, renderer=renderer, missing=True))
            continue
        native = Image.open(port_file).convert('RGB')
        candidates = []
        for delta in range(2 * args.window + 1):
            # The adapter captures completed = crossing input index + 2 + delta.
            path = retail_frames.get(retail_moment[key][0] + 2 + delta)
            if path and path.exists():
                reference = Image.open(path).convert('RGB')
                if reference.size != native.size:
                    reference = reference.resize(native.size, Image.Resampling.NEAREST)
                candidates.append((float(np.abs(thumb(native) - thumb(reference)).mean()), delta, delta, path, reference))
        if not candidates:
            frames.append(dict(moment=name, anchor=anchor, clock=offset, renderer=renderer, missing=True))
            continue
        _, _, delta, path, reference = min(candidates, key=lambda c: c[:3])
        image = f'moment-{i}-{key}.png'
        args.mode = 'retail'
        score = compare.comparison(reference, native, args, directory / image)
        same = [c for c in candidates if c[2] == 0]
        score.update(moment=name, anchor=anchor, clock=offset, image=image, temporal_offset=delta, renderer=renderer,
                     retail_clock=retail_moment[key][1], port_clock=port_moment.get(key, (None, None))[1],
                     retail_field=retail_moment[key][0], port_field=port_moment.get(key, (None, None))[0],
                     same_offset_mae=compare.metrics(same[0][4], native, args.threshold)[0]['mae'] if same else None,
                     retail=str(path), port=str(port_file),
                     candidate_thumb_mae={c[2]: round(c[0], 3) for c in candidates})
        frames.append(score)
        print(f'{name}: game state (only meaningful before divergence): retail delta +{delta}, MAE {score["mae"]:.3f}, over {score["pixels_over_threshold_percent"]:.1f}%, '
              f'block SSIM {score["block_ssim"]:.3f}', flush=True)
    manifest['status'] = 'complete' if all(not f.get('missing') for f in frames) else 'incomplete'
    checked = [f['renderer'] for f in frames if f.get('renderer')]
    manifest['renderer'] = ('pass' if len(checked) == len(frames) and all(r['pass'] for r in checked)
                            else 'fail' if any(not r['pass'] for r in checked) else 'incomplete')
    manifest['frames'] = frames
    (directory / 'scores.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
    write_index(directory, manifest, frames)


RENDERER_MAE = 1.0  # pass bound: the same stream through PCSX2's GS and the port's


def renderer_check(args, directory, i, key):
    """The renderer alone: the port's render of the moment against PCSX2 replaying the GS stream
    the port drew it from (port-gs/gs-KEY.gs), so it holds whatever the game state does."""
    dump = directory / 'port-gs' / f'gs-{key}.gs'
    if not dump.exists():
        return None
    work = directory / f'renderer-pcsx2-{i}'
    shutil.rmtree(work, ignore_errors=True)
    replay = argparse.Namespace(pcsx2=args.replay_pcsx2.resolve(), replay_timeout=args.replay_timeout,
                                threshold=args.threshold, line_shift=args.line_shift, mode='gs-replay')
    image = f'renderer-{i}-{key}.png'
    try:
        score = compare.replay_score(replay, dump.resolve(), work.resolve(), directory / image)
    except (RuntimeError, OSError) as exc:
        return {'error': str(exc), 'mae': float('nan'), 'image': None, 'worst_regions': [], 'pass': False}
    score.update({'image': image, 'pass': score['mae'] <= RENDERER_MAE})
    return score


def write_index(directory, manifest, frames):
    """Two rows per moment: the renderer check (pass/fail), then game state against retail."""
    rows = []  # (title, image path) for the contact sheet
    for f in frames:
        r = f.get('renderer')
        if r and r.get('image'):
            rows.append((f'{f["moment"]} | renderer: port vs PCSX2 replay of same stream | '
                         f'{"PASS" if r["pass"] else "FAIL"} MAE {r["mae"]:.4f}', r['image']))
        if not f.get('missing'):
            rows.append((f'{f["moment"]} | game state (only meaningful before divergence): retail '
                         f'+{f["temporal_offset"]} | MAE {f["mae"]:.3f}', f['image']))
    if rows:
        thumbs = []
        for title, path in rows:
            im = Image.open(directory / path)
            im.thumbnail((1200, 320))
            thumbs.append((title, im.copy()))
        sheet = Image.new('RGB', (1200, sum(im.height + 24 for _, im in thumbs)), '#171c24')
        draw = ImageDraw.Draw(sheet)
        y = 0
        for title, im in thumbs:
            draw.text((8, y + 5), title, fill='white')
            sheet.paste(im, (0, y + 24))
            y += im.height + 24
        sheet.save(directory / 'contact-sheet.png')

    def cell(value, fmt):
        return '' if value is None else format(value, fmt)
    table = []
    for f in frames:
        r = f.get('renderer') or {}
        verdict = ('PASS' if r['pass'] else 'FAIL') if r else 'no dump'
        link = f'<a href="{r["image"]}">' if r.get('image') else ''
        table.append(
            f'<tr><td rowspan="2">{html.escape(f["moment"])}<br>{f["anchor"]}{f["clock"]:+d}</td>'
            f'<td>{link}renderer: port vs PCSX2 replay of same stream{"</a>" if link else ""}</td>'
            f'<td><b>{verdict}</b></td><td>{cell(r.get("mae"), ".4f")}</td>'
            f'<td>{cell(r.get("pixels_over_threshold_percent"), ".2f")}</td><td>{cell(r.get("block_ssim"), ".4f")}</td>'
            f'<td>{html.escape(str([w["box"] for w in r.get("worst_regions", [])[:3]]) + (" " + r["error"] if r.get("error") else ""))}</td></tr>')
        if f.get('missing'):
            table.append('<tr><td>game state: retail frame missing</td><td colspan="5"></td></tr>')
        else:
            table.append(
                f'<tr><td><a href="{f["image"]}">game state (only meaningful before divergence)</a>: retail +{f["temporal_offset"]}, '
                f'clock {f["retail_clock"]} / {f["port_clock"]}</td><td>info</td><td>{f["mae"]:.3f}</td>'
                f'<td>{f["pixels_over_threshold_percent"]:.2f}</td><td>{f["block_ssim"]:.4f}</td>'
                f'<td>{html.escape(str([w["box"] for w in f["worst_regions"][:3]]))}</td></tr>')
    document = ('<!doctype html><meta charset="utf-8"><title>Race comparison</title>'
                '<style>body{background:#171c24;color:#e5edf7;font:15px system-ui;margin:24px}'
                'a{color:#76c9ff}table{border-collapse:collapse}td,th{padding:6px;border:1px solid #405068}'
                'img{max-width:100%}</style>'
                f'<h1>{html.escape(directory.name)}</h1>'
                f'<p>Renderer: <b>{html.escape(manifest.get("renderer", "?"))}</b> (each moment: the port&rsquo;s render '
                f'against PCSX2 replaying the GS stream the port drew it from, pass at MAE &le; {RENDERER_MAE}; '
                f'independent of game state). Status: {html.escape(manifest["status"])}.</p>'
                f'<p>Game state rows compare with retail PCSX2 at the same race-clock moment. They only mean '
                f'something until the two races diverge (camera, rider positions, AI); use the EE dumps for the '
                f'simulation. Anchors (fields): retail {manifest.get("retail_anchors")}, port '
                f'{manifest.get("port_anchors")}; countdown length {manifest.get("countdown_fields")}.</p>'
                '<table><tr><th>Moment</th><th>Check</th><th>Result</th><th>MAE</th><th>Over threshold %</th>'
                '<th>Block SSIM</th><th>Worst 64×64 boxes</th></tr>' + ''.join(table) + '</table>'
                '<p><a href="scores.json">scores.json</a></p>' + ('<img src="contact-sheet.png">' if rows else ''))
    (directory / 'index.html').write_text(document, encoding='utf-8')

if __name__ == '__main__':
    main()
