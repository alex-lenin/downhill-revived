"""Automatic independent retail PCSX2/native comparisons, or GS replay (Windows).

Retail mode requires the comparison adapter and references an existing BIOS in place.
GS replay mode needs no BIOS. See finding 024 for synchronization limits.
Requires Python, Pillow and NumPy, and the local PCSX2 2.8.2 installation.
"""
from __future__ import annotations

import argparse
import configparser
from decimal import Decimal, ROUND_CEILING
import hashlib
import html
import json
import math
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import time

import numpy as np
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/compare'
RATE = Decimal(60000) / Decimal(1001)
BUTTONS = {name: 1 << i for i, name in enumerate(
    'select l3 r3 start up right down left l2 r2 l1 r1 triangle circle cross square'.split())}
PRESSURES = 'right left up down triangle circle cross square l1 r1 l2 r2'.split()
STANDARD_PAD = ('45:3:triangle,65:3:cross,125:2:cross,140:2:cross,155:2:cross,'
                '170:2:cross,185:2:cross,200:2:cross,215:2:cross,235:2:start,'
                '255:2:cross,275:2:cross,295:2:cross,315:2:cross,335:2:cross,355:2:cross')
LIMIT = ('PCSX2 software replay of native GS packets; not an independent retail boot. '
         'Exact dump-local field pairing, with inherited native VRAM at the initial freeze. '
         'Camera, simulation, EE/VU output, loading and skipped movies are not validated. '
         'Delivered port vsyncs are not necessarily game updates; the host kernel can drop overdue fields.')


def field_at(seconds):
    return int((Decimal(str(seconds)) * RATE).to_integral_value(rounding=ROUND_CEILING))


def parse_pad(script):
    events = []
    for event in script.split(','):
        try:
            start, duration, chord = event.split(':')
            start, duration = Decimal(start), Decimal(duration)
            if not start.is_finite() or not duration.is_finite() or start < 0 or duration <= 0:
                raise ValueError()
            mask = 0
            for name in chord.split('+'):
                mask |= BUTTONS[name]
            begin, end = field_at(start), field_at(start + duration)
            if end <= begin:
                raise ValueError('interval contains no NTSC field')
            events.append((begin, end, mask, chord))
        except (ValueError, KeyError, ArithmeticError) as exc:
            raise ValueError(f'invalid pad interval: {event!r}') from exc
    return events


def frame_script(events):
    return ','.join(f'{a}:{b-a}:{chord}' for a, b, _, chord in events)


def write_movie(path, events, fields, players=1):
    """Independent serializer for PCSX2's documented v1 POWER_ON p2m2 format."""
    if players not in (1, 2):
        raise ValueError('p2m2 comparison supports one or two recorded pads')
    header = struct.pack('<B50s255s255sIIB', 1, b'PCSX2-2.8.2', b'DHD comparison',
                         b'Downhill Domination', fields, 0, 0)
    neutral = bytes([255, 255, 128, 128, 128, 128] + [0] * 12)
    with path.open('wb') as f:
        f.write(header)
        for n in range(fields):
            mask = 0
            for begin, end, bits, _ in events:
                if begin <= n < end:
                    mask |= bits
            pad = struct.pack('<H', mask ^ 65535) + bytes([128] * 4)
            pad += bytes(255 if mask & BUTTONS[name] else 0 for name in PRESSURES)
            f.write(pad + (pad if players == 2 else neutral))


def sha256(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def snapshot_exe(source, directory):
    target = directory / 'dhd.exe'
    for _ in range(3):
        before = sha256(source)
        shutil.copyfile(source, target)
        if before == sha256(target) == sha256(source):
            if source.with_suffix('.map').exists():
                shutil.copyfile(source.with_suffix('.map'), target.with_suffix('.map'))
            return target, before
    raise RuntimeError('port executable changed repeatedly during snapshot; retry after linking')


def stop(process):
    if process.poll() is None:
        if os.name == 'nt':
            # The reservation launcher can spawn a same-image child. Verify
            # every executable PID before stopping it; never tree-kill an
            # unchecked descendant.
            from owned_process import stop_verified
            if not isinstance(process.args, (list, tuple)) or not process.args:
                raise RuntimeError('cannot verify the comparison executable')
            stop_verified(process, process.args[0])
            return
        process.terminate()
        try:
            process.wait(timeout=10)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(timeout=10)


def run_port(args, directory, script, events, manifest):
    iso = os.environ.get('DHD_ISO')
    if not iso or not Path(iso).is_file():
        raise RuntimeError('DHD_ISO must reference your existing ISO (it is never copied)')
    exe, digest = snapshot_exe(args.exe, directory)
    binary = exe.read_bytes()
    hooks = b'DHD_AUTOPAD_FRAMES' in binary and b'DHD_GS_CAPTURE_FRAME_INTERVAL' in binary
    manifest['port_frame_schedule_hooks'] = hooks
    manifest['port_field_metadata_hook'] = b'compare: dump' in binary
    if args.clock == 'fields' and not hooks:
        raise RuntimeError('copied port executable lacks frame scheduling; relink the new hooks '
                           'or explicitly select --clock host for a legacy executable')
    dumps = directory / 'dumps'
    dumps.mkdir()
    # Do not inherit diagnostic runs, screen schedules, or other workers' scripts.
    env = {k: v for k, v in os.environ.items() if not k.startswith(('DHD_', 'ROUND_'))}
    env.update(DHD_ISO=iso, DHD_LOG='1', DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_SND_OFF='1',
               DHD_GS_CAPTURE=str(dumps), DHD_GS_CAPTURE_FIELDS=str(args.fields),
               DHD_PROFILE=str(directory / 'profile'))
    if args.clock == 'fields':
        env.update(DHD_AUTOPAD_FRAMES=frame_script(events),
                   DHD_GS_CAPTURE_FRAME=str(field_at(args.at)),
                   DHD_GS_CAPTURE_FRAME_INTERVAL=str(field_at(args.interval)))
    else:
        env.update(DHD_AUTOPAD=script, DHD_GS_CAPTURE_AT=str(args.at),
                   DHD_GS_CAPTURE_INTERVAL=str(args.interval))
    expected = math.floor((args.until - args.at) / args.interval + 1e-9) + 1
    manifest.update(port_exe_sha256=digest, requested_captures=expected,
                    clock=args.clock, pad_script=script, pad_frames=frame_script(events),
                    movie_bypass=True)
    deadline = time.monotonic() + args.timeout
    log_path = directory / 'port.log'
    print(f'Running isolated port; {expected} captures; timeout {args.timeout:g}s', flush=True)
    with log_path.open('wb') as log:
        process = subprocess.Popen([str(exe)], cwd=ROOT / 'build/port/link', env=env,
                                   stdout=log, stderr=subprocess.STDOUT)
        try:
            while process.poll() is None:
                text = log_path.read_text(errors='replace')
                if len(re.findall(r'gs: captured .* and \d+ matching native fields', text)) >= expected:
                    break
                if time.monotonic() >= deadline:
                    raise RuntimeError('port capture deadline exceeded; see port.log')
                time.sleep(0.25)
            manifest['port_exit_before_stop'] = process.poll()
        finally:
            stop(process)
    paths = sorted(dumps.glob('*.gs'))[:expected]
    if len(paths) != expected:
        raise RuntimeError(f'port exited after {len(paths)}/{expected} captures; see port.log')
    return paths


def read_dump(path):
    """Read only our legacy version-8 format; record display registers at each field."""
    with path.open('rb') as f:
        def take(n):
            data = f.read(n)
            if len(data) != n:
                raise ValueError(f'truncated GS dump: {path}')
            return data
        _, size = struct.unpack('<II', take(8))
        if size != 4194752:
            raise ValueError('only native version-8 dumps are supported')
        freeze = take(size)
        if struct.unpack_from('<I', freeze)[0] != 8:
            raise ValueError('wrong GS freeze version')
        regs = take(8192)
        states = []
        while tag := f.read(1):
            if tag == b'\0':
                take(1)
                take(struct.unpack('<I', take(4))[0])
            elif tag == b'\1':
                field = take(1)[0]
                get = lambda offset: struct.unpack_from('<Q', regs, offset)[0]
                circuit = 0 if get(0) & 1 else 1
                fb, display = get(0x70 + circuit * 0x20), get(0x80 + circuit * 0x20)
                w = ((display >> 32 & 4095) + 1) // ((display >> 23 & 15) + 1)
                h = ((display >> 44 & 2047) + 1) // ((display >> 27 & 3) + 1)
                states.append(dict(field=field, circuit=circuit, width=w, height=h,
                                   half_height=get(0x20) & 3 == 3,
                                   dbx=fb >> 32 & 2047, dby=fb >> 43 & 2047,
                                   block=(fb & 511) * 32, psm=fb >> 15 & 31))
            elif tag == b'\2':
                # FIFO reads have a qword count but no embedded payload.
                take(4)
            elif tag == b'\3':
                regs = take(8192)
            else:
                raise ValueError(f'unknown GS record {tag!r}')
    return states


def replay_pcsx2(args, dump, directory, count):
    directory.mkdir()
    data = directory / 'data'
    # 2.8.2 appends "PCSX2" to CustomDataPath on Windows.
    settings_dir = data / 'PCSX2/inis'
    settings_dir.mkdir(parents=True)
    config = configparser.ConfigParser(interpolation=None, strict=False)
    config.optionxform = str
    config.read(args.pcsx2.parent / 'inis/PCSX2.ini', encoding='utf-8-sig')
    settings = {
        'UI': dict(SettingsVersion='1', SetupWizardIncomplete='false', ConfirmShutdown='false',
                   StartPaused='false', PauseOnFocusLoss='false'),
        'EmuCore': dict(EnableRecordingTools='false', SaveStateOnShutdown='false'),
        'EmuCore/GS': dict(Renderer='13', upscale_multiplier='1', InterlaceMode='4',
                          SkipDuplicateFrames='false', dump='true', DumpGSData='true',
                          SaveFrame='true', SaveFrameStart='0', SaveFrameCount=str(count),
                          SaveFrameBy='1', SaveDrawBy='1', SaveDrawCount='1000000',
                          SaveRT='false', SaveTexture='false', SaveDepth='false', SaveInfo='false',
                          SWDumpDirectory=str(directory), HWDumpDirectory=str(directory)),
        'Logging': dict(EnableFileLogging='true'),
    }
    for section, values in settings.items():
        if not config.has_section(section):
            config.add_section(section)
        for key, value in values.items():
            config.set(section, key, value)
    with (settings_dir / 'PCSX2.ini').open('w', encoding='utf-8') as f:
        config.write(f)
    command = [str(args.pcsx2), '-datapath', str(data), '-batch', '-nogui',
               '-logfile', str(directory / 'emulog.txt'), '--', str(dump)]
    with (directory / 'stdout.log').open('wb') as log:
        process = subprocess.Popen(command, cwd=args.pcsx2.parent, stdout=log, stderr=subprocess.STDOUT)
        deadline = time.monotonic() + args.replay_timeout
        try:
            while process.poll() is None:
                files = list(directory.glob(f'*_f{count-1:05d}_fr*.*'))
                if files:
                    # PCSX2 texture dump saves synchronously. Give the last write time to finish.
                    time.sleep(0.3)
                    break
                if time.monotonic() >= deadline:
                    raise RuntimeError(f'PCSX2 replay timeout: {directory}')
                time.sleep(0.1)
        finally:
            stop(process)
    files = {}
    for path in directory.iterdir():
        match = re.search(r'_f(\d+)_fr([01])_', path.name)
        if match and path.suffix.lower() in ('.png', '.bmp'):
            key = int(match[1]), int(match[2])
            # A second replay loop can rewrite the same field. Keep the first sequence.
            files.setdefault(key, path)
    return files


def replay_score(args, dump, workdir, image):
    """Replay a native GS dump in PCSX2 (args.pcsx2, args.replay_timeout) and score the port's own
    render of its last field (DUMP-native-NNN.bmp) against PCSX2's: the renderer alone, whatever
    the game state. Writes the reference | native | difference image to `image`."""
    states = read_dump(dump)
    if len(states) < 2:
        raise RuntimeError(f'dump has no independently replayed field: {dump}')
    local = len(states) - 1
    native_path = dump.with_name(f'{dump.stem}-native-{local:03d}.bmp')
    native = Image.open(native_path).convert('RGB')
    ref_files = replay_pcsx2(args, dump, workdir, len(states))
    state = states[local]
    ref_path = ref_files.get((local, state['circuit']))
    if ref_path is None:
        raise RuntimeError(f'PCSX2 did not emit requested framebuffer for field {local}')
    if f'_{state["block"]:05x}_' not in ref_path.name:
        raise RuntimeError('PCSX2 framebuffer base does not match dump display registers')
    score = comparison(scanout(ref_path, state), native, args, image)
    score.update(dump=dump.name, dump_sha256=sha256(dump), dump_field=local, display=state,
                 reference=str(ref_path), native=str(native_path))
    return score


def scanout(path, state):
    raw = Image.open(path).convert('RGB')
    w, h = state['width'], state['height']
    source_h = (h + 1) // 2 if state['half_height'] else h
    # SW GetOutput removes the aligned block offset from the saved texture.
    block_w = 16 if state['psm'] in (2, 10) else 8
    x, y = state['dbx'] % block_w, state['dby'] % 8
    if w <= 0 or h <= 0 or x + w > raw.width or y + source_h > raw.height:
        raise ValueError(f'display crop exceeds PCSX2 framebuffer: {path}, {state}, {raw.size}')
    crop = raw.crop((x, y, x + w, y + source_h))
    if state['half_height']:
        crop = crop.resize((w, h), Image.Resampling.NEAREST)
    return crop


def metrics(reference, native, threshold, shift=0):
    a, b = np.asarray(reference, dtype=np.float32), np.asarray(native, dtype=np.float32)
    if a.shape != b.shape:
        raise ValueError(f'image size mismatch: {reference.size} vs {native.size}')
    if shift > 0:
        a, b = a[shift:], b[:-shift]
    elif shift < 0:
        a, b = a[:shift], b[-shift:]
    diff = np.abs(a - b)
    mae = float(diff.mean())
    over = float((diff.max(axis=2) > threshold).mean() * 100)
    # Small non-overlapping luminance windows: cheap, explicit block SSIM.
    gray_a, gray_b = a @ [0.299, 0.587, 0.114], b @ [0.299, 0.587, 0.114]
    h, w = (gray_a.shape[0] // 8) * 8, (gray_a.shape[1] // 8) * 8
    if h and w:
        windows = lambda x: x[:h, :w].reshape(h//8, 8, w//8, 8).transpose(0, 2, 1, 3).reshape(-1, 64)
        x, y = windows(gray_a), windows(gray_b)
        mx, my = x.mean(1), y.mean(1)
        vx, vy = x.var(1), y.var(1)
        cov = ((x - mx[:, None]) * (y - my[:, None])).mean(1)
        ssim = float((((2*mx*my + 6.5025)*(2*cov + 58.5225)) /
                      ((mx*mx + my*my + 6.5025)*(vx + vy + 58.5225))).mean())
    else:
        ssim = None
    regions = []
    for top in range(0, diff.shape[0], 64):
        for left in range(0, diff.shape[1], 64):
            patch = diff[top:top+64, left:left+64]
            regions.append(dict(box=[left, top + max(shift, 0), left + patch.shape[1],
                                     top + max(shift, 0) + patch.shape[0]], mae=float(patch.mean())))
    result = dict(mae=mae, pixels_over_threshold_percent=over, block_ssim=ssim,
                  vertical_shift=shift, scored_pixels=int(diff.shape[0]*diff.shape[1]),
                  worst_regions=sorted(regions, key=lambda x: x['mae'], reverse=True)[:5])
    return result, diff


def comparison(reference, native, args, target):
    raw, _ = metrics(reference, native, args.threshold)
    candidates = [(metrics(reference, native, args.threshold, shift), shift)
                  for shift in range(-args.line_shift, args.line_shift + 1)]
    (best, diff), _ = min(candidates, key=lambda x: (x[0][0]['mae'], abs(x[1])))
    # Shift correction is bounded; raw scores remain visible and never disappear.
    best['raw_mae'] = raw['mae']
    best['raw_pixels_over_threshold_percent'] = raw['pixels_over_threshold_percent']
    intensity = diff.max(axis=2) / 255
    heat = np.stack([intensity*255, np.minimum(intensity*2, 1)*160,
                     np.zeros_like(intensity)], axis=2).astype(np.uint8)
    heatmap = Image.new('RGB', reference.size)
    heatmap.paste(Image.fromarray(heat), (0, max(best['vertical_shift'], 0)))
    w, h = reference.size
    panel = Image.new('RGB', (w*3, h+32), '#171c24')
    draw = ImageDraw.Draw(panel)
    reference_label = 'PCSX2 retail' if getattr(args, 'mode', '') == 'retail' else 'PCSX2 software'
    for i, (label, im) in enumerate([(reference_label, reference), ('Native port', native),
                                   (f'Difference | MAE {best["mae"]:.3f}', heatmap)]):
        panel.paste(im, (i*w, 32))
        draw.text((i*w+8, 8), label, fill='white')
    for region in best['worst_regions'][:3]:
        box = region['box']
        draw.rectangle((2*w+box[0], 32+box[1], 2*w+box[2]-1, 32+box[3]-1), outline='cyan')
    panel.save(target)
    return best


def report(directory, manifest, frames):
    manifest['frames'] = frames
    (directory / 'scores.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
    thumbs = []
    for frame in frames:
        im = Image.open(directory / frame['image'])
        im.thumbnail((960, 250))
        thumbs.append(im.copy())
    if thumbs:
        sheet = Image.new('RGB', (960, sum(im.height+24 for im in thumbs)), '#171c24')
        draw = ImageDraw.Draw(sheet)
        y = 0
        for frame, im in zip(frames, thumbs):
            draw.text((8, y+5), f'{frame["dump"]}, field {frame["dump_field"]} | MAE {frame["mae"]:.3f}', fill='white')
            sheet.paste(im, (0, y+24))
            y += im.height+24
        sheet.save(directory / 'contact-sheet.png')
    rows = []
    for frame in frames:
        rows.append('<tr><td><a href="{image}">{dump}:{dump_field}</a></td><td>{vsync}</td>'
                    '<td>{host_seconds}</td><td>{mae:.4f}</td><td>{raw_mae:.4f}</td>'
                    '<td>{pixels_over_threshold_percent:.2f}%</td><td>{block_ssim}</td>'
                    '<td>{vertical_shift}</td><td>{temporal}</td><td>{same}</td><td>{grid}</td><td>{regions}</td></tr>'.format(
                        **frame, temporal=frame.get('temporal_offset', '—'), same=frame.get('same_field_mae', '—'),
                        grid=html.escape(str(frame.get('reference_original_size', '—'))) + ' → ' + html.escape(str(frame.get('native_size', '—'))),
                        regions=html.escape(str(frame['worst_regions'][:3]))))
    document = ('<!doctype html><meta charset="utf-8"><title>PCSX2 comparison</title>'
                '<style>body{background:#171c24;color:#e5edf7;font:15px system-ui;margin:24px}'
                'a{color:#76c9ff}table{border-collapse:collapse}td,th{padding:8px;border:1px solid #405068}'
                'img{max-width:100%}th{text-align:left}</style>'
                f'<h1>{html.escape(directory.name)}</h1><p>Status: {html.escape(manifest.get("status", "pending"))}</p>'
                f'<p>{html.escape(manifest.get("error", ""))}</p><p>{html.escape(manifest.get("limits", LIMIT))}</p>'
                f'<p>Reference search radius: ±{manifest.get("match_window", 0)} fields; explicit boot offset: '
                f'{manifest.get("reference_offset", 0)} fields.</p>'
                f'<p>RGB MAE in 0–255 units; a pixel exceeds threshold when any channel differs by '
                f'more than {manifest["threshold"]}. SSIM is mean 8×8 luminance block SSIM. '
                'A bounded vertical correction discards edge rows; raw MAE is retained. '
                'Cyan boxes mark the three worst 64×64 regions.</p>'
                '<table><tr><th>Dump:field</th><th>Port vsync</th><th>Host seconds</th>'
                '<th>MAE</th><th>Raw MAE</th><th>Pixels over threshold</th><th>Block SSIM</th>'
                '<th>Y shift</th><th>Temporal shift</th><th>Same-field MAE</th><th>Reference → native grid</th>'
                '<th>Worst regions</th></tr>' + ''.join(rows) + '</table>'
                '<p><a href="scores.json">Machine-readable scores and provenance</a></p>'
                + ('<img src="contact-sheet.png" alt="PCSX2, port, and difference contact sheet">' if frames else ''))
    (directory / 'index.html').write_text(document, encoding='utf-8')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('label')
    parser.add_argument('--mode', choices=('retail', 'gs-replay'), default='retail')
    parser.add_argument('--bios', type=Path, default=os.environ.get('DHD_PCSX2_BIOS'))
    parser.add_argument('--retail-pcsx2', type=Path, default=ROOT / 'build/port/compare-pcsx2-src/bin/pcsx2-dhd-compare.exe')
    parser.add_argument('--retail-renderer', type=int, default=13)
    parser.add_argument('--match-window', type=int, default=2, help='bounded reference-frame search radius')
    parser.add_argument('--reference-offset', type=int, default=0, help='explicit boot-frame offset, never inferred silently')
    parser.add_argument('--pad', default=STANDARD_PAD, help='interval string or a UTF-8 text file')
    parser.add_argument('--at', type=float, default=250)
    parser.add_argument('--interval', type=float, default=3)
    parser.add_argument('--until', type=float, default=280)
    parser.add_argument('--clock', choices=('fields', 'host'), default='fields')
    parser.add_argument('--fields', type=int, default=120, help='fields per GS dump; compare the last')
    parser.add_argument('--timeout', type=float, default=3600)
    parser.add_argument('--replay-timeout', type=float, default=90)
    parser.add_argument('--threshold', type=int, default=16)
    parser.add_argument('--line-shift', type=int, choices=(0, 1), default=1)
    parser.add_argument('--exe', type=Path, default=ROOT / 'build/port/link/dhd.exe')
    parser.add_argument('--pcsx2', type=Path, default=ROOT / 'build/port/pcsx2/pcsx2-qt.exe')
    parser.add_argument('--from-dumps', type=Path, help='reuse existing native dumps and sibling native BMPs')
    args = parser.parse_args()
    if args.from_dumps:
        args.mode = 'gs-replay'
    if not 0 <= args.match_window <= 600:
        parser.error('match window must be 0..600 fields')
    if args.mode == 'retail' and args.clock != 'fields':
        parser.error('retail mode requires --clock fields and the virtual clock')
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]*', args.label) or args.label in ('.', '..'):
        parser.error('label must be a single safe directory name')
    if not all(math.isfinite(x) for x in (args.at, args.interval, args.until, args.timeout, args.replay_timeout)):
        parser.error('times must be finite')
    if args.at < 0 or args.interval <= 0 or args.until < args.at or args.timeout <= 0 or args.replay_timeout <= 0:
        parser.error('invalid schedule or timeout')
    if not 2 <= args.fields <= 600 or not 0 <= args.threshold <= 255:
        parser.error('fields must be 2..600 and threshold 0..255')
    if args.mode == 'gs-replay' and args.fields >= field_at(args.interval) and not args.from_dumps:
        parser.error('capture length must be shorter than the interval')
    args.exe, args.pcsx2 = args.exe.resolve(), args.pcsx2.resolve()
    if args.mode == 'gs-replay' and not args.pcsx2.is_file():
        parser.error(f'PCSX2 not found: {args.pcsx2}')
    script = args.pad
    if ':' not in script and Path(script).is_file():
        script = Path(script).read_text(encoding='utf-8').strip()
    events = parse_pad(script)
    directory = OUT / args.label
    directory.mkdir(parents=True, exist_ok=False)
    manifest = dict(schema_version=1, mode=args.mode, limits=LIMIT,
                    threshold=args.threshold,
                    line_shift_bound=args.line_shift, schedule=vars(args).copy())
    manifest['schedule'] = {k: str(v) if isinstance(v, Path) else v for k, v in manifest['schedule'].items()}
    if args.mode == 'gs-replay':
        manifest['pcsx2_sha256'] = sha256(args.pcsx2)
    write_movie(directory / 'input.p2m2', events, field_at(args.until + 10))
    frames = []
    print(LIMIT if args.mode == 'gs-replay' else 'Independent retail PCSX2 comparison with virtual native clock', flush=True)
    try:
        if args.mode == 'retail':
            import pcsx2_retail
            import sys
            pcsx2_retail.run(args, directory, script, events, manifest, frames, sys.modules[__name__])
            manifest['status'] = 'complete'
            return
        if args.from_dumps:
            dumps = sorted(args.from_dumps.resolve().glob('*.gs'))
            if not dumps:
                raise RuntimeError('no native GS dumps found')
            manifest['reused_dumps'] = True
        else:
            dumps = run_port(args, directory, script, events, manifest)
        log_path = args.from_dumps.parent / 'port.log' if args.from_dumps else directory / 'port.log'
        timing = {}
        if log_path.exists():
            for prefix, local, vsync, host in re.findall(
                    r'compare: dump (.*?) field=(\d+) vsync=(\d+) host=([\d.]+)',
                    log_path.read_text(errors='replace')):
                timing[Path(prefix).name, int(local)] = int(vsync), float(host)
        for i, dump in enumerate(dumps):
            name = f'moment-{i:03d}-field-{len(read_dump(dump)) - 1:03d}.png'
            score = replay_score(args, dump, directory / f'pcsx2-{i:03d}', directory / name)
            vsync, host = timing.get((dump.stem, score['dump_field']), (None, None))
            score.update(image=name, vsync=vsync, host_seconds=host)
            local = score['dump_field']
            frames.append(score)
            print(f'{dump.name}:{local} MAE={score["mae"]:.4f} raw={score["raw_mae"]:.4f} '
                  f'over={score["pixels_over_threshold_percent"]:.2f}% SSIM={score["block_ssim"]:.5f}', flush=True)
        manifest['status'] = 'complete'
    except BaseException as exc:
        manifest.update(status='failed', error=str(exc))
        raise
    finally:
        report(directory, manifest, frames)
        print(directory / 'index.html', flush=True)


if __name__ == '__main__':
    main()
