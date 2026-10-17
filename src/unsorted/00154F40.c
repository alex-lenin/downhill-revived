/* UNIT 0x00154F40 0x00155090 */
#include "spatial_primitives.h"
#if defined(__mips__)
#define CallShared00211390_0 func_00211390
#else
#define CallShared00211390_0 Forward00211390_1
#endif
#include "object_registry.h"
typedef struct Body { unsigned int *x0; char pad4[0xC]; float x10[4]; char pad20[0x30]; float x50[4]; } Body;
typedef struct Obj { char pad0[0x792C]; Body *x792C; } Obj;
typedef struct Arg { Obj *x0; char pad4[0x24]; Body *x28; } Arg;
extern float func_002487A0(float);
extern int func_00211390(void *,float,int,int,Body **,int);

Body *func_00154F40(Arg *arg,float angle,float radius) {
 Body *nearby[16];
 float delta[4] __attribute__((aligned(16)));
 Obj *self=arg->x0;
 Body *best=0;
 float threshold=func_002487A0(angle);
 int found=CallShared00211390_0(arg->x28->x10,radius,1200,4499,nearby,15);
 int count;
 Body **p;
 if(found>0) {
  count=found;
  p=nearby;
  do {
   if((int)func_0017D3D0((*(*p)->x0>>7)&0x7FF)) {
    Body *item=*p;
    float length,dot,cosine;
    float *forward;
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(delta) : "r"(item->x10),"r"(self->x792C->x10) : "memory");
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(delta[0]),"f"(delta[1]),"f"(delta[2]));
    forward=self->x792C->x50;
    __asm__ volatile("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(dot) : "f"(forward[0]),"f"(forward[1]),"f"(forward[2]),"f"(delta[0]),"f"(delta[1]),"f"(delta[2]));
    cosine=dot*(1.0f/length);
    if(threshold<cosine && length<radius) {threshold=cosine;best=item;}
   }
   count--;p++;
  } while(count);
 }
 return best;
}
