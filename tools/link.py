#!/usr/bin/env python3
"""Link the image: write C++ name aliases, then run ld.

A C++ unit defines and calls mangled names (`func_0010A6E8__1A`, `func_0010A468__1B`) while
splat's asm uses the plain placeholder (`func_0010A6E8`) until config/symbol_addrs.txt is
updated. For every symbol in a C object whose name contains func_XXXXXXXX this writes
  PROVIDE(func_XXXXXXXX = mangled);   if the object defines it (asm callers find it)
  PROVIDE(mangled = func_XXXXXXXX);   if the object only references it (it calls asm)
PROVIDE only takes effect for symbols that are otherwise undefined, so this never overrides
a real definition.

Objects are passed to nm/ld through response files (@file): hundreds of object paths would
overflow Windows' 32K command-line limit.

Usage: link.py SCRIPT MAP OUT (OBJ | @RSPFILE)...   (SCRIPT INCLUDEs <dir of SCRIPT>/aliases.ld)
"""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from toolchain import exe
BU = os.path.join(ROOT, 'tools', 'compilers', 'ps2dev-binutils', 'mips64r5900el-ps2-elf-')
FUNC = re.compile(r'func_([0-9A-Fa-f]{8})')


def write_list(path, items):
    with open(path, 'w', newline='\n') as f:
        f.write('\n'.join(items) + '\n')


def main():
    script, mapf, out = sys.argv[1:4]
    objs = []
    for a in sys.argv[4:]:  # @file: a response file listing objects (see configure.py)
        objs += open(a[1:]).read().split() if a.startswith('@') else [a]
    rsp = out + '.objs'
    write_list(rsp, objs)
    c_objs = [o for o in objs if '/asm/' not in o.replace('\\', '/')]
    lines = []
    if c_objs:
        crsp = out + '.cobjs'
        write_list(crsp, c_objs)
        nm = subprocess.run(exe(BU + 'nm.exe') + ['-P', '@' + crsp], capture_output=True, text=True,
                            stdin=subprocess.DEVNULL)
        if nm.returncode:
            sys.exit(nm.stderr)
        defined, undefined = set(), set()
        for l in nm.stdout.split('\n'):
            f = l.split()
            if len(f) < 2 or f[0].endswith(':'):
                continue
            name, typ = f[0], f[1]
            m = FUNC.search(name)
            if not m or name == 'func_' + m.group(1) or name.endswith('.NON_MATCHING'):
                continue
            (undefined if typ == 'U' else defined).add((name, 'func_' + m.group(1)))
        for name, plain in sorted(defined):
            lines.append('PROVIDE(%s = %s);' % (plain, name))
        for name, plain in sorted(undefined - defined):
            lines.append('PROVIDE(%s = %s);' % (name, plain))
    write_list(os.path.join(os.path.dirname(script), 'aliases.ld'), lines)
    r = subprocess.run(exe(BU + 'ld.exe') + ['-m', 'elf32lr5900', '-T', script, '-Map', mapf,
                                             '-o', out, '@' + rsp], stdin=subprocess.DEVNULL)
    sys.exit(r.returncode)


if __name__ == '__main__':
    main()
