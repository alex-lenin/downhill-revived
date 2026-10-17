/* UNIT 0x00133C30 0x00133CB0 .sdata=0x29C2E0:0x29C2E8 */
#include "impulse.h"
struct Body { char pad[0x4b0]; union { unsigned long flags; struct { short low; short count; } half; } x4b0; };
float D_0029C2E0[2]={1750.0f,0.0f};

void func_00133C30(char *obj, char *rider) {
 float tmp[4] __attribute__((aligned(16)));
 if(*(int *)(obj+0x1dc)>=30) {
  struct Body *body=(struct Body *)(rider+0x40c0);
  float scale=D_0029C2E0[0];
  int one=1;
  body->x4b0.flags=(body->x4b0.flags&~0x20UL)|0x20UL;
  body->x4b0.half.count=one;
  __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)" : : "r"(tmp),"r"(rider+0x42a0),"f"(scale) : "$2");
  ApplyImpulseLegacySetup((ImpulseBody *)body, (Vector4 *)tmp, one, rider+0x42a0);
  *(int *)(obj+0x1dc)=0;
 }
}
