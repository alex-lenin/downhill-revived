"""Execute the real terrain-index interpolator with EE finite arithmetic."""
from pathlib import Path
import re
import subprocess
import sys
from pctarget import HOST_TARGET
ROOT = Path(__file__).resolve().parents[2]


def main():
    sys.path.insert(0, str(ROOT / 'port'))
    import patches
    code = (ROOT / 'src/unsorted/00111F98.c').read_text()
    for kind, old, new in patches.PATCHES['unsorted/00111F98.c']:
        if kind == 'text':
            assert code.count(old) == 1
            code = code.replace(old, new)
        else:
            code, count = re.subn(old[0], new, code)
            assert count == old[1]
    code, count = re.subn(r'^static inline float maxf.*$',
        'static float maxf(float a,float b){return a>b?a:b;}', code, flags=re.M)
    assert count == 1  # Only the unrelated inline max.s primitive is substituted.
    out = ROOT / 'build/port/runtime/terrain-float'; out.mkdir(parents=True, exist_ok=True)
    source = out / 'fixture.c'
    source.write_text('''#include "ps2float.h"
#include <assert.h>
#include <stdio.h>
#include <xmmintrin.h>
static float ps2_fsub(float a,float b){return ps2_float_sub(a,b);}
static float ps2_fmul(float a,float b){return ps2_float_mul(a,b);}
static float ps2_fdiv(float a,float b){return ps2_float_div(a,b);}
static int ps2_cvt_w_s(float a){return ps2_float_to_int(a);}
''' + code + r'''
char D_0029EA20[1]; static Point first,second; static int rowA,rowB;
int func_00119150(void *p,Point *point,int flag){return point==&first?rowA:rowB;}
int func_00119120(void *p,Point *point,int flag){return 0;}
void func_0010B290(Obj *obj,int *value){}
static void check(float z,float ground,float other,int a,int b,float weight,int expected){
 Body body={0}; Rider rider={0}; Obj obj={0}; int result=0;
 first.z=z;second.z=other;body.x18=ground;rider.x792C=&body;
 obj.x0=&rider;obj.x70=&first;obj.x74=&second;obj.x2B8=weight;rowA=a;rowB=b;
 func_00111F98(&obj,&result);assert(result==expected);
}
int main(void){
 _mm_setcsr((_mm_getcsr()&~0x680u)&~63u);
 check(10,5,0,4,8,0,6); check(10,10,10,5,5,0,5);
 check(10,10,10,5,6,0,(int32_t)(5u+0x7FFFFFFFu));
 check(10,10,10,8,4,0,(int32_t)(8u+0x80000000u));
 check(10,5,0,7,9,3.4028234663852886e38f,0);
 check(3.4028234663852886e38f,-3.4028234663852886e38f,-3.4028234663852886e38f,4,8,0,5);
 puts("Real terrain interpolation: flat, ordinary, signed zero and saturation cases pass with SSE traps");return 0;
}
''')
    exe = out/'fixture.exe'
    subprocess.run(['clang','--target='+HOST_TARGET,'-O1','-w','-msse2','-mfpmath=sse','-fwrapv',
                    '-ffp-contract=off','-I',str(ROOT/'port/runtime'),str(source),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)


if __name__ == '__main__': main()
