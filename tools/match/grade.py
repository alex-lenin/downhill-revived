#!/usr/bin/env python3
"""Byte-exact grader: compile a C/C++ source, compare one function's .text with the retail ELF.

Usage: grade.py [--cc NAME] [--lang c|c++] [--diff] SRC FUNC ADDR -- FLAGS...

The function's bytes in the object file are compared word-by-word with the ELF at ADDR.
Relocated fields are masked (R_MIPS_26 low 26 bits, HI16/LO16 low 16 bits), but a
R_MIPS_26 against a symbol named func_XXXXXX must resolve to exactly that address.
Exit 0 = exact match, 1 = mismatch, 2 = compile error.
"""
import os, re, struct, subprocess, sys, tempfile, argparse
import difflib, threading
import rabbitizer as rz

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ELF = os.path.join(ROOT, 'iso', 'SCUS_971.77')
COMPILERS = os.path.join(ROOT, 'tools', 'compilers')

R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_GPREL16 = 4, 5, 6, 7
GP = 0x2A40F0  # _gp, set by crt0 at 0x10a170


def sym_addr(name):
    """Address encoded in a placeholder symbol name (func_XXXXXX, D_XXXXXX), or None."""
    m = re.search(r'(?:func|D)_([0-9A-Fa-f]{6,8})(?![0-9A-Fa-f])', name)
    return int(m.group(1), 16) if m else None


def sext16(x):
    x &= 0xffff
    return x - 0x10000 if x & 0x8000 else x


def sections(d):
    shoff, = struct.unpack_from('<I', d, 32)
    shentsize, shnum, shstr = struct.unpack_from('<HHH', d, 46)
    secs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    so = secs[shstr][4]
    name = lambda off: d[off:d.index(b'\0', off)].decode()
    return [(name(so + s[0]),) + s for s in secs], name


def target_words(addr, n):
    d = open(ELF, 'rb').read()
    secs, _ = sections(d)
    t = [s for s in secs if s[0] == '.text'][0]
    base, off = t[4], t[5]
    return list(struct.unpack_from('<%dI' % n, d, off + addr - base))


def obj_function(path, func):
    """Return (words, relocs{word_index: (type, symname)}) for FUNC in a MIPS ELF32 .o."""
    d = open(path, 'rb').read()
    secs, name = sections(d)
    symtab = [s for s in secs if s[2] == 2][0]  # SHT_SYMTAB
    strtab = secs[symtab[7]]
    syms = []
    for i in range(symtab[6] // 16):
        st_name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, symtab[5] + i * 16)
        syms.append((name(strtab[5] + st_name), value, size, shndx))
    mangled = '%d%s' % (len(func), func)
    cand = [s for s in syms if s[2] and (s[0] == func or s[0].startswith(func + '__')
            or (s[0].startswith('_Z') and mangled in s[0]))]
    if not cand:
        raise SystemExit('symbol %s not found; have: %s' % (func, [s[0] for s in syms if s[2]]))
    sname, value, size, shndx = cand[0]
    text = secs[shndx]
    words = list(struct.unpack_from('<%dI' % (size // 4), d, text[5] + value))
    relocs = {}
    for s in secs:
        if s[2] == 9 and s[8] == shndx:  # SHT_REL for this section
            for i in range(s[6] // 8):
                r_off, r_info = struct.unpack_from('<II', d, s[5] + i * 8)
                if value <= r_off < value + size:
                    relocs[(r_off - value) // 4] = (r_info & 0xff, syms[r_info >> 8][0])
    return words, relocs


def wsl_path(p):
    p = os.path.abspath(p).replace('\\', '/')
    return '/mnt/' + p[0].lower() + p[2:]


# Linux i386 compilers run in WSL via wsl_cc.sh.
def compile_wsl(cc, lang, src, flags, out):
    script = wsl_path(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'wsl_cc.sh'))
    return run(['wsl', '-d', 'Ubuntu', '-u', 'root', '--', 'bash', script,
                cc, lang, wsl_path(src), wsl_path(out)] + flags)


def run(cmd, env=None):
    p = subprocess.run(cmd, env=env, capture_output=True, text=True, stdin=subprocess.DEVNULL)
    if p.returncode:
        sys.stderr.write(p.stderr)
    return p.returncode == 0


def win_env(cdir):
    env = dict(os.environ)
    env['PATH'] = os.path.join(cdir, 'dll') + os.pathsep + env['PATH']
    return env


# Assembler flags as each driver passes them (from `ee-gcc -v`), minus -G which comes from FLAGS.
AS_FLAGS = {'2.10': ['-EL', '-mabi=eabi'], '2.12': ['-mdebug'], 'ps2eeas': []}


def compile_src(cc, lang, src, flags, out):
    """CC is "sn" (the matching toolchain, tools/toolchain.py), a compiler package name, or
    "CC@ASPKG" to assemble CC's output with ASPKG's as."""
    if cc == 'sn':
        sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        import toolchain
        return toolchain.compile_c(src, out, lang, flags)
    cc, _, aspkg = cc.partition('@')
    if aspkg:
        s = out[:-2] + '.s'
        if not compile_src(cc, lang, src, flags + ['-S'], s):
            return False
        adir = os.path.join(COMPILERS, aspkg)
        g = [f for f in flags if f.startswith('-G')] or ['-G8']
        snas = os.path.join(adir, 'usr', 'local', 'sce', 'ee', 'gcc', 'ee', 'bin', 'Ps2EeAs.exe')
        if os.path.exists(snas):  # SN ProDG's own assembler (what ps2cc runs)
            return run([snas] + g + AS_FLAGS['ps2eeas'] + ['-o', out, s])
        exe = os.path.join(adir, 'ee', 'bin', 'as.exe')
        if not os.path.exists(exe):
            sys.exit('--as package must be a Windows one for now: %s' % aspkg)
        opt = [f for f in flags if f.startswith('-O')][-1:]
        return run([exe] + g + opt + AS_FLAGS['2.12'] + ['-o', out, s], win_env(adir))
    cdir = os.path.join(COMPILERS, cc)
    exe = os.path.join(cdir, 'bin', 'ee-gcc.exe')
    if not os.path.exists(exe):
        return compile_wsl(cc, lang, src, flags, out)
    mode = '-S' if '-S' in flags else '-c'
    return run([exe, '-x', lang, mode, src, '-o', out] + [f for f in flags if f != '-S'], win_env(cdir))


LAST = threading.local()  # per-thread details of the last grade(): .dist = word edit distance


def grade(cc, lang, src, func, addr, flags, show_diff=False, size=None):
    with tempfile.TemporaryDirectory() as tmp:
        out = os.path.join(tmp, 'out.o')
        if not compile_src(cc, lang, src, flags, out):
            LAST.dist = 999
            return None
        words, relocs = obj_function(out, func)
    tgt = target_words(addr, len(words) + 1)
    ok = True
    lines = []
    for i, w in enumerate(words):
        t = tgt[i]
        mask = 0xffffffff
        note = ''
        if i in relocs:
            typ, sym = relocs[i]
            want = sym_addr(sym)
            got = None
            if typ == R_MIPS_26:
                mask = 0xfc000000
                if want is not None:
                    want = want >> 2 & 0x3ffffff
                    got = t & 0x3ffffff
            elif typ == R_MIPS_HI16:
                mask = 0xffff0000
            elif typ == R_MIPS_LO16:
                mask = 0xffff0000
                his = [j for j, r in relocs.items() if r == (R_MIPS_HI16, sym) and j < i]
                if want is not None and his:
                    j = max(his)
                    want += (words[j] & 0xffff) * 0x10000 + sext16(w)
                    got = (tgt[j] & 0xffff) * 0x10000 + sext16(t)
            elif typ == R_MIPS_GPREL16:
                mask = 0xffff0000
                if want is not None:
                    want += sext16(w)
                    got = GP + sext16(t)
            else:
                note = ' [unchecked reloc type %d]' % typ
            if got is not None and got != want:
                ok = False
                note = ' <- %s resolves to %#x, target %#x' % (sym, want, got)
            note = note or ' [%s%s]' % (sym, '' if got is not None else ', target unchecked')
        same = (w & mask) == (t & mask)
        ok &= same
        a = addr + i * 4
        lines.append('%s %08x %-36s %08x %-36s%s' % (
            ' ' if same else '*', a, rz.Instruction(w, a, rz.InstrCategory.R5900).disassemble(),
            t, rz.Instruction(t, a, rz.InstrCategory.R5900).disassemble(), note))
    n = size // 4 if size else len(words)
    tw = target_words(addr, n)
    norm = lambda x: x & 0xffff0000 if x >> 26 in (2, 3) or x >> 26 in (0x0f, 0x09) else x
    sm = difflib.SequenceMatcher(None, [norm(x) for x in words], [norm(x) for x in tw], autojunk=False)
    LAST.dist = max(len(words), n) - sum(b.size for b in sm.get_matching_blocks())
    if size is not None and size != len(words) * 4:
        lines.append('size %#x != target size %#x' % (len(words) * 4, size))
        ok = False
    if show_diff:
        print('\n'.join(lines))
    return ok


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--cc', default='sn')
    ap.add_argument('--lang', default=None)
    ap.add_argument('--diff', action='store_true')
    ap.add_argument('--size', type=lambda x: int(x, 0), help='target function size in bytes')
    ap.add_argument('src'); ap.add_argument('func'); ap.add_argument('addr')
    ap.add_argument('flags', nargs=argparse.REMAINDER)
    a = ap.parse_args()
    flags = [f for f in a.flags if f != '--']
    lang = a.lang or ('c' if a.src.endswith('.c') else 'c++')
    r = grade(a.cc, lang, os.path.abspath(a.src), a.func, int(a.addr, 16), flags, a.diff, a.size)
    if r is None:
        sys.exit(2)
    print(('MATCH' if a.size else 'MATCH (size unchecked)') if r else 'DIFF')
    sys.exit(0 if r else 1)


if __name__ == '__main__':
    main()
