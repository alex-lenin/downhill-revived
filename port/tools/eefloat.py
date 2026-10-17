"""EE float semantics for natively compiled game units (finding 026). Rewrites a unit's LLVM IR,
before optimisation, so that float add, subtract and divide and all double arithmetic call the
helpers of port/runtime/ee_native_float.h, which build.py includes in every unit."""
import os
import re

HEADER = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'runtime', 'ee_native_float.h')

HELPERS = {('fadd', 'float'): 'ps2_native_fadd', ('fsub', 'float'): 'ps2_native_fsub',
           ('fdiv', 'float'): 'ps2_native_fdiv',
           ('fadd', 'double'): 'ps2_native_dadd', ('fsub', 'double'): 'ps2_native_dsub',
           ('fmul', 'double'): 'ps2_native_dmul', ('fdiv', 'double'): 'ps2_native_ddiv'}
BINARY = re.compile(r'^(\s*%[-\w.$]+ = )(fadd|fsub|fmul|fdiv) (float|double) ([^,\s]+), ([^,\s]+)$')
TRUNC = re.compile(r'^(\s*%[-\w.$]+ = )fptrunc double ([^,\s]+) to float$')
# Anything else that rounds and would escape the rewrite: fast-math flags, vectors, frem, metadata.
LEFT = re.compile(r'= (?:fadd|fsub|fdiv|frem|fptrunc)\b|= fmul\b(?! float )')
DEFINE = re.compile(r'^define [^@]*@"?(\w+)')
USED = re.compile(r'^(@llvm\.(?:compiler\.)?used = appending global )\[\d+ x ptr\] \[(.*)\](, section "llvm\.metadata")$')


def unpin(line):
    """The header marks the helpers used so that the front end emits them; once the calls exist,
    let the optimiser drop the ones a unit doesn't need."""
    m = USED.match(line)
    if not m:
        return line
    kept = [p for p in m.group(2).split(', ') if not p.startswith('ptr @ps2_native_')]
    return '%s[%d x ptr] [%s]%s' % (m.group(1), len(kept), ', '.join(kept), m.group(3)) if kept else None


def rewrite(ir):
    """The IR with EE float semantics, and how many operations it rewrote. Raises ValueError on an
    operation it can't rewrite, rather than leave it with host semantics."""
    out, count, skip = [], 0, False
    for line in ir.split('\n'):
        define = DEFINE.match(line)
        if define:
            skip = define.group(1).startswith('ps2_native_')  # the helpers themselves
        elif line == '}':
            skip = False
        if not skip:
            m = BINARY.match(line)
            if m and (m.group(2), m.group(3)) in HELPERS:
                kind = m.group(3)
                line = '%scall %s @%s(%s %s, %s %s)' % (m.group(1), kind, HELPERS[m.group(2), kind],
                                                        kind, m.group(4), kind, m.group(5))
                count += 1
            else:
                t = TRUNC.match(line)
                if t:
                    line = '%scall float @ps2_native_dtof(double %s)' % (t.group(1), t.group(2))
                    count += 1
                elif LEFT.search(line):
                    raise ValueError('EE float rewrite: unhandled %r' % line.strip())
        line = unpin(line)
        if line is not None:
            out.append(line)
    return '\n'.join(out), count
