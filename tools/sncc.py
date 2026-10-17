#!/usr/bin/env python3
"""Build one C/C++ translation unit with the matching toolchain.

  SN ee-gcc2953 (cc1 2.95.3 SN BUILD 1.36) -S -> expand INCLUDE_ASM -> SN ps2eeas -> .o

INCLUDE_ASM (include/include_asm.h) emits `.include "asm/nonmatchings/.../f.s"` into the
compiler's assembly. ps2eeas can't read splat's syntax, so each such include is replaced here
by the same function converted with tools/asm_sn.py; `.include "include/labels.inc"` (macros
ps2eeas crashes on) is dropped.
No instruction the compiler generated is altered.

Usage: sncc.py SRC OUT.o [EXTRA FLAGS...]   (writes OUT.o.d for ninja, OUT.s for inspection)
C uses toolchain.CFLAGS, C++ toolchain.CXXFLAGS; extra flags (from config/units.txt) follow.
"""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import toolchain
from asm_sn import convert

INC = re.compile(r'^\s*\.include\s+"([^"]+)"\s*$')
INCLUDE_DIRS = ['include', 'src']


def expand(asm, deps):
    # Note: cc1 writes `.extern sym, size` at the end of the file, and that is left alone.
    # ps2eeas expands `lw $2,sym` for a symbol of unknown size to lui+lw, except in a delay
    # slot (.set nomacro) where it must use one $gp-relative insn. Retail shows exactly this
    # (finding 004), so a $gp access outside a delay slot means the variable was defined
    # earlier in the same translation unit.
    out = []
    for line in asm.split('\n'):
        m = INC.match(line)
        if not m:
            out.append(line)
            continue
        path = m.group(1)
        if path.endswith('labels.inc') or path.endswith('macro.inc'):
            continue
        full = os.path.join(ROOT, path)
        deps.append(path)
        with open(full) as f:
            out.append(convert(f.read()))
    return '\n'.join(out)


SCE29 = 'ee-gcc2.9-991111'  # tools/compilers/<this>: a Linux i386 build, run in WSL


def wsl_path(p):
    p = os.path.abspath(p).replace('\\', '/')
    return '/mnt/%s%s' % (p[0].lower(), p[2:])


def sce29(src, raw, inc, extra):
    """C -> assembly with SCE ee-gcc 2.9 (-O2 -G8, no SN -fopt-stack), via WSL. The compiler is
    copied once to /opt/eegcc (it can't run from the Windows drive)."""
    c = wsl_path(os.path.join(ROOT, 'tools', 'compilers', SCE29))
    w = '/opt/eegcc/' + SCE29
    # the 32-bit compiler can't stat files on /mnt (EOVERFLOW): work on a Linux-side copy
    cmd = ("set -e; [ -d {w} ] || {{ mkdir -p /opt/eegcc && cp -r '{c}' /opt/eegcc/; }}; "
           "T=$(mktemp -d /tmp/sce29.XXXXXX); cp -r '{root}/include' $T/include; cp '{src}' $T/in.c; "
           "B=$(dirname $(find {w} -name cc1 -type f | head -1))/; cd $T; "
           "{w}/bin/ee-gcc -B $B -x c -O2 -G8 {extra} -I include -S in.c -o out.s; "
           "cp out.s '{raw}'; rm -rf $T").format(
        w=w, c=c, root=wsl_path(ROOT), extra=' '.join(extra), src=wsl_path(src), raw=wsl_path(raw))
    if os.name != 'nt':  # Linux (cloud): run the i386 compiler directly, no WSL
        c = os.path.join(ROOT, 'tools', 'compilers', SCE29)
        cmd = ("set -e; T=$(mktemp -d /tmp/sce29.XXXXXX); cp -r '{root}/include' $T/include; "
               "cp '{src}' $T/in.c; B=$(dirname $(find '{c}' -name cc1 -type f | head -1))/; cd $T; "
               "'{c}/bin/ee-gcc' -B $B -x c -O2 -G8 {extra} -I include -S in.c -o out.s; "
               "cp out.s '{raw}'; rm -rf $T").format(
            c=c, root=ROOT, extra=' '.join(extra), src=os.path.abspath(src), raw=os.path.abspath(raw))
        p = subprocess.run(['bash', '-c', cmd], capture_output=True, text=True)
        if p.returncode:
            sys.stderr.write(p.stdout + p.stderr)
        return p.returncode == 0
    sh = raw + '.sh'  # wsl.exe re-splits a `bash -c` argument, so the command goes in a script
    with open(sh, 'w', newline='\n') as f:
        f.write(cmd + '\n')
    p = subprocess.run(['wsl', '-d', 'Ubuntu', '--', 'bash', wsl_path(sh)], capture_output=True, text=True,
                       env=dict(os.environ, MSYS_NO_PATHCONV='1'))
    if p.returncode:
        sys.stderr.write(p.stdout + p.stderr)
    return p.returncode == 0


def main():
    src, out = sys.argv[1:3]
    extra = sys.argv[3:]
    toolchain.check()
    os.makedirs(os.path.dirname(out) or '.', exist_ok=True)
    s = out[:-2] + '.s'
    raw = out[:-2] + '.cc1.s'
    lang = 'c' if src.endswith('.c') else 'c++'
    inc = [x for d in INCLUDE_DIRS for x in ('-I', d)]
    dep = out + '.cpp.d'
    if '-sce29' in extra:  # a file built with SCE's own ee-gcc 2.9 (finding 010); still SN ps2eeas
        if not sce29(src, raw, inc, [x for x in extra if x != '-sce29']):
            sys.exit(1)
    elif not toolchain.run([toolchain.CC, '-x', lang, '-S', src, '-o', raw, '-Wp,-MD,' + dep]
                           + inc + (toolchain.CFLAGS if lang == 'c' else toolchain.CXXFLAGS) + extra):
        sys.exit(1)
    deps = []
    with open(raw) as f:
        asm = expand(f.read(), deps)
    with open(s, 'w', newline='\n') as f:
        f.write(asm)
    g = [x for x in toolchain.CFLAGS if x.startswith('-G')]
    if not toolchain.run([toolchain.PS2EEAS] + g + ['-o', out, s]):
        sys.exit(1)
    # ninja depfile: the preprocessor's deps plus every included asm file
    pre = open(dep).read().replace('\\\n', ' ').split(':', 1)[1].split() if os.path.exists(dep) else []
    with open(out + '.d', 'w', newline='\n') as f:
        f.write('%s: %s\n' % (out, ' '.join(p.replace('\\', '/') for p in pre + deps)))


if __name__ == '__main__':
    main()
