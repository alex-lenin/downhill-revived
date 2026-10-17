#!/usr/bin/env python3
"""Install the pinned toolchain into tools/compilers/ (never committed).

toolchain.lock.json lists components; every downloaded archive and every installed file is
checked against its SHA-256. A mismatch is an error, never silently accepted.

  github-mirror  files from a git repo at a pinned commit (SN ProDG 3.01: the matching
                 compiler cc1 2.95.3 SN BUILD 1.36, ps2eeas, period binutils)
  archive        members extracted from a hashed .tar.gz / .tar.zst (ps2dev binutils 2.45
                 for assembling/linking splat asm; MSYS2 i686 runtime DLLs they need)
  file           one hashed binary (objdiff-cli, for progress reports)

Usage: setup_toolchain.py            download what's missing, verify everything
       setup_toolchain.py --verify   verify only (no network)
       setup_toolchain.py --record   (maintainers) (re)record hashes of the mirror files
"""
import hashlib, json, os, subprocess, sys, tarfile, urllib.request

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOCK = os.path.join(ROOT, 'toolchain.lock.json')
DL = os.path.join(ROOT, 'tools', 'compilers', 'dl')
# Reads .tar.zst: Windows' bsdtar, elsewhere GNU tar (needs the zstd program installed).
BSDTAR = (os.path.join(os.environ.get('SystemRoot', r'C:\Windows'), 'System32', 'tar.exe')
          if os.name == 'nt' else 'tar')


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()


def download(url, out):
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with urllib.request.urlopen(url) as r, open(out + '.part', 'wb') as f:
        while True:
            b = r.read(1 << 20)
            if not b:
                break
            f.write(b)
    os.replace(out + '.part', out)


def mirror(c, verify_only, record):
    dest = lambda p: os.path.join(ROOT, c['install_dir'], *p.split('/'))
    if record:
        import re
        tree = json.loads(subprocess.check_output(
            ['gh', 'api', 'repos/%s/git/trees/%s?recursive=1' % (c['repo'], c['commit'])]))
        assert not tree['truncated']
        sel = re.compile(c['select'])
        c['files'] = {}
        for t in sorted(tree['tree'], key=lambda t: t['path']):
            if t['type'] == 'blob' and sel.match(t['path']):
                p = t['path']
                if not os.path.exists(dest(p)):
                    download(url(c, p), dest(p))
                c['files'][p] = sha256(dest(p))
        return 0
    bad = 0
    for p, want in c['files'].items():
        out = dest(p)
        if not verify_only and (not os.path.exists(out) or sha256(out) != want):
            download(url(c, p), out)
        if not os.path.exists(out) or sha256(out) != want:
            print('BAD', p); bad += 1
    return bad


def url(c, p):
    return 'https://raw.githubusercontent.com/%s/%s/%s' % (c['repo'], c['commit'], urllib.request.quote(p))


def archive(c, verify_only):
    arc = os.path.join(DL, c['filename'])
    bad = 0
    files = c['files']
    have = all(os.path.exists(os.path.join(ROOT, c['install_dir'], os.path.basename(m))) and
               sha256(os.path.join(ROOT, c['install_dir'], os.path.basename(m))) == h
               for m, h in files.items())
    if not have and not verify_only:
        if not os.path.exists(arc) or sha256(arc) != c['sha256']:
            print('downloading', c['url'])
            download(c['url'], arc)
        if sha256(arc) != c['sha256']:
            print('BAD archive hash', c['filename']); return 1
        os.makedirs(os.path.join(ROOT, c['install_dir']), exist_ok=True)
        for m in files:
            out = os.path.join(ROOT, c['install_dir'], os.path.basename(m))
            if arc.endswith('.zst'):  # Windows' bsdtar reads zstd; Python's tarfile can't
                data = subprocess.check_output([BSDTAR, '-xOf', arc, m])
            else:
                with tarfile.open(arc) as t:
                    data = t.extractfile(m).read()
            with open(out, 'wb') as f:
                f.write(data)
    for m, want in files.items():
        out = os.path.join(ROOT, c['install_dir'], os.path.basename(m))
        if not os.path.exists(out) or sha256(out) != want:
            print('BAD', out); bad += 1
    return bad


def single_file(c, verify_only):
    """kind "file": one downloaded binary (e.g. objdiff-cli), pinned by hash."""
    out = os.path.join(ROOT, c['install_dir'], c['filename'])
    if not verify_only and (not os.path.exists(out) or sha256(out) != c['sha256']):
        print('downloading', c['url'])
        download(c['url'], out)
    if not os.path.exists(out) or sha256(out) != c['sha256']:
        print('BAD', out)
        return 1
    return 0


def main():
    lock = json.load(open(LOCK))
    verify_only, record = '--verify' in sys.argv, '--record' in sys.argv
    bad = n = 0
    for c in lock['components']:
        if c['kind'] == 'github-mirror':
            bad += mirror(c, verify_only, record)
        elif c['kind'] == 'file':
            bad += 0 if record else single_file(c, verify_only)
        elif not record:
            bad += archive(c, verify_only)
        n += len(c.get('files', {})) or 1
    if record:
        with open(LOCK, 'w', newline='\n') as f:
            json.dump(lock, f, indent=2)
            f.write('\n')
    print('%d files in %d components, %d problems' % (n, len(lock['components']), bad))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
