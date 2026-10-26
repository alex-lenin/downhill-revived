"""Negative controls for the GS pass rule (port/tools/gs_verdict.py; the Rust renderer plan, 5.2).

Changes to this file may only add conditions: no check is ever relaxed.

    python port/tools/harness_negative.py [--keep]

Each negative must make gs_verdict.py exit 1, run as a separate process exactly as a harness would:
  1. crash      a fake test program that prints a passing run, then exits 0xC0000409
  2. vram       a real VRAM log against a copy with one word flipped
  3. count      a real VRAM log against a copy missing its last checkpoint
                (and the harness line "checkpoints differ in number")
  4. pixels     "3 pixels differ" (test_present's line) in an otherwise passing run
  5. start      the real test_gpu whose renderer does not start (Vulkan asked for WARP's fallback
                adapter, which it has not): it must exit nonzero and be red; plus a fallback line
                ("gs: WebGPU backend unavailable; using D3D11") in an otherwise passing run
  6. renderer   the real run without its "gs: renderer dhd_render" lines
  7. backend    the real run reporting d3d11 instead of wgpu
Then the same rule must be green on today's output (x86_64, WARP): test_gpu (DHD_GS_EARLY_DEPTH=0)
with its VRAM log, test_present (its real "0 pixels differ" lines), test_depth (its real "pair
differs" lines; 8192 cases) and test_native_transfers on the GPU (wgpu, and wgpu without dual source).
Exit 0 only when every negative is red and every baseline run is green.

This script builds its own copies of the test programs (port/gs sources over the Rust renderer,
dhd_render.lib from port/tools/rustgs.py with the test-hooks feature) under
build/port/harness-negative, so edits elsewhere cannot change what it checks. The Direct3D 11
and C wgpu-native renderers it once also ran are gone (finding 088, CL2). Needs LLVM (clang-cl) and
Visual Studio's libraries, like test_gs_wgpu.py."""
import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gs_verdict  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
GS = ROOT / 'port' / 'gs'
OUT = ROOT / 'build' / 'port' / 'harness-negative'
VERDICT = Path(__file__).resolve().parent / 'gs_verdict.py'
VSWHERE = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe')
LLVM = Path(os.path.expanduser('~')) / 'scoop' / 'apps' / 'llvm' / 'current' / 'bin'
LIBS = ['dxgi.lib', 'dxguid.lib', 'user32.lib']
RUST_INCLUDE = ROOT / 'port' / 'render' / 'include'
GS_SOURCES = ['memory.c', 'gs.c', 'gpu_rust.c']
STATUS_STACK_BUFFER_OVERRUN = 0xC0000409
RENDERER_LINE = 'gs: renderer dhd_render (harness_negative)\n'  # what every real run prints first

results = []  # (ok, line)


def record(ok, line):
    results.append((ok, line))
    print(('ok    ' if ok else 'WRONG ') + line, flush=True)


_RUST = []


def rust_build():
    """rustgs's report for dhd_render.lib with the test hooks (built once)."""
    if not _RUST:
        import rustgs  # cargo only through rustgs (A1a)
        try:
            _RUST.append(rustgs.build(rustgs.X64, ['test-hooks'], 'release'))
        except rustgs.Refused as why:
            sys.exit('harness_negative: rustgs refused: %s' % why)
    return _RUST[0]


def build(name, sources, out=None):
    """The x86_64 test build test_gs_wgpu.py makes (gpu_rust.c over dhd_render.lib, DHD_GS_TEST_BUILD)."""
    out = out or OUT / 'x86_64'
    out.mkdir(parents=True, exist_ok=True)
    report = rust_build()
    system = list(report['native_static_libs']) + [x for x in LIBS if x not in report['native_static_libs']]
    vs = subprocess.check_output([str(VSWHERE), '-latest', '-property', 'installationPath'], text=True).strip()
    exe = out / (name + '.exe')
    cmd = out / (name + '.cmd')
    cmd.write_text('@set "PATH=%%PATH%%;%s"\r\n@call "%s" x64 >nul\r\n@if errorlevel 1 exit /b 1\r\n'
                   '@set "PATH=%s;%%PATH%%"\r\n'
                   'clang-cl -m64 /nologo /O2 /MT -Wno-deprecated-declarations /clang:-ffp-contract=off '
                   '/DDHD_GS_TEST_BUILD=1 /I"%s" /I"%s" %s /Fe:"%s" /link "%s" %s\r\n'
                   % (VSWHERE.parent, Path(vs) / 'VC/Auxiliary/Build/vcvarsall.bat', LLVM,
                      GS, RUST_INCLUDE, ' '.join('"%s"' % (GS / s) for s in sources), exe,
                      report['lib'], ' '.join(system)))
    p = subprocess.run(['cmd', '/c', str(cmd)], cwd=out, capture_output=True, text=True)
    if p.returncode:
        sys.exit('harness_negative: %s did not build:\n%s%s' % (name, p.stdout, p.stderr))
    return exe


def build_fake_crash():
    """A test program that prints a passing wgpu run, then dies as a failed stack cookie does."""
    out = OUT / 'fake'
    out.mkdir(parents=True, exist_ok=True)
    src = out / 'fake_crash.c'
    src.write_text('#include <stdio.h>\n#include <windows.h>\n'
                   'int main(void) {\n'
                   '    fprintf(stderr, "gs: renderer dhd_render (fake)\\n");\n'
                   '    fprintf(stderr, "gs: backend wgpu\\n");\n'
                   '    printf("wgpu WARP raster tests: 0 failures\\n");\n'
                   '    fflush(stdout); fflush(stderr);\n'
                   '    ExitProcess(0xC0000409u);\n'
                   '}\n', encoding='utf-8')
    exe = out / 'fake_crash.exe'
    clang = shutil.which('clang') or str(LLVM / 'clang.exe')
    subprocess.run([clang, '--target=x86_64-pc-windows-msvc', '-O1', '-w', str(src), '-o', str(exe)], check=True)
    return exe


def run(exe, env, args=()):
    """test_gs_wgpu.py's run(): no inherited DHD_* variable, plus env."""
    full = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    full.update(env)
    full = {k: v for k, v in full.items() if v is not None}
    p = subprocess.run([str(exe), *args], cwd=exe.parent, env=full, capture_output=True, text=True, timeout=3600)
    return p.returncode, p.stdout + p.stderr


def verdict_cli(args, text=None):
    """gs_verdict.py as a process: (exit code, its output)."""
    p = subprocess.run([sys.executable, str(VERDICT), *args], input=text, capture_output=True, text=True)
    return p.returncode, (p.stdout + p.stderr).strip()


def judge_cli(code, text, backend=None, expect=()):
    args = ['--code', str(code), '-']
    if backend:
        args += ['--backend', backend]
    for e in expect:
        args += ['--expect', e]
    return verdict_cli(args, text)


def expect_red(name, code_out):
    code, out = code_out
    first = next((l for l in out.splitlines() if l.startswith('VERDICT')), out[:200])
    record(code == 1, 'negative %-9s gs_verdict exit %d (want 1): %s' % (name, code, first))


def expect_green(name, code_out):
    code, out = code_out
    record(code == 0, 'baseline %-24s gs_verdict exit %d (want 0)%s' % (name, code, '' if code == 0 else '\n' + out))


def write_vram_copies(log):
    """A copy of the VRAM log with one word flipped, and one without its last checkpoint."""
    data = bytearray(log.read_bytes())
    step = gs_verdict.CHECKPOINT_NAME + gs_verdict.VRAM_BYTES
    flipped = OUT / 'vram-flipped.bin'
    i = step + gs_verdict.CHECKPOINT_NAME + 4 * 123457  # a word in the second checkpoint
    data[i] ^= 0x01
    flipped.write_bytes(bytes(data))
    data[i] ^= 0x01
    short = OUT / 'vram-short.bin'
    short.write_bytes(bytes(data[:-step]))
    same = OUT / 'vram-same.bin'
    same.write_bytes(bytes(data))
    return flipped, short, same


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--keep', action='store_true', help='keep the run outputs under build/port/harness-negative')
    args = ap.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)

    # ---- the baseline (real runs), which the negatives below reuse ----
    test_gpu = build('test_gpu', GS_SOURCES + ['test_gpu_stubs.c', 'test_gpu.c'])
    log = OUT / 'vram-warp-early0-wgpu.bin'
    if log.exists():
        log.unlink()
    code, text = run(test_gpu, {'DHD_GS_VRAM_LOG': str(log), 'DHD_GS_TEST_HARDWARE': '0', 'DHD_GS_EARLY_DEPTH': '0'})
    (OUT / 'test_gpu-wgpu.txt').write_text('exit %d\n%s' % (code, text), encoding='utf-8')
    gpu_run = (code, text)
    expect_green('test_gpu warp-early0 wgpu',
                 judge_cli(code, text, backend='wgpu', expect=[r'^wgpu WARP raster tests: 0 failures$']))
    record(log.is_file(), 'test_gpu wrote its VRAM log %s' % log.name)
    flipped, short, same = write_vram_copies(log)
    expect_green('test_gpu VRAM log vs its copy', verdict_cli(['--vram', str(log), str(same)]))

    test_present = build('test_present', GS_SOURCES + ['test_gpu_stubs.c', 'test_present.c'])
    code, text = run(test_present, {'DHD_GS_TEST_HARDWARE': '0'})
    (OUT / 'test_present.txt').write_text('exit %d\n%s' % (code, text), encoding='utf-8')
    expect_green('test_present warp', judge_cli(code, text, backend='wgpu',
                                                expect=[r'^scanout 320x224 on a 640x448 window: 0 pixels differ$',
                                                        r'^wgpu WARP present tests: 0 failures$']))
    present_text = text

    test_depth = build('test_depth', GS_SOURCES + ['test_gpu_stubs.c', 'test_depth.c'])
    code, text = run(test_depth, {'DHD_GS_TEST_HARDWARE': '0'}, ['8192'])
    (OUT / 'test_depth.txt').write_text('exit %d\n%s' % (code, text), encoding='utf-8')
    expect_green('test_depth warp 8192', judge_cli(code, text, expect=[r'^depth probe on WARP / .*: exact matches everywhere$']))

    transfers = build('test_native_transfers', ['test_native_transfers.c', 'gpu_rust.c', 'gs.c', 'memory.c'])
    for name, env in (('wgpu', {}), ('wgpu-no-dual', {'DHD_WGPU_DUAL_SOURCE': '0'})):
        code, text = run(transfers, env)
        (OUT / ('test_native_transfers-%s.txt' % name)).write_text('exit %d\n%s' % (code, text), encoding='utf-8')
        expect_green('native_transfers ' + name, judge_cli(code, text, backend='wgpu',
                                                           expect=[r'^clear then full-page CPU upload: PASS']))

    # ---- the negatives ----
    crash = build_fake_crash()
    code, text = run(crash, {})
    record(code & 0xFFFFFFFF == STATUS_STACK_BUFFER_OVERRUN, 'fake crash exited 0x%08X' % (code & 0xFFFFFFFF))
    expect_red('crash', judge_cli(code, text, backend='wgpu', expect=[r'^wgpu WARP raster tests: 0 failures$']))
    # the same output with exit 0 is green: the exit code alone makes it red
    expect_green('fake crash output, exit 0', judge_cli(0, text, backend='wgpu'))

    expect_red('vram', verdict_cli(['--vram', str(log), str(flipped)]))
    expect_red('count', verdict_cli(['--vram', str(log), str(short)]))
    expect_red('count-txt', judge_cli(0, RENDERER_LINE + 'test_gpu warp VRAM: checkpoints differ in number\n'))

    pixels = present_text.replace('on a 640x448 window: 0 pixels differ', 'on a 640x448 window: 3 pixels differ', 1)
    record(pixels != present_text, 'planted "3 pixels differ" into the real test_present output')
    expect_red('pixels', judge_cli(0, pixels, backend='wgpu'))
    expect_red('pixels-of', judge_cli(0, RENDERER_LINE + 'picture x: 3 of 1000 pixels differ\n'))
    # a summary's "full_vram_words=N" is no failure count, "failures=N" is one
    expect_green('failures=0', judge_cli(0, RENDERER_LINE + 'hardware integration checks=9 full_vram_words=4096 failures=0\n'))
    expect_red('failures=N', judge_cli(0, RENDERER_LINE + 'hardware integration checks=9 full_vram_words=4096 failures=2\n'))

    # A renderer start that fails: Vulkan has no fallback (WARP) adapter.
    failing = {'DHD_GS_TEST_HARDWARE': '0', 'DHD_GS_EARLY_DEPTH': '0',
               'DHD_WGPU_BACKEND': 'vulkan', 'DHD_WGPU_ADAPTER': 'fallback'}
    code, text = run(test_gpu, failing)
    (OUT / 'start-fails.txt').write_text('exit %d\n%s' % (code, text), encoding='utf-8')
    record(code != 0, 'a renderer that does not start exited %d (want nonzero)' % code)
    expect_red('start', judge_cli(code, text, backend='wgpu'))
    expect_red('fallback-txt', judge_cli(0, RENDERER_LINE + 'gs: WebGPU backend unavailable; using D3D11\n'
                                            'wgpu WARP raster tests: 0 failures\n', backend='wgpu'))

    code, text = gpu_run
    without = '\n'.join(line for line in text.splitlines() if not line.startswith('gs: renderer dhd_render'))
    record(without != text, 'removed the "gs: renderer dhd_render" lines from the real test_gpu output')
    expect_red('renderer', judge_cli(code, without, backend='wgpu'))
    other = text.replace('wgpu WARP raster tests:', 'd3d11 WARP raster tests:')
    record(other != text, 'made the real test_gpu output report d3d11')
    expect_red('backend', judge_cli(code, other, backend='wgpu'))

    if not args.keep:
        for f in (flipped, short, same):
            f.unlink(missing_ok=True)
    bad = [line for ok, line in results if not ok]
    print('harness_negative: %d checks, %d wrong' % (len(results), len(bad)))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
