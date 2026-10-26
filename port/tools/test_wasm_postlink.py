"""wasm_postlink.py on a synthetic module: each kind of mismatched indirect call found in
dhd_core.wasm (build/audit/w7f-indirect.md) traps without the step and works with it, with the
values the i686 build produces.

The same C runs three ways: native i686 (the reference), wasm32 -> wasm2c -> x64 unfixed (must
trap on every mismatched case) and fixed by wasm_postlink.py (must match i686 on every word i686
defines, and the documented choice on the others). All products stay under build/.
"""
import os
import subprocess
import sys
from pathlib import Path

from build_host64 import ROOT, WABT, WASI, LLVM, MSVC, SDK, VERSION as VER
from prepare_wasm2c import specialize_dispatch
import wasm_postlink
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches

OUT = ROOT / 'build/w7f/postlink-test'
RT = WABT / 'share/wabt/wasm2c'
BINARYEN = (DEVCACHES / 'binaryen/binaryen-version_133')

# Callees record what they saw in rec[]; each case returns the caller's view of the result.
SOURCE = r'''
typedef unsigned int u32;
typedef unsigned long long u64;
#define NOINLINE __attribute__((noinline))
volatile u32 rec[12];
static void clear(void) { for (int i = 0; i < 12; i++) rec[i] = 0xEEEEEEEEu; }
static u32 bits(float f) { union { float f; u32 u; } x; x.f = f; return x.u; }

NOINLINE void v1(u32 a) { rec[0] = a; }
NOINLINE void v1b(u32 a) { rec[0] = a + 1; }
NOINLINE void vv(void) { rec[0] = 0x77; }
NOINLINE u32 r8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h) {
    rec[0] = a; rec[1] = b; rec[2] = c; rec[3] = d; rec[4] = e; rec[5] = f; rec[6] = g; rec[7] = h;
    return a * 3 + h;
}
NOINLINE void vf(u32 a, float f) { rec[0] = a; rec[1] = bits(f); }
NOINLINE void vl(u32 a, u64 x) { rec[0] = a; rec[1] = (u32)x; rec[2] = (u32)(x >> 32); }
NOINLINE double rd(u32 a) { rec[0] = a; return (double)a + 0.1; }
NOINLINE u64 rl(u32 a) { rec[0] = a; return ((u64)0x11223344u << 32) | (a + 0x55667788u); }
NOINLINE u32 tx(u32 a, u32 b) { rec[0] = a; rec[1] = b; return a + b; }
/* small, with locals and a loop: the step clones it rather than calling it */
NOINLINE u32 lp(u32 n, u32 k) { u32 s = 0; for (u32 i = 0; i < n; i++) s = s * 31 + (i ^ k); rec[0] = n; rec[1] = k; return s; }
/* build_wasm.py's ABI thunk for a caller that declares tx as void(void): the wasm table holds
   it, the i686 build holds tx itself. */
NOINLINE void tx__as__v(void) { tx(0, 0); }
#ifdef __wasm__
#define TX tx__as__v
#else
#define TX tx
#endif

typedef void (*fn)(void);
fn volatile table[] = {0, (fn)v1, (fn)r8, (fn)v1b, (fn)vf, (fn)vl, (fn)rd, (fn)rl, (fn)TX, (fn)vv, (fn)lp};
enum { V1 = 1, R8, V1B, VF, VL, RD, RL, TXI, VV, LP };

typedef u32 (*i8_i)(u32, u32, u32, u32, u32, u32, u32, u32);
typedef void (*i_v)(u32);
typedef u32 (*i_i)(u32);
typedef void (*v_v)(void);
typedef void (*ii_v)(u32, u32);
typedef void (*iii_v)(u32, u32, u32);
typedef void (*i6_v)(u32, u32, u32, u32, u32, u32);
typedef float (*i_f)(u32);
typedef u32 (*ii_i)(u32, u32);
typedef u32 (*iiii_i)(u32, u32, u32, u32);

#ifdef __wasm__
__attribute__((export_name("rec_get")))
#endif
u32 rec_get(u32 i) { return rec[i]; }

#ifdef __wasm__
__attribute__((export_name("run_case")))
#endif
u32 run_case(u32 k) {
    clear();
    switch (k) {
    case 0: return ((i8_i)table[R8])(1, 2, 3, 4, 5, 6, 7, 8);            /* control: same type */
    case 1: return ((i8_i)table[V1])(11, 12, 13, 14, 15, 16, 17, 18);     /* jalr -> void C: extra slots */
    case 2: ((i_v)table[R8])(21); return 0;                               /* vtable -> recompiled: missing slots */
    case 3: return ((i_i)table[V1B])(31);                                 /* int caller, void callee: 0 */
    case 4: ((i6_v)table[R8])(41, 42, 43, 44, 45, 46); return 0;          /* the type with no table entry */
    case 5: ((ii_v)table[VF])(51, 0x3FC00000u); return 0;                 /* int slot read as float */
    case 6: ((iii_v)table[VL])(61, 0x89ABCDEFu, 0x01234567u); return 0;   /* two slots read as i64 */
    case 7: return bits(((i_f)table[RD])(71));                            /* double result read as float */
    case 8: return bits(((i_f)table[V1B])(81));                           /* float read from a void callee */
    case 9: return ((ii_i)table[TXI])(91, 92);                            /* thunk entry: the definition runs */
    case 10: return ((i_i)table[RL])(101);                                /* i64 result read as int: eax */
    case 11: ((v_v)table[R8])(); return 0;                                /* no arguments at all */
    case 12: ((v_v)table[0])(); return 0;                                 /* null entry still traps */
    case 13: ((ii_v)table[VV])(131, 132); return 0;                       /* void(void) target, extra slots */
    case 14: return ((iiii_i)table[LP])(9, 141, 142, 143);                /* cloned target with locals */
    }
    return 0xDEAD;
}
'''

DRIVER = r'''
#include <stdio.h>
#include <windows.h>
typedef unsigned int u32;
#ifdef NATIVE
u32 run_case(u32), rec_get(u32);
#define RUN(k) run_case(k)
#define REC(i) rec_get(i)
#else
#include "probe.h"
#include "wasm-rt-impl.h"
static w2c_probe p;
#define RUN(k) w2c_probe_run_case(&p, k)
#define REC(i) w2c_probe_rec_get(&p, i)
#endif
int main(void) {
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
#ifndef NATIVE
    wasm_rt_init();
    wasm2c_probe_instantiate(&p);
#endif
    for (u32 k = 0; k < 15; k++) {
#ifdef NATIVE
        if (k == 12) { printf("%u TRAP\n", k); continue; }  /* a null call faults natively */
#else
        wasm_rt_trap_t t = WASM_RT_SETJMP(g_wasm_rt_jmp_buf);
        if (t) { printf("%u TRAP %d\n", k, (int)t); continue; }
#endif
        u32 r = RUN(k);
        printf("%u %08x", k, r);
        for (u32 i = 0; i < 8; i++) printf(" %08x", REC(i));
        printf("\n");
    }
    return 0;
}
'''

E = 0xEEEEEEEE  # untouched record word
# Per case: words compared with i686 (indices into [result, rec0..rec7]), and the documented
# value of every word i686 leaves undefined (missing slots, eax that is not a constant).
CASES = {
    0: ([0, 1, 2, 3, 4, 5, 6, 7, 8], {}),
    1: ([1, 2], {0: 0}),
    2: ([0, 1], {2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0}),
    3: ([1], {0: 0}),
    4: ([0, 1, 2, 3, 4, 5, 6], {7: 0, 8: 0}),
    5: ([0, 1, 2, 3], {}),
    6: ([0, 1, 2, 3, 4], {}),
    7: ([0, 1, 2], {}),
    8: ([0, 1, 2], {}),
    9: ([0, 1, 2, 3], {}),
    10: ([0, 1, 2], {}),
    11: ([], {1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0}),
    13: ([1, 2], {0: 0}),
    14: ([0, 1, 2, 3], {}),
}
MISMATCHED = set(CASES) - {0}


def run(args, **kw):
    r = subprocess.run(list(map(str, args)), cwd=OUT, capture_output=True, text=True, timeout=300,
                       env=dict(os.environ, TEMP=str(OUT), TMP=str(OUT)), **kw)
    if r.returncode:
        raise RuntimeError(' '.join(map(str, args)) + '\n' + r.stdout + r.stderr)
    return r.stdout


def parse(text):
    out = {}
    for line in text.splitlines():
        f = line.split()
        out[int(f[0])] = 'TRAP' if f[1] == 'TRAP' else [int(x, 16) for x in f[1:]]
    return out


def native(arch, sources, exe, defines=(), extra=()):
    inc = ['/imsvc' + str(MSVC / 'include')] + ['/imsvc' + str(SDK / 'Include' / VER / x) for x in ('ucrt', 'um', 'shared')]
    target = 'i686' if arch == 'x86' else 'x86_64'
    flags = ['--target=%s-pc-windows-msvc' % target, '/O2', '/clang:-ffp-model=strict', '/clang:-ffp-contract=off',
             '/clang:-Wno-everything', *inc, *['-D' + d for d in defines], *extra]
    objs = []
    for s in sources:
        obj = exe.parent / ('%s-%s.obj' % (exe.stem, s.stem))
        run([LLVM / 'clang-cl.exe', *flags, '/c', s, '/Fo' + str(obj)])
        objs.append(obj)
    run([LLVM / 'clang-cl.exe', '--target=%s-pc-windows-msvc' % target, *objs, '/Fe' + str(exe), '/link',
         '/libpath:' + str(MSVC / 'lib' / arch), '/libpath:' + str(SDK / 'Lib' / VER / 'ucrt' / arch),
         '/libpath:' + str(SDK / 'Lib' / VER / 'um' / arch)])
    return parse(run([exe]))


def through_wasm2c(wasm, name):
    d = OUT / name
    d.mkdir(exist_ok=True)
    run([WABT / 'bin/wasm2c.exe', wasm, '-n', 'probe', '-o', d / 'probe.c'])
    c = (d / 'probe.c').read_text()
    (d / 'probe.c').write_text(specialize_dispatch(c))  # the pipeline's guarded dispatch, on every table
    (d / 'driver.c').write_text(DRIVER)
    return native('x64', [d / 'driver.c', d / 'probe.c', RT / 'wasm-rt-impl.c', RT / 'wasm-rt-mem-impl.c'], d / 'probe.exe',
                  extra=['-I' + str(WABT / 'include'), '-I' + str(RT), '-I' + str(d)])


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / 'probe_src.c').write_text(SOURCE)
    (OUT / 'driver.c').write_text(DRIVER)
    ref = native('x86', [OUT / 'probe_src.c', OUT / 'driver.c'], OUT / 'i686.exe', defines=['NATIVE'])
    run([WASI / 'bin/clang.exe', '--target=wasm32', '-O2', '-nostdlib', '-Wno-everything', '-Wl,--no-entry',
         OUT / 'probe_src.c', '-o', OUT / 'probe.wasm'])
    report = wasm_postlink.postlink(OUT / 'probe.wasm', OUT / 'fixed.wasm')
    run([WABT / 'bin/wasm-validate.exe', OUT / 'fixed.wasm'])
    unfixed = through_wasm2c(OUT / 'probe.wasm', 'unfixed')
    fixed = through_wasm2c(OUT / 'fixed.wasm', 'fixed')
    fails = []
    for k in sorted(set(CASES) | {12}):
        if k == 12:
            if fixed[k] != 'TRAP' or unfixed[k] != 'TRAP':
                fails.append('case 12: the null entry must trap with and without the step')
            continue
        if k in MISMATCHED and unfixed[k] != 'TRAP':
            fails.append('case %d: no trap without the step: %s' % (k, unfixed[k]))
        if k == 0 and unfixed[k] != ref[k]:
            fails.append('case 0 (control) differs without the step')
        got = fixed[k]
        if got == 'TRAP':
            fails.append('case %d traps with the step' % k)
            continue
        same, chosen = CASES[k]
        for i in same:
            if got[i] != ref[k][i]:
                fails.append('case %d word %d: %08x, i686 %08x' % (k, i, got[i], ref[k][i]))
        for i, v in chosen.items():
            if got[i] != v:
                fails.append('case %d word %d: %08x, documented choice %08x' % (k, i, got[i], v))
    # for comparison only: binaryen's FuncCastEmulation on the same module (not a pass condition)
    run([BINARYEN / 'bin/wasm-opt.exe', '--all-features', '--fpcast-emu', OUT / 'probe.wasm', '-o', OUT / 'fpcast.wasm'])
    fpcast = through_wasm2c(OUT / 'fpcast.wasm', 'fpcast')
    lines = ['case | i686 | unfixed | fixed | fpcast-emu (words that differ from i686)']
    fp_diffs = 0
    for k in sorted(fixed):
        show = lambda r: r if r == 'TRAP' else ' '.join('%08x' % x for x in r)
        bad = [] if k == 12 else ['trap'] if fpcast[k] == 'TRAP' else [
            str(i) for i in CASES[k][0] if fpcast[k][i] != ref[k][i]]
        fp_diffs += bool(bad)
        lines.append('%d | %s | %s | %s | %s (%s)' % (k, show(ref[k]), show(unfixed[k]), show(fixed[k]),
                                                     show(fpcast[k]), ','.join(bad) or 'same'))
    (OUT / 'results.txt').write_text('\n'.join(lines) + '\n')
    print('\n'.join(lines))
    print('postlink: %d sites, %d shadow tables, %d adapters (%d cloned targets), %d thunk entries unwrapped' % (
        report['sites'], report['shadow_tables'], report['adapters'], report['cloned_targets'],
        report['thunk_entries_unwrapped']))
    print('fpcast-emu: %d of %d cases differ from i686 on a word i686 defines' % (fp_diffs, len(CASES)))
    if report['cloned_targets'] < 2:
        fails.append('expected the void(void) and loop targets to be cloned')
    if fails:
        print('FAIL\n' + '\n'.join(fails))
        return 1
    print('PASS wasm_postlink: %d mismatched cases trap without the step and match i686 with it '
          '(%d words compared with i686, %d documented choices), control and null unchanged' % (
              len(MISMATCHED), sum(len(CASES[k][0]) for k in CASES), sum(len(CASES[k][1]) for k in CASES)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
