"""Compile the game's VU0 microprograms (vcallms entries) to C, one function per entry (finding 090 SP1).

    python port/tools/vu0_aot.py build/wasm/vu0aot/vu0_aot.inc

build_wasm.py generates it into its build folder; the output holds game-derived code and stays
under build/. vu0_interp.c includes it with -DPS2_VU0_COMPILED.

The interpreter (vu0_interp.c run()) models VU0 timing with run-time state: the cycle, when each VF
register is ready, the 4-cycle MAC/status flag queue. At a vcallms that state always starts the
same way (cycle 0, nothing pending), and in this package every branch condition the timing could
depend on is either a known VI value (BAL/JR, IADDIU constants) or splits the code into separate
paths. So this generator runs the interpreter's timing model here, at build time, along every path
from each entry, and writes C that does only the arithmetic:
- stalls, ready times and the flag queue are resolved statically: each flag read (FMAND, FSAND, ...)
  names the FMAC whose MAC it sees, and only those MACs are built (lazy flags);
- the sticky status bits are two vector ORs per FMAC, and the final MAC/status come from the last
  FMAC, as drain_flags leaves them;
- VF/ACC/VI/I live in C locals and are written back once, at the end of each path;
- a branch on a run-time VI value forks the code (if/else) at the branch, delay slot included.
Arithmetic follows the interpreter's VU0 path exactly: operands go through arithmetic4 (the
interpreter's per-lane arithmetic(): denormals to signed zero, exponent 255 to the largest finite),
then one SSE operation each, in the same order (MADD/MSUB: ACC +- (a * b)); MAX/MINI/MOVE/MR32/ABS
keep raw bits; ITOF/FTOI are the interpreter's own scalar expressions. MAC flags are the Z and S
lanes (fmac_flags4, as the native routines in vu0_native.h): under the VU mode results are finite
and normal or zero, so U and O are never set (the same reason as there; vu0c_diff checks it on
edge values).

An entry is compiled only if every instruction on every path is one this generator models; any
other (DIV/SQRT/RSQRT/WAITQ, CLIP and the clip flags, FSSET/FSEQ/FSOR and sticky reads, R, memory at
a run-time address, JR to a run-time address, more than PATH_BUDGET pairs) leaves that entry on the
interpreter. A compiled entry runs only where VU0 micro memory holds the exact words of every pair
it can reach (checked once per ps2_vu0_code_version, vu0_interp.c vu0_compiled_code); its words are
filled from the game's .vutext at image load (ps2_vu_aot_load), never kept in the executable.
No upstream implementation is copied.
"""
from pathlib import Path
import copy
import os
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from vu1_aot import program, fills, WORDS_MACRO, fill_function  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
PACKAGE = 0x26CF80          # the game's one VU0 package (finding 019)
FMAC_LATENCY = FLAG_LATENCY = 4
PATH_BUDGET = 4000          # pairs emitted per entry, over all paths
MASK = 0x1FF                # VU0 micro memory: 512 pairs

ADD, SUB, MADD, MSUB, MAX, MINI, MUL = range(7)
KIND_NAME = ['ADD', 'SUB', 'MADD', 'MSUB', 'MAX', 'MINI', 'MUL']


class Reject(Exception):
    pass


def sext(v, bits):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) & 1 else v


def decode_upper(hi):
    """The interpreter's upper() for VU0, as data: (kind, source, bc, fs, ft, dest reg or 'acc', d,
    stall registers, sets MAC flags). kind is an FMAC kind, or 'NOP', 'ITOF', 'FTOI', 'ABS'.
    source: 0 field bc of ft, 1 Q, 2 I, 3 ft."""
    op, d = hi & 0x3F, hi >> 21 & 15
    ft, fs, fd = hi >> 16 & 31, hi >> 11 & 31, hi >> 6 & 31
    if op >= 0x3C:
        sub, bc = (fd << 2) | (op & 3), op & 3
        if sub == 0x2F:
            return dict(kind='NOP', stalls=[fs])
        if 0x10 <= sub <= 0x13:
            return dict(kind='ITOF', bc=bc, fs=fs, reg=ft, d=d, stalls=[fs], flags=False)
        if 0x14 <= sub <= 0x17:
            return dict(kind='FTOI', bc=bc, fs=fs, reg=ft, d=d, stalls=[fs], flags=False)
        if sub == 0x1D:
            return dict(kind='ABS', fs=fs, reg=ft, d=d, stalls=[fs], flags=False)
        if sub == 0x1F:
            raise Reject('CLIP')
        if sub < 0x10 or 0x18 <= sub < 0x1C:
            kind, source, stalls = (sub >> 2) if sub < 0x10 else MUL, 0, [fs, ft]
        elif sub in (0x1C, 0x20, 0x21, 0x24, 0x25):
            kind, source, stalls = {0x1C: MUL, 0x20: ADD, 0x21: MADD, 0x24: SUB, 0x25: MSUB}[sub], 1, [fs]
        elif sub in (0x1E, 0x22, 0x23, 0x26, 0x27):
            kind, source, stalls = {0x1E: MUL, 0x22: ADD, 0x23: MADD, 0x26: SUB, 0x27: MSUB}[sub], 2, [fs]
        elif sub in (0x28, 0x29, 0x2A, 0x2C, 0x2D):
            kind, source, stalls = {0x28: ADD, 0x29: MADD, 0x2A: MUL, 0x2C: SUB, 0x2D: MSUB}[sub], 3, [fs, ft]
        else:
            raise Reject('upper %08X' % hi)  # OPMULA and undefined
        return dict(kind=kind, source=source, bc=bc, fs=fs, ft=ft, reg='acc', d=d, stalls=stalls + [fs], flags=True)
    bc = op & 3
    if op < 0x1C:
        kind, source, stalls = op >> 2, 0, [ft]
    elif op in (0x1C, 0x20, 0x21, 0x24, 0x25):
        kind, source, stalls = {0x1C: MUL, 0x20: ADD, 0x21: MADD, 0x24: SUB, 0x25: MSUB}[op], 1, []
    elif op in (0x1D, 0x1E, 0x1F, 0x22, 0x23, 0x26, 0x27):
        kind, source, stalls = {0x1D: MAX, 0x1E: MUL, 0x1F: MINI, 0x22: ADD, 0x23: MADD, 0x26: SUB, 0x27: MSUB}[op], 2, []
    elif 0x28 <= op <= 0x2F and op != 0x2E:
        kind, source, stalls = [ADD, MADD, MUL, MAX, SUB, MSUB, None, MINI][op - 0x28], 3, [ft]
    else:
        raise Reject('upper %08X' % hi)  # OPMSUB
    return dict(kind=kind, source=source, bc=bc, fs=fs, ft=ft, reg=fd, d=d, stalls=stalls + [fs],
                flags=kind not in (MAX, MINI))


BRANCHES = (0x20, 0x21, 0x24, 0x25, 0x28, 0x29, 0x2C, 0x2D, 0x2E, 0x2F)


class Path_:
    """One path's static state: the interpreter's timing state, what is known of VI, and the C names
    holding the current VF/ACC values."""

    def __init__(self):
        self.cycle = 0
        self.flag_cycle = 0
        self.vf_ready = [0] * 32
        self.queue = []            # MAC updates: (at, writer index)
        self.vi = {0: 0}           # register -> known value (uint16)
        self.vf = {}               # register -> C name of its current value
        self.vf_dirty = set()
        self.acc = None
        self.acc_dirty = False
        self.i_bits = None         # I as set by LOI on this path (known bits)
        self.i_dirty = False
        self.vi_dirty = set()
        self.writers = []          # (C name of the result, dest) of every MAC writer so far
        # C names whose value the C compiler knows (VF0, LOI constants and what is made of them
        # without rounding): an operation on two of them must not be folded at compile time,
        # where the rounding mode is round-to-nearest, not the VU's toward zero.
        self.const = {'VU0C_VF0'}
        self.pc = 0
        self.branch = -1
        self.ending = False
        self.steps = 0


class Generator:
    def __init__(self, pairs):
        self.pairs = pairs
        self.n = 0
        self.reached = set()
        self.emitted = 0
        self.uses_i = False
        self.uses_q = False

    def name(self, prefix):
        self.n += 1
        return '%s%d' % (prefix, self.n)

    # ---- values ----
    def vf_read(self, p, out, r):
        if r == 0:
            return 'VU0C_VF0'
        if r not in p.vf:
            v = self.name('f%d_' % r)
            out.append('__m128 %s = _mm_loadu_ps(c->vf[%d].f);' % (v, r))
            p.vf[r] = v
        return p.vf[r]

    def acc_read(self, p, out):
        if p.acc is None:
            p.acc = self.name('acc')
            out.append('__m128 %s = _mm_loadu_ps(c->vacc);' % p.acc)
        return p.acc

    def vf_write(self, p, out, r, d, value):
        """write_vf / write_vf_bits: the fields in d of r (nothing for VF0)."""
        if r == 0:
            return
        if d != 15:
            old = self.vf_read(p, out, r)
            v = self.name('f%d_' % r)
            out.append('__m128 %s = vu0c_blend(%s, %s, %d);' % (v, old, value, d))
            if old in p.const and value in p.const:
                p.const.add(v)
            value = v
        p.vf[r] = value
        p.vf_dirty.add(r)

    def ready(self, p, r, at):
        if r and at > p.vf_ready[r]:
            p.vf_ready[r] = at

    def stall(self, p, r):
        if r and p.vf_ready[r] > p.cycle:
            p.cycle = p.vf_ready[r]

    def i_value(self, p):
        self.uses_i = True
        if p.i_bits is not None:
            v = '_mm_castsi128_ps(_mm_set1_epi32((int)0x%08Xu))' % p.i_bits
            p.const.add(v)
            return v
        return 'vu0c_i'

    def mac_at_read(self, p):
        """(C expression of the MAC a flag read sees now, whether an FMAC of this run is its source)."""
        landed = [w for at, w in p.queue if at <= p.flag_cycle]
        if not landed:
            return 'mac_in', False
        name, d = p.writers[landed[-1]]
        return 'vu0c_mac(%s, %d)' % (name, d), True

    # ---- one pair ----
    def upper(self, p, out, hi):
        """Returns (C name of the result, decoded upper) or (None, decoded) for NOP; emits nothing
        that writes state (the commit comes after the lower instruction)."""
        u = decode_upper(hi)
        for r in u['stalls']:
            self.stall(p, r)
        kind = u['kind']
        if kind == 'NOP':
            return None, u
        a = self.vf_read(p, out, u['fs'])
        res = self.name('u')
        if kind == 'ITOF':  # int to float rounds: no folding
            out.append('__m128 %s = vu0c_itof(%s, %d, %d);' % (res, a, u['bc'], a in p.const))
            return res, u
        if kind == 'FTOI':  # an exact scaling and a truncation
            out.append('__m128 %s = vu0c_ftoi(%s, %d);' % (res, a, u['bc']))
            if a in p.const:
                p.const.add(res)
            return res, u
        if kind == 'ABS':
            out.append('__m128 %s = _mm_and_ps(%s, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)));' % (res, a))
            if a in p.const:
                p.const.add(res)
            return res, u
        source = u['source']
        if source == 0:
            ft = self.vf_read(p, out, u['ft'])
            b, const_b = 'lane4(%s, %d)' % (ft, u['bc']), ft in p.const
        elif source == 1:
            self.uses_q = True
            b, const_b = 'vu0c_q', False
        elif source == 2:
            b = self.i_value(p)
            const_b = b in p.const
        else:
            b = self.vf_read(p, out, u['ft'])
            const_b = b in p.const
        if kind in (MAX, MINI):  # selects bits: no rounding
            out.append('__m128 %s = minmax4(%s, %s, %d);' % (res, a, b, 1 if kind == MAX else 0))
            if a in p.const and const_b:
                p.const.add(res)
            return res, u
        # Both factors known when the C is compiled: keep the operation for run time, under the VU
        # rounding mode, instead of letting the compiler fold it.
        opaque = 1 if a in p.const and const_b else 0
        if kind in (MADD, MSUB):
            acc = self.acc_read(p, out)
            out.append('__m128 %s = vu0c_%s(%s, %s, %s, %d);' % (res, 'madd' if kind == MADD else 'msub', acc, a, b, opaque))
        else:
            out.append('__m128 %s = vu0c_%s(%s, %s, %d);' % (res, KIND_NAME[kind].lower(), a, b, opaque))
        return res, u

    def lower(self, p, out, lo, pc):
        """The interpreter's lower() for VU0: returns the branch target (instruction index), -1, or
        ('fork', C condition, target) for a branch on a run-time value."""
        op = lo >> 25
        it, is_, d = lo >> 16 & 31, lo >> 11 & 31, lo >> 21 & 15
        imm11 = sext(lo, 11)
        target = pc + 1 + imm11
        vt, vs = it & 15, is_ & 15

        def vi_expr(r):
            return '%du' % p.vi[r] if r in p.vi else 'vi[%d]' % r

        def set_vi(r, expr, known=None):
            r &= 15
            if not r:
                return
            out.append('vi[%d] = (uint16_t)(%s);' % (r, expr))
            p.vi_dirty.add(r)
            if known is None:
                p.vi.pop(r, None)
            else:
                p.vi[r] = known & 0xFFFF

        def known(*rs):
            return all(r in p.vi for r in rs)

        def static_address(r, offset):
            if r not in p.vi:
                raise Reject('memory at a run-time address')
            q = (p.vi[r] + offset) & 0xFFFFFFFF
            if q & 0x400:
                raise Reject('VU1 register file')
            return q & 0xFF

        if op == 0x40:
            sub = lo & 0x3F
            if sub < 0x3C:
                id_ = lo >> 6 & 31
                if sub == 0x30:
                    set_vi(id_, '%s + %s' % (vi_expr(vs), vi_expr(vt)), p.vi[vs] + p.vi[vt] if known(vs, vt) else None)
                elif sub == 0x31:
                    set_vi(id_, '%s - %s' % (vi_expr(vs), vi_expr(vt)), p.vi[vs] - p.vi[vt] if known(vs, vt) else None)
                elif sub == 0x32:
                    k = sext(id_, 5)
                    set_vi(it, '%s + %d' % (vi_expr(vs), k), p.vi[vs] + k if known(vs) else None)
                elif sub == 0x34:
                    set_vi(id_, '%s & %s' % (vi_expr(vs), vi_expr(vt)), p.vi[vs] & p.vi[vt] if known(vs, vt) else None)
                elif sub == 0x35:
                    set_vi(id_, '%s | %s' % (vi_expr(vs), vi_expr(vt)), p.vi[vs] | p.vi[vt] if known(vs, vt) else None)
                else:
                    raise Reject('lower %08X' % lo)
                return -1
            sub3 = ((lo >> 6 & 31) << 2) | (sub & 3)
            if sub3 in (0x30, 0x31):  # MOVE, MR32
                self.stall(p, is_)
                v = self.vf_read(p, out, is_)
                if sub3 == 0x31:
                    v2 = self.name('m')
                    out.append('__m128 %s = _mm_shuffle_ps(%s, %s, 0x39);' % (v2, v, v))
                    if v in p.const:
                        p.const.add(v2)
                    v = v2
                self.vf_write(p, out, it, d, v)
                self.ready(p, it, p.cycle + FMAC_LATENCY)
                return -1
            if sub3 == 0x3D:  # MFIR
                v = self.name('m')
                out.append('__m128 %s = _mm_castsi128_ps(_mm_set1_epi32((int16_t)%s));' % (v, vi_expr(vs)))
                if vs in p.vi:
                    p.const.add(v)
                self.vf_write(p, out, it, d, v)
                self.ready(p, it, p.cycle + FMAC_LATENCY)
                return -1
            if sub3 == 0x3C:  # MTIR
                self.stall(p, is_)
                v = self.vf_read(p, out, is_)
                set_vi(it, 'vu0c_lane_bits(%s, %d)' % (v, lo >> 21 & 3))
                return -1
            raise Reject('lower %08X' % lo)
        if op == 0x00:  # LQ
            q = static_address(vs, imm11)
            v = self.name('m')
            out.append('__m128 %s = _mm_loadu_ps(c->vu0_mem[%d].f);' % (v, q))
            self.vf_write(p, out, it, d, v)
            self.ready(p, it, p.cycle + FMAC_LATENCY)
            return -1
        if op == 0x01:  # SQ
            self.stall(p, is_)
            q = static_address(vt, imm11)
            v = self.vf_read(p, out, is_)
            out.append('vu0c_store(c->vu0_mem[%d].f, %s, %d);' % (q, v, d))
            return -1
        if op == 0x04:  # ILW
            q = static_address(vs, imm11)
            for comp in range(4):
                if d >> (3 - comp) & 1:
                    set_vi(it, 'c->vu0_mem[%d].w[%d]' % (q, comp))
            return -1
        if op == 0x05:  # ISW
            q = static_address(vs, imm11)
            for comp in range(4):
                if d >> (3 - comp) & 1:
                    out.append('c->vu0_mem[%d].w[%d] = %s;' % (q, comp, vi_expr(vt)))
            return -1
        if op in (0x08, 0x09):  # IADDIU / ISUBIU
            imm15 = (lo >> 10 & 0x7800) | (lo & 0x7FF)
            sign = '+' if op == 0x08 else '-'
            k = (p.vi[vs] + imm15 if op == 0x08 else p.vi[vs] - imm15) if known(vs) else None
            set_vi(it, '%s %s %d' % (vi_expr(vs), sign, imm15), k)
            return -1
        if op in (0x18, 0x1A, 0x1B):  # FMEQ, FMAND, FMOR
            mac, _ = self.mac_at_read(p)
            expr = {0x18: '(%s & 0xFFFF) == %s', 0x1A: '%s & %s', 0x1B: '%s | %s'}[op] % (mac, vi_expr(vs))
            set_vi(it, expr)
            return -1
        if op == 0x16:  # FSAND
            imm12 = (lo >> 10 & 0x800) | (lo & 0x7FF)
            if imm12 & 0xFC0:
                raise Reject('sticky status read')
            mac, landed = self.mac_at_read(p)
            status = '((status_in & ~0xFu) | vu0c_status(%s))' % mac if landed else 'status_in'
            set_vi(it, '%s & 0x%X' % (status, imm12))
            return -1
        if op == 0x20:
            return target
        if op == 0x21:  # BAL
            set_vi(it, '%d' % (pc + 2), pc + 2)
            return target
        if op in (0x24, 0x25):  # JR, JALR
            if vs not in p.vi:
                raise Reject('JR to a run-time address')
            dest = p.vi[vs]
            if op == 0x25:
                set_vi(it, '%d' % (pc + 2), pc + 2)
            return dest
        if op in (0x28, 0x29, 0x2C, 0x2D, 0x2E, 0x2F):
            if op in (0x28, 0x29):
                if known(vt, vs):
                    taken = (p.vi[vt] == p.vi[vs]) == (op == 0x28)
                    return target if taken else -1
                cond = '%s %s %s' % (vi_expr(vt), '==' if op == 0x28 else '!=', vi_expr(vs))
            else:
                rel = {0x2C: '<', 0x2D: '>', 0x2E: '<=', 0x2F: '>='}[op]
                if known(vs):
                    s = sext(p.vi[vs], 16)
                    taken = eval('%d %s 0' % (s, rel))
                    return target if taken else -1
                cond = '(int16_t)%s %s 0' % (vi_expr(vs), rel)
            return ('fork', cond, target)
        raise Reject('lower %08X' % lo)

    def commit(self, p, out, res, u):
        """commit_upper: the MAC update (queued at cycle + 4) and the result."""
        if res is None:
            return
        d = u['d']
        if u.get('flags'):
            p.writers.append((res, d))
            p.queue.append((p.cycle + FLAG_LATENCY, len(p.writers) - 1))
            out.append('VU0C_STICKY(%s, %d);' % (res, d))
        if u['reg'] == 'acc':
            old = self.acc_read(p, out) if d != 15 else None
            if d != 15:
                v = self.name('acc')
                out.append('__m128 %s = vu0c_blend(%s, %s, %d);' % (v, old, res, d))
                if old in p.const and res in p.const:
                    p.const.add(v)
                res = v
            p.acc, p.acc_dirty = res, True
            return
        self.vf_write(p, out, u['reg'], d, res)
        self.ready(p, u['reg'], p.cycle + FMAC_LATENCY)

    def finish(self, p, out):
        """The end of the run: drain_flags, then the registers back to the CPU."""
        for r in sorted(p.vf_dirty):
            out.append('_mm_storeu_ps(c->vf[%d].f, %s);' % (r, p.vf[r]))
        if p.acc_dirty:
            out.append('_mm_storeu_ps(c->vacc, %s);' % p.acc)
        for r in sorted(p.vi_dirty):
            out.append('c->vi[%d] = vi[%d];' % (r, r))
        if p.i_dirty:
            out.append('{ const uint32_t bits = 0x%08Xu; memcpy(&c->i, &bits, 4); }' % p.i_bits)
        if p.writers:
            name, d = p.writers[-1]
            out.append('{ const unsigned mac = vu0c_mac(%s, %d);' % (name, d))
            out.append('  c->vu0_ctrl[0] = (status_in & 0xFF0u) | vu0c_status(mac) | VU0C_STICKY_BITS();')
            out.append('  c->vu0_ctrl[1] = mac; }')
        else:
            out.append('c->vu0_ctrl[0] = status_in; c->vu0_ctrl[1] = mac_in;')
        out.append('c->vu0_ctrl[2] &= 0xFFFFFFu;')
        out.append('c->vu0_ctrl[10] = 0x%X;' % (p.pc * 8))
        out.append('return;')

    def run(self, p, out, depth=0):
        """Runs one path from p (the interpreter's run() loop) until the program ends or forks."""
        while True:
            if p.steps > 4096:
                raise Reject('no end')
            pc = p.pc
            if pc not in self.pairs:
                raise Reject('pair %04X outside the package' % (pc * 8))
            self.reached.add(pc)
            self.emitted += 1
            if self.emitted > PATH_BUDGET:
                raise Reject('over the path budget')
            lo, hi = self.pairs[pc]
            out.append('/* %04X */' % (pc * 8))
            p.flag_cycle = p.cycle          # apply_flags; update_q has nothing to do (no divider)
            res, u = self.upper(p, out, hi)
            nxt = -1
            if hi >> 31:                    # I bit: the lower word is I
                p.i_bits, p.i_dirty = lo, True
                self.uses_i = True
                out.append('vu0c_i = _mm_castsi128_ps(_mm_set1_epi32((int)0x%08Xu));' % lo)
            else:
                nxt = self.lower(p, out, lo, pc)
            self.commit(p, out, res, u)
            p.cycle += 1
            p.steps += 1
            fork = isinstance(nxt, tuple)
            if p.ending:
                p.pc = (p.branch & MASK) if p.branch >= 0 else (pc + 1) & MASK
                self.finish(p, out)
                return
            if hi >> 30 & 1:
                p.ending = True
            if p.branch >= 0:
                p.pc, p.branch = p.branch & MASK, -1
            else:
                p.pc = (pc + 1) & MASK
            if fork:
                _, cond, target = nxt
                taken = copy.deepcopy(p)
                taken.branch = target
                flag = self.name('b')
                out.append('const int %s = %s;' % (flag, cond))
                out.append('if (%s) {' % flag)
                body = []
                self.run(taken, body, depth + 1)
                out += ['    ' + line for line in body]
                out.append('} else {')
                body = []
                self.run(p, body, depth + 1)
                out += ['    ' + line for line in body]
                out.append('}')
                return
            if nxt >= 0:
                p.branch = nxt


def compile_entry(pairs, entry):
    """C lines of the function for one entry (byte address), the pairs it reaches, or Reject."""
    g = Generator(pairs)
    p = Path_()
    p.pc = entry // 8
    body = []
    g.run(p, body)
    head = ['static void vu0c_%04X(ps2_cpu *c)' % entry, '{',
            '    const unsigned status_in = c->vu0_ctrl[0] & 0xFFFu, mac_in = c->vu0_ctrl[1] & 0xFFFFu;',
            '    __m128 sticky_zero = _mm_setzero_ps(), sticky_sign = _mm_setzero_ps();',
            '    uint16_t vi[16]; memcpy(vi, c->vi, sizeof vi);']
    if g.uses_i:
        head.append('    __m128 vu0c_i = _mm_load1_ps(&c->i);')
    if g.uses_q:
        head.append('    const __m128 vu0c_q = _mm_load1_ps(&c->q);')
    head.append('    (void)mac_in; (void)vi;')
    return head + ['    ' + line for line in body] + ['}'], g.reached, g.emitted


def entries(pairs):
    """Where a vcallms can start: micro address 0 and the pair after each E bit's delay slot."""
    found = {0}
    for pc, (lo, hi) in pairs.items():
        if hi >> 30 & 1 and pc + 2 in pairs:
            found.add((pc + 2) * 8)
    return sorted(found)


def generate(destination, verbose=False):
    destination = Path(destination).resolve()
    if ROOT / 'build' not in destination.parents:
        raise ValueError('game-derived VU0 code must stay under build/')
    data = Path(os.environ.get('DHD_VUTEXT', ROOT / 'assets/vutext.bin')).read_bytes()
    pairs, where = program(data, PACKAGE)
    lines = ['/* Generated by port/tools/vu0_aot.py from local microcode. Do not commit. */']
    lines += WORDS_MACRO
    table, fill_entries, compiled = [], [], []
    for entry in entries(pairs):
        try:
            code, reached, emitted = compile_entry(pairs, entry)
        except Reject as e:
            if verbose:
                print('vu0 %04X: interpreter (%s)' % (entry, e))
            continue
        runs = []
        for pc in sorted(reached):
            if runs and runs[-1][0] + runs[-1][1] == pc:
                runs[-1][1] += 1
            else:
                runs.append([pc, 1])
        words = [w for pc in sorted(reached) for w in pairs[pc]]
        name = 'vu0c_words_%04X' % entry
        lines += code
        lines += ['AOT_WORDS(%s, %d, ' % (name, len(words)) + ','.join('0x%08Xu' % w for w in words) + ')']
        fill_entries += fills(name, data, [where[pc] for pc in sorted(reached)], words)
        spans, offset = [], 0
        for pc, count in runs:
            spans.append('{%d, %d, %d}' % (pc, count, offset))
            offset += 2 * count
        lines.append('static const unsigned vu0c_runs_%04X[%d][3] = {%s};  /* pc, pairs, word */'
                     % (entry, len(runs), ', '.join(spans)))
        table.append(entry)
        compiled.append((entry, len(reached), emitted))
        if verbose:
            print('vu0 %04X: compiled, %d pairs reached, %d emitted' % (entry, len(reached), emitted))
    lines.append('static const struct { unsigned entry; const unsigned (*runs)[3]; '
                 'unsigned n_runs; const uint32_t *words; } vu0c_programs[] = {')
    lines += ['    {0x%X, vu0c_runs_%04X, sizeof vu0c_runs_%04X / sizeof *vu0c_runs_%04X, vu0c_words_%04X},'
              % (e, e, e, e, e) for e in table]
    lines.append('};')
    # Called through a switch, not function pointers: a pointer would put each program in the wasm
    # module's function table and move the table index of every function after it, which the game
    # keeps in RAM for its callbacks (state_trace_codeptrs.txt), so the state trace would differ.
    lines.append('static void vu0c_run(unsigned k, ps2_cpu *c) {')
    lines.append('    switch (k) {')
    lines += ['    case %d: vu0c_%04X(c); return;' % (k, e) for k, e in enumerate(table)]
    lines += ['    }', '}']
    lines.append('enum { VU0C_PROGRAMS = %d };' % len(table))
    lines += fill_function('vu0c_words', fill_entries)
    text = '\n'.join(lines) + '\n'
    destination.parent.mkdir(parents=True, exist_ok=True)
    if not destination.exists() or destination.read_bytes() != text.encode():
        destination.write_bytes(text.encode())
    return compiled


if __name__ == '__main__':
    out = sys.argv[1] if len(sys.argv) > 1 else ROOT / 'build/vu0aot/vu0_aot.inc'
    done = generate(out, verbose=True)
    print('VU0 compiled: %d entries' % len(done))
