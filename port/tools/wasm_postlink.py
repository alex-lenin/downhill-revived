"""Post-link fix for indirect calls whose type differs from the target's (dhd_core.wasm).

The decompiled game calls some function pointers (vtables, callbacks, recompiled jalr) with a
type that differs from the target's definition. MIPS and i686 tolerate this; wasm traps. This
step rewrites the linked module so every call_indirect reaches its target with i686 semantics:

- One shadow table per call type Ts, the size of the original table. Entry i is the original
  entry's function when its type is Ts, otherwise an adapter of type Ts that calls it directly.
  Null entries stay null; the size is the same, so null and out-of-range indices still trap.
- Every call_indirect / return_call_indirect of type Ts on table 0 is patched to its shadow
  table. Only the table-index immediate changes (same LEB width), so a matching call costs
  exactly what it cost before: wasm2c's type check sees the same type, no extra instruction.
- Table 0 is unchanged; the host still calls the module's callbacks through it.
- An entry that is one of build_wasm.py's ABI thunks (func_X__as_<tag>, a pure forward to
  func_X) is unwrapped: the shadow entry is func_X itself or its adapter, because i686 calls
  the definition with the caller's stack, not the thunk's view of it.

Adapter semantics (i686 cdecl, 4-byte stack slots):
- Arguments map by stack slot, not by position: the caller's arguments become a slot list
  (i32/f32 one slot by bits, i64/f64 two slots low then high), the callee reads its parameters
  from the same slots, bit for bit. Extra slots are dropped, as on i686.
- Missing slots are 0. On i686 they are whatever lies above the caller's arguments on its stack
  (its locals, the return address): build-dependent, not reproducible. 0 is the choice
  build_wasm.py's direct-call thunks make too.
- Results: a void caller drops the value. An integer caller of a void (or float) callee gets 0
  (on i686, whatever eax/edx the callee leaves). i64 from i32: edx is 0. An i32 caller of an i64
  callee gets eax (the low half). A float caller of a float callee of the other width gets
  the x87 conversion (f32.demote_f64 / f64.promote_f32). A float caller of an integer or void
  callee reads an empty x87 stack (the SSE-math build keeps it empty): the masked stack
  underflow stores the real indefinite, 0xFFC00000 / 0xFFF8000000000000.
- A small target (body up to CLONE_LIMIT bytes) whose parameters are a prefix of the call's,
  with the same result, is cloned with the extra parameters appended and unread: the same
  code at the cost of a matching call, one call level less than an adapter.

Usage: python port/tools/wasm_postlink.py IN.wasm OUT.wasm [--report FILE.json]
       python port/tools/wasm_postlink.py --check IN.wasm | --check-c dhd_core.c ...  (census only)
From build_wasm.py: wasm_postlink.postlink(src, dst, report=path).
"""
import argparse
import json
import re
import sys
from pathlib import Path

I32, I64, F32, F64, V128 = 0x7F, 0x7E, 0x7D, 0x7C, 0x7B
VALTYPE = {I32: 'i32', I64: 'i64', F32: 'f32', F64: 'f64', V128: 'v128', 0x70: 'funcref', 0x6F: 'externref'}
TAG = {I32: 'i', I64: 'l', F32: 'f', F64: 'd', V128: 'v'}
THUNK = re.compile(r'^(.+)__as_[ilfd]*_[ilfdv]$')
F32_INDEFINITE = 0xFFC00000
F64_INDEFINITE = 0xFFF8000000000000


# ---- LEB128 ------------------------------------------------------------------------------------

def uleb(b, i):
    n = shift = 0
    while True:
        x = b[i]
        i += 1
        n |= (x & 0x7F) << shift
        shift += 7
        if not x & 0x80:
            return n, i


def sleb(b, i):
    n = shift = 0
    while True:
        x = b[i]
        i += 1
        n |= (x & 0x7F) << shift
        shift += 7
        if not x & 0x80:
            if x & 0x40:
                n -= 1 << shift
            return n, i


def enc_u(n, width=0):
    out = bytearray()
    while True:
        x = n & 0x7F
        n >>= 7
        if n or len(out) + 1 < width:
            out.append(x | 0x80)
        else:
            out.append(x)
            return bytes(out)


def enc_s(n):
    out = bytearray()
    while True:
        x = n & 0x7F
        n >>= 7
        if (n == 0 and not x & 0x40) or (n == -1 and x & 0x40):
            out.append(x)
            return bytes(out)
        out.append(x | 0x80)


def vec(items):
    items = list(items)
    return enc_u(len(items)) + b''.join(items)


def name_bytes(s):
    s = s.encode()
    return enc_u(len(s)) + s


def sig_tag(sig):
    params, results = sig
    return ''.join(TAG[t] for t in params) + '_' + (''.join(TAG[t] for t in results) or 'v')


def sig_text(sig):
    params, results = sig
    return '(%s)->%s' % (','.join(VALTYPE[t] for t in params), ','.join(VALTYPE[t] for t in results) or 'void')


# ---- instructions ------------------------------------------------------------------------------
#
# Every opcode the module may contain, with its immediates. An unknown opcode is an error: a
# wrong length would misread every later instruction, so the step refuses rather than guesses.

MEMARG_OPS = set(range(0x28, 0x3F))
NO_IMM = {0x00, 0x01, 0x05, 0x0B, 0x0F, 0x19, 0x1A, 0x1B, 0xD1} | set(range(0x45, 0xC5))


def memarg(b, i):
    align, i = uleb(b, i)
    if align & 0x40:  # multi-memory: a memory index follows
        _, i = uleb(b, i)
    _, i = uleb(b, i)
    return i


def blocktype(b, i):
    if b[i] == 0x40 or b[i] in VALTYPE:
        return i + 1
    _, i = sleb(b, i)
    return i


def instructions(b, i, end):
    """Yields (offset, opcode, sub-opcode or None, immediate offset, next offset)."""
    while i < end:
        at, op = i, b[i]
        i += 1
        sub, imm = None, i
        if op in NO_IMM:
            pass
        elif op in (0x02, 0x03, 0x04, 0x06):
            i = blocktype(b, i)
        elif op in (0x0C, 0x0D, 0x10, 0x12, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0xD2, 0x07, 0x08,
                    0x09, 0x18, 0x14, 0x15):
            _, i = uleb(b, i)
        elif op == 0x0E:
            n, i = uleb(b, i)
            for _ in range(n + 1):
                _, i = uleb(b, i)
        elif op in (0x11, 0x13):
            _, i = uleb(b, i)
            _, i = uleb(b, i)
        elif op == 0x1C:
            n, i = uleb(b, i)
            i += n
        elif op in MEMARG_OPS:
            i = memarg(b, i)
        elif op in (0x3F, 0x40):
            i += 1
        elif op in (0x41, 0x42):
            _, i = sleb(b, i)
        elif op == 0x43:
            i += 4
        elif op == 0x44:
            i += 8
        elif op == 0xD0:
            i += 1
        elif op == 0xFC:
            sub, i = uleb(b, i)
            imm = i
            if sub <= 7:
                pass
            elif sub == 8:
                _, i = uleb(b, i)
                i += 1
            elif sub == 9 or sub == 13:
                _, i = uleb(b, i)
            elif sub == 10:
                i += 2
            elif sub == 11:
                i += 1
            elif sub in (12, 14):
                _, i = uleb(b, i)
                _, i = uleb(b, i)
            elif sub in (15, 16, 17):
                _, i = uleb(b, i)
            else:
                raise ValueError('unknown 0xFC opcode %d at %#x' % (sub, at))
        elif op == 0xFD:
            sub, i = uleb(b, i)
            imm = i
            if sub <= 11 or sub in (92, 93):
                i = memarg(b, i)
            elif sub in (12, 13):
                i += 16
            elif 21 <= sub <= 34:
                i += 1
            elif 84 <= sub <= 91:
                i = memarg(b, i) + 1
        elif op == 0xFE:
            sub, i = uleb(b, i)
            imm = i
            i = i + 1 if sub == 3 else memarg(b, i)
        else:
            raise ValueError('unknown opcode %#x at %#x' % (op, at))
        yield at, op, sub, imm, i


# ---- the module --------------------------------------------------------------------------------

class Module:
    def __init__(self, data):
        data = bytes(data)
        if data[:8] != b'\0asm\x01\0\0\0':
            raise ValueError('not a wasm module (version 1)')
        self.sections = []  # [id, payload, custom name or None]
        i = 8
        while i < len(data):
            sid = data[i]
            size, j = uleb(data, i + 1)
            payload = data[j:j + size]
            cname = None
            if sid == 0:
                n, k = uleb(payload, 0)
                cname = payload[k:k + n].decode()
            self.sections.append([sid, payload, cname])
            i = j + size
        self.types, self.func_types, self.imported_funcs, self.imported_tables = [], [], 0, 0
        self.tables, self.bodies, self.elems, self.names = [], [], [], {}
        for sid, p, cname in self.sections:
            getattr(self, '_parse_%d' % sid, lambda p, c: None)(p, cname)

    def section(self, sid, cname=None):
        for s in self.sections:
            if s[0] == sid and s[2] == cname:
                return s
        return None

    def _parse_1(self, p, _):
        n, i = uleb(p, 0)
        for _ in range(n):
            if p[i] != 0x60:
                raise ValueError('type form %#x (only plain function types are supported)' % p[i])
            np_, i = uleb(p, i + 1)
            params = tuple(p[i:i + np_])
            i += np_
            nr, i = uleb(p, i)
            results = tuple(p[i:i + nr])
            i += nr
            self.types.append((params, results))

    def _parse_2(self, p, _):
        n, i = uleb(p, 0)
        for _ in range(n):
            for _ in range(2):
                ln, i = uleb(p, i)
                i += ln
            kind = p[i]
            i += 1
            if kind == 0:
                t, i = uleb(p, i)
                self.func_types.append(t)
                self.imported_funcs += 1
            elif kind == 1:
                self.imported_tables += 1
                i += 1
                flags = p[i]
                _, i = uleb(p, i + 1)
                if flags & 1:
                    _, i = uleb(p, i)
            elif kind == 2:
                flags = p[i]
                _, i = uleb(p, i + 1)
                if flags & 1:
                    _, i = uleb(p, i)
            elif kind == 3:
                i += 2
            elif kind == 4:
                i += 1
                _, i = uleb(p, i)
            else:
                raise ValueError('import kind %d' % kind)

    def _parse_3(self, p, _):
        n, i = uleb(p, 0)
        for _ in range(n):
            t, i = uleb(p, i)
            self.func_types.append(t)

    def _parse_4(self, p, _):
        n, i = uleb(p, 0)
        for _ in range(n):
            reftype, flags = p[i], p[i + 1]
            lo, i = uleb(p, i + 2)
            hi = None
            if flags & 1:
                hi, i = uleb(p, i)
            self.tables.append((reftype, lo, hi))

    def _parse_9(self, p, _):
        """Active segments fill self.table_init[table]; the others only keep their place."""
        n, i = uleb(p, 0)
        for _ in range(n):
            flags, i = uleb(p, i)
            table, offset, funcs = 0, None, []
            if flags in (2, 6):
                table, i = uleb(p, i)
            if not flags & 1:
                if p[i] != 0x41:
                    raise ValueError('element offset is not i32.const')
                offset, i = sleb(p, i + 1)
                if p[i] != 0x0B:
                    raise ValueError('element offset expression')
                i += 1
            if flags & 3:
                i += 1  # elemkind or reftype
            cnt, i = uleb(p, i)
            for _ in range(cnt):
                if flags & 4:
                    if p[i] == 0xD2:
                        f, i = uleb(p, i + 1)
                    elif p[i] == 0xD0:
                        f, i = None, i + 2
                    else:
                        raise ValueError('element expression %#x' % p[i])
                    if p[i] != 0x0B:
                        raise ValueError('element expression end')
                    i += 1
                else:
                    f, i = uleb(p, i)
                funcs.append(f)
            self.elems.append((flags, table, offset, funcs))

    def _parse_10(self, p, _):
        n, i = uleb(p, 0)
        self.code_count_width = i
        for _ in range(n):
            size, j = uleb(p, i)
            self.bodies.append(p[j:j + size])
            i = j + size

    def _parse_0(self, p, cname):
        if cname != 'name':
            return
        n, i = uleb(p, 0)
        i += n
        while i < len(p):
            kind = p[i]
            size, j = uleb(p, i + 1)
            if kind == 1:
                cnt, k = uleb(p, j)
                for _ in range(cnt):
                    idx, k = uleb(p, k)
                    ln, k = uleb(p, k)
                    self.names[idx] = p[k:k + ln].decode('utf-8', 'replace')
                    k += ln
            i = j + size

    # ---- queries ----

    def ftype(self, f):
        return self.types[self.func_types[f]]

    def name(self, f):
        return self.names.get(f, 'f%d' % f)

    def table_entries(self, t=0):
        size = (self.tables[t - self.imported_tables][1] if t >= self.imported_tables else 0)
        out = [None] * size
        for flags, table, offset, funcs in self.elems:
            if offset is None or table != t:
                continue
            if offset + len(funcs) > size:
                raise ValueError('element segment beyond table %d' % t)
            out[offset:offset + len(funcs)] = funcs
        return out

    def body_code(self, k):
        """(body bytes, offset of the first instruction)."""
        b = self.bodies[k]
        n, i = uleb(b, 0)
        for _ in range(n):
            _, i = uleb(b, i)
            i += 1
        return b, i

    def call_sites(self):
        """[(defined function index k, opcode offset, type, table, table-imm offset, width)]."""
        out = []
        for k in range(len(self.bodies)):
            b, start = self.body_code(k)
            for at, op, sub, imm, nxt in instructions(b, start, len(b)):
                if op in (0x11, 0x13):
                    t, j = uleb(b, imm)
                    table, e = uleb(b, j)
                    out.append((k, at, t, table, j, e - j))
        return out

    def real_target(self, f):
        """The definition behind one of build_wasm.py's ABI thunks, else f."""
        m = THUNK.match(self.names.get(f, ''))
        if not m or f < self.imported_funcs:
            return f
        b, start = self.body_code(f - self.imported_funcs)
        calls = []
        for at, op, sub, imm, nxt in instructions(b, start, len(b)):
            if op in (0x10, 0x12):
                calls.append(uleb(b, imm)[0])
            elif op in (0x11, 0x13, 0x23, 0x24) or op in MEMARG_OPS or op in (0xFC, 0xFE):
                return f  # not a pure forward
        if len(calls) == 1 and self.names.get(calls[0]) == m.group(1):
            return calls[0]
        return f

    # ---- writing ----

    def encode(self, drop_debug=False):
        out = bytearray(b'\0asm\x01\0\0\0')
        for sid, p, cname in self.sections:
            if drop_debug and sid == 0 and cname and cname.startswith('.debug'):
                continue
            out.append(sid)
            out += enc_u(len(p)) + p
        return bytes(out)


# ---- adapters ----------------------------------------------------------------------------------

def caller_slots(params):
    """Bytecode that pushes each i32 stack slot of the caller's arguments."""
    slots = []
    for k, t in enumerate(params):
        get = b'\x20' + enc_u(k)
        if t == I32:
            slots.append(get)
        elif t == F32:
            slots.append(get + b'\xBC')  # i32.reinterpret_f32
        elif t in (I64, F64):
            wide = get + (b'\xBD' if t == F64 else b'')  # i64.reinterpret_f64
            slots.append(wide + b'\xA7')  # low half: i32.wrap_i64
            slots.append(wide + b'\x42\x20\x88\xA7')  # high half: i64.const 32, i64.shr_u, wrap
        else:
            raise ValueError('no i686 slot mapping for %s' % VALTYPE[t])
    return slots


def adapter_body(caller, callee_index, callee):
    """The body of an adapter of type `caller` that calls callee_index of type `callee`."""
    (cp, cr), (dp, dr) = caller, callee
    if len(cr) > 1 or len(dr) > 1:
        raise ValueError('multi-value results')
    slots = caller_slots(cp)
    zero = b'\x41\x00'

    def slot(k):
        return slots[k] if k < len(slots) else zero

    code, k = bytearray(), 0
    for t in dp:
        if t == I32:
            code += slot(k)
            k += 1
        elif t == F32:
            code += slot(k) + b'\xBE'  # f32.reinterpret_i32
            k += 1
        elif t in (I64, F64):
            code += slot(k) + b'\xAD' + slot(k + 1) + b'\xAD\x42\x20\x86\x84'  # lo | hi << 32
            if t == F64:
                code += b'\xBF'  # f64.reinterpret_i64
            k += 2
        else:
            raise ValueError('no i686 slot mapping for %s' % VALTYPE[t])
    code += b'\x10' + enc_u(callee_index)
    r = cr[0] if cr else None
    e = dr[0] if dr else None
    if r == e:
        pass
    elif r is None:
        code += b'\x1A'
    elif r == I32 and e == I64:
        code += b'\xA7'
    elif r == I64 and e == I32:
        code += b'\xAD'  # edx: 0
    elif r == F32 and e == F64:
        code += b'\xB6'
    elif r == F64 and e == F32:
        code += b'\xBB'
    else:
        if e is not None:
            code += b'\x1A'
        if r == I32:
            code += b'\x41\x00'  # i32.const 0
        elif r == I64:
            code += b'\x42\x00'  # i64.const 0
        elif r == F32:
            code += b'\x43' + F32_INDEFINITE.to_bytes(4, 'little')
        elif r == F64:
            code += b'\x44' + F64_INDEFINITE.to_bytes(8, 'little')
        else:
            raise ValueError('result %s' % VALTYPE[r])
    return b'\x00' + bytes(code) + b'\x0B'


CLONE_LIMIT = 256  # bytes of target body: a smaller one is cloned instead of called


def clone_body(m, g, caller, table_of):
    """A copy of g's body that takes the caller's arguments, when g's parameters are a prefix of
    them and the results are the same: the extra arguments become unused parameters, as i686
    leaves them unread. None when that does not apply. The same instructions run, one call
    level less than an adapter that calls g. The clone's own call_indirect sites move to their
    shadow tables, as g's do."""
    (cp, cr), (dp, dr) = caller, m.ftype(g)
    if g < m.imported_funcs or cr != dr or tuple(cp[:len(dp)]) != tuple(dp) or len(cp) == len(dp):
        return None
    b = m.bodies[g - m.imported_funcs]
    if len(b) > CLONE_LIMIT:
        return None
    b, start = m.body_code(g - m.imported_funcs)
    extra, out, last = len(cp) - len(dp), bytearray(b[:start]), start
    for at, op, sub, imm, nxt in instructions(b, start, len(b)):
        if op in (0x20, 0x21, 0x22):
            idx, _ = uleb(b, imm)
            if idx >= len(dp):  # a declared local: after the new parameters
                out += b[last:imm] + enc_u(idx + extra)
                last = nxt
        elif op in (0x11, 0x13):
            t, j = uleb(b, imm)
            table, e = uleb(b, j)
            if table == 0 and t in table_of:
                out += b[last:j] + enc_u(table_of[t], e - j)
                last = e
    return bytes(out + b[last:])


# ---- the step ----------------------------------------------------------------------------------

def plan(m):
    """What the step does to module m: shadow tables, adapters, sites. No mutation."""
    entries = m.table_entries(0)
    sites = [s for s in m.call_sites() if s[3] == 0]
    site_types = sorted({s[2] for s in sites})
    real = {f: m.real_target(f) for f in set(entries) if f is not None}
    shadows, adapters = {}, {}
    for ts in site_types:
        sig = m.types[ts]
        if V128 in sig[0] or V128 in sig[1]:
            continue  # no i686 slot mapping; left on table 0 and reported
        row = []
        for f in entries:
            if f is None:
                row.append(None)
                continue
            g = real[f]
            if m.func_types[g] == ts or m.ftype(g) == sig:
                row.append(g)
            else:
                key = (ts, g)
                if key not in adapters:
                    adapters[key] = len(adapters)
                row.append(key)
        if row != entries:
            shadows[ts] = row
    return entries, sites, real, shadows, adapters


def apply(m):
    """Rewrites m in place. Returns the report dictionary."""
    entries, sites, real, shadows, adapters = plan(m)
    nfuncs = len(m.func_types)
    nimports = m.imported_funcs
    first_table = m.imported_tables + len(m.tables)
    table_of = {ts: first_table + k for k, ts in enumerate(sorted(shadows))}
    adapter_index = {key: nfuncs + k for key, k in adapters.items()}
    adapter_names = {}
    for (ts, g), k in adapters.items():
        adapter_names[nfuncs + k] = '%s__icall_%s' % (m.name(g), sig_tag(m.types[ts]))

    # 1. sites: the table-index immediate, same LEB width
    bodies = [bytearray(b) for b in m.bodies]
    patched = 0
    for k, at, t, table, imm, width in sites:
        if t not in table_of:
            continue
        new = table_of[t]
        if width == 1 and new > 0x7F:
            raise ValueError('shadow table index %d needs a wider LEB at %s' % (new, m.name(nimports + k)))
        bodies[k][imm:imm + width] = enc_u(new, width)
        patched += 1
    # 2. adapters: function section entries, code bodies, names
    new_bodies, clones = [], 0
    for (ts, g), k in sorted(adapters.items(), key=lambda kv: kv[1]):
        body = clone_body(m, g, m.types[ts], table_of)
        clones += body is not None
        new_bodies.append(body or adapter_body(m.types[ts], g, m.ftype(g)))
    count_width_before = len(enc_u(len(m.bodies)))
    all_bodies = [bytes(b) for b in bodies] + new_bodies
    for s in m.sections:
        sid = s[0]
        if sid == 3:
            n, i = uleb(s[1], 0)
            extra = [enc_u(ts) for (ts, g), k in sorted(adapters.items(), key=lambda kv: kv[1])]
            s[1] = enc_u(n + len(extra)) + s[1][i:] + b''.join(extra)
        elif sid == 4:
            n, i = uleb(s[1], 0)
            reftype, lo, hi = m.tables[0]
            limits = (b'\x01' + enc_u(lo) + enc_u(hi)) if hi is not None else (b'\x00' + enc_u(lo))
            extra = [bytes([0x70]) + limits for _ in table_of]
            s[1] = enc_u(n + len(extra)) + s[1][i:] + b''.join(extra)
        elif sid == 9:
            n, i = uleb(s[1], 0)
            segs = []
            for ts in sorted(shadows):
                row = shadows[ts]
                k = 0
                while k < len(row):
                    if row[k] is None:
                        k += 1
                        continue
                    start = k
                    while k < len(row) and row[k] is not None:
                        k += 1
                    funcs = [adapter_index[x] if isinstance(x, tuple) else x for x in row[start:k]]
                    segs.append(b'\x02' + enc_u(table_of[ts]) + b'\x41' + enc_s(start) + b'\x0B' + b'\x00' +
                                vec(enc_u(f) for f in funcs))
            s[1] = enc_u(n + len(segs)) + s[1][i:] + b''.join(segs)
        elif sid == 10:
            s[1] = vec(enc_u(len(b)) + b for b in all_bodies)
        elif sid == 0 and s[2] == 'name' and adapter_names:
            s[1] = rename(s[1], adapter_names)
    m.bodies = all_bodies
    m.func_types += [ts for (ts, g), k in sorted(adapters.items(), key=lambda kv: kv[1])]
    m.tables += [m.tables[0]] * len(table_of)
    m.names.update(adapter_names)
    drop_debug = len(enc_u(len(all_bodies))) != count_width_before
    unwrapped = sorted({f for f, g in real.items() if g != f})
    by_type = {}
    for ts, row in shadows.items():
        kinds = {}
        for x in row:
            if isinstance(x, tuple):
                kinds[sig_text(m.ftype(x[1]))] = kinds.get(sig_text(m.ftype(x[1])), 0) + 1
        by_type[sig_text(m.types[ts])] = {
            'table': table_of[ts], 'sites': sum(1 for s in sites if s[2] == ts),
            'entries': sum(1 for x in row if x is not None),
            'direct': sum(1 for x in row if x is not None and not isinstance(x, tuple)),
            'adapted_by_target_type': dict(sorted(kinds.items(), key=lambda kv: -kv[1]))}
    unmapped = sorted({sig_text(m.types[s[2]]) for s in sites if s[2] not in table_of
                       and any(f is not None and m.ftype(real[f]) != m.types[s[2]] for f in entries)})
    return {'sites': len(sites), 'patched_sites': patched, 'shadow_tables': len(table_of),
            'adapters': len(adapters), 'cloned_targets': clones, 'thunk_entries_unwrapped': len(unwrapped),
            'debug_sections_dropped': drop_debug, 'types': by_type, 'unmapped_types': unmapped,
            '_drop_debug': drop_debug}


def rename(payload, extra):
    """The name section with `extra` {index: name} appended to the function-name map."""
    n, i = uleb(payload, 0)
    out = bytearray(payload[:i + n])
    i += n
    done = False
    while i < len(payload):
        kind = payload[i]
        size, j = uleb(payload, i + 1)
        body = payload[j:j + size]
        if kind == 1:
            cnt, k = uleb(body, 0)
            body = enc_u(cnt + len(extra)) + body[k:] + b''.join(
                enc_u(idx) + name_bytes(nm) for idx, nm in sorted(extra.items()))
            done = True
        out += bytes([kind]) + enc_u(len(body)) + body
        i = j + size
    if not done:
        raise ValueError('name section has no function names')
    return bytes(out)


def check(m):
    """Static type-trap census: per call_indirect site, whether its table holds an entry of
    another type. Returns {'sites', 'no_compatible_entry', 'mixed_table', 'clean'} counts and
    the type rows."""
    tables = {}
    rows = {}
    for k, at, t, table, imm, width in m.call_sites():
        if table not in tables:
            tables[table] = [m.func_types[f] if f is not None else None for f in m.table_entries(table)]
        types = tables[table]
        sig = m.types[t]
        compatible = sum(1 for x in types if x is not None and m.types[x] == sig)
        other = sum(1 for x in types if x is not None and m.types[x] != sig)
        r = rows.setdefault((sig_text(sig), table), {'sites': 0, 'compatible': compatible, 'other': other})
        r['sites'] += 1
    no_entry = sum(r['sites'] for r in rows.values() if r['compatible'] == 0)
    mixed = sum(r['sites'] for r in rows.values() if r['compatible'] and r['other'])
    total = sum(r['sites'] for r in rows.values())
    return {'sites': total, 'no_compatible_entry': no_entry, 'mixed_table': mixed,
            'clean': total - no_entry - mixed,
            'rows': [dict(type=k[0], table=k[1], **v) for k, v in sorted(rows.items(), key=lambda kv: -kv[1]['sites'])]}


def check_c(paths):
    """The same census on WABT 1.0.42 wasm2c output (one file or the split units): each
    CALL_INDIRECT's function type against every RefFunc type its table is initialised with.
    Types compare by their 32-byte hash, as CHECK_CALL_INDIRECT does."""
    text = '\n'.join(Path(p).read_text() for p in paths)
    hashes = {n: re.sub(r'\s', '', h) for n, h in re.findall(
        r'static const u8 (w2c_\w+_t\d+)\[\] = \{([^}]*)\}', text)}
    segs = {n: re.findall(r'\{RefFunc, (w2c_\w+?_t\d+),', body) for n, body in re.findall(
        r'static const wasm_elem_segment_expr_t (elem_segment_exprs_\w+)\[\] = \{(.*?)\n\};', text, re.S)}
    tables = {}
    for t, seg in re.findall(r'funcref_table_init\(&instance->(w2c_\w+), (elem_segment_exprs_\w+),', text):
        tables.setdefault(t, set()).update(hashes.get(x, x) for x in segs.get(seg, ()))
    rows = {}
    for t, ft in re.findall(r'(?<![\w])(?:CHECK_)?CALL_INDIRECT\(instance->(w2c_\w+), (?:[^\n]*?, )??(w2c_\w+?_t\d+), ', text):
        h = hashes.get(ft, ft)
        r = rows.setdefault((ft, t), {'sites': 0, 'compatible': sum(1 for x in tables.get(t, ()) if x == h),
                                      'other_types': sum(1 for x in tables.get(t, ()) if x != h)})
        r['sites'] += 1
    return {'sites': sum(r['sites'] for r in rows.values()),
            'no_compatible_entry': sum(r['sites'] for r in rows.values() if not r['compatible']),
            'mixed_table': sum(r['sites'] for r in rows.values() if r['compatible'] and r['other_types']),
            'tables': len(tables)}


def postlink(src, dst, report=None):
    """Reads src, writes the fixed module to dst (may be the same path). Returns the report."""
    m = Module(Path(src).read_bytes())
    before = check(m)
    result = apply(m)
    data = m.encode(drop_debug=result.pop('_drop_debug'))
    after = check(Module(data))
    result['before'], result['after'] = before, after
    if after['no_compatible_entry'] or after['mixed_table']:
        raise RuntimeError('call_indirect sites still reach entries of another type: %s' % after['rows'])
    Path(dst).write_bytes(data)
    if report:
        Path(report).write_text(json.dumps(result, indent=1))
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('src', type=Path, nargs='?')
    p.add_argument('dst', type=Path, nargs='?')
    p.add_argument('--report', type=Path, help='JSON report')
    p.add_argument('--check', action='store_true', help='only print the static type-trap census of src')
    p.add_argument('--check-c', type=Path, nargs='+', metavar='C', help='only print the census of wasm2c output')
    a = p.parse_args()
    if a.check_c:
        print(json.dumps(check_c(a.check_c), indent=1))
        return
    if a.check:
        print(json.dumps(check(Module(a.src.read_bytes())), indent=1))
        return
    r = postlink(a.src, a.dst, a.report)
    print('wasm_postlink: %d call_indirect sites, %d patched onto %d shadow tables, %d adapters, '
          '%d thunk entries unwrapped; type traps before: %d sites with no compatible entry, %d on mixed '
          'tables; after: %d, %d' % (r['sites'], r['patched_sites'], r['shadow_tables'], r['adapters'],
                                      r['thunk_entries_unwrapped'], r['before']['no_compatible_entry'],
                                      r['before']['mixed_table'], r['after']['no_compatible_entry'],
                                      r['after']['mixed_table']))


if __name__ == '__main__':
    sys.exit(main())
