"""The public release: build/release/DownhillRevived-<version>-win.zip (and the same files unzipped
in build/release/DownhillRevived-<version>/), built from the 64-bit game on the Rust renderer.

    python port/tools/build_wasm.py                      # the game core, build/wasm/dhd_core.wasm
    python port/tools/package_release.py                 # dhd.exe from it, checked, zipped
    python port/tools/package_release.py --wasm PATH     # from another dhd_core.wasm
    python port/tools/package_release.py --exe PATH      # a ready build_host64.py dhd64.exe (link.rsp beside it)

dhd.exe is build_host64.py's dhd64.exe (wasm2c, clang-cl, lld-link; the Rust renderer,
port/render, linked in as dhd_render.lib) under build/release-work/, with an x64 SDL3 static
library (sdl3.py's by default, or --sdl-lib/--sdl-include). In the package it is named dhd.exe:
the name every earlier release used, so the updater of a 32-bit release (0.3.1 and before) puts it
in place of the old one (finding 088, CL3).

The version is port/include/version.h's DHD_VERSION, the one the exe was built with (--version,
if given, must be the same). Beside the zip goes latest.json, the update manifest the game's
updater reads (port/hle/update.c, finding 054): {"version", "url" (the zip as a GitHub release
asset of tag v<version>, or --base-url + the zip's name), "sha256" (the zip's), "size", "page"}.
Publish both as assets of the GitHub release tagged v<version>.

The zip holds one folder, DownhillRevived-<version>, with:
  dhd.exe                  the 64-bit game; it imports only Windows' own DLLs (checked)
  README.txt               how to play (port/release/README.txt)
  LICENSE.txt              the GPL-3.0 text (LICENSE)
  THIRD-PARTY-NOTICES.txt  port/release/THIRD-PARTY-NOTICES.txt, then the licences of the Rust
                           crates linked into dhd.exe, generated from port/render/Cargo.lock and
                           the crates' own licence files (rust_notices)
  dxcompiler.dll           Microsoft's DirectX Shader Compiler, x64, unchanged (port/tools/dxc.py):
                           the renderer compiles its Direct3D 12 shaders with it (finding 084 §14)

The package is refused if:
  - port/ or src/ has uncommitted changes to tracked files (a release is built from committed source);
  - dhd.exe is not an x64 executable linked with exactly one dhd_render.lib (link.rsp beside it), or
    that library is unfit (rust_lib_problems: it names a folder of this machine, or
    check_rust_lib.problems finds one of its refusals), or dhd.exe wasn't built with the version;
  - dhd.exe holds an absolute path (a builder's folder, C:\\Users\\..., the repository's own path), in
    ASCII or UTF-16 (a drive path "X:\\..." in ASCII: in the data sections, where strings are; in
    machine code such bytes are an instruction's operand). SDL's own list of USB vibration device
    paths (SDL_OWN_PATHS) is the one exception;
  - dhd.exe holds a server address other than the official one (dhd.skuza.app, the built-in
    default): another workers.dev host, DHD_NET_SERVER when set, or the server line of a dhd.ini
    named by --settings;
  - dhd.exe imports a DLL that is not Windows' own;
  - a packaged file name looks like game data, or any packaged file holds a run of 64 bytes or more
    of the retail ELF (port/tools/scan_retail.py) other than the string literals and tables the
    decompiled source itself holds (scan_retail.SOURCE) and the Rust library's own bytes. The
    game's data comes from the player's disc (port/hle/game_data.c);
  - a Rust crate linked in has a licence other than the permissive ones in LICENCES, or no text
    for the one it is used under.
A third-party binary (third_party in main) is checked by its SHA-256 instead: it must be exactly the file
its publisher released (pinned in its fetch script), so it can hold neither game data nor a path or
server of ours. The byte scans would only find coincidences in it (dxcompiler.dll holds the words
0..14, as a run of the ELF's .data does, and the text "\\temp").
"""
import argparse
import functools
import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import zipfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dxc  # noqa: E402
import scan_retail  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
RELEASE = ROOT / 'port' / 'release'
RENDER = ROOT / 'port' / 'render'
VERSION_H = ROOT / 'port' / 'include' / 'version.h'
WORK = ROOT / 'build' / 'release-work'
# Where players get releases: latest.json names the zip as an asset of the release v<version>.
RELEASES = 'https://github.com/skuza-oss/downhill-revived/releases'


def header_version():
    """DHD_VERSION from port/include/version.h: the one place the release's version is written."""
    m = re.search(r'#define\s+DHD_VERSION\s+"([^"]+)"', VERSION_H.read_text(encoding='utf-8'))
    if not m:
        sys.exit('%s: no DHD_VERSION' % VERSION_H)
    return m.group(1)


GAME_DATA = re.compile(r'\.(iso|bin|img|elf|irx|mc2|ps2|raw|vpk|vag|pss)$|^SCUS_|bios', re.I)
# an absolute Windows path: a drive letter, a separator, then at least two printable characters
ABSOLUTE = re.compile(rb'(?<![A-Za-z0-9])[A-Za-z]:[\\/][\x20-\x7e]{2,}')
# The official public race server (port/hle/host.c): the one server address a release may name.
OFFICIAL_HOST = b'dhd.skuza.app'
# any workers.dev host (other_hosts allows only the official one)
WORKERS_HOST = re.compile(rb'[A-Za-z0-9.-]*workers\.dev', re.I)
# user folders, and the builder's own account name and checkout folders
FORBIDDEN = [b'\\Users\\', b'/Users/'] + [
    s.encode() for s in {os.environ.get('USERNAME', ''), os.environ.get('USER', '')} if len(s) > 2] + [
    p.encode() for folder in (ROOT, ROOT.parent) if len(folder.parts) > 1
    for p in (str(folder), str(folder).replace('\\', '/'), str(folder)[2:], str(folder)[2:].replace('\\', '/'))]
# SDL_windows.c's list of USB vibration device paths: SDL's own text, not a builder's folder.
SDL_OWN_PATHS = ('C:/Windows/USB_Vibration', 'C:/Windows/USB Vibration')
# Windows' own DLLs: dhd.exe may import nothing else (no DLL is shipped beside it to be imported;
# dxcompiler.dll is loaded by the renderer, not imported).
SYSTEM_DLL = re.compile(r'^(kernel32|user32|gdi32|advapi32|shell32|ole32|oleaut32|ws2_32|winmm|winhttp|'
                        r'comdlg32|dxgi|api-ms-win-.*|ext-ms-.*|ucrtbase|onecoreuap|'
                        r'bcrypt|imm32|version|setupapi|cfgmgr32|combase|uxtheme|dwmapi|hid|hidapi|'
                        r'shcore|msvcrt|mfplat|xinput1_4|xinput9_1_0|dinput8|wintrust|crypt32|'
                        r'ntdll|bcryptprimitives)\.dll$', re.I)  # the last two: Rust std's
RUST_TRIPLE = 'x86_64-pc-windows-msvc'   # the Rust renderer's native target
RUST_LIB = 'dhd_render.lib'              # build_host64.py links it (rustgs.py builds it)


def utf16_strings(data):
    """UTF-16LE text in data, as bytes (each character's low byte), for the same checks."""
    return b'\n'.join(m.group(0)[::2] for m in re.finditer(rb'(?:[\x20-\x7e]\x00){4,}', data))


def server_strings(settings):
    found = [os.environ.get('DHD_NET_SERVER', '')]
    if settings:
        for line in open(settings, encoding='utf-8-sig'):
            key, _, value = line.partition('=')
            if key.strip() in ('server', 'DHD_NET_SERVER'):
                found.append(value.split(';')[0].strip().strip('"'))
    out = set()
    for s in filter(None, found):
        host = re.sub(r'^\w+://', '', s).split('/')[0]
        if host.encode().lower() == OFFICIAL_HOST:
            continue  # the official server may be named
        out.add(s.encode())
        if host:
            out.add(host.encode())  # the host name alone, too
    return out


def other_hosts(text):
    """workers.dev hosts in text other than the official server."""
    return sorted({m.group(0).lower() for m in WORKERS_HOST.finditer(text)} - {OFFICIAL_HOST})


def without_code(data):
    """data with the bytes of the PE's executable sections zeroed. The compiler puts every string in a
    data section, never in code; in code a 4-byte immediate after an opcode can spell a drive path by
    chance (push 0x407c2f3a is 'h:/|@'), depending only on where the linker placed things."""
    out = bytearray(data)
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    assert data[:2] == b'MZ' and data[pe:pe + 4] == b'PE\0\0', 'dhd.exe is not a PE file'
    count, optional = struct.unpack_from('<H', data, pe + 6)[0], struct.unpack_from('<H', data, pe + 20)[0]
    for k in range(count):
        header = pe + 24 + optional + 40 * k
        size, offset = struct.unpack_from('<II', data, header + 16)
        if struct.unpack_from('<I', data, header + 36)[0] & 0x20000000:  # IMAGE_SCN_MEM_EXECUTE
            out[offset:offset + size] = bytes(size)
    return bytes(out)


def chance_bytes(data, m):
    """True for an ASCII drive-path match that can't be a string: a C string literal ends in a NUL,
    and a real path is long. A short run (under 16 bytes) followed by another byte, or right after a
    byte that is not ASCII, is numbers that spell a path by chance, like the float table in 0.2.8's
    .rdata (3.4421 is the bytes 76 3A 5C 40) or the C runtime's double table "\\xe5Y:/(A6>\\0"."""
    if len(m.group(0)) >= 16:
        return False
    return (m.end() < len(data) and data[m.end()] != 0) or (m.start() > 0 and data[m.start() - 1] >= 0x80)


@functools.lru_cache(maxsize=None)
def library_roots(library):
    """The folders of the machine that built a static library (its first three levels, like
    C:\\Users\\runneradmin\\.cargo\\), as absolute paths in the library's own bytes show them."""
    roots = set()
    for m in ABSOLUTE.finditer(Path(library).read_bytes()):
        cuts = [i for i, c in enumerate(m.group(0)) if c in b'\\/']
        if len(cuts) >= 4:
            roots.add(m.group(0)[:cuts[3] + 1])
    return frozenset(roots)


def game_data_runs(data, elf, libraries=()):
    """The runs of the retail ELF in data (scan_retail.scan) that are neither the source's own
    (scan_retail.allowed) nor bytes of a static library linked in."""
    blobs = [Path(library).read_bytes() for library in libraries]
    return [(at, e, n) for at, e, n in scan_retail.scan(data, elf)
            if not scan_retail.allowed(elf, e, n) and not any(data[at:at + n] in b for b in blobs)]


def rust_lib_problems(rust_lib):
    """What makes the Rust renderer's library (dhd_render.lib, built by rustgs.py) unfit to link into a
    package. Its build-machine folders must be none of ours: library_roots(rust_lib) may hold only the
    Rust toolchain's own (rustup's std ships compiler_builtins with Rust CI's C:\\a\\rust\\rust\\...;
    check_rust_lib.toolchain_roots), never a folder of this machine (rustgs remaps CARGO_HOME and
    RUSTUP_HOME; an unremapped library names them). check_rust_lib.problems (rustgs's own refusals:
    paths, markers, the dynamic C runtime) must find nothing either."""
    import check_rust_lib  # noqa: E402  (imports this module back, lazily)
    if not Path(rust_lib).is_file():
        return ['%s: no such library' % rust_lib]
    sysroot = check_rust_lib.sysroot_lib(RUST_TRIPLE)
    toolchain = set(check_rust_lib.toolchain_roots(str(sysroot)))
    problems = ['%s: build-machine folder %r (not the Rust toolchain\'s): built without rustgs.py\'s path remap?'
                % (Path(rust_lib).name, root.decode('ascii', 'replace'))
                for root in sorted(library_roots(str(rust_lib)) - toolchain)]
    return problems + check_rust_lib.problems(str(rust_lib), sysroot=sysroot)


def exe_problems(exe, servers):
    """What makes exe unfit to publish (see the module's doc). The Rust library's paths are never
    exempt: none of them may reach the exe (rust_lib_problems)."""
    data = exe.read_bytes()
    problems = []
    for text, kind in ((data, 'ASCII'), (utf16_strings(data), 'UTF-16')):
        # drive paths: in the data sections only (see without_code); every other check reads it all
        scanned = without_code(text) if kind == 'ASCII' else text
        for m in ABSOLUTE.finditer(scanned):
            if kind == 'ASCII' and chance_bytes(scanned, m):
                continue
            problems.append('%s absolute path %r' % (kind, m.group(0)[:120].decode('ascii', 'replace')))
        for host in other_hosts(text):
            problems.append('%s server host %r' % (kind, host.decode('ascii', 'replace')))
        lowered = text.lower()
        for s in FORBIDDEN + sorted(servers):
            if s.lower() in lowered:
                problems.append('%s string %r' % (kind, s.decode('ascii', 'replace')))
    return [p for p in problems if not any(s in p for s in SDL_OWN_PATHS)]


def linked_rust_lib(exe):
    """The one dhd_render.lib build_host64.py linked into exe, from the link.rsp it wrote beside it."""
    rsp = exe.with_name('link.rsp')
    if not rsp.exists():
        sys.exit('%s: no link.rsp beside it (build_host64.py writes one); not packaged' % exe)
    entries = [Path(line.strip().strip('"')) for line in rsp.read_text(encoding='utf-8').splitlines() if line.strip()]
    gs = [e for e in entries if e.name.lower() in ('wgpu_native.lib', RUST_LIB)]
    if len(gs) != 1 or gs[0].name.lower() != RUST_LIB:
        sys.exit('%s: link.rsp names %s, not exactly one %s: not built on the Rust renderer'
                 % (exe, ', '.join(map(str, gs)) or 'no GS library', RUST_LIB))
    problems = rust_lib_problems(gs[0])
    if problems:
        sys.exit('%s is not fit to link into a package:\n  %s' % (gs[0], '\n  '.join(problems[:30])))
    return gs[0]


def imported_dlls(exe):
    """The DLL names in the executable's import table (llvm-readobj, LLVM on PATH)."""
    out = subprocess.run(['llvm-readobj', '--coff-imports', str(exe)], capture_output=True, text=True,
                         encoding='utf-8', errors='replace')
    if out.returncode:
        sys.exit('llvm-readobj failed on %s: %s' % (exe, out.stderr[-2000:]))
    return sorted(set(m.group(1) for m in re.finditer(r'Name:\s*(\S+\.dll)', out.stdout, re.I)))


# ---- the Rust crates' licences ----

# The licences a linked crate may be used under, in the order one is chosen from an "A OR B";
# the value names the crate's licence file (its name holds the word), when it has several.
LICENCES = {'MIT': 'mit', 'Zlib': 'zlib', 'ISC': 'isc', 'BSD-2-Clause': 'bsd', 'BSD-3-Clause': 'bsd',
            'Apache-2.0': 'apache', 'Unicode-3.0': 'unicode', 'CC0-1.0': 'cc0'}
LICENCE_FILE = re.compile(r'^(licen[cs]e|copying|copyright|notice|unlicense)', re.I)
MIT_TEMPLATE = """Copyright (c) %s

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."""


def linked_crates():
    """The crates linked into dhd_render.lib: cargo metadata (offline, --locked: Cargo.lock's
    versions) with the native feature for the x64 Windows target, walked from dhd_render through
    normal dependencies. Proc-macro crates run only inside the compiler: neither they nor what only
    they use reach the library."""
    r = subprocess.run(['cargo', 'metadata', '--offline', '--locked', '--format-version', '1', '--features', 'native',
                        '--filter-platform', RUST_TRIPLE], cwd=str(RENDER), capture_output=True, text=True,
                       encoding='utf-8', errors='replace')
    if r.returncode:
        sys.exit('cargo metadata failed in %s:\n%s' % (RENDER, r.stderr[-2000:]))
    meta = json.loads(r.stdout)
    packages = {p['id']: p for p in meta['packages']}
    nodes = {n['id']: n for n in meta['resolve']['nodes']}
    root = next(i for i, p in packages.items() if p['name'] == 'dhd_render')
    seen, todo = set(), [root]
    while todo:
        i = todo.pop()
        if i in seen or any('proc-macro' in t['kind'] for t in packages[i]['targets']):
            continue
        seen.add(i)
        todo += [d['pkg'] for d in nodes[i]['deps'] if any(k['kind'] is None for k in d['dep_kinds'])]
    seen.discard(root)
    return sorted((packages[i] for i in seen), key=lambda p: (p['name'], p['version']))


def crate_licence(package):
    """[(licence, text or None)] a crate is used under: one licence of each "AND" part (the first in
    LICENCES of its "OR" choices), with the crate's licence file for it. None: no file of its own
    (Apache-2.0: the text at the end; CC0-1.0: none needed; MIT: the MIT text with the authors)."""
    expression = (package.get('license') or '').replace('/', ' OR ')
    if not expression:
        sys.exit('Rust crate %s %s names no licence; not packaged' % (package['name'], package['version']))
    folder = Path(package['manifest_path']).parent
    files = sorted(f for f in folder.iterdir() if f.is_file() and LICENCE_FILE.match(f.name))
    out = []
    for part in re.split(r'\s+AND\s+', expression.replace('(', ' ').replace(')', ' ').strip()):
        choices = [c.strip() for c in re.split(r'\s+OR\s+', part)]
        chosen = next((l for l in LICENCES if l in choices), None)
        if not chosen:
            sys.exit('Rust crate %s %s: licence %r has none of %s; not packaged'
                     % (package['name'], package['version'], expression, ', '.join(LICENCES)))
        named = [f for f in files if LICENCES[chosen] in f.name.lower()]
        plain = [f for f in files if not re.search(r'mit|apache|zlib|bsd|isc|unicode|cc0', f.name, re.I)
                 and not f.name.lower().startswith(('notice', 'copyright'))]
        source = named[0] if named else plain[0] if plain and len(choices) == 1 else None
        text = source.read_text(encoding='utf-8', errors='replace').strip() if source else None
        if text is None and chosen == 'MIT':
            text = MIT_TEMPLATE % ', '.join(re.sub(r'\s*<[^>]*>', '', a) for a in package.get('authors') or
                                            ['the %s developers' % package['name']])
        if text is None and chosen not in ('Apache-2.0', 'CC0-1.0'):
            sys.exit('Rust crate %s %s: no %s licence file in %s; not packaged'
                     % (package['name'], package['version'], chosen, folder))
        if chosen == 'Apache-2.0':  # its NOTICE file travels with it (Apache-2.0 section 4d)
            notices = [f for f in files if f.name.lower().startswith('notice')]
            text = '\n\n'.join(f.read_text(encoding='utf-8', errors='replace').strip() for f in notices) or None
        out.append((chosen, text))
    return out


def indent(text):
    return '\n'.join(('  ' + line).rstrip() for line in text.replace('\r\n', '\n').split('\n'))


def rust_notices():
    """The notices of the Rust crates linked into dhd.exe (linked_crates), as text: a list, then each
    licence text once with the crates it covers, then the Apache License 2.0 text (also the WABT
    runtime's, THIRD-PARTY-NOTICES.txt)."""
    crates = linked_crates()
    rows, texts, apache = [], {}, None
    for p in crates:
        used = crate_licence(p)
        rows.append('  %s %s  %s%s  %s' % (p['name'], p['version'], p.get('license'),
                                           '' if len(used) == 1 and used[0][0] == p.get('license') else
                                           ' (used under %s)' % ' AND '.join(l for l, _ in used),
                                           p.get('repository') or ''))
        for licence, text in used:
            if text:
                key = ' '.join(text.split())
                texts.setdefault(key, (licence, text, []))[2].append('%s %s' % (p['name'], p['version']))
        folder = Path(p['manifest_path']).parent
        if apache is None:
            apache = next((f.read_text(encoding='utf-8', errors='replace').strip() for f in sorted(folder.iterdir())
                           if LICENCE_FILE.match(f.name) and 'apache' in f.name.lower()
                           and 'Apache License' in f.read_text(encoding='utf-8', errors='replace')[:400]), None)
    if apache is None:
        sys.exit('no Apache License 2.0 text among the Rust crates\' licence files; not packaged')
    out = ['Rust crates linked into dhd.exe',
           '===============================',
           '',
           'The graphics renderer (port/render, part of this project, GPL-3.0-or-later) is written in Rust and',
           'linked into dhd.exe with these crates (versions from port/render/Cargo.lock). A crate offered under',
           'several licences is used under the first of MIT, Zlib, ISC, BSD, Apache-2.0. Their licence texts',
           'follow the list; the Apache License 2.0 text is at the end.',
           ''] + rows
    for licence, text, names in texts.values():
        out += ['', '%s: %s' % (licence, ', '.join(names)), '', indent(text)]
    out += ['', 'Apache License 2.0 (the crates above used under it, and the WABT runtime)', '', indent(apache)]
    return '\n'.join(out) + '\n'


# ---- building and packaging ----

def crlf(data):
    if isinstance(data, Path):
        data = data.read_bytes()
    return data.replace(b'\r\n', b'\n').replace(b'\n', b'\r\n')


def source_changes():
    """Uncommitted changes to tracked files under port/ and src/ (git status lines)."""
    r = subprocess.run(['git', 'status', '--porcelain', '--untracked-files=no', '--', 'port', 'src'], cwd=str(ROOT),
                       capture_output=True, text=True)
    if r.returncode:
        sys.exit('git status failed: %s' % r.stderr.strip())
    return r.stdout.splitlines()


def build_exe(wasm, sdl_lib, sdl_include, jobs):
    """dhd64.exe from the module, by build_host64.py on the Rust renderer, under WORK."""
    cmd = [sys.executable, str(ROOT / 'port' / 'tools' / 'build_host64.py'), str(wasm),
           '--num-outputs', '8', '--sdl-lib', str(sdl_lib), '--sdl-include', str(sdl_include),
           '--out', str(WORK), '--jobs', str(jobs)]
    print('building: ' + ' '.join(cmd), flush=True)
    result = subprocess.run(cmd, capture_output=True, text=True, encoding='utf-8', errors='replace')
    if result.returncode:
        sys.exit('build_host64.py failed:\n' + (result.stdout + result.stderr)[-4000:])
    print(result.stdout.strip().splitlines()[-1] if result.stdout.strip() else 'built')
    return WORK / 'dhd64.exe'


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    source = ap.add_mutually_exclusive_group()
    source.add_argument('--wasm', type=Path, default=ROOT / 'build' / 'wasm' / 'dhd_core.wasm',
                        help='the dhd_core.wasm to build dhd.exe from (default build/wasm/dhd_core.wasm)')
    source.add_argument('--exe', type=Path, help="a ready build_host64.py dhd64.exe (its link.rsp beside it)")
    ap.add_argument('--sdl-lib', type=Path, help='x64 SDL3 static library (default: sdl3.py\'s x64 build)')
    ap.add_argument('--sdl-include', type=Path, help='its include directory (with --sdl-lib)')
    ap.add_argument('--jobs', type=int, default=2)
    ap.add_argument('--version', help="must equal port/include/version.h's DHD_VERSION (the default)")
    ap.add_argument('--out', type=Path, help='default build/release/DownhillRevived-<version>-win.zip')
    ap.add_argument('--base-url', help="latest.json's zip address is this + the zip's name (default: the GitHub "
                    "release asset of tag v<version>), for a self-hosted update source")
    ap.add_argument('--settings', type=Path, help="a dhd.ini whose server line must not be in the package")
    ap.add_argument('--elf', type=Path, default=ROOT / 'iso' / 'SCUS_971.77', help='your retail ELF, for the game-data scan')
    a = ap.parse_args()
    version = header_version()
    if a.version and a.version != version:
        sys.exit('--version %s, but port/include/version.h says %s (the exe reports that one); bump the header '
                 'and build again' % (a.version, version))
    a.version = version
    if not re.fullmatch(r'[0-9]+\.[0-9]+\.[0-9]+', a.version):
        sys.exit('version %r: MAJOR.MINOR.PATCH digits only (the updater and the server compare them)' % a.version)
    name = 'DownhillRevived-%s' % a.version
    out = (a.out or ROOT / 'build' / 'release' / (name + '-win.zip')).resolve()
    if ROOT / 'build' not in out.parents or out.drive.upper() == 'C:':
        sys.exit('the zip goes under build/ (never tracked, never on C:)')
    changes = source_changes()
    if changes:
        sys.exit('port/ or src/ has uncommitted changes; a release is built from committed source:\n  '
                 + '\n  '.join(changes[:20]))
    if not a.elf.exists():
        sys.exit('%s: the retail ELF is needed to check that no game data is packaged' % a.elf)
    notices = rust_notices()  # before the build: a crate with an unusable licence stops it early

    if a.exe:
        exe = a.exe.resolve()
        if not exe.exists():
            sys.exit('%s: no such file' % exe)
    else:
        wasm = a.wasm.resolve()
        if not wasm.exists():
            sys.exit('%s: no such module (python port/tools/build_wasm.py writes it)' % wasm)
        print('module: %s (%d bytes, sha256 %s)' % (wasm, wasm.stat().st_size, hashlib.sha256(wasm.read_bytes()).hexdigest()))
        if a.sdl_lib:
            if not a.sdl_include:
                sys.exit('--sdl-lib needs --sdl-include')
            sdl_lib, sdl_include = a.sdl_lib, a.sdl_include
        else:
            import sdl3  # noqa: E402
            sdl_lib, sdl_include = sdl3.build(sdl3.X64_TARGET)
        exe = build_exe(wasm, Path(sdl_lib).resolve(), Path(sdl_include).resolve(), a.jobs)
    data = exe.read_bytes()
    pe = int.from_bytes(data[0x3C:0x40], 'little') if data[:2] == b'MZ' else -1
    if pe < 0 or data[pe:pe + 4] != b'PE\0\0' or data[pe + 4:pe + 6] != b'\x64\x86':  # IMAGE_FILE_MACHINE_AMD64
        sys.exit('%s is not an x64 (AMD64) executable' % exe)
    rust_lib = linked_rust_lib(exe)
    if b'dhd-port %s ' % a.version.encode() not in data:
        sys.exit('%s was not built with version %s (port/include/version.h); build it again' % (exe, a.version))
    problems = exe_problems(exe, server_strings(a.settings))
    if problems:
        sys.exit('dhd.exe is not fit to publish:\n  ' + '\n  '.join(problems[:30]))
    dlls = imported_dlls(exe)
    foreign = [d for d in dlls if not SYSTEM_DLL.match(d)]
    if foreign:
        sys.exit('dhd.exe imports DLLs that are not Windows\' own: %s' % ', '.join(foreign))

    files = {
        'dhd.exe': data,
        'README.txt': crlf(RELEASE / 'README.txt'),
        'LICENSE.txt': crlf(ROOT / 'LICENSE'),
        'THIRD-PARTY-NOTICES.txt': crlf((RELEASE / 'THIRD-PARTY-NOTICES.txt').read_bytes().rstrip() + b'\n\n'
                                        + notices.encode('utf-8')),
        'dxcompiler.dll': dxc.dll('x64').read_bytes(),
    }
    # Third-party binaries: the publisher's exact bytes (see the module's doc).
    third_party = {'dxcompiler.dll': dxc.DLL_SHA256['x64']}
    elf = a.elf.read_bytes()
    for file_name, content in files.items():
        if GAME_DATA.search(file_name):
            sys.exit('%s looks like game data; not packaged' % file_name)
        if file_name in third_party:
            if hashlib.sha256(content).hexdigest() != third_party[file_name]:
                sys.exit('%s is not the pinned third-party file; not packaged' % file_name)
            continue
        runs = game_data_runs(content, elf, [rust_lib] if file_name == 'dhd.exe' else [])
        if runs:
            sys.exit('%s holds game data (runs of the retail ELF):\n  %s' % (file_name, '\n  '.join(
                '+0x%X: %d bytes = %s' % (at, n, scan_retail.elf_place(elf, e)) for at, e, n in runs[:20])))
        # the text files may show the official server and example.org addresses, nothing else
        hosts = set(h.lower().rstrip(b'.') for h in re.findall(rb'wss?://([A-Za-z0-9.:-]+)', content)) - {
            b'races.example.org', OFFICIAL_HOST}
        if file_name != 'dhd.exe' and (hosts or other_hosts(content) or any(
                s.lower() in content.lower() for s in FORBIDDEN + sorted(server_strings(a.settings)))):
            sys.exit('%s names a server or a local path; not packaged' % file_name)

    folder = out.with_name(name)  # the same files unzipped, to try before publishing
    if folder.exists():
        shutil.rmtree(folder)
    folder.mkdir(parents=True)
    with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as z:
        for file_name, content in files.items():
            (folder / file_name).write_bytes(content)
            info_entry = zipfile.ZipInfo('%s/%s' % (name, file_name), date_time=(2026, 1, 1, 0, 0, 0))
            info_entry.compress_type = zipfile.ZIP_DEFLATED
            info_entry.external_attr = 0o644 << 16
            z.writestr(info_entry, content)
    zip_hash = hashlib.sha256(out.read_bytes()).hexdigest()
    # The updater's manifest (port/hle/update.c): published beside the zip as a release asset.
    zip_url = (a.base_url.rstrip('/') + '/' + out.name if a.base_url
               else '%s/download/v%s/%s' % (RELEASES, a.version, out.name))
    manifest_path = out.with_name('latest.json')
    manifest_path.write_text(json.dumps({
        'name': 'Downhill Revived', 'version': a.version, 'url': zip_url, 'sha256': zip_hash,
        'size': out.stat().st_size, 'page': '%s/tag/v%s' % (RELEASES, a.version)}, indent=1) + '\n', encoding='utf-8')
    commit = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=str(ROOT), capture_output=True, text=True).stdout.strip()
    print('%s (%d KB)' % (out, out.stat().st_size // 1024))
    print('  zip sha256 %s; unzipped in %s' % (zip_hash, folder))
    print('  %s: version %s, %s' % (manifest_path, a.version, zip_url))
    print('  dhd.exe (x64, Rust renderer %s) sha256 %s, source %s' % (rust_lib, hashlib.sha256(data).hexdigest(), commit[:9]))
    print('  imports (all Windows DLLs): %s' % ', '.join(dlls))
    return 0


if __name__ == '__main__':
    sys.exit(main())
