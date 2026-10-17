#!/usr/bin/env python3
"""Source rules a candidate must follow to count as decompiled (docs/WORKER.md).

violation(text, func) returns why the source breaks a hard rule, or None:
  - FUNC itself is still INCLUDE_ASM (a match then is just the original assembly);
  - the file has no definition of FUNC at all (same: retail asm stays, e.g. a misnamed file);
  - register pinning (`register int x asm("$s0")`).
warnings(text) lists inline asm that WORKER.md allows only in narrow cases: instructions
outside VU0/MMI, the EE FPU extras and the privileged ones (fine only inside a copied Sony
libvu0 body), and empty asm used to steer scheduling. The proven 1/length tail from WORKER.md
(lui $2, 0x3f80; mtc1; nop; div.s after sqrt.s) is not flagged. Final verification of the worker pools
rejects ordinary FPU/GPR asm, so a worker should treat a warning as a dead end.
The volatile Abs helper (docs/findings/013) and libgcc's longlong.h umul_ppmm (finding 014) are
allowed in their one exact form.
tools/try_func.py runs both before compiling, so a worker sees them on every try; tools/promote.py
refuses a violation too.

usage: python tools/source_rules.py FUNC SRC
"""
import ast, re, sys

# VU0 macro mode (v*, qmtc2/qmfc2/cfc2/ctc2/lqc2/sqc2), MMI, EE FPU extras, privileged.
ALLOWED = set('''mula.s madda.s madd.s msub.s msuba.s adda.s suba.s max.s min.s rsqrt.s
sqrt.s lq sq ei di sync sync.p mfc0 mtc0 mfc1 mtc1 cache lqc2 sqc2 pabsw pabsh paddw paddh paddb paddsw
paddsh paddsb padduw padduh paddub psubw psubh psubb psubsw psubsh psubsb psubuw psubuh psubub
pceqw pceqh pceqb pcgtw pcgth pcgtb pmaxw pmaxh pminw pminh pand por pxor pnor pextlw pextlh
pextlb pextuw pextuh pextub ppacw ppach ppacb pcpyld pcpyud pcpyh pexch pexew prot3w prevh
pmfhl pmthl pmultw pmultuw pmulth pmaddw pmadduw pmaddh pmsubw pmsubh psllw psllh psraw psrah
psrlw psrlh psllvw psravw psrlvw'''.split())
VU0_MOVE = re.compile(r'(?:qmtc2|qmfc2|cfc2|ctc2)(?:\.\w+)?$')

# C strings and character literals stay; comments become a space.
TOKENS = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
STRING = re.compile(r'"(?:\\.|[^"\\])*"')
ASM = re.compile(r'\b(?:asm|__asm|__asm__)\s*(?:(?:volatile|__volatile|__volatile__)\s*)?\(')


def _clean(text):
    text = TOKENS.sub(lambda m: ' ' if m[0].startswith(('/*', '//')) else m[0], text)
    return text.replace('\\\n', '')


def _close(text, i):
    """Index after the parenthesis group that opens at text[i]."""
    depth = 0
    for j in range(i, len(text)):
        depth += {'(': 1, ')': -1}.get(text[j], 0)
        if depth == 0:
            return j + 1
    return len(text)


TAIL = re.compile(r'\s*(?:const\s*)?(?:(?:asm|__asm__|__attribute__)\s*\()?')
KR_NAMES = re.compile(r'\(\s*\w+(?:\s*,\s*\w+)*\s*\)')
KR_PARAMS = re.compile(r'(?:\s*[A-Za-z_][\w\s*,\[\]]*;)*\s*\{')  # K&R parameter declarations


def _body_follows(text, start, end):
    kr = KR_NAMES.fullmatch(text, start, end)
    while True:  # const, asm("label"), __attribute__((...)) between ) and {
        t = TAIL.match(text, end)
        if t.group().endswith('('):
            end = _close(text, t.end() - 1)
        elif t.group().strip() == 'const':
            end = t.end()
        else:
            return bool((KR_PARAMS if kr else re.compile(r'\s*\{')).match(text, end))


def defines(text, func):
    """True if text has a body for func: `func(...) {`, a definition labelled asm("func"), or a
    body for a name declared as asm("func"). FUNC__<suffix> is unit_merge's rename of a chunk's
    copy; FUNC_body counts for the functions entered two words into their range (the
    never-executed prefix is emitted as data, see src/unsorted/001E3CD0.c)."""
    names = [re.escape(func) + r'(?:_body|__\w+)?']
    for m in re.finditer(r'\b(\w+)\s*(\((?:[^()]|\([^()]*\))*\))\s*(?:asm|__asm__)\s*\(\s*"'
                         + re.escape(func) + r'"\s*\)\s*([;{])', text):
        if m.group(3) == '{':
            return True
        names.append(re.escape(m.group(1)))  # an alias declaration; its definition may come later
    for name in names:
        for m in re.finditer(r'\b' + name + r'\s*\(', text):
            if _body_follows(text, m.end() - 1, _close(text, m.end() - 1)):
                return True
    return False


def violation(text, func=None):
    text = _clean(text)
    if func and re.search(r'\bINCLUDE_ASM\s*\([^,]+,\s*' + re.escape(func) + r'\s*\)', text):
        return 'assigned function %s still uses INCLUDE_ASM, not decompiled C/C++' % func
    if func and not defines(text, func):
        return 'no definition of %s in this file (a match then is just the original assembly)' % func
    if re.search(r'\bregister\s+[^;{}()]+\b(?:asm|__asm|__asm__)\s*\(', text):
        return 'register pinning (register ... asm("reg")) is not allowed'
    return None


# Proven forms from WORKER.md ("The game's vector helpers") that use ordinary instructions:
# the 1/length tail after sqrt.s (lui $2, 0x3f80; mtc1 $2, inv; nop; div.s inv, inv, len).
PROVEN_TAIL = re.compile(r'lui \$2, ?0x3f80 mtc1 \$2, ?%\d nop div\.s %\d, ?%\d, ?%\d')
# The game's Abs helper (WORKER.md, docs/findings/013): one volatile abs.s, output and input in FPRs.
PROVEN_ABS = re.compile(r'(?:asm|__asm|__asm__)\s+(?:volatile|__volatile|__volatile__)\s*\(\s*"abs\.s\s+%0,\s*%1"'
                        r'\s*:\s*"=&?f"\s*\(\s*\w+\s*\)\s*:\s*"f"\s*\(\s*\w+\s*\)\s*\)')
# libgcc's umul_ppmm from gcc's longlong.h (MIPS, docs/findings/014): multu with LO/HI outputs.
_OPND = r'\s*\(\s*(?:\(\s*(?:USItype|UWtype)\s*\)\s*)?(?:\w+|\(\s*\w+\s*\))\s*\)'  # (x), ((T) x), ((T) (x))
PROVEN_UMUL = re.compile(r'(?:asm|__asm|__asm__)\s*\(\s*"multu %2,\s*%3"\s*:\s*"=l"' + _OPND
                         + r'\s*,\s*"=h"' + _OPND + r'\s*:\s*"d"' + _OPND + r'\s*,\s*"d"' + _OPND + r'\s*\)(?!\s*:)')


def _drop_proven(lines):
    for i, line in enumerate(lines):
        if line.startswith('sqrt.s') and PROVEN_TAIL.fullmatch(' '.join(
                re.sub(r'\s+', ' ', l) for l in lines[i + 1:i + 5])):
            return lines[:i + 1] + lines[i + 5:]
    return lines


def warnings(text):
    text = _clean(text)
    out = []
    for block in ASM.finditer(text):
        if PROVEN_ABS.match(text, block.start()) or PROVEN_UMUL.match(text, block.start()):
            continue
        head = re.match(r'\s*((?:"(?:\\.|[^"\\])*"\s*)+)', text[block.end():])
        if not head:
            continue
        try:
            code = ''.join(ast.literal_eval(s) for s in STRING.findall(head[1]))
        except (ValueError, SyntaxError):
            continue
        if re.fullmatch(r'\s*[A-Za-z_.$][\w.$]*\s*', code) and not code.strip() in ALLOWED:
            continue  # asm("D_x") symbol name on a declaration, not an instruction
        lines = [l.strip() for l in re.split(r'[\n;]', code) if l.strip()]
        lines = _drop_proven(lines)
        if not lines and not code.strip():
            out.append('empty inline asm (steers scheduling/registers)')
        for line in lines:
            op = line.split()[0]
            if not (op in ALLOWED or op.startswith(('v', '.')) or op.endswith(':') or VU0_MOVE.match(op)):
                out.append('inline asm "%s"' % line)
    return sorted(set(out))


def report(text, func=None):
    """Lines try_func prints; returns (lines, rejected)."""
    why = violation(text, func)
    if why:
        return ['REJECTED: ' + why + ' (docs/WORKER.md). Not compiled; this can never count.'], True
    return ['WARNING: %s: WORKER.md allows inline asm only for VU0/MMI/EE FPU extras/privileged '
            'instructions (or a copied libvu0 body); ordinary FPU/GPR asm is rejected at final '
            'verification. Write it in C.' % w for w in warnings(text)], False


if __name__ == '__main__':
    lines, rejected = report(open(sys.argv[2], encoding='utf-8', errors='replace').read(), sys.argv[1])
    print('\n'.join(lines) or 'ok')
    sys.exit(3 if rejected else 0)
