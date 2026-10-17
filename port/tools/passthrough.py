"""Finds calls that pass an argument only in a register: func_X() called with no arguments while
func_X's definition takes parameters. On the EE the caller's $a0..$a3 still hold the values the
callee reads (the C matches the retail bytes), but on the PC nothing is passed. Each site
needs its argument written out (if the PS2 build still matches) or a PC patch (port/patches.py).
usage: python port/tools/passthrough.py"""
import glob, os, re

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main():
    defs, srcs = {}, {}
    for p in glob.glob(os.path.join(ROOT, 'src', '**', '*.c*'), recursive=True):
        t = open(p, encoding='utf-8', errors='replace').read()
        srcs[os.path.relpath(p, ROOT).replace(os.sep, '/')] = t
        for m in re.finditer(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-F]{8})\s*\(([^)]*)\)\s*\{', t, re.M):
            a = m.group(2).strip()
            if a not in ('', 'void'):
                defs[m.group(1)] = a
    for p, t in sorted(srcs.items()):
        for m in re.finditer(r'(?<![\w.>])(func_[0-9A-F]{8})\s*\(\s*\)', t):
            name = m.group(1)
            if name not in defs:
                continue
            line = t[t.rfind('\n', 0, m.start()) + 1:t.find('\n', m.end())]
            if re.match(r'\s*(extern\s+)?(static\s+)?[A-Za-z_]\w*[\s\*]+%s\s*\(\s*\)\s*(asm\([^)]*\))?\s*;' % name,
                        line) and not re.match(r'\s*return\b', line):
                continue  # a declaration
            print('%s:%d: %s() takes (%s)' % (p, t.count('\n', 0, m.start()) + 1, name, defs[name]))
    fewer_arguments(srcs)
    returns_in_register(srcs)


def count_args(t, i):
    """Number of top-level arguments of the call whose '(' is at t[i]; None if unbalanced."""
    depth, n, empty = 0, 1, True
    for j in range(i, min(len(t), i + 2000)):
        c = t[j]
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
            if depth == 0:
                return 0 if empty else n
        elif c == ',' and depth == 1:
            n += 1
        elif not c.isspace() and depth >= 1:
            empty = False
    return None


def fewer_arguments(srcs):
    """Calls passing some arguments but fewer than the definition takes: the rest travel in the
    caller's own $a registers on the EE (a wrapper forwarding its parameters), not on the PC."""
    params = {}
    for t in srcs.values():
        for m in re.finditer(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-F]{8})\s*\(([^)]*)\)\s*\{', t, re.M):
            a = m.group(2).strip()
            if a not in ('', 'void') and '...' not in a:
                params[m.group(1)] = len(a.split(','))
    for p, t in sorted(srcs.items()):
        for m in re.finditer(r'(?<![\w.>])(func_[0-9A-F]{8})\s*\(', t):
            want = params.get(m.group(1))
            if not want:
                continue
            n = count_args(t, m.end() - 1)
            line = t[t.rfind('\n', 0, m.start()) + 1:t.find('\n', m.end())]
            declaration = re.match(r'\s*(extern\s+)?(static\s+)?[A-Za-z_]\w*[\s\*]+(\w+[\s\*]+)*%s\s*\(' % m.group(1),
                                   line) and not re.match(r'\s*return\b', line)
            if n and n < want and not declaration:
                print('%s:%d: %s(%d arguments) takes %d' % (p, t.count('\n', 0, m.start()) + 1, m.group(1), n, want))


def returns_in_register(srcs):
    """The other way round: a function defined void whose result a caller uses. On the EE its
    $v0 still holds what the function's last call returned; on the PC it is garbage."""
    void = set()
    for t in srcs.values():
        void.update(re.findall(r'^(?:static\s+)?void\s+(func_[0-9A-F]{8})\s*\([^;{]*\)\s*\{', t, re.M))
    for p, t in sorted(srcs.items()):
        for m in re.finditer(r'(?<![\w.>])(func_[0-9A-F]{8})\s*\(', t):
            if m.group(1) not in void:
                continue
            before = t[:m.start()].rstrip()
            if not before or before[-1] in ';{}:' or re.search(r'(\belse|\bdo|\)\s*\)|^\s*if\s*\(.*\))$', before[-200:]) \
                    or re.search(r'\b(extern|void|static|int|char)[\s\*]*$', before) or before.endswith(','):
                continue  # a statement (or a declaration, or an argument list we can't judge)
            if before[-1] == ')' and not re.search(r'\(\s*(?:int|unsigned|char|short|void)\s*\**\s*\)$', before):
                continue  # `if (x) func_X();` style
            line = t.count('\n', 0, m.start()) + 1
            print('%s:%d: %s is void but its value is used: %s' % (
                p, line, m.group(1), t[t.rfind('\n', 0, m.start()) + 1:t.find('\n', m.end())].strip()[:100]))


if __name__ == '__main__':
    main()
