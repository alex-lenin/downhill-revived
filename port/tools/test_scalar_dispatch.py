"""Execute typed native scalar callbacks from synthetic EE jalr instructions."""
from pathlib import Path
from unittest.mock import patch
import subprocess
import recomp


def main():
    root=Path(recomp.ROOT);out=root/'build/port/runtime/scalar-dispatch';out.mkdir(parents=True,exist_ok=True)
    contracts={
        'callback_stack':dict(ret='float',args=['float']*10,variadic=False),
        'callback_float':dict(ret='float',args=['i32','float','i32'],variadic=False),
        'callback_double':dict(ret='double',args=['double','i32'],variadic=False),
        'callback_wide':dict(ret='i64',args=['i32','i64','i32'],variadic=False),
        'callback_quad':dict(ret='i128',args=['i32','i64','i128','i32'],variadic=False),
        'callback_void':dict(ret='void',args=['i64'],variadic=False)}
    recomp.write_scalar_dispatch(contracts,str(out/'dispatch.c'))
    insns=[(0x1000,'jalr',['$t9']),(0x1004,'nop',[])]
    with patch.object(recomp,'parse',return_value=(insns,set())):fn=recomp.translate('probe',set(),{})
    call='\n'.join(fn.lines)
    assert 'ps2_recomp_scalar_call' in call
    source='''#include "recomp.h"
#include "quad_abi.h"
#include <stdio.h>
ps2_cpu ps2;
void (*ps2_graphics_sync)(void);
void (*ps2_intc_read)(void);
static float stack_args[4] __attribute__((aligned(16)));
float callback_stack(float a,float b,float c,float d,float e,float f,float g,float h,float i,float j) {return a+j;}
static unsigned failures;
float callback_float(unsigned a,float b,unsigned c) { return a+b-c; }
double callback_double(double a,unsigned b) { return a+b; }
uint64_t callback_wide(unsigned a,uint64_t b,unsigned c) { return a+b-c; }
ps2_u128 callback_quad(unsigned a,uint64_t b,ps2_u128 q,unsigned c) {
 failures+=a!=7||b!=0x8877665544332211ULL||c!=19;
 return q ^ ((ps2_u128)0xfedcba9876543210ULL<<64);
}
void callback_void(uint64_t value) {failures+=value!=0xaabbccdd11223344ULL;}
uint32_t callback_integer(uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e,uint32_t f,uint32_t g,uint32_t h) {return a+b;}
#include "dispatch.c"
static void invoke(void *target) {GPR(25)=(uintptr_t)target;
''' + call + '''
}
int main(void) {
 for(unsigned trial=0;trial<65;trial++) {
  GPR(4)=7;FPR(12)=(int)trial*.125f;GPR(5)=11;
  invoke(callback_float);
  if(FPR(0)!=7+(int)trial*.125f-11)return 1;
  GPR(4)=ps2_d2u(1234567890.125);GPR(5)=9;
  invoke(callback_double);
  if(ps2_u2d(GPR(2))!=1234567899.125)return 2;
  GPR(4)=7;GPR(5)=0x8877665544332211ULL+trial;GPR(6)=19;
  invoke(callback_wide);
  if(GPR(2)!=0x8877665544332211ULL+trial+7-19)return 3;
  ps2_q128 q={{0}},expected;q.w[0]=trial;q.w[1]=0x12345678;q.w[2]=0x90abcdef;q.w[3]=0xfedcba98;
  GPR(4)=7;GPR(5)=0x8877665544332211ULL;ps2.gpr[6]=q;GPR(7)=19;
  expected=ps2_scalar_quad(ps2_quad_scalar(q)^((ps2_u128)0xfedcba9876543210ULL<<64));
  invoke(callback_quad);
  if(__builtin_memcmp(&ps2.gpr[2],&expected,16))return 4;
 }
 GPR(4)=0xaabbccdd11223344ULL;GPR(2)=0x13579;
 invoke(callback_void);if(GPR(2)!=0x13579)return 5;
 GPR(4)=3;GPR(5)=5;invoke(callback_integer);if(GPR(2)!=8)return 6;
 FPR(12)=1.25f;GPR(29)=(uintptr_t)stack_args;stack_args[0]=2;stack_args[2]=-3.5f;
 invoke(callback_stack);if(FPR(0)!=-2.25f)return 7;
 if(failures)return 8;
 puts("Scalar jalr dispatch: FP/double/DI/TI, following arguments, FP stack spill, void and integer fallback pass");
 return 0;
}
'''
    path=out/'test.c';path.write_text(source);exe=out/'test.exe'
    subprocess.run(['clang','--target=i686-pc-windows-msvc','-fuse-ld=lld','-O2','-msse2','-mfpmath=sse','-I'+str(root/'port/runtime'),str(path),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)


if __name__=='__main__':main()
