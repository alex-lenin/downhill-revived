"""Execute the NPC selector's real distance-adjustment statement."""
from pathlib import Path
import subprocess
import sys
from pctarget import HOST_TARGET
ROOT=Path(__file__).resolve().parents[2]


def main():
    sys.path.insert(0,str(ROOT/'port'))
    import patches
    code=(ROOT/'src/unsorted/00125178.cpp').read_text()
    for kind,old,new in patches.PATCHES['unsorted/00125178.cpp']:
        assert kind=='text' and code.count(old)==1
        code=code.replace(old,new)
    statement=next(line.strip() for line in code.splitlines() if 'distance = ps2_fadd' in line)
    out=ROOT/'build/port/runtime/npc-distance-float';out.mkdir(parents=True,exist_ok=True)
    source=out/'fixture.cpp'
    source.write_text('''#include "ps2float.h"
#include <assert.h>
#include <stdio.h>
#include <xmmintrin.h>
static float ps2_fsub(float a,float b){return ps2_float_sub(a,b);}
static float ps2_fmul(float a,float b){return ps2_float_mul(a,b);}
static float ps2_fadd(float a,float b){return ps2_float_add(a,b);}
static float Min(float a,float b){return a<b?a:b;}
static float speed;static unsigned reads;
static float func_001385A8(void *p){reads++;return speed;}
static char target[0x40C4];
static float adjust(float distance){
 struct Local {void *x144;} local={target}; Local *obj=&local;
'''+statement+r'''
 return distance;
}
static void check(float distance,float value,float expected){
 speed=value;reads=0;assert(adjust(distance)==expected);assert(reads==1);
}
int main(void){
 _mm_setcsr((_mm_getcsr()&~0x680u)&~63u);
 check(10,.5f,15);check(10,1,10);check(10,2,10);check(10,-1,30);
 check(3.4028234663852886e38f,.45f,3.4028234663852886e38f);
 check(-3.4028234663852886e38f,.45f,-3.4028234663852886e38f);
 check(0,-3.4028234663852886e38f,0);
 puts("Real NPC distance adjustment: ordinary, signed, sentinel and zero-product cases pass with SSE traps");return 0;
}
''')
    exe=out/'fixture.exe'
    subprocess.run(['clang','--target='+HOST_TARGET,'-O1','-w','-msse2','-mfpmath=sse','-fwrapv',
                    '-ffp-contract=off','-I',str(ROOT/'port/runtime'),str(source),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)


if __name__=='__main__':main()
