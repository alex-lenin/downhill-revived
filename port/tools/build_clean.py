"""Build the PC executable the launchers use from a committed revision, in a clean checkout.

    python port/tools/build_clean.py [--rev HEAD] [--install]

The build works in a git worktree of the revision (build/port/clean/tree), so uncommitted edits
in the main checkout and objects left in build/port/obj by earlier partial builds can't get into
the executable. The generated, untracked inputs the build needs (splat's asm/ and assets/, about
37 MB, and the ELF they come from, iso/SCUS_971.77) are copied into the worktree (this drive
refuses junctions and symlinks). Then, inside the
worktree:
  port/tools/build.py -j 6                      every src/ unit, from scratch (game code -O1)
  DHD_BUILD_VU_AOT=1 port/tools/link.py          runtime, recompiled functions, data, link
The executable is copied to build/port/clean/<short rev>/dhd.exe with a manifest (revision, exe
hash, flags); --install also copies it to build/port/fast-preview/native/dhd.exe, keeping the
replaced one as dhd-previous.exe. Needs LLVM (found on PATH or in scoop's default place).

The plain command is the release build (finding 053): from a fresh clone, with only the player's
executable split by tools/split.py, it is as fast as the old local launcher builds (VU1 AOT and
the native VU1 kernels come from DHD_BUILD_VU_AOT=1, which this script always sets) and passes
gate_native_renderer.py. port/tools/package_release.py zips its dhd.exe for players.

--recomp-basis RSP (developers only, never for a release) takes the recompiled asm-only functions
(build/port/recomp) from an accepted link's object list instead, by name, and links again. The
older accepted executables (round 28 and native16) carry a frozen set of those objects, most
compiled before commit 8815d61e (INTC_STAT for sceGsSyncV); built from the tree they differ from
it only in one EE thread stack (finding 043). It only keeps a local executable comparable with
those old ones:

    python port/tools/build_clean.py --recomp-basis build/port/r29/native16/objects.rsp --install

Why (finding 043): a bare `link.py` links whatever build/port/obj holds. That directory held
objects from 2026-10-14, compiled before later port patches (the Huffman padding and fabsf fixes)
and shared-header changes, and the link crashed at startup in the native renderer runs.
"""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CLEAN = ROOT / 'build' / 'port' / 'clean'
TREE = CLEAN / 'tree'
UNTRACKED = ('asm', 'assets')  # generated from the local ELF, never in git
ELF = Path('iso') / 'SCUS_971.77'  # the local ELF itself: data.py checks the data image against it
LLVM_DEFAULT = Path(os.path.expanduser('~')) / 'scoop' / 'apps' / 'llvm' / 'current' / 'bin'


def git(*args, cwd=ROOT):
    return subprocess.run(['git', *args], cwd=cwd, check=True, capture_output=True, text=True).stdout.strip()


def sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def prepare_tree(rev):
    commit = git('rev-parse', rev)
    if not (TREE / '.git').exists():
        TREE.parent.mkdir(parents=True, exist_ok=True)
        git('worktree', 'add', '--detach', str(TREE), commit)
    else:
        if git('status', '--porcelain', '--untracked-files=no', cwd=TREE):
            sys.exit('%s has local changes; it must stay a clean checkout' % TREE)
        git('checkout', '--detach', commit, cwd=TREE)
    for name in UNTRACKED:
        link, target = TREE / name, ROOT / name
        if not target.exists():
            sys.exit('missing %s (see docs/STATE.md, Environment)' % target)
        if link.exists():
            shutil.rmtree(link)
        shutil.copytree(target, link)
    if not (ROOT / ELF).exists():
        sys.exit('missing %s (see docs/STATE.md, Environment)' % (ROOT / ELF))
    (TREE / ELF).parent.mkdir(exist_ok=True)
    shutil.copyfile(ROOT / ELF, TREE / ELF)
    # pinned third-party downloads (SDL3's source, port/tools/sdl3.py checks its hash): no second
    # download when this checkout has them
    if (ROOT / 'build' / 'downloads').is_dir():
        shutil.copytree(ROOT / 'build' / 'downloads', TREE / 'build' / 'downloads', dirs_exist_ok=True)
    return commit


def run(cmd, env, log):
    t = time.time()
    with open(log, 'w', encoding='utf-8') as f:
        p = subprocess.run(cmd, cwd=TREE, env=env, stdout=f, stderr=subprocess.STDOUT)
    print('%s: exit %d, %.0f s (log %s)' % (' '.join(cmd[1:3]), p.returncode, time.time() - t, log))
    if p.returncode:
        sys.exit(open(log, encoding='utf-8', errors='replace').read()[-3000:])


def relink_with_basis(basis, env):
    """Links again with the recompiled objects of an accepted link (same names), as link.py does."""
    sys.path.insert(0, str(TREE / 'port' / 'tools'))
    from pctarget import HOST_TARGET, IMAGE_BASE
    link = TREE / 'build' / 'port' / 'link'
    accepted = {}
    for line in Path(basis).read_text().splitlines():
        path = line.strip().strip('"')
        if path:
            name = Path(path).name.split('-', 1)[-1] if Path(path).name[:5].isdigit() else Path(path).name
            accepted[name] = path
    objects, swapped = [], 0
    for line in (link / 'objs.rsp').read_text().splitlines():
        if '/build/port/recomp/' in line and Path(line).name in accepted:
            line = accepted[Path(line).name].replace(os.sep, '/')
            swapped += 1
        objects.append(line)
    objects.append(str(link / 'missing.c.o').replace(os.sep, '/'))
    rsp = link / 'objs-basis.rsp'
    rsp.write_text(''.join('"%s"\n' % o for o in objects))
    exe = link / 'dhd-basis.exe'
    clang = shutil.which('clang', path=env.get('PATH')) or 'clang'  # Windows searches the parent's PATH
    p = subprocess.run([clang, '--target=' + HOST_TARGET, '-fuse-ld=lld', '-o', str(exe), '@' + str(rsp),
                        '-Wl,d3dcompiler.lib', '-Wl,d3d11.lib', '-Wl,dxgi.lib', '-Wl,user32.lib', '-Wl,winhttp.lib',
                        '-Wl,ole32.lib',
                        '-Wl,/map:' + str(exe.with_suffix('.map')), '-Wl,/subsystem:windows', '-Wl,/entry:mainCRTStartup', '-Wl,/safeseh:no',
                        '-Wl,/demangle:no', '-Wl,/force', '-Wl,/errorlimit:0', '-Wl,/base:0x%X' % IMAGE_BASE, '-Wl,/fixed',
                        '-Wl,/dynamicbase:no', '-Xlinker', '/section:.rdata,RW'], env=env, capture_output=True, text=True)
    if p.returncode or not exe.exists():
        sys.exit(p.stderr[-3000:])
    # /force links unresolved symbols as null: a Windows import left out (winhttp.lib, the online
    # races' WebSocket) crashed at its first call, so any unresolved import fails the build.
    imports = sorted({line.split('__imp_', 1)[1].split()[0] for line in p.stderr.splitlines()
                      if 'undefined symbol' in line and '__imp_' in line})
    if imports:
        sys.exit('relink left Windows imports unresolved (a library missing): ' + ', '.join(imports[:20]))
    print('relinked with %d recompiled objects from %s' % (swapped, basis))
    return exe


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--rev', default='HEAD')
    ap.add_argument('-j', type=int, default=6)
    ap.add_argument('--install', action='store_true', help='copy to build/port/fast-preview/native/dhd.exe')
    ap.add_argument('--skip-compile', action='store_true', help='relink only (objects already built for this rev)')
    ap.add_argument('--recomp-basis', type=Path, help="an accepted link's object list: take its recompiled functions")
    ap.add_argument('--tree', type=Path, help='worktree to build in (default build/port/clean/tree), e.g. for another branch')
    a = ap.parse_args()
    global TREE
    if a.tree:
        TREE = a.tree.resolve()
    commit = prepare_tree(a.rev)
    env = dict(os.environ)
    if not shutil.which('clang', path=env.get('PATH')) and LLVM_DEFAULT.exists():
        env['PATH'] = str(LLVM_DEFAULT) + os.pathsep + env.get('PATH', '')
    env['DHD_BUILD_VU_AOT'] = '1'
    env.pop('DHD_STACK_PROTECT', None)
    logs = CLEAN / 'logs'
    logs.mkdir(parents=True, exist_ok=True)
    if not a.skip_compile:
        obj = TREE / 'build' / 'port' / 'obj'
        if obj.exists():
            shutil.rmtree(obj)  # nothing stale: every unit is compiled again
        run([sys.executable, 'port/tools/build.py', '-j', str(min(a.j, 6))], env, logs / 'build.log')
    run([sys.executable, 'port/tools/link.py'], env, logs / 'link.log')
    exe = TREE / 'build' / 'port' / 'link' / 'dhd.exe'
    if a.recomp_basis:
        exe = relink_with_basis(a.recomp_basis.resolve(), env)
    out = CLEAN / (commit[:8] + ('-basis' if a.recomp_basis else ''))
    out.mkdir(parents=True, exist_ok=True)
    shutil.copy2(exe, out / 'dhd.exe')
    shutil.copy2(exe.with_suffix('.map'), out / 'dhd.map')
    manifest = dict(commit=commit, exe_sha256=sha256(out / 'dhd.exe'), vu_aot=True, game_opt='-O1',
                    recomp_basis=str(a.recomp_basis) if a.recomp_basis else None,
                    built=time.strftime('%Y-%m-%d %H:%M:%S'), recipe='port/tools/build_clean.py')
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=1) + '\n')
    print('dhd.exe %s from %s -> %s' % (manifest['exe_sha256'][:16], commit[:8], out))
    if a.install:
        dst = ROOT / 'build' / 'port' / 'fast-preview' / 'native' / 'dhd.exe'
        dst.parent.mkdir(parents=True, exist_ok=True)
        if dst.exists():
            shutil.copy2(dst, dst.with_name('dhd-previous.exe'))
        shutil.copy2(out / 'dhd.exe', dst)
        (dst.parent / 'build.json').write_text(json.dumps(manifest, indent=1) + '\n')
        # The settings launcher (port/launcher) beside it: Launcher.cmd -> Launcher.ps1 -> dhd.ini.
        shutil.copy2(ROOT / 'port' / 'launcher' / 'Launcher.ps1', dst.parent.parent / 'Launcher.ps1')
        (dst.parent.parent / 'Launcher.cmd').write_text(
            '@echo off\r\npowershell.exe -NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File "%~dp0Launcher.ps1"\r\n')
        print('installed', dst)


if __name__ == '__main__':
    main()
