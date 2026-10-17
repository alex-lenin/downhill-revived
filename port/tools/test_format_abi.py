"""Execute real generated formatted wrappers with mixed native vararg widths."""
from pathlib import Path
from unittest.mock import patch
import subprocess
import recomp


def main():
    root=Path(recomp.ROOT);out=root/'build/port/runtime/format-abi';out.mkdir(parents=True,exist_ok=True)
    names=list(recomp.FORMATTED_VARARGS)
    insns=[(0x1000,'jr',['$ra']),(0x1004,'nop',[])]
    with patch.object(recomp,'OUT',str(out)),patch.object(recomp,'parse',return_value=(insns,set())):
        for n in names:recomp.write(n,set(names),recomp.ABI_SIGNATURES)
    source='''#include "recomp.h"
#include "quad_abi.h"
#include <stdio.h>
ps2_cpu ps2;void (*ps2_graphics_sync)(void);void (*ps2_intc_read)(void);
static unsigned char stack[2048] __attribute__((aligned(16)));
void ps2_recomp_enter(void) {GPR(29)=(uintptr_t)(stack+sizeof stack);}
#include "func_00254D08.c"
#include "func_00254D68.c"
static const char format[]="%*.*f %lx %llx %p %d %d %f %llx %d %%";
int main(void) {
 ps2_q128 q={{0}};q.w[0]=0x11223344;q.w[1]=0x55667788;q.w[2]=0x99aabbcc;q.w[3]=0xddeeff00;
 for(unsigned mode=0;mode<2;mode++) {
  GPR(2)=55;
  if(mode)func_00254D68(1,(uintptr_t)format,12,5,1.25,0x8877665544332211ULL,
                       ps2_quad_scalar(q),(void *)0x1234,-7,23,-2.5,ps2_quad_scalar(q),31);
  else func_00254D08((uintptr_t)format,12,5,1.25,0x8877665544332211ULL,
                    ps2_quad_scalar(q),(void *)0x1234,-7,23,-2.5,ps2_quad_scalar(q),31);
  unsigned r=mode?6:5;uintptr_t base=(uintptr_t)(stack+sizeof stack-1024);
  if(GPR(r)!=12||GPR(r+1)!=5||ps2_u2d(GPR(r+2))!=1.25||GPR(r+3)!=0x8877665544332211ULL||
     __builtin_memcmp(&ps2.gpr[r+4],&q,16)||UW(r+5)!=0x1234)return 1;
  if(!mode) {
   if(SW(11)!=-7||*(uint64_t *)base!=23||ps2_u2d(*(uint64_t *)(base+8))!=-2.5||
      __builtin_memcmp((void *)(base+16),&q,16)||*(uint64_t *)(base+32)!=31)return 2;
  } else if(*(uint64_t *)base!=S32(-7)||*(uint64_t *)(base+8)!=23||
            ps2_u2d(*(uint64_t *)(base+16))!=-2.5||__builtin_memcmp((void *)(base+32),&q,16)||
            *(uint64_t *)(base+48)!=31)return 3;
  if(GPR(29)!=(uintptr_t)(stack+sizeof stack))return 4;
 }
 puts("Formatted EE varargs: promoted doubles, DI/TI values, stars, following arguments and spills pass");
 return 0;
}
'''
    path=out/'test.c';path.write_text(source);exe=out/'test.exe'
    subprocess.run(['clang','--target=i686-pc-windows-msvc','-fuse-ld=lld','-O2','-I'+str(root/'port/runtime'),str(path),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)


if __name__=='__main__':main()
