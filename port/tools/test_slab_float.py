"""Execute real segment/slab clipping with finite EE bounding sentinels."""
from pathlib import Path
import re
import subprocess
import sys
from pctarget import HOST_TARGET
ROOT = Path(__file__).resolve().parents[2]


def main():
    sys.path.insert(0, str(ROOT/'port'))
    import patches
    code=(ROOT/'src/unsorted/002145F8.c').read_text()
    for kind,old,new in patches.PATCHES['unsorted/002145F8.c']:
        assert kind=='text' and code.count(old)==1
        code=code.replace(old,new)
    for name,compare in [('maxf','>'),('minf','<')]:
        code,count=re.subn(r'^static inline float '+name+r'.*$',
            'static float '+name+'(float a,float b){return a'+compare+'b?a:b;}',code,flags=re.M)
        assert count==1
    out=ROOT/'build/port/runtime/slab-float';out.mkdir(parents=True,exist_ok=True)
    source=out/'fixture.c'
    source.write_text('''#include "ps2float.h"
#include <assert.h>
#include <stdio.h>
#include <xmmintrin.h>
static float ps2_fsub(float a,float b){return ps2_float_sub(a,b);}
static float ps2_fmul(float a,float b){return ps2_float_mul(a,b);}
static float ps2_fadd(float a,float b){return ps2_float_add(a,b);}
static float ps2_fdiv(float a,float b){return ps2_float_div(a,b);}
'''+code+r'''
float D_0029DA74[]={0},D_0029DA78[]={1};
static int clip(float *start,float *end,float *lo,float *hi,float radius,float *near,float *far){
 *near=0;*far=1;return func_002145F8(start,end,near,far,lo,hi,radius);
}
int main(void){
 float n,f,start[3]={-2,0,0},end[3]={2,0,0},lo[3]={-1,-1,-1},hi[3]={1,1,1};
 _mm_setcsr((_mm_getcsr()&~0x680u)&~63u);
 assert(clip(start,end,lo,hi,0,&n,&f)==1 && n==.25f && f==.75f);
 assert(clip(start,end,lo,hi,.5f,&n,&f)==1 && n==.125f && f==.875f);
 start[0]=end[0]=2;assert(!clip(start,end,lo,hi,.5f,&n,&f));
 start[0]=end[0]=0;assert(clip(start,end,lo,hi,0,&n,&f)==1 && n==0 && f==1);
 for(unsigned i=0;i<3;i++){lo[i]=-3.4028234663852886e38f;hi[i]=3.4028234663852886e38f;}
 end[0]=-.5f;assert(clip(start,end,lo,hi,0,&n,&f)==1 && n==0 && f==1);
 end[0]=.5f;assert(clip(start,end,lo,hi,0,&n,&f)==1 && n==0 && f==1);
 start[0]=-3.4028234663852886e38f;end[0]=3.4028234663852886e38f;
 assert(clip(start,end,lo,hi,0,&n,&f)==1 && n==0 && f==0);
 puts("Real slab clipping: ordinary, stationary, expanded, signed sentinel and overflow cases pass with SSE traps");return 0;
}
''')
    exe=out/'fixture.exe'
    subprocess.run(['clang','--target='+HOST_TARGET,'-O1','-w','-msse2','-mfpmath=sse','-fwrapv',
                    '-ffp-contract=off','-I',str(ROOT/'port/runtime'),str(source),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)


if __name__=='__main__':main()
