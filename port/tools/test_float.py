"""32-bit exceptional arithmetic and game/host SSE trap regression."""
from pathlib import Path
import os
import subprocess
import recomp
import sys

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/runtime/float'


def grid_conversion():
    sys.path.insert(0, str(ROOT / 'port'))
    import patches
    text = (ROOT / 'src/unsorted/00117A10.c').read_text()
    for kind, old, new in patches.PATCHES['unsorted/00117A10.c']:
        assert kind == 'text' and text.count(old) == 1
        text = text.replace(old, new)
    unit = OUT / 'grid.c'
    unit.write_text('#include "port.h"\n' + text)
    obj = unit.with_suffix('.o')
    subprocess.run(['clang', '--target=i686-pc-windows-itanium', '-O1', '-c',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off',
                    '-I' + str(ROOT / 'port/include'), str(unit), '-o', str(obj)], check=True)
    driver = OUT / 'grid_driver.c'
    driver.write_text('''#include "ps2float.h"
#include <xmmintrin.h>
#include <stdio.h>
int ps2_cvt_w_s(float a) { return ps2_float_to_int(a); }
int func_00117A10(void *, float);
int main(void) {
 float values[3] = {0,0,1};
 float inputs[] = {0,1,3.1f,511,1022,1023,1e37f,-1e37f};
 int expected[] = {1,2,4,512,1023,1023,1023,0};
 _mm_setcsr(_mm_getcsr() & ~(0x680u | 63u));
 for (unsigned i=0;i<8;i++) if (func_00117A10(values,inputs[i]) != expected[i]) return 1;
 values[2]=0;
 if (func_00117A10(values,1e37f)) return 2;
 puts("Real bounding-grid conversion: ordinary, sentinel and zero-step cases pass");
 return 0;
}
''')
    exe = driver.with_suffix('.exe')
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O2',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off',
                    '-I' + str(ROOT / 'port/runtime'), str(driver), str(obj), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


def recompiled_float():
    operations = ['add.s', 'sub.s', 'mul.s', 'div.s', 'madd.s', 'msub.s',
                  'adda.s', 'suba.s', 'mula.s', 'madda.s', 'msuba.s']
    source = '#include "recomp.h"\n#include <xmmintrin.h>\n#include <stdio.h>\n'
    source += 'ps2_cpu ps2;\nfloat ps2_fdiv(float a, float b) { return ps2_float_div(a,b); }\n'
    for i, op in enumerate(operations):
        fn = recomp.Func('float_probe', set(), {})
        args = ['$f3', '$f1', '$f2'] if i < 6 else ['$f1', '$f2']
        fn.insn(op, args, op + ' ' + ','.join(args))
        source += 'static void probe%d(void) { %s }\n' % (i, ' '.join(fn.lines))
    source += '''
static void (*probes[])(void) = {probe0,probe1,probe2,probe3,probe4,probe5,
                                probe6,probe7,probe8,probe9,probe10};
/* input A, input B, ACC, exact output; includes the live bounding sentinel subtraction */
static unsigned cases[][4] = {
 {0x7f7fffff,0x7f7fffff,0,0x7f7fffff},
 {0xff7fffff,0x7f7fffff,0,0xff7fffff},
 {0x7f7fffff,0x40000000,0,0x7f7fffff},
 {0,0,0,0x7f7fffff},
 {0x7f7fffff,0x40000000,0,0x7f7fffff},
 {0x7f7fffff,0x40000000,0,0xff7fffff},
 {0x7f7fffff,0x7f7fffff,0,0x7f7fffff},
 {0xff7fffff,0x7f7fffff,0,0xff7fffff},
 {0x7f7fffff,0x40000000,0,0x7f7fffff},
 {0x7f7fffff,0x40000000,0,0x7f7fffff},
 {0x7f7fffff,0x40000000,0,0xff7fffff}
};
int main(void) {
 _mm_setcsr(_mm_getcsr() & ~(0x680u | 63u));
 for (unsigned i=0;i<11;i++) {
  __builtin_memcpy(&ps2.fpr[1], &cases[i][0], 4);
  __builtin_memcpy(&ps2.fpr[2], &cases[i][1], 4);
  __builtin_memcpy(&ps2.fpu_acc, &cases[i][2], 4);
  probes[i]();
  unsigned actual;
  __builtin_memcpy(&actual, i<6 ? &ps2.fpr[3] : &ps2.fpu_acc, 4);
  if (actual != cases[i][3]) { printf("probe %u: %08X != %08X\\n", i, actual, cases[i][3]); return 1; }
 }
 puts("Recompiled EE exceptional arithmetic: 11 operations pass with SSE traps enabled");
 return 0;
}
'''
    path = OUT / 'recompiled_float.c'
    path.write_text(source)
    exe = path.with_suffix('.exe')
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O2',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off',
                    '-I' + str(ROOT / 'port/runtime'), str(path), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


def ir_rewrite():
    import eefloat
    ir = '\n'.join(['@llvm.used = appending global [2 x ptr] [ptr @ps2_native_fadd, ptr @keep], section "llvm.metadata"',
                    'define float @f(float %a, float %b, double %c) {',
                    '  %1 = fadd float %a, %b', '  %2 = fmul float %1, %b', '  %3 = fdiv float %2, 3.000000e+00',
                    '  %4 = fmul double %c, %c', '  %5 = fptrunc double %4 to float', '  ret float %5', '}',
                    'define internal float @ps2_native_fadd(float %a, float %b) {', '  %1 = fadd float %a, %b',
                    '  ret float %1', '}'])
    out, count = eefloat.rewrite(ir)
    assert count == 4, count
    for want in ('call float @ps2_native_fadd(float %a, float %b)', '%2 = fmul float %1, %b',
                 'call float @ps2_native_fdiv(float %2, float 3.000000e+00)', 'call double @ps2_native_dmul(double %c, double %c)',
                 'call float @ps2_native_dtof(double %4)', '[1 x ptr] [ptr @keep]'):
        assert want in out, (want, out)
    assert out.count('fadd float') == 1  # the helper's own body is left alone
    for bad in ('  %1 = fadd <4 x float> %a, %b', '  %1 = fadd fast float %a, %b', '  %1 = frem float %a, %b'):
        try:
            eefloat.rewrite('define void @g() {\n%s\n}' % bad)
        except ValueError:
            continue
        raise AssertionError(bad)
    print('EE float IR rewrite: add/sub/div, double and truncation calls; unhandled forms rejected')


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    exe = OUT / 'test_float.exe'
    rt = ROOT / 'port/runtime'
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-O2',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off', '-Wno-deprecated-declarations',
                    str(rt / 'test_float.c'), str(rt / 'fp_trap.c'), '-o', str(exe),
                    '-Wl,/opt:noicf', '-Wl,/fixed', '-Wl,/dynamicbase:no',
                    '-Wl,/map:' + str(exe.with_suffix('.map'))], check=True)
    env = os.environ.copy()
    env.pop('DHD_FP_TRAP', None)
    subprocess.run([str(exe)], env=env, check=True)
    env['DHD_FP_TRAP'] = '1'
    for mode in ('game', 'host', 'rearm'):
        run = subprocess.run([str(exe), mode], env=env, capture_output=True, text=True)
        assert run.returncode == 17, (mode, run.returncode, run.stderr)
        assert 'FP trap:' in run.stderr and '_func_fp_div+' in run.stderr, run.stderr
        assert 'xmm0:' in run.stderr and 'instruction:' in run.stderr, run.stderr
    print('Game trap, host one-instruction retry and frame-boundary rearm pass')
    recompiled_float()
    ir_rewrite()
    grid_conversion()


if __name__ == '__main__':
    main()
