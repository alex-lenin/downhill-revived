"""Execute the real float-tween evaluator with EE exceptional arithmetic."""
from pathlib import Path
import subprocess
import sys
import transform
from pctarget import HOST_TARGET
ROOT = Path(__file__).resolve().parents[2]


def main():
    sys.path.insert(0, str(ROOT / 'port'))
    import patches
    code = (ROOT / 'src/unsorted/00208708.cpp').read_text()
    for kind, old, new in patches.PATCHES['unsorted/00208708.cpp']:
        assert kind == 'text' and code.count(old) == 1
        code = code.replace(old, new)
    code = transform.rewrite_minmax(code)
    out = ROOT / 'build/port/runtime/tween-float'; out.mkdir(parents=True, exist_ok=True)
    source = out / 'fixture.cpp'
    source.write_text('''#include "ps2float.h"
#include <assert.h>
#include <stdio.h>
#include <xmmintrin.h>
static float PS2_MIN(float a,float b){return a<b?a:b;}
static float ps2_fdiv(float a,float b){return ps2_float_div(a,b);}
static float ps2_fmul(float a,float b){return ps2_float_mul(a,b);}
static float ps2_fadd(float a,float b){return ps2_float_add(a,b);}
static float port_animation_fields(float ticks){return ticks;}  /* the 60 path */
''' + code + r'''
static int tick;
extern "C" int func_00202DA8(void){return tick;}
static void check(int duration,int elapsed,float delta,float start,float expected,int active){
 float value=0; FloatTween obj={&value,start,delta,100,duration}; tick=100+elapsed;
 assert(func_00208708(&obj)==active); assert(value==expected);
}
int main(void){
 _mm_setcsr((_mm_getcsr()&~0x680u)&~63u);
 check(2,1,4,4,6,1); check(2,2,4,4,8,0); check(2,4,4,4,8,0);
 check(0,2,4,4,8,0); check(0,0,4,4,8,0);
 check(2,-1,4,4,2,1); check(-2,1,4,4,2,1);
 check(0,-1,0,4,4,1); check(0,-1,1e20f,0,-3.4028234663852886e38f,1);
 check(1,1,3.4028234663852886e38f,3.4028234663852886e38f,3.4028234663852886e38f,0);
 puts("Real float tween: instant, zero slope, ordinary, signed and overflow cases pass with SSE traps");
 return 0;
}
''')
    exe = out / 'fixture.exe'
    subprocess.run(['clang', '--target='+HOST_TARGET, '-O1', '-w', '-msse2', '-mfpmath=sse',
                    '-ffp-contract=off', '-I', str(ROOT/'port/runtime'), '-I', str(ROOT/'include'),
                    str(source), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == '__main__': main()
