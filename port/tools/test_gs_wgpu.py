"""The GS renderer's test programs (finding 084, the Rust renderer plan): build them for x86_64 over
the Rust renderer (port/gs/gpu_rust.c + dhd_render.lib), run them, judge every run.

    python port/tools/test_gs_wgpu.py [--depth-cases N] [--skip-hardware] [--only gpu|depth|present]

- test_gpu: every check on WARP and on the GPU (each with and without DHD_GS_EARLY_DEPTH); the raw
  VRAM after each section is logged (DHD_GS_VRAM_LOG, vram-<device>-wgpu.bin), and gs_golden.py
  compare test-gpu holds those logs to the goldens word for word.
- test_depth: Z from the depth plane, CPU doubles vs raster.wgsl's exact evaluation.
- test_present: the renderer's surface on a window that is never shown (phase W1), each presented
  texture read back and checked against the scanout picture.

The Direct3D 11 renderer and the C wgpu-native renderer these programs were once compared with are
gone (finding 088, CL2); their outputs live on in the goldens (DHD_DEVCACHES/dhd-gs-golden), which
were recorded with every Direct3D 11 and WebGPU result identical.

Every run is judged by port/tools/gs_verdict.py (the one pass rule; every run must print "gs:
renderer dhd_render ..."); any problem exits 1. Of the DHD_* variables in the caller's environment
only DHD_TEST_EXPECT_SURFACE and DHD_TEST_EE_MXCSR reach a test. dhd_render.lib comes from
port/tools/rustgs.py (release, cargo feature test-hooks for gpu_rust_test.c's dhdr_test_* calls),
with rustc's native-static-libs. Needs LLVM (clang-cl) and Visual Studio's libraries. Outputs stay in
build/port/gs-wgpu/x86_64."""
import argparse
import functools
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gs_verdict  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
GS = ROOT / 'port' / 'gs'
OUT = ROOT / 'build' / 'port' / 'gs-wgpu'
VSWHERE = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe')
LLVM = Path(os.path.expanduser('~')) / 'scoop' / 'apps' / 'llvm' / 'current' / 'bin'
ARCH = 'x86_64'                    # the Rust renderer is x64 (and the browser) only
# gpu_rust.c's adapter choice and video memory (CreateDXGIFactory1, IID_IDXGIFactory1, IID_IDXGIDevice)
LIBS = ['dxgi.lib', 'dxguid.lib']
RUST_FEATURES = ('test-hooks',)    # gpu_rust_test.c (DHD_GS_TEST_BUILD) calls dhdr_test_* (task HK)
RUST_INCLUDE = ROOT / 'port' / 'render' / 'include'
# What the GS test programs call themselves: user32 for test_present.c's and test_native_transfers.c's
# never-shown window (RegisterClassA, CreateWindowExA).
RUST_TEST_LIBS = ['user32.lib']
# The only DHD_* variables a harness forwards from its caller. Every other DHD_* is stripped, so a
# shell setting can never change what a referee compares.
FORWARDED = ('DHD_TEST_EXPECT_SURFACE', 'DHD_TEST_EE_MXCSR')
# The test programs' shared sources: GS memory and state, and the renderer (gpu_rust.c includes
# gpu_rust_test.c itself in the test build).
GS_SOURCES = ['memory.c', 'gs.c', 'gpu_rust.c']


@functools.lru_cache(maxsize=None)
def rust_build():
    """rustgs's report for the Rust renderer of a test build (built once per process; rustgs's own
    gates, version pins, path checks and cache apply)."""
    import rustgs  # cargo only through rustgs (A1a)
    try:
        return rustgs.build(rustgs.X64, list(RUST_FEATURES), 'release')
    except rustgs.Refused as why:
        sys.exit('rustgs: refused: %s' % why)


def gs_libraries():
    """The GS library a build links: exactly dhd_render.lib."""
    libs = [Path(rust_build()['lib'])]
    if [p.name for p in libs] != ['dhd_render.lib']:
        sys.exit('gs_libraries: expected exactly dhd_render.lib, got %s' % [p.name for p in libs])
    return libs


def gs_system_libs():
    """rustc's native-static-libs for dhd_render.lib (rustgs checked them against the S1 list), plus
    LIBS and RUST_TEST_LIBS."""
    libs = list(rust_build()['native_static_libs'])
    return libs + [x for x in LIBS + RUST_TEST_LIBS if x not in libs]


def harness_env(env=None):
    """The environment of one test-program run: the caller's variables minus its DHD_* ones (except
    FORWARDED), plus env."""
    full = {k: v for k, v in os.environ.items() if not k.startswith('DHD_') or k in FORWARDED}
    full.update(env or {})
    return full


def compile_command(sources, exe, out, flags=''):
    """The vcvars + clang-cl command file that builds exe from port/gs sources over dhd_render.lib."""
    vs = subprocess.check_output([str(VSWHERE), '-latest', '-property', 'installationPath'], text=True).strip()
    cmd = out / (exe.stem + '.cmd')
    cmd.write_text('@set "PATH=%%PATH%%;%s"\r\n@call "%s" x64 >nul\r\n@if errorlevel 1 exit /b 1\r\n'
                   '@set "PATH=%s;%%PATH%%"\r\n'
                   'clang-cl -m64 /nologo /O2 /MT -Wno-deprecated-declarations /clang:-ffp-contract=off %s'
                   '/DDHD_GS_TEST_BUILD=1 /I"%s" /I"%s" %s /Fe:"%s" /link %s %s\r\n'
                   % (VSWHERE.parent, Path(vs) / 'VC/Auxiliary/Build/vcvarsall.bat', LLVM, flags,
                      GS, RUST_INCLUDE, ' '.join('"%s"' % (GS / s) for s in sources), exe,
                      ' '.join('"%s"' % p for p in gs_libraries()), ' '.join(gs_system_libs())))
    return cmd


def build(name, sources):
    """name.exe from port/gs sources (GS_SOURCES among them, or the program's own copies) under OUT."""
    out = OUT / ARCH
    out.mkdir(parents=True, exist_ok=True)
    exe = out / (name + '.exe')
    cmd = compile_command(sources, exe, out)
    p = subprocess.run(['cmd', '/c', str(cmd)], cwd=out, capture_output=True, text=True)
    if p.returncode:
        sys.exit('%s did not build:\n%s%s' % (name, p.stdout, p.stderr))
    return exe


def run(exe, env, args=()):
    p = subprocess.run([str(exe), *args], cwd=exe.parent, env=harness_env(env), capture_output=True, text=True,
                       timeout=3600)
    return p.returncode, p.stdout + p.stderr


def test_gpu(devices, results, problems):
    exe = build('test_gpu', GS_SOURCES + ['test_gpu_stubs.c', 'test_gpu.c'])
    # Every VRAM log of an earlier run goes, so the directory only ever holds this run's devices
    # (gs_golden.py compare test-gpu reads it; a device not run now must be missing, not stale).
    for stale in exe.parent.glob('vram-*.bin'):
        stale.unlink()
    for device in devices:
        log = exe.parent / ('vram-%s-wgpu.bin' % device)
        env = {'DHD_GS_VRAM_LOG': str(log), 'DHD_GS_TEST_HARDWARE': '0' if device.startswith('warp') else '1'}
        if device.endswith('early0'):
            env['DHD_GS_EARLY_DEPTH'] = '0'
        code, text = run(exe, env)
        last = text.strip().splitlines()[-1] if text.strip() else '(no output)'
        fails = [line for line in text.splitlines() if line.startswith('FAIL')]
        results.append('test_gpu %s: %s%s' % (device, last, '' if code == 0 else ' (exit %d)' % code))
        results.extend('    ' + f for f in fails[:5])
        problems.extend(gs_verdict.judge(code, text, backend='wgpu', label='test_gpu %s' % device))
        if not log.is_file():
            problems.append('test_gpu %s: no VRAM log %s' % (device, log.name))


def test_depth(devices, cases, results, problems):
    exe = build('test_depth', GS_SOURCES + ['test_gpu_stubs.c', 'test_depth.c'])
    for device in devices:
        code, text = run(exe, {'DHD_GS_TEST_HARDWARE': '0' if device.startswith('warp') else '1'}, [str(cases)])
        results.append('test_depth %s (exit %d):' % (device, code))
        results.extend('    ' + line for line in text.strip().splitlines())
        problems.extend(gs_verdict.judge(code, text, expect=[r'^depth probe on (?:WARP|hardware) / .*: exact matches everywhere$'],
                                         label='test_depth %s' % device))


def test_present(devices, results, problems):
    exe = build('test_present', GS_SOURCES + ['test_gpu_stubs.c', 'test_present.c'])
    # Direct3D 12 (the renderer's API) on each device, and Vulkan on the GPU.
    for device in devices + ([] if 'hardware' not in devices else ['hardware-vulkan']):
        env = {'DHD_GS_TEST_HARDWARE': '0' if device.startswith('warp') else '1'}
        if device.endswith('vulkan'):
            env['DHD_WGPU_BACKEND'] = 'vulkan'
        code, text = run(exe, env)
        results.append('test_present %s (exit %d):' % (device, code))
        results.extend('    ' + line for line in text.strip().splitlines())
        problems.extend(gs_verdict.judge(code, text, backend='wgpu', label='test_present %s' % device))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--depth-cases', type=int, default=1 << 20)
    ap.add_argument('--skip-hardware', action='store_true')
    ap.add_argument('--only', choices=['gpu', 'depth', 'present'], action='append')
    args = ap.parse_args()
    # WARP's shader JIT loses results for some option sets (finding 084), so WARP runs with
    # DHD_GS_EARLY_DEPTH=0 too; the GPU runs both sets.
    devices = ['warp', 'warp-early0'] + ([] if args.skip_hardware else ['hardware', 'hardware-early0'])
    results, problems = [], []
    if not args.only or 'gpu' in args.only:
        test_gpu(devices, results, problems)
    if not args.only or 'depth' in args.only:
        test_depth([d for d in devices if not d.endswith('early0')], args.depth_cases // 4, results, problems)
    if not args.only or 'present' in args.only:
        test_present([d for d in devices if not d.endswith('early0')], results, problems)
    print('\n'.join(results))
    sys.exit(gs_verdict.verdict(problems))


if __name__ == '__main__':
    main()
