"""Build the Android arm64 host: the wasm2c module, the POSIX host and the Rust renderer as
libmain.so, SDL's Android entry (finding 091).

    python port/tools/build_host64_android.py build/wasm/dhd_core.wasm [--jobs 2]

The same portable core the Windows x64 build compiles (port/tools/build_host64.py), prepared the
same way (prepare_wasm2c.py: the mirror-address adapter, exact f32 literals), with the host units of
the Linux build (build_host64_linux.py) and, on top of them, what drawing needs there: the Rust
renderer (gpu_rust.c over libdhd_render.a, rustgs.py --target aarch64-linux-android, Vulkan), the
sharp font, the SDL window, sound and controllers (libSDL3.so, sdl3.py --android) and the Android
start (port/host/android_main.c: SDL_main; launcher.c's main is dhd_launcher_main here).

Cross-compiled on Windows with the NDK's clang (sdl3.android_ndk(): ANDROID_NDK_HOME, else the
newest NDK under ANDROID_HOME) for aarch64-linux-android API sdl3.ANDROID_API (30, Android 11),
-march=armv8.2-a (the Snapdragon 888's cores and every arm64 phone since 2018). WABT's wasm2c is the
Windows one (its C output is the same on every host). Objects go through the shared object cache
(objcache.py). Bounds are checked by guard pages (WASM_RT_MEMCHECK_GUARD_PAGES), as on Windows
(finding 091, AND2).

Output (build/android, never committed): lib/arm64-v8a/libmain.so (stripped) and libSDL3.so, the
two libraries the APK carries (port/android/build_apk.py), and libmain.debug.so, the same library
with its symbols (llvm-addr2line -e build/android/libmain.debug.so 0xOFFSET for a crash report's
libmain.so+0xOFFSET frames).
"""
import argparse
import hashlib
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gen_host64_imports import generate  # noqa: E402
import objcache  # noqa: E402
from prepare_wasm2c import check_launcher_exports, module_units, prepare_units  # noqa: E402
import sdl3  # noqa: E402
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches

ROOT = Path(__file__).resolve().parents[2]
WABT = (DEVCACHES / 'wabt/wabt-1.0.42')
SIMDE = (DEVCACHES / 'simde-src')
SOURCE = ROOT / 'port/host64'
TARGET = 'aarch64-linux-android%d' % sdl3.ANDROID_API
# Tuning only (the instruction set stays -march=armv8.2-a): the Snapdragon 888's big cores are a
# Cortex-X1 and three A78s, the same pipeline family.
TUNE = 'cortex-a78'

# The host units: the Linux build's (build_host64_linux.HOST_UNITS, SDL sound), with the Rust renderer
# and the sharp font instead of the null renderer, and the Android start.
HOST_UNITS = [
    'port/host/host_posix.c', 'port/host/sound_out_sdl.c', 'port/host/sdl_host.c', 'port/host/android_main.c',
    'port/host/android_perf.c', 'port/runtime/sha256.c',
    'port/hle/host.c', 'port/hle/graphics_queue.c', 'port/hle/vif.c', 'port/runtime/vu0_interp.c',
    'port/gs/memory.c', 'port/gs/gs.c', 'port/gs/display.c', 'port/gs/present.c', 'port/gs/gpu_rust.c',
    'port/gs/texpack_linux.c', 'port/gs/font_sharp.c', 'port/gs/capture.c',
    'port/hle/gamepad.c', 'port/hle/keyboard.c', 'port/hle/text_input.c', 'port/hle/movie.c',
    'port/hle/splash.c', 'port/hle/update.c', 'port/hle/fps.c',
    'port/net/client.c', 'port/net/hash.c', 'port/net/json.c', 'port/net/net.c',
    'port/net/ws_posix.c',
] + ['port/third_party/libmpeg2/%s.c' % n for n in (
    'alloc', 'cpu_accel', 'cpu_state', 'decode', 'header', 'idct', 'motion_comp', 'slice')]
# The units that include SDL3's headers (host64_census.SDL_UNITS, and the Android start).
SDL_UNITS = ('port/hle/gamepad.c', 'port/hle/text_input.c', 'port/gs/present.c', 'port/host/sdl_host.c',
             'port/host/sound_out_sdl.c', 'port/host/android_main.c')
GLUE = ('guest_memory.c', 'guest_memory_posix.c', 'fiber_posix.c', 'fp_mode.c', 'bridge.c', 'diagnostics.c',
        'launcher.c')
# Only SDL's entry leaves the library: everything else may be garbage-collected (--gc-sections), as
# /opt:ref does for dhd64.exe.
EXPORTS = '{\n  global: SDL_main;\n  local: *;\n};\n'


def main():
    p = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    p.add_argument('module', type=Path, help='build/wasm/dhd_core.wasm (build_wasm.py)')
    p.add_argument('--out', type=Path, default=ROOT / 'build/android')
    p.add_argument('--jobs', type=int, default=2)
    p.add_argument('--num-outputs', type=int, default=8, help='WABT split C output count')
    p.add_argument('--wabt', type=Path, default=WABT)
    p.add_argument('--simde', type=Path, default=SIMDE)
    pgo = p.add_mutually_exclusive_group()
    pgo.add_argument('--pgo-generate', action='store_true',
                     help='instrumented build: a run writes files/dhd-<pid>.profraw at its exit (host_exit)')
    pgo.add_argument('--pgo-use', type=Path, metavar='PROFDATA',
                     help='optimize with this profile (llvm-profdata merge of the .profraw files)')
    a = p.parse_args()
    out = a.out.resolve()
    if out.drive.upper() == 'C:':
        p.error('build output must stay off C:')
    work = out / 'work'
    for d in (out, work, work / 'obj', work / 'tmp'):
        d.mkdir(parents=True, exist_ok=True)
    ndk = sdl3.android_ndk()
    bin_dir = ndk / 'toolchains/llvm/prebuilt/windows-x86_64/bin'
    clang = bin_dir / 'clang.exe'
    if not clang.exists():
        p.error('no NDK clang at %s' % clang)
    env = dict(os.environ, TMP=str(work / 'tmp'), TEMP=str(work / 'tmp'), TMPDIR=str(work / 'tmp'))
    commands = []

    def run(cmd, timeout=600):
        cmd = list(map(str, cmd))
        commands.append(cmd)
        r = subprocess.run(cmd, cwd=work, env=env, capture_output=True, text=True,
                           encoding='utf-8', errors='replace', timeout=timeout)
        if r.returncode:
            raise RuntimeError(' '.join(cmd) + '\n' + r.stdout + r.stderr)
        return r.stdout + r.stderr

    try:
        sdl_lib, sdl_include, _java = sdl3.android_build()
        import rustgs  # cargo through rustgs only (its flags, version gate, checks and cache)
        try:
            report = rustgs.build(rustgs.ANDROID)
        except rustgs.Refused as why:
            raise RuntimeError('rustgs: refused: %s' % why)
        rt = a.wabt / 'share/wabt/wasm2c'
        units = module_units(work, run, a.wabt / 'bin/wasm2c.exe', a.module.resolve(), (), a.num_outputs)
        rows, exports = generate(work / 'core.h', work, linux=True)
        check_launcher_exports(exports)
        prepare_units(units, work, exact_f32=True)
        # The module runs on one OS thread (its fibers), so WABT's call-depth counter can be a plain
        # global instead of thread-local: a TLS lookup (TLSDESC in a shared library) on every wasm
        # call and return otherwise. A copy of wasm-rt.h lets the build choose the storage class, as
        # build_host64.py does for dhd64.exe; the depth check itself (limit 512) is unchanged.
        header = (a.wabt / 'include/wasm-rt.h').read_text()
        block = '#ifdef WASM_RT_C11_AVAILABLE\n#define WASM_RT_THREAD_LOCAL _Thread_local\n'
        if header.count(block) != 1:
            raise ValueError('wasm-rt.h: WASM_RT_THREAD_LOCAL block not found (WABT version changed?)')
        start = header.index(block); end = header.index('#endif\n', start) + len('#endif\n')
        (work / 'wasm-rt.h').write_text(header[:start] + '#ifndef WASM_RT_THREAD_LOCAL\n' + header[start:end] +
                                        '#endif\n' + header[end:])
        # Profile-guided optimization (AND2): only code layout and inlining change (FP stays strict).
        # The profile is copied under its hash, so the object cache keys follow its content.
        pgo_flags = []
        if a.pgo_generate:
            pgo_flags = ['-fprofile-generate']
        elif a.pgo_use:
            data = a.pgo_use.read_bytes()
            profile = work / ('pgo-%s.profdata' % hashlib.sha256(data).hexdigest()[:16])
            profile.write_bytes(data)
            pgo_flags = ['-fprofile-use=' + str(profile)]
        common = [clang, '--target=' + TARGET, '-c', '-O3', '-march=armv8.2-a', '-mtune=' + TUNE, '-fPIC',
                  '-fomit-frame-pointer', '-ffp-model=strict', '-ffp-contract=off',
                  '-DWASM_RT_THREAD_LOCAL=',
                  # Bounds by guard pages, as dhd64.exe (finding 082): guest_memory.c reserves 8 GiB
                  # (every wasm32 address plus offset) and commits only what is mapped, so an
                  # out-of-bounds access faults and guest_memory_posix.c's reporter names the guest
                  # address. WABT's fault-to-trap handler stays off (ours handles the faults).
                  '-DWASM_RT_MEMCHECK_GUARD_PAGES=1', '-DWASM_RT_SKIP_SIGNAL_RECOVERY=1',
                  '-ftrivial-auto-var-init=zero', '-fno-strict-aliasing',
                  '-ffunction-sections', '-fdata-sections', *pgo_flags,
                  '-fwrapv', '-pthread', '-w',
                  '-DWASM_RT_STACK_DEPTH_COUNT=1', '-DWASM_RT_MAX_CALL_STACK_DEPTH=512',
                  '-DWASM_RT_TRAP_HANDLER=host64_trap',
                  '-I' + str(work), '-I' + str(a.wabt / 'include'), '-I' + str(rt), '-I' + str(SOURCE),
                  '-I' + str(ROOT / 'port/runtime'),
                  '-I' + str(ROOT / 'port/include'), '-I' + str(ROOT / 'port/host'),
                  '-I' + str(a.simde)]
        # no builder folders in the library (__FILE__ of assert): the checkout, this output and WABT
        for old, new in ((ROOT, 'src'), (work, 'gen'), (a.wabt, 'wabt'), (ndk, 'ndk')):
            for spelling in dict.fromkeys((str(old), str(old).replace('\\', '/'))):
                common += ['-ffile-prefix-map=%s=%s' % (spelling, new)]
        import vu1_aot  # VU1's blocks as C (DHD_VUTEXT or assets/vutext.bin), for vu0_interp.c
        gen = work / 'gen'
        gen.mkdir(exist_ok=True)
        vu1_aot.generate(gen / 'vu1_aot.inc')
        objdir = work / 'obj'

        def compile_one(source):
            source = Path(source)
            rel = source.relative_to(ROOT).as_posix() if source.is_relative_to(ROOT) else source.name
            extra = ['-I' + str(gen)] if rel.startswith('port/') else []
            if rel in SDL_UNITS:
                extra += ['-DDHD_HAVE_SDL', '-I' + str(Path(sdl_include).resolve())]
            if rel.endswith('gs/gpu_rust.c'):
                extra += ['-I' + str(ROOT / 'port/render/include')]  # dhd_render.h
            if rel.endswith('vu0_interp.c'):
                extra += ['-DPS2_VU_AOT']
            if rel.endswith('host64/launcher.c'):
                extra += ['-Dmain=dhd_launcher_main']  # SDL_main (android_main.c) calls it
            if 'libmpeg2/' in rel:
                extra += ['-I' + str(ROOT / 'port/third_party/libmpeg2')]
            obj = objdir / (rel.replace('/', '_') + '.o')
            cmd = list(map(str, common + extra + [source, '-o', obj]))
            objcache.compile(cmd, str(obj), lambda c: run(c, timeout=1800) and None,
                             roots=[(work, 'out')], cwd=work, env=env, hit='')
            return obj

        with ThreadPoolExecutor(a.jobs) as pool:
            host_objects = list(pool.map(compile_one, [ROOT / u for u in HOST_UNITS]))
        library = work / 'libhost64.a'
        library.unlink(missing_ok=True)
        run([bin_dir / 'llvm-ar.exe', 'rcs', library, *host_objects])
        glue = [rt / 'wasm-rt-impl.c', work / 'imports_gen.c'] + [SOURCE / n for n in GLUE]
        with ThreadPoolExecutor(a.jobs) as pool:
            objects = list(pool.map(compile_one, units + glue))
        exports_map = work / 'exports.map'
        exports_map.write_text(EXPORTS)
        lib_dir = out / 'lib' / 'arm64-v8a'
        lib_dir.mkdir(parents=True, exist_ok=True)
        debug = out / 'libmain.debug.so'
        # rustc's list for the static library (checked by rustgs against
        # port/render/native-static-libs-aarch64-linux-android.txt)
        rust_libs = [x for x in report['native_static_libs'] if x not in ('-lc',)]
        rsp = work / 'link.rsp'
        rsp.write_text('\n'.join('"%s"' % str(x).replace('\\', '/') for x in
                                 objects + [library, report['lib'], sdl_lib]))
        run([clang, '--target=' + TARGET, '-shared', *pgo_flags[:1], '-o', debug, '@' + str(rsp),
             '-Wl,-soname,libmain.so', '-Wl,--version-script=' + str(exports_map),
             '-Wl,--gc-sections', '-Wl,--no-undefined', '-Wl,--build-id=sha1',
             '-Wl,-z,max-page-size=16384', '-Wl,-z,noexecstack', '-Wl,-z,relro', '-Wl,-z,now',
             '-landroid', *rust_libs, '-lm'], timeout=1800)
        run([bin_dir / 'llvm-strip.exe', '--strip-unneeded', '-o', lib_dir / 'libmain.so', debug])
        run([bin_dir / 'llvm-strip.exe', '--strip-unneeded', '-o', lib_dir / 'libSDL3.so', sdl_lib])
        (work / 'failure.log').unlink(missing_ok=True)
        size = (lib_dir / 'libmain.so').stat().st_size
        print('PASS: %d generated imports; %d Android host objects; %s (%.1f MB) and libSDL3.so; NDK %s'
              % (len(rows), len(host_objects), lib_dir / 'libmain.so', size / 1048576, ndk.name), flush=True)
    except (RuntimeError, ValueError) as error:
        (work / 'failure.log').write_text(str(error))
        raise SystemExit(str(error)[-6000:])
    finally:
        (work / 'commands.json').write_text(json.dumps(commands, indent=2) + '\n')


if __name__ == '__main__':
    main()
