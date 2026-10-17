/* UNIT 0x00145998 0x00145BD0 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
#define P(o,n) (*(void **)((char *)(o)+(n)))
#define I(o,n) (*(int *)((char *)(o)+(n)))
#define F(o,n) (*(float *)((char *)(o)+(n)))
typedef float Vec[4];
extern char D_002AF2C0[];
extern int func_0011C0D0(void *,void *,int,float);
#if defined(__mips__)
extern float func_00211980(void *,void *,int,float);
#endif
extern void func_00221888(void *,float *,float *),func_00145BD0(void *,void *,int,int);
void func_00145998(void *obj,int force) {
 Vec local,world,adjustment;void *pos,*body;unsigned long **list,*item,mask,set;int count;float height,delta;float *transformed;
 if(I(obj,0x1AC) || force || func_0011C0D0(D_002AF2C0,(char *)P(obj,8)+0x10,1,150.0f)) {
 pos=(char *)obj+0x180;transformed=world;mask=0xFDFFFFFFFFFFFFFFUL;list=(unsigned long **)((char *)P(obj,12)+0x1510);count=11;
 do {item=*list;if(item) *item &= mask;list++;count--;}while(count>=0);
 height=CallGroundProbe((char *)P(obj,8)+0x10,pos,0,5.0f);
 set=0x0200000000000000UL;mask=0xFDFFFFFFFFFFFFFFUL;list=(unsigned long **)((char *)P(obj,12)+0x1510);count=11;
 do {item=*list;if(item) *item=(*item & mask)|set;list++;count--;}while(count>=0);
 local[2]=height;local[0]=0.0f;local[1]=0.0f;func_00221888((char *)P(obj,8)+0x40,local,transformed);
 delta=world[2]-F(obj,0x1A0);
 if(force || delta<0.02f || delta<F(obj,0x1A4)) {
 body=P(obj,8);local[2]=delta;local[0]=0.0f;local[1]=0.0f;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(adjustment):"r"(local),"r"((char *)body+0x40):"memory");
 F(body,0x18)-=adjustment[2];F(obj,0x1A4)=0.0f;func_00145BD0(obj,pos,force,1);
 } else {
 float amount;void *otherBody;amount=F(obj,0x1A4)+0.02f;otherBody=P(obj,8);F(obj,0x1A4)=amount;F(otherBody,0x18)-=amount;func_00145BD0(obj,pos,0,0);
 }
 __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(*(Vec *)((char *)obj+0x40)):"r"((char *)P(obj,8)+0x10):"$2");
 }
}
