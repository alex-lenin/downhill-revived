"""The public release zip: build/release/DownhillRevived-<version>-win.zip.

    python port/tools/build_clean.py
    python port/tools/package_release.py --exe build/port/clean/<rev>/dhd.exe

The version is port/include/version.h's DHD_VERSION, the one the exe was built with (--version,
if given, must be the same). Beside the zip goes latest.json, the update manifest the game's
updater reads (port/hle/update.c, finding 054): {"version", "url" (the zip as a GitHub release
asset of tag v<version>, or --base-url + the zip's name), "sha256" (the zip's), "size", "page"}.
Publish both as assets of the GitHub release tagged v<version>.

The zip holds one folder, DownhillRevived-<version>, with:
  dhd.exe                  a plain build_clean.py executable (VU1 AOT on, no local object basis)
  README.txt               how to play (port/release/README.txt)
  LICENSE.txt              the GPL-3.0 text (LICENSE)
  THIRD-PARTY-NOTICES.txt  port/release/THIRD-PARTY-NOTICES.txt
dhd.exe needs only Windows' own DLLs (the C runtime is linked in), so nothing else is shipped.

The zip is refused if dhd.exe:
  - isn't what build_clean.py wrote (manifest.json beside it: same hash, VU1 AOT, no --recomp-basis);
  - holds an absolute path (a builder's folder, C:\\Users\\..., the repository's own path), in
    ASCII or UTF-16 (a drive path "X:\\..." in ASCII: in the data sections, where strings are; in
    machine code such bytes are an instruction's operand);
  - holds a server address other than the official one (dhd-online.downhill-revived.workers.dev,
    the built-in default): another workers.dev host, DHD_NET_SERVER when set, or the server line of a
    dhd.ini named by --settings;
or if a packaged file name looks like game data, or any packaged file holds a run of 64 bytes or
more of the retail ELF (port/tools/scan_retail.py) other than the string literals and tables the
decompiled source itself holds (scan_retail.SOURCE). The game's data comes from the player's disc
(port/hle/game_data.c).
"""
import argparse
import hashlib
import json
import os
import re
import struct
import sys
import zipfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import scan_retail  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
RELEASE = ROOT / 'port' / 'release'
VERSION_H = ROOT / 'port' / 'include' / 'version.h'
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
OFFICIAL_HOST = b'dhd-online.downhill-revived.workers.dev'
# any workers.dev host (other_hosts allows only the official one)
WORKERS_HOST = re.compile(rb'[A-Za-z0-9.-]*workers\.dev', re.I)
# user folders, and the builder's own account name and checkout folders
FORBIDDEN = [b'\\Users\\', b'/Users/'] + [
    s.encode() for s in {os.environ.get('USERNAME', ''), os.environ.get('USER', '')} if len(s) > 2] + [
    p.encode() for folder in (ROOT, ROOT.parent) if len(folder.parts) > 1
    for p in (str(folder), str(folder).replace('\\', '/'), str(folder)[2:], str(folder)[2:].replace('\\', '/'))]


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


def exe_problems(exe, servers):
    data = exe.read_bytes()
    problems = []
    for text, kind in ((data, 'ASCII'), (utf16_strings(data), 'UTF-16')):
        # drive paths: in the data sections only (see without_code); every other check reads it all
        for m in ABSOLUTE.finditer(without_code(text) if kind == 'ASCII' else text):
            problems.append('%s absolute path %r' % (kind, m.group(0)[:120].decode('ascii', 'replace')))
        for host in other_hosts(text):
            problems.append('%s server host %r' % (kind, host.decode('ascii', 'replace')))
        lowered = text.lower()
        for s in FORBIDDEN + sorted(servers):
            if s.lower() in lowered:
                problems.append('%s string %r' % (kind, s.decode('ascii', 'replace')))
    return problems


def crlf(path):
    return path.read_bytes().replace(b'\r\n', b'\n').replace(b'\n', b'\r\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--exe', type=Path, required=True, help='build/port/clean/<rev>/dhd.exe (manifest.json beside it)')
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
    if ROOT / 'build' not in out.parents:
        sys.exit('the zip goes under build/ (never tracked)')

    exe = a.exe.resolve()
    manifest = exe.with_name('manifest.json')
    info = json.loads(manifest.read_text()) if manifest.exists() else {}
    digest = hashlib.sha256(exe.read_bytes()).hexdigest()
    if not info.get('vu_aot') or info.get('exe_sha256') != digest:
        sys.exit('%s: not a build_clean.py executable with VU1 AOT (see %s); not packaged' % (exe, manifest.name))
    if info.get('recomp_basis'):
        sys.exit('%s was linked with a local object basis (%s); a release is built without one' % (exe, info['recomp_basis']))
    if b'dhd-port %s ' % a.version.encode() not in exe.read_bytes():
        sys.exit('%s was not built with version %s (port/include/version.h); build it again' % (exe, a.version))
    servers = server_strings(a.settings)
    problems = exe_problems(exe, servers)
    if problems:
        sys.exit('dhd.exe is not fit to publish:\n  ' + '\n  '.join(problems[:30]))

    files = {
        'dhd.exe': exe.read_bytes(),
        'README.txt': crlf(RELEASE / 'README.txt'),
        'LICENSE.txt': crlf(ROOT / 'LICENSE'),
        'THIRD-PARTY-NOTICES.txt': crlf(RELEASE / 'THIRD-PARTY-NOTICES.txt'),
    }
    if not a.elf.exists():
        sys.exit('%s: the retail ELF is needed to check that no game data is packaged' % a.elf)
    elf = a.elf.read_bytes()
    for file_name, data in files.items():
        if GAME_DATA.search(file_name):
            sys.exit('%s looks like game data; not packaged' % file_name)
        runs = [(at, e, n) for at, e, n in scan_retail.scan(data, elf) if not scan_retail.allowed(elf, e, n)]
        if runs:
            sys.exit('%s holds game data (runs of the retail ELF):\n  %s' % (file_name, '\n  '.join(
                '+0x%X: %d bytes = %s' % (at, n, scan_retail.elf_place(elf, e)) for at, e, n in runs[:20])))
        # the text files may show the official server and example.org addresses, nothing else
        hosts = set(h.lower().rstrip(b'.') for h in re.findall(rb'wss?://([A-Za-z0-9.:-]+)', data)) - {
            b'races.example.org', OFFICIAL_HOST}
        if file_name != 'dhd.exe' and (hosts or other_hosts(data) or any(s.lower() in data.lower() for s in FORBIDDEN + sorted(servers))):
            sys.exit('%s names a server or a local path; not packaged' % file_name)
    out.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as z:
        for file_name, data in files.items():
            info_entry = zipfile.ZipInfo('%s/%s' % (name, file_name), date_time=(2026, 1, 1, 0, 0, 0))
            info_entry.compress_type = zipfile.ZIP_DEFLATED
            info_entry.external_attr = 0o644 << 16
            z.writestr(info_entry, data)
    zip_hash = hashlib.sha256(out.read_bytes()).hexdigest()
    # The updater's manifest (port/hle/update.c): published beside the zip as a release asset.
    zip_url = (a.base_url.rstrip('/') + '/' + out.name if a.base_url
               else '%s/download/v%s/%s' % (RELEASES, a.version, out.name))
    manifest_path = out.with_name('latest.json')
    manifest_path.write_text(json.dumps({
        'name': 'Downhill Revived', 'version': a.version, 'url': zip_url, 'sha256': zip_hash,
        'size': out.stat().st_size, 'page': '%s/tag/v%s' % (RELEASES, a.version)}, indent=1) + '\n', encoding='utf-8')
    print('%s (%d KB)' % (out, out.stat().st_size // 1024))
    print('  zip sha256 %s' % zip_hash)
    print('  %s: version %s, %s' % (manifest_path, a.version, zip_url))
    print('  dhd.exe sha256 %s, from %s (built %s)' % (digest, info.get('commit', '?')[:8], info.get('built', '?')))
    return 0


if __name__ == '__main__':
    sys.exit(main())
