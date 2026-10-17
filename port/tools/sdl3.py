"""SDL3 for the PC build: generic controllers (finding 059).

    python port/tools/sdl3.py          prints the static library it built (or had built)

link.py calls build(). It downloads SDL's release source once (pinned version, SHA-256 checked;
nothing else is accepted), unpacks it under build/port/sdl3 and builds a static library with CMake
and Ninja for the host target (32-bit Windows, clang, the static C runtime that dhd.exe uses).
Only the joystick, gamepad, HIDAPI and event subsystems are built: no audio, video or render.
SDL is zlib-licensed (port/release/THIRD-PARTY-NOTICES.txt).

The download is kept in build/downloads (build_clean.py copies it into its worktree), so a
rebuild needs no network. Needs CMake on PATH (or in its default place), LLVM's clang and the
ninja Python package (requirements.txt)."""
import hashlib
import os
import shutil
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pctarget import HOST_TARGET  # noqa: E402

VERSION = '3.4.18'
URL = 'https://github.com/libsdl-org/SDL/releases/download/release-%s/SDL3-%s.tar.gz' % (VERSION, VERSION)
SHA256 = '9c75cf16330322c217dedd2e0609f1124f1b54b8633e763467b4684d0f4334a3'

ROOT = Path(__file__).resolve().parents[2]
DOWNLOADS = ROOT / 'build' / 'downloads'
OUT = ROOT / 'build' / 'port' / 'sdl3'
CMAKE_DEFAULT = Path(os.environ.get('ProgramFiles', r'C:\Program Files')) / 'CMake' / 'bin' / 'cmake.exe'

# Everything but the controllers off. SDL_JOYSTICK brings the drivers: XInput, RawInput,
# Windows.Gaming.Input, DirectInput (generic pads, with the built-in mapping database) and HIDAPI
# (DualSense, DualShock 4, Switch Pro, 8BitDo and others, rumble included).
OPTIONS = dict(SDL_SHARED='OFF', SDL_STATIC='ON', SDL_TEST_LIBRARY='OFF', SDL_TESTS='OFF', SDL_EXAMPLES='OFF',
               SDL_INSTALL='OFF', SDL_AUDIO='OFF', SDL_VIDEO='OFF', SDL_GPU='OFF', SDL_RENDER='OFF',
               SDL_CAMERA='OFF', SDL_POWER='OFF', SDL_SENSOR='OFF', SDL_DIALOG='OFF', SDL_TRAY='OFF',
               SDL_HAPTIC='OFF', SDL_JOYSTICK='ON', SDL_HIDAPI='ON', SDL_HIDAPI_JOYSTICK='ON',
               SDL_VIRTUAL_JOYSTICK='OFF', SDL_LIBC='ON', SDL_ASSEMBLY='OFF',
               SDL_DISABLE_INSTALL='ON', SDL_DISABLE_UNINSTALL='ON',
               # SDL's CMake looks for dinput.h only when audio or video is on, which would leave
               # out the DirectInput driver the generic pads need: the Windows SDK has it.
               HAVE_DINPUT_H='1',
               # GameInput (a C++ driver for Microsoft's newest API, off by default on desktop
               # Windows) is left out.
               HAVE_GAMEINPUT_H='0',
               # atan from SDL's own libm (fdlibm) instead of the C runtime's: the runtime's atan
               # brings a table of doubles in which package_release.py's path check reads
               # "Y:/(A6>" (SDL's dynamic API table references every SDL function, SDL_atan too)
               LIBC_HAS_ATAN='0', LIBC_HAS_ATANF='0', LIBC_HAS_ATAN2='0', LIBC_HAS_ATAN2F='0')


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            h.update(block)
    return h.hexdigest()


def fetch():
    tarball = DOWNLOADS / ('SDL3-%s.tar.gz' % VERSION)
    if not tarball.exists() or sha256(tarball) != SHA256:
        DOWNLOADS.mkdir(parents=True, exist_ok=True)
        part = tarball.with_suffix('.part')
        print('SDL3: downloading', URL)
        with urllib.request.urlopen(URL, timeout=120) as r, open(part, 'wb') as f:
            shutil.copyfileobj(r, f)
        got = sha256(part)
        if got != SHA256:
            part.unlink()
            sys.exit('SDL3: %s has SHA-256 %s, expected %s' % (URL, got, SHA256))
        os.replace(part, tarball)
    return tarball


def tool(name, default=None):
    found = shutil.which(name)
    if found:
        return found
    if default and Path(default).exists():
        return str(default)
    sys.exit('SDL3: %s not found (docs: README.md, Building)' % name)


def build():
    """The static library and its include directory; built once per version."""
    lib = OUT / VERSION / 'build' / 'SDL3-static.lib'
    include = OUT / VERSION / 'src' / ('SDL3-%s' % VERSION) / 'include'
    stamp = OUT / VERSION / 'built.txt'
    recipe = repr(sorted(OPTIONS.items())) + HOST_TARGET
    if lib.exists() and stamp.exists() and stamp.read_text() == recipe:
        return lib, include
    tarball = fetch()
    src_parent = OUT / VERSION / 'src'
    if src_parent.exists():
        shutil.rmtree(src_parent)
    src_parent.mkdir(parents=True)
    with tarfile.open(tarball) as t:
        t.extractall(src_parent, filter='data')
    src = src_parent / ('SDL3-%s' % VERSION)
    bld = OUT / VERSION / 'build'
    if bld.exists():
        shutil.rmtree(bld)
    import ninja
    cmake = tool('cmake', CMAKE_DEFAULT)
    clang = tool('clang')
    cmd = [cmake, '-S', str(src), '-B', str(bld), '-G', 'Ninja',
           '-DCMAKE_MAKE_PROGRAM=' + os.path.join(ninja.BIN_DIR, 'ninja.exe'),
           '-DCMAKE_C_COMPILER=' + clang, '-DCMAKE_C_COMPILER_TARGET=' + HOST_TARGET,
           '-DCMAKE_CXX_COMPILER=' + clang.replace('clang', 'clang++'), '-DCMAKE_CXX_COMPILER_TARGET=' + HOST_TARGET,
           '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded',
           '-DCMAKE_C_FLAGS=-msse2', '-DCMAKE_POLICY_DEFAULT_CMP0091=NEW']
    cmd += ['-D%s=%s' % kv for kv in sorted(OPTIONS.items())]
    for step in (cmd, [cmake, '--build', str(bld)]):
        p = subprocess.run(step, capture_output=True, text=True)
        if p.returncode:
            sys.exit('SDL3 build failed:\n' + (p.stdout + p.stderr)[-4000:])
    if not lib.exists():
        sys.exit('SDL3: %s missing after the build' % lib)
    stamp.write_text(recipe)
    return lib, include


# The Windows libraries the static SDL3 needs at link time.
SYSTEM_LIBS = ['setupapi.lib', 'winmm.lib', 'imm32.lib', 'version.lib', 'ole32.lib', 'oleaut32.lib',
               'advapi32.lib', 'shell32.lib', 'user32.lib', 'gdi32.lib', 'cfgmgr32.lib', 'uuid.lib']


if __name__ == '__main__':
    print(*build())
