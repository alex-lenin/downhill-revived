#!/usr/bin/env python3
"""Try a C/C++ candidate for one function in a private, full build of the image.

First a quick check (tools/quick_check.py): compile SRC alone and compare FUNC's bytes with
retail outside the fields the linker fills in. A difference there is final, so try_func reports
DIFF without building the image. Otherwise (and with --full-build) it continues:

Builds the whole image with SRC as the unit covering FUNC (replacing any configured unit
there), in build-try/<FUNC>/, links it at retail addresses and compares FUNC word for word
with retail (tools/diff_func.py). Uses exactly the same compiler, assembler and linker as the
real build, so a MATCH here is a MATCH in the image. Workers can run several in parallel.

Usage: try_func.py FUNC SRC [--end ADDR] [--data SECTION=A:B ...] [--flag -fX ...]
  If SRC starts with a UNIT marker (see tools/configure.py) and no options are given, the
  marker's end, data ranges and flags are used.
  --end   unit end (default: FUNC's end; give the next function's start for multi-function SRC)
  --data  data range SRC provides (e.g. .rodata=0x288C20:0x288C50 for a jump table)
  --full-build  skip the quick check (always link and compare the whole image)
Exit 0 = FUNC matches (and prints whether the rest of the image still matches).
"""
import argparse, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from diff_func import lookup


def quick(a, py, name, addr, size, src, B):
    """Compile SRC alone and compare FUNC outside relocated fields. Returns the exit code when
    that settles it (compile error, larger than retail, a difference), None when only the full
    build can decide (no difference found, or FUNC not found in the object)."""
    from quick_check import compare
    from diff_func import show
    o = '%s/quick/%s.o' % (B, name)
    r = subprocess.run([py, 'tools/sncc.py', src, o] + a.flag, cwd=ROOT, capture_output=True,
                       text=True)
    if r.returncode:
        out = r.stdout + r.stderr
        sys.stdout.write(out)
        why = next((l.strip() for l in out.split('\n') if 'error' in l.lower()), '')
        print('BUILD FAILED: compile error' + (': ' + why[:200] if why else ''))
        return 2
    c = compare(name, addr, size, os.path.join(ROOT, o))
    if c is None:
        return None
    rw, bw, differs, notes, csize = c
    if csize > size:
        print('BUILD FAILED: candidate compiled larger than retail (%s is %#x bytes, retail %#x; '
              'quick check, image not built)' % (name, csize, size))
        return 2
    bad = sum(differs)
    if not bad:
        return None
    show(name, addr, rw, bw, differs, 'quiet' if a.quiet else 'compact' if a.compact else 'full',
         notes)
    print('DIFF %s: %d of %d words differ (quick check of the compiled object; '
          'relocated fields not compared)' % (name, bad, len(rw)))
    print('whole image: not built (the function differs; the image is built once it does not)')
    return 1


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func')
    ap.add_argument('src')
    ap.add_argument('--end')
    ap.add_argument('--data', action='append', default=[])
    ap.add_argument('--flag', action='append', default=[])
    ap.add_argument('--quiet', action='store_true')
    ap.add_argument('--compact', action='store_true', help='short output: only the differing '
                    'words with context, and a short build-failure reason (display only)')
    ap.add_argument('--full-build', action='store_true',
                    help='skip the quick check and always build the whole image')
    ap.add_argument('--tag', default='', help='separate build dir build-try/<TAG>/<FUNC>, so '
                    'several workers can try the same function at once')
    a = ap.parse_args()
    name, addr, size = lookup(a.func)
    faddr = addr
    src = os.path.relpath(os.path.abspath(a.src), ROOT).replace('\\', '/')
    # WORKER.md's source rules first: a rule break can never count, whatever the bytes say.
    from source_rules import report
    lines, rejected = report(open(os.path.join(ROOT, src), encoding='utf-8', errors='replace').read(),
                             name)
    if lines:
        print('\n'.join(lines))
    if rejected:
        sys.exit(3)
    from configure import unit_marker
    marker = unit_marker(src)
    if marker and not (a.end or a.data or a.flag):  # the candidate declares its own unit
        addr = marker[0]  # a multi-function unit may start before FUNC
        a.end = '%X' % marker[1]
        a.data = ['%s=0x%X:0x%X' % (k, lo, hi) for k, (lo, hi) in marker[3].items()]
        a.flag = marker[4]
    if a.end:
        end = int(a.end, 16)
    else:  # the next function's start (alignment padding belongs to the unit)
        from configure import text_functions, TEXT_END
        end = next((v for v, _ in text_functions() if v > addr), TEXT_END)
    B = 'build-try/%s%s' % (a.tag + '/' if a.tag else '', name)
    # One build per directory at a time: concurrent runs with the same tag would clobber each
    # other's files and fail with a spurious BUILD FAILED. Wait for the other run instead.
    import time
    os.makedirs(os.path.join(ROOT, B), exist_ok=True)
    lock = os.path.join(ROOT, B, '.try.lock')
    for _ in range(3600):
        try:
            fd = os.open(lock, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
            break
        except FileExistsError:
            if time.time() - os.path.getmtime(lock) > 900:  # stale (killed run)
                os.remove(lock)
            time.sleep(0.5)
    import atexit
    atexit.register(lambda: (os.close(fd), os.remove(lock)))
    py = sys.executable
    if not a.full_build:
        q = quick(a, py, name, faddr, size, src, B)
        if q is not None:
            sys.exit(q)
    # The full build reads the main build's sources: wait while promote.py rewrites them.
    import srclock
    held = srclock.shared()
    held.__enter__()
    atexit.register(lambda: held.__exit__(None, None, None))
    unit = ' '.join(['0x%08X 0x%08X %s' % (addr, end, src)] + a.data + a.flag)
    r = subprocess.run([py, 'tools/configure.py', '--build-dir', B, '--extra-unit', unit],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout + r.stderr)
        print('CONFIGURE FAILED (bad unit range? END must be a function start)')
        sys.exit(2)
    r = subprocess.run([py, '-m', 'ninja', '-f', B + '/build.ninja', B + '/dhd.elf'],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        out = r.stdout + r.stderr
        noise = ['Warning: .end directive', 'macro invoked from here']
        if a.compact:  # also drop ninja progress, the link command and a known linker warning
            noise += ['ninja: build stopped', 'tools/link.py ', "`D_0029DE30'",
                      'alignment discrepancies can cause', 'FAILED: [code=']
        sys.stdout.write('\n'.join(l for l in out.split('\n') if not any(x in l for x in noise)
                                   and not (a.compact and l.startswith('['))))
        why = ('candidate compiled larger than retail (or a --data range too small)'
               if 'location counter backwards' in out or 'is too big' in out else
               next((l.strip() for l in out.split('\n') if 'error' in l.lower()), ''))
        print('BUILD FAILED' + (': ' + why[:200] if why else ''))
        sys.exit(2)
    d = subprocess.run([py, 'tools/diff_func.py', name, '--elf', B + '/dhd.elf']
                       + (['--quiet'] if a.quiet else []) + (['--compact'] if a.compact else []),
                       cwd=ROOT)
    c = subprocess.run([py, 'tools/check_image.py', B + '/dhd.elf'], cwd=ROOT,
                       capture_output=True, text=True)
    last = c.stdout.strip().split('\n')[-1]
    print('whole image: %s' % last)
    if d.returncode == 0 and c.returncode:
        print(c.stdout)
    sys.exit(d.returncode)


if __name__ == '__main__':
    main()
