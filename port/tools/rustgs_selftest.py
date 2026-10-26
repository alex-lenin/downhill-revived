"""rustgs_selftest.py: the referee of port/tools/rustgs.py (task A1a).

    python port/tools/rustgs_selftest.py [--keep]

Exit 0 when every check passes. Each check prints `rustgs_selftest: ok <check>`; the first failure
prints `rustgs_selftest: FAIL <check>: <why>` and exits 1. It takes a few minutes (one x64 release
build of dhd_render in a second tree; the dependencies come from the seed).

What it checks
--------------
1. `no-fp-guard` refused: `build` and `test` with `--features no-fp-guard` (alone and with
   test-hooks) exit nonzero with a `rustgs: refused:` line naming no-fp-guard. (The feature exists for
   the guard's negative control, run by hand with cargo; finding 085.)
2. Cache hit: an x64 release build of this tree, then the same build again: the second reports
   `"cache": "hit"`, the same key and the same sha256, and the lib's bytes hash to it.
3. A second worktree: this tree's sources copied to a scratch directory (port/render without
   target, port/gs, port/tools, .cargo, toolchain.lock.json; no git), built there with `--no-cache`
   and a hostile environment (CARGO_INCREMENTAL=1, RUSTFLAGS set): the lib's sha256, the cache key
   and the rustflags equal tree 1's, and neither lib holds either tree's path.
4. A planted absolute include refused: tree 2's src/lib.rs gets
   `include_str!("D:/.../planted.txt")` (a file outside the repo); the build exits nonzero with a
   `rustgs: refused:` line and writes no report.
5. A planted version drift refused: tree 2's Cargo.toml pins naga =29.0.4 and its Cargo.lock is
   updated to it (`cargo update --offline -p naga --precise 29.0.4`; the registry cache has it); the
   build exits nonzero with a `rustgs: refused:` line naming naga, and writes no report.
Scratch trees and the target directories rustgs made for them are deleted afterwards (`--keep`
keeps them).

The rustgs.py contract this assumes (plan section 3.1, with the F0b changes below)
---------------------------------------------------------------------------------
- `rustgs.py build --target T [--features a,b] [--profile release|fixloop] [--no-cache]
  [--report FILE]`; also `test`, `run`, `tree` (plan 3.1). rustgs finds the repo root from its own
  path (port/tools/rustgs.py -> ../..); it needs no git. Every cargo call runs with cwd
  port/render, `--locked --offline`, `-j 2`.
- `--report FILE` writes JSON after a successful build: `lib` (absolute path of the .lib / .wasm),
  `sha256` (of the lib's bytes), `cache` ("hit" or "miss"), `key` (the cache key), `rustflags` (the
  list passed as CARGO_ENCODED_RUSTFLAGS), `target_dir` (the CARGO_TARGET_DIR used), `glue`
  (wasm-bindgen's files; [] natively). `--no-cache` builds with cargo even on a hit (and stores
  nothing). No report is written when the build fails or is refused.
- A refusal prints one line starting `rustgs: refused:` (what and why) and exits nonzero.
- The gates (plan 3.1): `cargo tree -e normal -i naga|wgpu-core|wgpu-hal` prints exactly 29.0.3;
  `no-fp-guard` refused outside this selftest; a source including a file by an absolute path, or
  any compiled input outside the tree, CARGO_HOME and RUSTUP_HOME, refused (the dep-info `.d`
  file of the build lists every input; scanning sources for absolute `include_*!` paths is the
  cheap pre-check); check_rust_lib.py's refusals after the build.

The flag and seed contract (F0b; replaces plan 3.1's flags where they differ)
-----------------------------------------------------------------------------
Flags must not depend on the worktree, so every worktree builds the same lib, shares the
DHD_DEVCACHES/dhd-objcache entries and sccache's compiled dependencies, and can start from the seeds:
- CARGO_ENCODED_RUSTFLAGS is the only flag source (RUSTFLAGS removed from cargo's environment):
  - msvc: `-Ctarget-feature=+crt-static`;
  - every target: `--remap-path-prefix=<CARGO_HOME>=cargo` and `--remap-path-prefix=<RUSTUP_HOME>=rustup`,
    each in the backslash and the forward-slash spelling (4 flags). NO remap of the target dir or
    the repo root (they differ per worktree; check_rust_lib.py refuses any leak of them instead);
  - wasm32: `-Clink-arg=<a>` for each of `--import-memory --export-memory --no-stack-first
    --global-base=1610612736 -z stack-size=1048576 --initial-memory=1761607680
    --max-memory=2147483648 --export=__heap_base --export=__data_end --export=__stack_pointer`
    (decimal: wasm-ld refuses 0x numbers here).
- CARGO_INCREMENTAL is removed from cargo's environment (sccache refuses to run with it set); the
  profiles decide (release: off; fixloop: `incremental = true`, which sccache passes through).
- CARGO_TARGET_DIR = DHD_DEVCACHES/dhd-cargo/<sha1(worktree path)[:8]> (a C: path refused). On first
  use of <triple>/<profile> in it, `DHD_DEVCACHES/dhd-cargo/seed/<triple>/<profile>/` is copied there
  with mtimes kept (shutil.copytree; a copy that drops mtimes rebuilds every dependency). Each seed
  has a `seed.json` (rustflags, the LF-normalised Cargo.lock's sha256, rustc -vV, the commands); a
  seed whose rustflags, lock or rustc differ from the build's is not used. The seeds were built by:
    x64:   cargo rustc --lib --crate-type staticlib --features native
           cargo test --features native --no-run        (the unwind variants the tests link)
    wasm:  cargo rustc --lib --crate-type cdylib --no-default-features --features web
  for both profiles (release, fixloop). A target dir's `.rustc_info.json` caches rustc's probe; a
  failed probe (e.g. a bad environment) stays cached until that file is deleted.
- `test` sets DHDR_TEST_DXC to port/tools/dxc.py's x64 dxcompiler.dll (the DXC tests fail without it).
"""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import tempfile
import sys
from pathlib import Path
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches

REPO = Path(__file__).resolve().parents[2]
X64 = 'x86_64-pc-windows-msvc'
CARGO_TARGETS = (DEVCACHES / 'dhd-cargo')
SEED = CARGO_TARGETS / 'seed'


class Fail(Exception):
    pass


def ok(check):
    print(f'rustgs_selftest: ok {check}', flush=True)


def rustgs(tree, args, env=None, report=None):
    cmd = [sys.executable, str(tree / 'port/tools/rustgs.py'), *args]
    if report is not None:
        if report.exists():
            report.unlink()
        cmd += ['--report', str(report)]
    print('  +', ' '.join(cmd), flush=True)
    r = subprocess.run(cmd, cwd=str(tree), env=env, capture_output=True, text=True, errors='replace')
    return r


def tail(r, n=25):
    lines = (r.stdout + r.stderr).splitlines()
    return '\n'.join(lines[-n:])


def refused(check, r, words, report=None):
    """r must be a refusal: nonzero exit, a `rustgs: refused:` line naming one of `words`, no report."""
    if r.returncode == 0:
        raise Fail(f'{check}: rustgs exited 0; it must refuse\n{tail(r)}')
    lines = [l for l in (r.stdout + r.stderr).splitlines() if l.startswith('rustgs: refused:')]
    if not lines:
        raise Fail(f'{check}: exit {r.returncode} without a "rustgs: refused:" line\n{tail(r)}')
    if not any(w.lower() in l.lower() for l in lines for w in words):
        raise Fail(f'{check}: the refusal names none of {words}: {lines}')
    if report is not None and report.exists():
        raise Fail(f'{check}: a refused build wrote a report')
    ok(check)


def built(check, r, report):
    if r.returncode != 0:
        raise Fail(f'{check}: rustgs exited {r.returncode}\n{tail(r)}')
    if not report.exists():
        raise Fail(f'{check}: no report written')
    rep = json.loads(report.read_text())
    for field in ('lib', 'sha256', 'cache', 'key', 'rustflags', 'target_dir', 'glue'):
        if field not in rep:
            raise Fail(f'{check}: the report has no "{field}"')
    lib = Path(rep['lib'])
    if not lib.is_file():
        raise Fail(f'{check}: the report\'s lib {lib} is not a file')
    sha = hashlib.sha256(lib.read_bytes()).hexdigest()
    if sha != rep['sha256']:
        raise Fail(f'{check}: the lib hashes to {sha}, the report says {rep["sha256"]}')
    if rep['cache'] not in ('hit', 'miss'):
        raise Fail(f'{check}: cache is {rep["cache"]!r}')
    return rep


def spellings(path):
    p = os.path.normpath(str(path))
    return {p, p.replace('\\', '/'), p.lower(), p.replace('\\', '/').lower()}


def no_paths(check, lib, trees):
    data = Path(lib).read_bytes().lower()
    for t in trees:
        for s in spellings(t):
            if s.lower().encode() in data:
                raise Fail(f'{check}: {lib} holds the tree path {s}')


def copy_tree(dst):
    """The files a build reads, as a second worktree would hold them (bytes kept, CRLF included)."""
    skip = shutil.ignore_patterns('target', '__pycache__')
    for rel in ('port/render', 'port/gs', 'port/tools', '.cargo'):
        if (REPO / rel).exists():
            shutil.copytree(REPO / rel, dst / rel, ignore=skip)
    for rel in ('toolchain.lock.json', '.gitattributes'):
        if (REPO / rel).exists():
            shutil.copy2(REPO / rel, dst / rel)


def remove_target(rep, made):
    """Deletes a target dir rustgs made for a scratch tree (never the seeds, never tree 1's)."""
    t = Path(rep['target_dir']).resolve()
    if t.parent == CARGO_TARGETS.resolve() and t != SEED.resolve() and t not in made:
        made.append(t)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--keep', action='store_true', help='keep the scratch trees and their target dirs')
    args = ap.parse_args()
    if not (REPO / 'port/tools/rustgs.py').is_file():
        print('rustgs_selftest: FAIL setup: port/tools/rustgs.py does not exist (task A1a)')
        return 1
    scratch_root = Path(os.environ.get('DHD_SCRATCH') or tempfile.gettempdir())
    if scratch_root.resolve().drive.upper() == 'C:':
        print('rustgs_selftest: FAIL setup: the scratch directory must not be on C:')
        return 1
    scratch = scratch_root / f'rustgs-selftest-{os.getpid()}'
    scratch.mkdir(parents=True)
    made_targets = []
    tree2 = scratch / 'tree2'
    try:
        # 1. no-fp-guard
        for cmd in (['build', '--target', X64, '--features', 'no-fp-guard', '--profile', 'fixloop'],
                    ['build', '--target', X64, '--features', 'test-hooks,no-fp-guard', '--profile', 'fixloop'],
                    ['test', '--target', X64, '--features', 'no-fp-guard', '--profile', 'fixloop', '--', '--test', 'device_smoke']):
            report = scratch / 'refused.json'
            refused(f'no-fp-guard refused ({cmd[0]} --features {cmd[4]})', rustgs(REPO, cmd, report=report if cmd[0] == 'build' else None),
                    ['no-fp-guard'], report)

        # 2. the second build of the same tree is a cache hit
        release = ['build', '--target', X64, '--profile', 'release']
        r1 = built('tree 1 build', rustgs(REPO, release, report=scratch / 'r1.json'), scratch / 'r1.json')
        r2 = built('tree 1 rebuild', rustgs(REPO, release, report=scratch / 'r2.json'), scratch / 'r2.json')
        if r2['cache'] != 'hit':
            raise Fail(f'cache hit: the second build reports cache {r2["cache"]!r}')
        if (r2['sha256'], r2['key']) != (r1['sha256'], r1['key']):
            raise Fail('cache hit: the second build has another sha256 or key')
        ok('cache hit on the second build')

        # 3. a second worktree builds the same bytes
        copy_tree(tree2)
        env = dict(os.environ)
        env['CARGO_INCREMENTAL'] = '1'
        env['RUSTFLAGS'] = '-Copt-level=0'
        r = rustgs(tree2, release + ['--no-cache'], env=env, report=scratch / 't2.json')
        t2 = built('tree 2 build', r, scratch / 't2.json')
        remove_target(t2, made_targets)
        if Path(t2['target_dir']).resolve() == Path(r1['target_dir']).resolve():
            raise Fail('second worktree: tree 2 used tree 1\'s target dir')
        if t2['rustflags'] != r1['rustflags']:
            raise Fail(f'second worktree: the rustflags differ:\n  {r1["rustflags"]}\n  {t2["rustflags"]}')
        if t2['key'] != r1['key']:
            raise Fail('second worktree: the cache key differs (it depends on the worktree)')
        if t2['sha256'] != r1['sha256']:
            raise Fail(f'second worktree: lib sha256 {t2["sha256"]} != tree 1\'s {r1["sha256"]}')
        no_paths('second worktree', r1['lib'], [REPO, tree2])
        no_paths('second worktree', t2['lib'], [REPO, tree2])
        ok('equal lib sha256 from a second worktree')

        # 4. a planted absolute include_str!
        lib_rs = tree2 / 'port/render/src/lib.rs'
        clean_lib = lib_rs.read_bytes()
        planted = scratch / 'planted.txt'
        planted.write_text('planted outside the repo\n')
        path = str(planted).replace('\\', '/')
        lib_rs.write_bytes(clean_lib + (
            '\r\n#[doc(hidden)]\r\npub const RUSTGS_SELFTEST_PLANTED: &str = include_str!("%s");\r\n' % path).encode())
        report = scratch / 'planted.json'
        refused('planted include_str!("D:/...") refused', rustgs(tree2, release + ['--no-cache'], report=report),
                ['include', 'planted', 'absolute', 'outside'], report)
        lib_rs.write_bytes(clean_lib)

        # 5. a planted version drift
        toml = tree2 / 'port/render/Cargo.toml'
        text = toml.read_bytes()
        pin = b'naga = { version = "=29.0.3"'
        if text.count(pin) != 1:
            raise Fail('version drift: Cargo.toml has no single naga =29.0.3 pin to plant over')
        toml.write_bytes(text.replace(pin, b'naga = { version = "=29.0.4"'))
        u = subprocess.run(['cargo', 'update', '--offline', '-p', 'naga', '--precise', '29.0.4'],
                           cwd=str(tree2 / 'port/render'), capture_output=True, text=True, errors='replace')
        if u.returncode != 0 or b'"29.0.4"' not in (tree2 / 'port/render/Cargo.lock').read_bytes():
            raise Fail(f'version drift: could not plant naga 29.0.4 (is it in the registry cache?)\n{u.stderr[-2000:]}')
        report = scratch / 'drift.json'
        refused('planted version drift (naga 29.0.4) refused', rustgs(tree2, release + ['--no-cache'], report=report),
                ['naga', '29.0.4'], report)
    except Fail as e:
        print(f'rustgs_selftest: FAIL {e}', flush=True)
        return 1
    finally:
        if args.keep:
            print(f'rustgs_selftest: kept {scratch} and {[str(t) for t in made_targets]}')
        else:
            for t in made_targets:
                shutil.rmtree(t, ignore_errors=True)
            shutil.rmtree(scratch, ignore_errors=True)
    print('rustgs_selftest: all checks passed')
    return 0


if __name__ == '__main__':
    sys.exit(main())
