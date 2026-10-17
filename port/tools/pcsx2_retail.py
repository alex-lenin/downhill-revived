"""Independent retail boot/capture backend for compare_pcsx2.py.

BIOS and ISO are referenced in place. All configs and captures are under build/.
Uses the opt-in adapter built by prepare_pcsx2_compare.py, not GS packet replay.
"""
from pathlib import Path
import configparser
import math
import os
import re
import shutil
import subprocess
import time

import numpy as np
from PIL import Image

RETAIL_LIMIT = ('Independent retail PCSX2 boot versus native port. Both receive identically quantized frame-indexed pad input; '
                'the port uses a deterministic HLE scheduler clock, not EE instruction cycle accounting. '
                'Startup movies are bypassed only in the port, and boot/load timing can differ. '
                'Same boot-vsync numbers alone do not prove matching simulation state. '
                'Any nearest-image temporal search and scanline correction are recorded explicitly.')


def bios_file(requested):
    if requested:
        file = Path(requested).expanduser().resolve()
        if not file.is_file():
            raise RuntimeError('BIOS file does not exist')
        return file
    directory = Path.home() / 'Documents/PCSX2/bios'
    files = list(directory.glob('*.ROM0'))
    if len(files) != 1:
        raise RuntimeError('set DHD_PCSX2_BIOS or --bios to your existing BIOS ROM0 file')
    return files[0].resolve()


def config(directory, bios, renderer):
    data = directory / 'pcsx2-data'
    settings = data / 'PCSX2/inis'
    settings.mkdir(parents=True)
    values = {
        'UI': dict(SettingsVersion='1', SetupWizardIncomplete='false', ConfirmShutdown='false',
                   StartPaused='false', PauseOnFocusLoss='false', RenderToSeparateWindow='true'),
        'Folders': dict(Bios=str(bios.parent)),
        'Filenames': dict(BIOS=bios.name),
        'EmuCore': dict(EnableFastBoot='true', EnableRecordingTools='false', SaveStateOnShutdown='false',
                        EnableCheats='false', EnableWideScreenPatches='false', EnableNoInterlacingPatches='false',
                        ManuallySetRealTimeClock='true', RtcYear='0', RtcMonth='1', RtcDay='1'),
        'EmuCore/GS': dict(Renderer=str(renderer), upscale_multiplier='1', InterlaceMode='4',
                          SkipDuplicateFrames='false', pcrtc_antiblur='false', pcrtc_offsets='false',
                          VsyncEnable='false', VsyncQueueSize='1', OsdShowSpeed='false', OsdShowFPS='false'),
        'EmuCore/Speedhacks': dict(EECycleRate='0', EECycleSkip='0', vuThread='false'),
        'SPU2/Output': dict(OutputMuted='true'),
        'MemoryCards': dict(Slot1_Enable='false', Slot2_Enable='false'),
        'Logging': dict(EnableFileLogging='true', EnableTimestamps='true'),
        'InputSources': dict(Keyboard='false', Mouse='false', SDL='false', XInput='false', DInput='false'),
        'Pad1': dict(Type='DualShock2'), 'Pad2': dict(Type='DualShock2'),
    }
    ini = configparser.ConfigParser(interpolation=None)
    ini.optionxform = str
    for section, settings_values in values.items():
        ini[section] = settings_values
    with (settings / 'PCSX2.ini').open('w', encoding='utf-8') as file:
        ini.write(file)
    return data


def run_reference(args, directory, targets, manifest, compare):
    iso = os.environ.get('DHD_ISO')
    if not iso or not Path(iso).is_file():
        raise RuntimeError('DHD_ISO must reference your existing ISO')
    bios = bios_file(args.bios)
    emulator = args.retail_pcsx2.resolve()
    if not emulator.is_file() or b'DHD_COMPARE_ADAPTER=2' not in emulator.read_bytes():
        raise RuntimeError('build the retail adapter with python port/tools/prepare_pcsx2_compare.py')
    # A source rebuild must not replace or lock the executable of an active run.
    # Only public emulator runtime files are copied; BIOS/ISO never enter this tree.
    runtime = directory / 'pcsx2-runtime'
    runtime.mkdir()
    source_directory = emulator.parent
    isolated_emulator = runtime / emulator.name
    shutil.copyfile(emulator, isolated_emulator)
    for dll in source_directory.glob('*.dll'):
        shutil.copyfile(dll, runtime / dll.name)
    for name in ('resources', 'QtPlugins', 'D3D12', 'translations'):
        if (source_directory / name).exists():
            shutil.copytree(source_directory / name, runtime / name)
    if (source_directory / 'qt.conf').exists():
        shutil.copyfile(source_directory / 'qt.conf', runtime / 'qt.conf')
    emulator = isolated_emulator
    captures = directory / 'retail'
    captures.mkdir()
    data = config(directory, bios, args.retail_renderer)
    fields = sorted({field + args.reference_offset + offset for field in targets
                     for offset in range(-args.match_window, args.match_window+1)
                     if field + args.reference_offset + offset > 0})
    if not fields:
        raise RuntimeError('no positive reference field targets')
    env = {key: value for key, value in os.environ.items() if not key.startswith(('DHD_', 'ROUND_'))}
    env.update(DHD_COMPARE_MOVIE=str(directory / 'input.p2m2'), DHD_COMPARE_CAPTURE=str(captures),
               DHD_COMPARE_FIELDS=','.join(map(str, fields)), DHD_COMPARE_UNTIL=str(max(fields)+1))
    if args.retail_renderer != 13:
        env['DHD_COMPARE_CAPTURE_GPU'] = str(captures)
    command = [str(emulator), '-datapath', str(data), '-batch', '-nogui', '-fastboot', '-unlimited',
               '-logfile', str(directory / 'pcsx2.log'), '--', str(Path(iso).resolve())]
    manifest.update(retail_pcsx2_sha256=compare.sha256(emulator), reference_fields_requested=fields,
                    bios_referenced_in_place=True, bios_filename=bios.name, retail_renderer=args.retail_renderer,
                    reference_offset=args.reference_offset, match_window=args.match_window,
                    memory_cards_disabled=True, startup_movies='retail plays; native explicit bypass')
    print('Booting retail PCSX2 with frame-indexed movie input', flush=True)
    log_path = directory / 'retail-stdout.log'
    with log_path.open('wb') as file:
        process = subprocess.Popen(command, cwd=emulator.parent, env=env, stdout=file, stderr=subprocess.STDOUT)
        deadline = time.monotonic() + args.timeout
        try:
            while process.poll() is None:
                emulog = directory / 'pcsx2.log'
                if emulog.exists() and 'ReportErrorAsync: Startup Error' in emulog.read_text(errors='replace'):
                    raise RuntimeError('retail PCSX2 startup failed; see pcsx2.log')
                if time.monotonic() >= deadline:
                    raise RuntimeError('retail PCSX2 deadline exceeded; see pcsx2.log and retail-stdout.log')
                time.sleep(0.2)
            if process.returncode:
                raise RuntimeError(f'retail PCSX2 exited {process.returncode}; see retail-stdout.log')
        finally:
            compare.stop(process)
    log = log_path.read_text(errors='replace')
    if 'DHD_COMPARE_ADAPTER=2' not in log:
        raise RuntimeError('retail adapter did not acknowledge movie playback')
    metadata = {int(frame): dict(input_index=int(index), field_parity=int(parity),
                                width=int(width), height=int(height))
                for frame, index, parity, width, height in re.findall(
                    r'DHD_COMPARE_CAPTURE completed=(\d+) input_index=(\d+) parity=(\d+) size=(\d+)x(\d+)', log)}
    missing = set(fields)-set(metadata)
    if missing:
        raise RuntimeError(f'retail capture missing fields {sorted(missing)[:8]}')
    manifest['retail_capture_metadata'] = metadata
    return {field: captures / f'field-{field:06d}.png' for field in fields}


def run_native(args, directory, script, events, manifest, compare):
    iso = os.environ.get('DHD_ISO')
    if not iso or not Path(iso).is_file():
        raise RuntimeError('DHD_ISO must reference your existing ISO')
    executable, digest = compare.snapshot_exe(args.exe, directory)
    binary = executable.read_bytes()
    for marker in (b'DHD_FRAME_CLOCK', b'DHD_AUTOPAD_FRAMES', b'DHD_CAPTURE_FRAME_INTERVAL'):
        if marker not in binary:
            raise RuntimeError('port image lacks virtual-clock comparison hooks; rebuild or use build_compare_port.py')
    env = {key: value for key, value in os.environ.items() if not key.startswith(('DHD_', 'ROUND_'))}
    env.update(DHD_ISO=iso, DHD_FRAME_CLOCK='1', DHD_AUTOPAD_FRAMES=compare.frame_script(events),
               DHD_LOG='1', DHD_SKIP_MOVIES='1', DHD_MEMCARD='0', DHD_SND_OFF='1', DHD_SCREENSHOT=str(directory / 'native'),
               DHD_PROFILE=str(directory / 'profile'),
               DHD_CAPTURE_FRAME=str(compare.field_at(args.at)),
               DHD_CAPTURE_FRAME_INTERVAL=str(compare.field_at(args.interval)))
    count = math.floor((args.until - args.at)/args.interval + 1e-9)+1
    manifest.update(port_exe_sha256=digest, virtual_clock=True, pad_script=script,
                    pad_frames=compare.frame_script(events), native_requested_captures=count)
    log_path = directory / 'port.log'
    with log_path.open('wb') as file:
        process = subprocess.Popen([str(executable)], cwd=compare.ROOT / 'build/port/link', env=env,
                                   stdout=file, stderr=subprocess.STDOUT)
        deadline = time.monotonic()+args.timeout
        try:
            while process.poll() is None:
                log = log_path.read_text(errors='replace')
                if len(re.findall(r'compare: capture .*? field=\d+', log)) >= count:
                    break
                if time.monotonic() >= deadline:
                    raise RuntimeError('native virtual-clock capture deadline exceeded')
                time.sleep(0.2)
        finally:
            compare.stop(process)
    metadata = re.findall(r'compare: capture (.*?) field=(\d+) host=([\d.]+) game=([\d.]+)',
                          log_path.read_text(errors='replace'))
    if len(metadata) < count:
        raise RuntimeError(f'native virtual-clock run produced {len(metadata)}/{count} captures')
    return [(Path(path), int(field), float(wall), float(game)) for path, field, wall, game in metadata[:count]]


def choose_reference(native, paths, target):
    """Bounded nearest-frame matching on 64x56 RGB thumbnails; never expands the window."""
    small = np.asarray(native.resize((64, 56), Image.Resampling.BOX), dtype=np.float32)
    candidates = []
    for field, path in paths.items():
        reference = Image.open(path).convert('RGB')
        thumb = np.asarray(reference.resize((64, 56), Image.Resampling.BOX), dtype=np.float32)
        candidates.append((float(np.abs(small-thumb).mean()), abs(field-target), field, path))
    _, _, field, path = min(candidates)
    return field, path


def run(args, directory, script, events, manifest, frames, compare):
    manifest.update(mode='retail', limits=RETAIL_LIMIT)
    captures = run_native(args, directory, script, events, manifest, compare)
    targets = [field for _, field, _, _ in captures]
    # Ensure the movie contains every requested field, including an explicit reference offset.
    compare.write_movie(directory / 'input.p2m2', events,
                        max(targets)+args.reference_offset+args.match_window+5)
    references = run_reference(args, directory, targets, manifest, compare)
    for i, (native_path, target, wall, game) in enumerate(captures):
        native = Image.open(native_path).convert('RGB')
        requested = target+args.reference_offset
        candidates = {field: path for field, path in references.items() if abs(field-requested) <= args.match_window}
        selected, path = choose_reference(native, candidates, requested)
        reference = Image.open(path).convert('RGB')
        original_size = reference.size
        # Compare the displayed image at a common native grid when game modes
        # differ. Record this explicitly; it does not assert matching viewports.
        if reference.size != native.size:
            reference = reference.resize(native.size, Image.Resampling.NEAREST)
        name = f'moment-{i:03d}-native-{target:06d}-retail-{selected:06d}.png'
        score = compare.comparison(reference, native, args, directory / name)
        score.update(image=name, dump='retail', dump_field=selected, vsync=target, host_seconds=wall,
                     game_seconds=game, reference_field=selected, reference_target=requested,
                     temporal_offset=selected-requested, reference_original_size=original_size,
                     native_size=native.size, resolution_normalized=original_size != native.size,
                     reference=str(path), native=str(native_path))
        same = Image.open(references[requested]).convert('RGB')
        if same.size != native.size:
            same = same.resize(native.size, Image.Resampling.NEAREST)
        score['same_field_mae'] = compare.metrics(same, native, args.threshold)[0]['mae']
        frames.append(score)
        print(f'native {target}, retail {selected}, temporal offset {selected-requested:+d}: '
              f'MAE {score["mae"]:.4f}, over {score["pixels_over_threshold_percent"]:.2f}%, '
              f'block SSIM {score["block_ssim"]:.5f}', flush=True)
