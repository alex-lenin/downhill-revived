"""Add the parentheses WGSL's grammar requires where naga accepts an expression without them.

    python port/tools/wgsl_parens.py port/gs/raster.wgsl [--check]

naga (wgpu) parses binary operators with C's precedence: || < && < | < ^ < & < == != < relational
< shifts < + - < * / %. The WGSL grammar (Tint, so Chrome) is stricter: the operands of & | ^ and
of << >> must be unary expressions, a relational operand cannot be a comparison or a bitwise
expression, and && and || cannot take each other or a bitwise expression unparenthesized
(finding 084, section 12). This parses each expression with naga's precedence and puts
parentheses around every operand the grammar would reject, exactly as naga grouped it, so the
meaning is unchanged; nothing else in the file changes. --check: report, write nothing (exit 1
when something would change).
"""
import re
import sys

TOKEN = re.compile(r'''
    (?P<space>\s+) | (?P<comment>//[^\n]*) |
    (?P<number>0[xX][0-9a-fA-F]+[iu]?|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?[iuf]?) |
    (?P<ident>[A-Za-z_]\w*) |
    (?P<op><<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||->|\+\+|--|[-+*/%&|^]=|[-+*/%&|^~!<>=(){}\[\],;:.@])
''', re.X)
TEMPLATED = {'vec2', 'vec3', 'vec4', 'mat2x2', 'mat3x3', 'mat4x4', 'array', 'bitcast', 'ptr', 'atomic', 'var',
             'texture_storage_2d', 'texture_2d'}
LEVEL = {'||': 1, '&&': 2, '|': 3, '^': 4, '&': 5, '==': 6, '!=': 6, '<': 7, '>': 7, '<=': 7, '>=': 7,
         '<<': 8, '>>': 8, '+': 9, '-': 9, '*': 10, '/': 10, '%': 10}
CLASS = {1: 'or', 2: 'and', 3: 'bit', 4: 'bit', 5: 'bit', 6: 'rel', 7: 'rel', 8: 'shift', 9: 'add', 10: 'mul'}
STARTS = {'=', 'return', '(', ',', '[', '+=', '-=', '*=', '/=', '%=', '|=', '&=', '^=', '<<=', '>>='}
UNARY = {'-', '!', '~', '&', '*'}


def tokens(text):
    out, pos = [], 0
    while pos < len(text):
        m = TOKEN.match(text, pos)
        if not m:
            raise SyntaxError('cannot tokenize at %d: %r' % (pos, text[pos:pos + 20]))
        if m.lastgroup not in ('space', 'comment'):
            out.append((m.group(), m.start(), m.end()))
        pos = m.end()
    return out


class Parser:
    def __init__(self, toks):
        self.t = toks
        self.wraps = []  # (start, end) character spans to parenthesize

    def peek(self, i):
        return self.t[i][0] if i < len(self.t) else None

    def skip_template(self, i):
        """i at '<' after a templated name: the index after the matching '>'."""
        depth = 0
        while i < len(self.t):
            v = self.t[i][0]
            if v == '<':
                depth += 1
            elif v == '>':
                depth -= 1
            elif v == '>>':
                depth -= 2
            i += 1
            if depth <= 0:
                return i
        raise SyntaxError('unclosed template')

    def args(self, i, close):
        """i after an opening bracket: parse comma-separated expressions up to close."""
        while self.peek(i) != close:
            i, _ = self.expr(i)
            if self.peek(i) == ',':
                i += 1
            elif self.peek(i) != close:
                raise SyntaxError('expected %s at char %d, got %r' % (close, self.t[i][1], self.peek(i)))
        return i + 1

    def primary(self, i):
        v = self.peek(i)
        if v in UNARY:
            j, _ = self.primary(i + 1)
            return j, None
        if v == '(':
            return self.args(i + 1, ')'), None
        if v is None:
            raise SyntaxError('unexpected end')
        i += 1
        if v in TEMPLATED and self.peek(i) == '<':
            i = self.skip_template(i)
        while True:  # postfix: calls, members, indexing
            n = self.peek(i)
            if n == '(':
                i = self.args(i + 1, ')')
            elif n == '[':
                i = self.args(i + 1, ']')
            elif n == '.':
                i += 2
            else:
                return i, None

    def expr(self, i, minimum=1):
        """Precedence climbing: returns (end index, operator level of the top node or None)."""
        start = i
        i, top = self.primary(i)
        left = (start, i, top)
        while True:
            op = self.peek(i)
            level = LEVEL.get(op)
            if level is None or level < minimum:
                return i, left[2]
            right_start = i + 1
            i, right_top = self.expr(right_start, level + 1)
            self.check(op, left, (right_start, i, right_top), level)
            left = (start, i, level)

    def check(self, op, left, right, level):
        parent = CLASS[level]
        for side, (a, b, child) in (('left', left), ('right', right)):
            if child is None:
                continue
            kind = CLASS[child]
            same = side == 'left' and child == level
            if parent == 'bit':
                ok = same and self.t[a:b] and self.op_at(a, b, level) == op
            elif parent == 'shift':
                ok = False
            elif parent == 'add':
                ok = kind == 'mul' or (side == 'left' and kind == 'add')
            elif parent == 'mul':
                ok = side == 'left' and kind == 'mul'
            elif parent == 'rel':
                ok = kind in ('shift', 'add', 'mul')
            else:  # 'and', 'or'
                ok = kind in ('rel', 'shift', 'add', 'mul') or same
            if not ok:
                self.wraps.append((self.t[a][1], self.t[b - 1][2]))

    def op_at(self, a, b, level):
        """The top-level operator of the left operand (same precedence level): its last one."""
        depth, found = 0, None
        for v, _, _ in self.t[a:b]:
            if v in '([':
                depth += 1
            elif v in ')]':
                depth -= 1
            elif depth == 0 and LEVEL.get(v) == level:
                found = v
        return found


def fix(text):
    toks = tokens(text)
    p = Parser(toks)
    i = 0
    while i < len(toks):
        v = toks[i][0]
        if v in STARTS and i + 1 < len(toks) and toks[i + 1][0] not in (')', ']', ';'):
            if v == '(' and i > 0 and toks[i - 1][0] in ('workgroup_size', 'builtin', 'binding', 'group', 'location'):
                i += 1
                continue
            try:
                i, _ = p.expr(i + 1)
                continue
            except SyntaxError:
                pass  # a declaration or other non-expression after '(' or ','
        i += 1
    inserts = [(a, 0, -(b - a), '(') for a, b in p.wraps] + [(b, 1, b - a, ')') for a, b in p.wraps]
    out, last = [], 0
    for pos, _, _, s in sorted(set(inserts)):
        out.append(text[last:pos]); out.append(s); last = pos
    out.append(text[last:])
    return ''.join(out), len(set(p.wraps))


def main():
    path = sys.argv[1]
    text = open(path, encoding='utf-8', newline='').read()
    new, count = fix(text)
    if new != text:
        changed = sum(1 for a, b in zip(text.splitlines(), new.splitlines()) if a != b)
        print('%s: %d parentheses on %d lines' % (path, count, changed))
        if '--check' in sys.argv:
            sys.exit(1)
        open(path, 'w', encoding='utf-8', newline='').write(new)
    else:
        print('%s: nothing to change' % path)


if __name__ == '__main__':
    main()
