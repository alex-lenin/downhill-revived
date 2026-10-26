"""DXC (dxcompiler.dll) for the renderer's DX12 shaders (finding 084 §14): download, SHA-256
check, unpack.

    python port/tools/dxc.py            prints the x64 DLL (for dhd64.exe)

The Rust renderer (wgpu; gpu_rust.c's device_init hands it the path) compiles its shaders with DXC
when dxcompiler.dll is next to the executable (ten times faster than FXC on a cold start), so the
x64 package ships it. The release archive of microsoft/DirectXShaderCompiler is pinned by version
and SHA-256 (nothing else is accepted) and kept out of the tree: under %DHD_DXC_CACHE% if set, else DHD_DEVCACHES/dxc if that
exists, else build/downloads/dxc. Only bin/x64/dxcompiler.dll and the licence texts are
unpacked, to <cache>/<VERSION>/. dxcompiler.dll is LLVM's University of Illinois/NCSA licence with
MIT parts (port/release/THIRD-PARTY-NOTICES.txt); dxil.dll (Microsoft's licence) is not needed by
wgpu and not shipped."""
import hashlib
import os
import shutil
import sys
import urllib.request
import zipfile
from pathlib import Path
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches

VERSION = 'v1.9.2609'
ARCHIVE = 'dxc_2026_09_29.zip'
URL = 'https://github.com/microsoft/DirectXShaderCompiler/releases/download/%s/%s' % (VERSION, ARCHIVE)
SHA256 = 'ad31b1fc8443175d204f77a611fdb3ef2ec42759bdc2f1167368de24a4a7e7f1'
ARCHES = {'x64': 'bin/x64/dxcompiler.dll'}
# The DLL itself: the package ships only these bytes (package_release.py checks them).
DLL_SHA256 = {'x64': '0689e2fee0a1c9df8662a59680469f24e2a3ad7ce6bbbcc9284f7a83172bfc59'}
LICENCES = ['LICENSE-LLVM.txt', 'LICENCE-MIT.txt']

ROOT = Path(__file__).resolve().parents[2]


def cache():
    if os.environ.get('DHD_DXC_CACHE'):
        return Path(os.environ['DHD_DXC_CACHE'])
    if (DEVCACHES / 'dxc').is_dir():
        return (DEVCACHES / 'dxc')
    return ROOT / 'build' / 'downloads' / 'dxc'


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            h.update(block)
    return h.hexdigest()


def fetch():
    """The unpacked directory (bin/x64, the licences), downloading the archive once."""
    base = cache() / VERSION
    out = base / 'unpacked'
    stamp = out / 'sha256.txt'
    if stamp.exists() and stamp.read_text().strip() == SHA256:
        return out
    archive = base / ARCHIVE
    if not archive.exists() or sha256(archive) != SHA256:
        archive.parent.mkdir(parents=True, exist_ok=True)
        part = archive.with_suffix('.part')
        print('dxc: downloading', URL)
        with urllib.request.urlopen(URL, timeout=300) as r, open(part, 'wb') as f:
            shutil.copyfileobj(r, f)
        got = sha256(part)
        if got != SHA256:
            part.unlink()
            sys.exit('dxc: %s has SHA-256 %s, expected %s' % (URL, got, SHA256))
        os.replace(part, archive)
    if out.exists():
        shutil.rmtree(out)
    with zipfile.ZipFile(archive) as z:
        for member in z.infolist():   # the archive's names use backslashes
            name = member.filename.replace('\\', '/')
            if name in ARCHES.values() or name in LICENCES:
                target = out / name
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(z.read(member))
    missing = [n for n in list(ARCHES.values()) + LICENCES if not (out / n).exists()]
    if missing:
        sys.exit('dxc: %s lacks %s' % (ARCHIVE, ', '.join(missing)))
    for arch, name in ARCHES.items():
        if sha256(out / name) != DLL_SHA256[arch]:
            sys.exit('dxc: %s in %s is not the pinned file' % (name, ARCHIVE))
    stamp.write_text(SHA256 + '\n')
    return out


def dll(arch):
    """dxcompiler.dll for 'x64' (dhd64.exe), checked against DLL_SHA256."""
    path = fetch() / ARCHES[arch]
    if sha256(path) != DLL_SHA256[arch]:
        sys.exit('dxc: %s is not the pinned file (SHA-256 %s); delete %s' % (path, DLL_SHA256[arch], path.parent))
    return path


if __name__ == '__main__':
    for arch in ARCHES:
        print(arch, dll(arch))
