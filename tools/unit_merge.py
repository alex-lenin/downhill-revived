#!/usr/bin/env python3
"""Merge already-decompiled units into a candidate whose UNIT covers them as INCLUDE_ASM.

A worker sometimes matches a function inside a bigger translation unit (shared $gp data must be
defined in the TU), written with INCLUDE_ASM for its neighbours. If some neighbours have C in
src/unsorted/ by now, promote.py refuses the candidate (it would turn them back into asm). This
pastes each such unit's C in place of its INCLUDE_ASM lines.

The pasted files declare the same globals/functions with their own (often different) types. To
keep every file's code exactly as it was, each pasted file's extern declarations of names that
other parts also declare get a private alias: `extern char D_x__c2[] asm("D_x");` (GCC asm label;
same symbol, the file's own type), and the file's body uses the alias. Typedef/struct names that
collide are renamed the same way. A data definition that already exists earlier is dropped (same
initializer required). Only try_func decides whether the result matches.

Finally every alias the compiler accepts is undone (X__cN -> X): only names that two chunks
declare differently keep one.

usage: python tools/unit_merge.py CANDIDATE OUT
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import text_functions, units as src_units, unit_marker  # noqa: E402

INC = re.compile(r'^INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\);\s*$', re.M)
WORD = r'(?<![\w$])%s(?![\w$])'


def statements(text):
    """Top-level statements (brace depth 0, ending in ';'), with their spans. Skips # lines."""
    out, depth, start, i, linkage = [], 0, 0, 0, []  # linkage: depths of `extern "C" {` blocks (transparent)
    while i < len(text):
        c = text[i]
        if depth == 0 and re.match(r'extern\s+"C"\s*\{', text[i:i + 20]):
            i = text.index('{', i) + 1
            linkage.append(depth)
            start = i
            continue
        if c == '}' and depth == 0 and linkage:
            linkage.pop()
            i += 1
            start = i
            continue
        if c == '#' and (i == 0 or text[i - 1] == '\n'):
            j = text.find('\n', i)
            while j > 0 and text[j - 1] == '\\':  # backslash-continued macro
                j = text.find('\n', j + 1)
            i = start = (j if j >= 0 else len(text)) + 1
            continue
        if text.startswith('/*', i):
            i = text.find('*/', i) + 2
            if depth == 0 and not text[start:i - 2].strip('/* \n'):
                start = i
            continue
        if c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                k = i + 1
                while k < len(text) and text[k] in ' \t':
                    k += 1
                # end of a function body: not a statement. `struct {...} name;` and
                # `struct {...} *name, x[2];` continue with declarators up to the ';'.
                if not re.match(r'[\w \t*,\[\]]*;', text[k:k + 200]):
                    start = i + 1
        elif c == ';' and depth == 0:
            out.append((start, i + 1))
            start = i + 1
        i += 1
    return out


def split_top(s, sep=','):
    parts, depth, cur = [], 0, ''
    for c in s:
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        if c == sep and depth == 0:
            parts.append(cur)
            cur = ''
        else:
            cur += c
    return parts + [cur]


def declared(stmt):
    """(kind, names) of a top-level statement: extern / typedef / definition / other."""
    s = re.sub(r'/\*.*?\*/', ' ', stmt, flags=re.S).strip().rstrip(';')
    s = re.sub(r'__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)', ' ', s)  # not a declarator
    if s.startswith('typedef'):
        names = [re.findall(r'\w+', d)[-1] for d in split_top(s.split('}')[-1] if '}' in s else s)
                 if re.findall(r'\w+', d)]
        if '}' not in s:  # typedef int x, *y;  -> names after the base type
            names = [re.findall(r'(\w+)\s*(?:\[[^\]]*\])*\s*$|\(\s*\*\s*(\w+)\s*\)', d.split('(')[0] if '(*' not in d else d)
                     for d in split_top(s[7:])]
            names = [a or b for m in names for a, b in m[:1]]
        return 'typedef', names
    if re.match(r'(struct|union|enum)\s+\w+\s*\{', s) and s.endswith('}'):
        return 'tag', [re.match(r'(?:struct|union|enum)\s+(\w+)', s).group(1)]
    m = re.match(r'(extern\s+(?:"C"\s+)?)?(.*)', s, re.S)
    decls, names = split_top(m.group(2)), []
    for i, d in enumerate(decls):
        d = d.split('=')[0]
        if i == 0:  # strip the base type: the declarator starts at the last word before ( [ or end
            d = re.sub(r'^.*?((?:\*\s*)*\(?\s*\*?\s*\w+\s*(?:\(|\[|\)|$))', r'\1', d.strip(), flags=re.S)
        n = re.findall(r'\w+', d.split('(')[0] if not re.match(r'\s*\(\s*\*', d) else d.split(')')[0])
        if n:
            names.append(n[-1])
    return ('extern' if m.group(1) else 'def'), names


SYM = re.compile(r'(?<![\w$])((?:D|func|jtbl)_[0-9A-F]{8}|[A-Za-z_]\w*_[0-9A-F]{8})(?![\w$])')


def chunk_plan(text):
    """What a pasted chunk keeps (functions and data it defines) and renames (everything else that
    is a project symbol, plus its own typedef/struct names)."""
    defined_funcs = set(re.findall(r'^[\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{', text, re.M))
    defs, keep = {}, set(defined_funcs)
    keep |= set(re.findall(r'\.globl\s+(\w+)', text))  # symbols a top-level asm() block defines
    for a, b in statements(text):
        s = text[a:b]
        bare = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S).strip()
        if bare.startswith(('extern', 'typedef')) or '(' in bare.split('=')[0]:
            continue
        for p in split_top(bare.rstrip(';')):
            m = SYM.search(p.split('=')[0])
            if m:
                defs[m.group(1)] = s
                keep.add(m.group(1))
    syms = set(SYM.findall(text)) - keep
    bare = re.sub(r'__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)', ' ', text)
    types = set(re.findall(r'\btypedef\b[^;{]*?(?:\{[^{}]*(?:\{[^{}]*\}[^{}]*)*\})?[^;{]*?\b(\w+)\s*(?:\[[^\]]*\]\s*)*;', bare))
    types |= set(re.findall(r'\b(?:struct|union|enum)\s+(\w+)\s*\{', bare))
    return defs, syms, types


def label_externs(text, names):
    """In extern statements, give each declarator of a renamed name asm("original")."""
    out, last = [], 0
    for a, b in statements(text):
        stmt = text[a:b]
        head = re.match(r'(\s*(?:/\*.*?\*/\s*)*extern\s+(?:"C"\s+)?)', stmt, re.S)
        if not head:
            continue
        parts = split_top(stmt[head.end():].rstrip().rstrip(';'))
        new = []
        for i, p in enumerate(parts):
            # the declarator's name: before any parameter list (or inside "(*name)")
            m = SYM.search(p.split('(')[0]) or (SYM.search(p) if '(*' in p else None)
            if m and m.group(1) in names and not re.search(r'\basm\s*\(|__asm__', p):
                p = p.rstrip() + ' asm("@@%s@@")' % m.group(1)[::-1]
            new.append(p)
        out.append(text[last:a] + head.group(1) + ','.join(new) + ';')
        last = b
    return ''.join(out) + text[last:]


def chunk_names(text):
    ext, defs, types = {}, {}, set()
    for a, b in statements(text):
        kind, names = declared(text[a:b])
        for n in names:
            if kind == 'extern':
                ext[n] = (a, b)
            elif kind in ('typedef', 'tag'):
                types.add(n)
            elif kind == 'def' and '(' not in text[a:b].split('=')[0]:
                defs[n] = text[a:b]
    for m in re.finditer(r'\b(struct|union|enum)\s+(\w+)\s*\{', text):
        types.add(m.group(2))
    funcs = set(re.findall(r'^[\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{', text, re.M))
    return ext, defs, types, funcs


def alias(text, names, k):
    """Rename `names` in a chunk; extern declarators of them get asm("original")."""
    if not names:
        return text
    # labels first, with placeholders for the original names (the rename below must not touch them)
    out, last = [], 0
    for a, b in statements(text):
        kind, dn = declared(text[a:b])
        if kind != 'extern' or not set(dn) & names:
            continue
        stmt = text[a:b]
        body = stmt.rstrip().rstrip(';')
        head = re.match(r'\s*(?:/\*.*?\*/\s*)*extern\s+(?:"C"\s+)?', body, re.S)
        parts = split_top(body[head.end():])
        new = []
        for p in parts:
            n = [x for x in dn if re.search(WORD % x, p)]
            if n and n[-1] in names:
                p = p.rstrip() + ' asm("@@%s@@")' % n[-1][::-1]
            new.append(p)
        out.append(text[last:a] + body[:head.end()] + ','.join(new) + ';')
        last = b
    text = ''.join(out) + text[last:]
    for n in sorted(names, key=len, reverse=True):
        text = re.sub(WORD % n, '%s__c%d' % (n, k), text)
    return re.sub(r'@@(\w+)@@', lambda mm: mm.group(1)[::-1], text)


def rodata_syms(lo, hi):
    """Names of the splat .rodata blocks starting in [lo, hi)."""
    from configure import BLOCK_VRAM
    lines = open(os.path.join(ROOT, 'asm', 'data', 'rodata.rodata.s')).read().split('\n')
    out = []
    for i, l in enumerate(lines):
        if l.startswith('nonmatching'):
            v = next((BLOCK_VRAM.search(x) for x in lines[i + 1:i + 4] if BLOCK_VRAM.search(x)), None)
            if v and lo <= int(v.group(1), 16) < hi:
                out.append(l.split()[1].rstrip(','))
    return out


def main():
    cand, dst = sys.argv[1], sys.argv[2]
    text = open(cand, encoding='utf-8').read()
    m = unit_marker(cand)
    lo, hi = m[0], m[1]
    cpp = cand.endswith('.cpp')
    starts = {f[:-2]: v for v, f in text_functions()}
    covered = [u for u in src_units() if lo <= u[0] and u[1] <= hi]
    partial = [u for u in src_units() if u[0] < hi and u[1] > lo and u not in covered]
    if partial:
        sys.exit('units overlap the range only partly: %s' % [u[2] for u in partial])
    first = text.split('\n', 1)
    marker, body = first[0], first[1]
    # data ranges: the candidate's plus each pasted unit's (contiguous per section)
    ranges = dict(m[3])
    chunks, seen_ext, seen_def, seen_types, seen_funcs = [], {}, {}, set(), set()
    e0, d0, t0, f0 = chunk_names(body)
    seen_ext.update({n: 0 for n in e0})
    seen_def.update(d0)
    seen_types |= t0
    seen_funcs |= f0
    prev = body  # text before the current chunk (renamed chunks don't match SYM any more)
    for k, (s, e, src, _, _) in enumerate(sorted(covered), 1):
        own = chunk_names(open(os.path.join(ROOT, src), encoding='utf-8').read())[3]
        if own and own <= f0:  # the candidate is already a copy of this unit (widened around it)
            print('%s: already in the candidate, not pasted' % src)
            continue
        um = unit_marker(os.path.join(ROOT, src))
        gap_fill = ''
        for sec, (a, b) in um[3].items():
            if sec in ranges:
                ra, rb = ranges[sec]
                if a > rb and sec == '.rodata':  # retail .rodata in between (e.g. an earlier function's
                    # literals, extern in the candidate): keep it here from the splat output
                    gap_fill = ''.join('INCLUDE_RODATA("build/rodata", %s);\n' % n for n in rodata_syms(rb, a))
                elif not (a <= rb and b >= ra):
                    sys.exit('%s: %s range 0x%X:0x%X not contiguous with 0x%X:0x%X' % (src, sec, a, b, ra, rb))
                ranges[sec] = (min(a, ra), max(b, rb))
            else:
                ranges[sec] = (a, b)
        if um[4]:
            sys.exit('%s has its own flags %s' % (src, um[4]))
        t = open(os.path.join(ROOT, src), encoding='utf-8').read().split('\n', 1)[1]
        t = re.sub(r'^#include "include_asm.h"\n', '', t, flags=re.M)
        if src.endswith('.cpp') and not cpp:
            sys.exit('%s is C++ but the candidate is C' % src)
        defs, syms, types = chunk_plan(t)
        # duplicate data definitions: drop the later one (must be identical)
        for n, d in defs.items():
            if n in seen_def:
                norm = lambda x: re.sub(r'(?<![\w$])(0[xX][0-9A-Fa-f]+|[0-9]+)(?![\w$])',  # 0x7FFF == 32767
                                        lambda q: str(int(q.group(1), 0 if q.group(1)[:2] in ('0x', '0X') else 10)),
                                        re.sub(r'\s*([^\w\s$])\s*', r'\1', re.sub(r'\s+', ' ', x)).strip())
                if norm(d) != norm(seen_def[n]):
                    sys.exit('%s defines %s differently: %r vs %r' % (src, n, d, seen_def[n]))
                t = t.replace(d, '', 1)
        seen_def.update(defs)
        # functions this chunk defines that earlier text declares (maybe with another prototype): define
        # them under an alias, declared first with asm("real name")
        fdefs = [f for f in set(re.findall(r'^[\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{', t, re.M))
                 if f not in ('if', 'while', 'for', 'switch', 'return') and re.search(WORD % f, prev)]
        t = label_externs(t, syms)
        for f in fdefs:
            m2 = re.search(r'^([^\n;{}]*?)\b%s\s*\(([^;{]*)\)\s*\{' % f, t, re.M)
            if 'static' in m2.group(1):  # a local helper: a plain rename is enough
                continue
            decl = 'extern %s%s(%s) asm("@@%s@@");\n' % (m2.group(1), f, m2.group(2), f[::-1])
            t = t[:m2.start()] + decl + t[m2.start():]
        # existing asm("X") labels name the real symbol: protect them from the rename
        t = re.sub(r'((?:__asm__|asm)\s*\(\s*")(\w+)("\s*\))',
                   lambda mm: mm.group(1) + '@@' + mm.group(2)[::-1] + '@@' + mm.group(3), t)
        for n in sorted(syms | set(fdefs) | types, key=len, reverse=True):
            t = re.sub(WORD % n, '%s__c%d' % (n, k), t)
        t = re.sub(r'@@(\w+)@@', lambda mm: mm.group(1)[::-1], t)
        if cpp and not src.endswith('.cpp'):
            t = 'extern "C" {\n%s\n}\n' % t.strip('\n')
        names = [f for f, v in starts.items() if s <= v < e]
        prev += t
        chunks.append((s, names, gap_fill + '/* ---- merged from %s ---- */\n%s' % (src, t.strip('\n'))))
    lines = body
    for s, names, t in chunks:
        incs = [mm for mm in INC.finditer(lines) if mm.group(1) in names]
        if not incs:
            sys.exit('no INCLUDE_ASM for %s in the candidate' % names[0])
        a = incs[0].start()
        for mm in reversed(incs[1:]):
            lines = lines[:mm.start()] + lines[mm.end() + 1:]
        lines = lines[:a] + t + lines[incs[0].end():]
    secs = ' '.join('%s=0x%X:0x%X' % (sec, a, b) for sec, (a, b) in sorted(ranges.items()))
    head = '/* UNIT 0x%08X 0x%08X%s */' % (lo, hi, ' ' + secs if secs else '')
    if m[4]:
        head = head[:-3] + ' ' + ' '.join(m[4]) + ' */'
    # a pasted unit's own INCLUDE_ASM names the asm file, never a per-chunk alias
    lines = re.sub(r'(INCLUDE_ASM\(\s*"[^"]*"\s*,\s*)(\w+?)__c\d+(\s*\))', r'\1\2\3', lines)
    text, kept = unalias(head + '\n' + lines, m[4], cand.endswith('.cpp'))
    open(dst, 'w', newline='\n').write(text)
    print('%s: merged %d units (%s); per-chunk aliases kept for %d conflicting names' % (
        dst, len(chunks), ', '.join(u[2].split('/')[-1] for u in sorted(covered)), kept))


def unalias(text, flags, cpp):
    """Undo the per-chunk aliases (X__cN -> X) the compiler accepts: only names that two chunks
    declare differently keep theirs. Compiles to find the conflicts; returns (text, number kept)."""
    import subprocess, tempfile, shutil
    import toolchain
    names = set(m.group(1) for m in re.finditer(r'\b(\w+?)__c\d+\b', text))
    keep = set()

    def collapse():
        t = re.sub(r'\b(\w+?)__c(\d+)\b', lambda m: m.group(0) if m.group(1) in keep else m.group(1), text)
        # `extern T X asm("X");` -> `extern T X;`
        return re.sub(r'\b(\w+)(\s*\([^;{]*\)|(?:\s*\[[^\]]*\])+)?\s*(?:__asm__|asm)\s*\(\s*"(\w+)"\s*\)(?=\s*[;,={])',
                      lambda m: m.group(1) + (m.group(2) or '') if m.group(1) == m.group(3) else m.group(0), t)

    tmp = tempfile.mkdtemp(prefix='unalias_')
    try:
        for _ in range(40):
            t = collapse()
            name = 'u.cpp' if cpp else 'u.c'
            open(os.path.join(tmp, name), 'w').write(t)
            inc = [x for d in ('include', 'src') for x in ('-I', os.path.join(ROOT, d))]
            r = subprocess.run(toolchain.exe(toolchain.CC) + ['-S', name, '-o', 'u.s'] + inc +
                               list(toolchain.CXXFLAGS if cpp else toolchain.CFLAGS) + list(flags),
                               cwd=tmp, capture_output=True, text=True)
            if r.returncode == 0:
                return t, len(keep)
            out = r.stdout + r.stderr
            bad = set(re.findall(r"`(?:struct |union |enum )?(\w+)'", out)) & names
            if not bad - keep:  # attribute the errors to the alias names on the offending lines
                src_lines = t.split('\n')
                for n in [int(x) for x in re.findall(r'u\.c(?:pp)?:(\d+):', out)][:20]:
                    bad |= set(re.findall(r'\w+', src_lines[n - 1])) & names
            if not bad - keep:
                break
            keep |= bad
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return text, len(names)  # could not resolve: keep every alias


if __name__ == '__main__':
    main()
