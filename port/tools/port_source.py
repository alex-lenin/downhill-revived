"""The source and IR steps every game-unit build shares (build.py, build_wasm.py): the port patches
(port/patches.py) and the 16-byte alignment of aggregate locals."""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import patches  # noqa: E402  (port/patches.py)
import transform  # noqa: E402


def keep_lines(old, new):
    """new padded with newlines to old's line count, so later lines keep their numbers."""
    return new + '\n' * max(0, old.count('\n') - new.count('\n'))


def apply_patches(rel, text):
    """The port patches of one src/ unit (rel: its path under src/, '/' separated)."""
    for kind, a, b in patches.PATCHES.get(rel, []):
        if kind == 'text':
            if text.count(a) != 1:
                raise ValueError('patch text found %d times: %r' % (text.count(a), a[:60]))
            text = text.replace(a, keep_lines(a, b))
        elif kind == 're':
            pattern, count = a
            text, n = re.subn(pattern, b, text)
            if n != count:
                raise ValueError('patch pattern found %d times, not %d: %r' % (n, count, pattern))
        else:
            mask = transform.code_mask(text)
            heads = list(re.finditer(r'\b%s\s*\([^;{)]*\)\s*\{' % a, mask))
            if len(heads) != 1:
                raise ValueError('patch: %d definitions of %s' % (len(heads), a))
            s = heads[0].end() - 1
            e = transform.close_paren(mask, s) + 1
            text = text[:s] + keep_lines(text[s:e], b) + text[e:]
    return text


# A local array, struct, union, class or 128-bit object (a stack slot in the frontend's IR).
AGGREGATE_ALLOCA = re.compile(r'(= alloca (?:\[|\{|<|i128\b|%"?(?:struct|union|class)\.)[^\n]*?, align )(\d+)')


def align_aggregate_allocas(ir):
    """The EE stack keeps such objects 16-byte aligned, and lq/sq/lqc2/sqc2 ignore the low 4
    address bits: recompiled code (recomp.h M128) and the inline asm runtime (ps2asm.c) both
    do, and the EE float rewrite copies them with SIMD. The host stack only aligns them to their
    type, so give every aggregate local 16 bytes, whatever path its address takes to a quadword
    access (finding 021)."""
    return AGGREGATE_ALLOCA.sub(lambda m: m[1] + str(max(16, int(m[2]))), ir)
