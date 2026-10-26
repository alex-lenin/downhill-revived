"""The one pass rule for every GS renderer harness (the Rust renderer plan, section 5.2).

Changes to this file may only add conditions: no check is ever relaxed.

Every harness (test_gs_wgpu.py, replay_gs.py, test_native_transfers.py, gs_golden.py, ...) runs
its test programs and hands each run to judge(); the run passes only when judge() returns no
problems. Problems, one string each, for a run that:

  - exits nonzero (a crash such as 0xC0000409 included; a renderer that does not start included);
  - prints DIFFER (the word, any case-sensitive occurrence);
  - prints a line starting FAIL, or a "NAME: FAIL" result (test_native_transfers);
  - prints "checkpoints differ in number" or "checkpoint names differ";
  - prints "N pixels differ", "N of M pixels differ", "N incorrect pixels", "N words differ",
    "N failures", "failures=N" or "VRAM differs" with N > 0 (parsed, so test_present's "0 pixels differ" is
    fine), or a test_depth summary whose d3d11_vs_cpu or exact_vs_cpu count is > 0 (test_depth's
    "pair differs" lines and pair_vs_cpu are reports, not failures);
  - prints a fallback line ("using D3D11", "is not in this build", "native renderer unavailable",
    "DHD_GS_NO_FALLBACK");
  - was asked for a backend (backend='wgpu', the only one left) and does not report it, or reports
    another one. Reports: "gs: backend NAME", "backend: NAME" (test_native_transfers), "NAME
    WARP|hardware raster|present tests:" (test_gpu, test_present), "Replayed N fields (NAME)"
    (replay). Every report must name the requested backend;
  - has no "gs: renderer dhd_render ..." line (every test program links the Rust renderer; the
    Direct3D 11 and C wgpu-native renderers are gone, finding 088 CL2);
  - lacks a line matching one of the expect patterns (regular expressions, re.search per line).

judge() takes the output of ONE child process. A harness's own comparisons (VRAM logs, PPM files,
goldens) report through compare_vram_logs() or their own problem lists; the native renderer's
picture counts between runs are information, never fed to judge().

    python port/tools/gs_verdict.py --code N [--backend wgpu] [--expect RE]... OUTPUT|-
    python port/tools/gs_verdict.py --vram A.bin B.bin

Both forms print the problems and exit 1 when there are any, 0 otherwise (2: bad usage).
"""
import argparse
import re
import sys
from pathlib import Path

VRAM_BYTES = 4 * 1024 * 1024
CHECKPOINT_NAME = 32
FALLBACK_MARKERS = ('using D3D11', 'is not in this build', 'native renderer unavailable', 'DHD_GS_NO_FALLBACK')
RUST_RENDERER = re.compile(r'^gs: renderer dhd_render\b')
BACKEND_REPORTS = (
    re.compile(r'^gs: backend (\S+)\s*$'),
    re.compile(r'^backend: (\S+)\s*$'),
    re.compile(r'^(\S+) (?:WARP|hardware) (?:raster|present) tests:'),
    re.compile(r'^Replayed \d+ fields \((\S+)\)'),
)
# (pattern, description): the first group is a count that must be 0
COUNTS = (
    (re.compile(r'(\d+) of \d+ pixels differ'), 'pixels differ'),
    (re.compile(r'(?<![\d ]of )(?<!\d)(\d+) pixels differ'), 'pixels differ'),
    (re.compile(r'(\d+) incorrect pixels'), 'incorrect pixels'),
    (re.compile(r'(\d+) words differ'), 'words differ'),
    (re.compile(r'VRAM differs in (\d+) words'), 'VRAM words differ'),
    (re.compile(r'(?<![=\w])(\d+) failures\b(?!=)'), 'failures'),  # not "words=N failures=0"
    (re.compile(r'\bfailures=(\d+)'), 'failures'),  # test_hw_renderer's "... failures=N" (gate G1)
    (re.compile(r'\bd3d11_vs_cpu=(\d+)'), 'D3D11 depth differs from the CPU'),
    (re.compile(r'\bexact_vs_cpu=(\d+)'), 'exact depth differs from the CPU'),
    (re.compile(r'(\d+) with VRAM differences'), 'pictures with VRAM differences'),
)
FAIL_LINE = re.compile(r'^\s*FAIL\b|: FAIL\b')
DIFFER = re.compile(r'DIFFER')
CHECKPOINTS = re.compile(r'checkpoints differ in number|checkpoint names differ')
VRAM_DIFFERS = re.compile(r'\bVRAM differs\b(?! in 0 words)')


def exit_name(code):
    """'exit 1', 'exit 0xC0000409 (crash)', 'signal 9'."""
    if code is None:
        return 'no exit code (timed out or not run)'
    if code < 0 and code > -64:
        return 'signal %d' % -code
    unsigned = code & 0xFFFFFFFF
    if unsigned >= 0xC0000000:
        return 'exit 0x%08X (crash)' % unsigned
    return 'exit %d' % code


def judge(code, text, backend=None, expect=(), label=''):
    """Problems of one test-program run (code: its exit code; text: stdout + stderr); [] = pass."""
    if backend is not None and backend != 'wgpu':
        raise ValueError('backend must be wgpu, not %r' % (backend,))
    prefix = label + ': ' if label else ''
    problems = []
    text = text or ''
    lines = text.replace('\r\n', '\n').replace('\r', '\n').split('\n')
    if code != 0:
        problems.append(prefix + exit_name(code))
    if not text.strip():
        problems.append(prefix + 'no output')
    reports = []
    rust_line = False
    for line in lines:
        if DIFFER.search(line):
            problems.append(prefix + 'DIFFER: ' + line.strip())
        if FAIL_LINE.search(line):
            problems.append(prefix + 'FAIL: ' + line.strip())
        if CHECKPOINTS.search(line):
            problems.append(prefix + 'checkpoints: ' + line.strip())
        if VRAM_DIFFERS.search(line):
            problems.append(prefix + 'VRAM differs: ' + line.strip())
        for pattern, what in COUNTS:
            for m in pattern.finditer(line):
                if int(m.group(1)):
                    problems.append(prefix + '%s %s: %s' % (m.group(1), what, line.strip()))
        if any(marker in line for marker in FALLBACK_MARKERS):
            problems.append(prefix + 'fallback: ' + line.strip())
        for pattern in BACKEND_REPORTS:
            m = pattern.search(line)
            if m:
                reports.append(m.group(1))
        if RUST_RENDERER.search(line):
            rust_line = True
    if backend is not None:
        if not reports:
            problems.append(prefix + 'backend %s requested but the run reports no backend' % backend)
        for name in sorted(set(reports) - {backend}):
            problems.append(prefix + 'backend %s requested but the run reports %s' % (backend, name))
    if not rust_line:
        problems.append(prefix + "no 'gs: renderer dhd_render' line (not the Rust renderer)")
    for pattern in expect:
        if not any(re.search(pattern, line) for line in lines):
            problems.append(prefix + 'missing expected line /%s/' % pattern)
    return problems


def read_vram_log(path):
    """DHD_GS_VRAM_LOG: per checkpoint a 32-byte name, then the 4 MiB VRAM. None if malformed."""
    data = Path(path).read_bytes()
    step = CHECKPOINT_NAME + VRAM_BYTES
    if len(data) % step:
        return None
    return [(data[i:i + CHECKPOINT_NAME].rstrip(b'\0').decode('latin-1'), data[i + CHECKPOINT_NAME:i + step])
            for i in range(0, len(data), step)]


def compare_vram_logs(a, b, label=''):
    """Problems comparing two DHD_GS_VRAM_LOG files word for word; [] = identical, same checkpoints."""
    prefix = label + ': ' if label else ''
    problems = []
    for path in (a, b):
        if not Path(path).is_file():
            problems.append(prefix + 'VRAM log %s is missing' % path)
    if problems:
        return problems
    la, lb = read_vram_log(a), read_vram_log(b)
    for path, log in ((a, la), (b, lb)):
        if log is None:
            problems.append(prefix + 'VRAM log %s is truncated (not whole checkpoints)' % path)
        elif not log:
            problems.append(prefix + 'VRAM log %s has no checkpoints' % path)
    if problems:
        return problems
    if len(la) != len(lb):
        return [prefix + 'checkpoints differ in number: %d vs %d' % (len(la), len(lb))]
    if [n for n, _ in la] != [n for n, _ in lb]:
        return [prefix + 'checkpoint names differ: %s vs %s' % ([n for n, _ in la], [n for n, _ in lb])]
    for (name, x), (_, y) in zip(la, lb):
        if x != y:
            words = sum(1 for i in range(0, len(x), 4) if x[i:i + 4] != y[i:i + 4])
            problems.append(prefix + 'checkpoint %s: %d words differ' % (name, words))
    return problems


def verdict(problems, out=sys.stdout):
    """Prints the problems; returns the exit code (1 if any, else 0)."""
    for p in problems:
        print('VERDICT FAIL ' + p, file=out)
    if not problems:
        print('VERDICT PASS', file=out)
    return 1 if problems else 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('output', nargs='?', help="a run's stdout + stderr in a file, or - for standard input")
    ap.add_argument('--code', type=lambda s: int(s, 0), help="the run's exit code (0x... accepted)")
    ap.add_argument('--backend', choices=('wgpu',))
    ap.add_argument('--expect', action='append', default=[], metavar='RE')
    ap.add_argument('--label', default='')
    ap.add_argument('--vram', nargs=2, metavar=('A', 'B'), help='compare two DHD_GS_VRAM_LOG files instead')
    args = ap.parse_args(argv)
    if args.vram:
        return verdict(compare_vram_logs(*args.vram, label=args.label))
    if args.output is None or args.code is None:
        ap.print_usage(sys.stderr)
        print('gs_verdict: OUTPUT and --code are required (or --vram A B)', file=sys.stderr)
        return 2
    text = sys.stdin.read() if args.output == '-' else Path(args.output).read_text(encoding='utf-8', errors='replace')
    return verdict(judge(args.code, text, backend=args.backend, expect=args.expect, label=args.label))


if __name__ == '__main__':
    sys.exit(main())
