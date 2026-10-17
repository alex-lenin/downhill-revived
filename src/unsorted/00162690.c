/* UNIT 0x00162690 0x001627C8 */
#include "particle_emission.h"
#include "particle.h"
typedef unsigned int u128 __attribute__((mode(TI)));
struct Params { char pad0[0x10]; float x10[4]; float x20[4]; char pad30[0x10]; float x40[4][4]; };
struct Globals { char pad0[0x180]; int x180; short x184; unsigned short x186; };
extern unsigned short D_0029DBAC;
extern float D_0026E960[4];
extern struct Globals D_0027BFC0;
extern int func_00161868(int,void *);
extern struct Particle *func_0022DD88(void *);
#if defined(__mips__)
void func_00162690(struct Particle **owner,int active,struct Params *params,float scale)
#else
void SharedCall00162690(struct Particle ** owner, int active, struct Shared00162690_struct_Params * params_view, float scale)
#endif
{
#if !defined(__mips__)
    struct Params * params = (struct Params *)params_view;
#endif

 float v[4] __attribute__((aligned(16)));
 struct Particle *p;
 u128 pos;
 if(active) {
  p=*owner;
  if(!p) {
   int value=func_00161868(0x30607080,params->x20);
   p=func_0022DD88(func_0022CDC0((Vector4 *)(params->x10), 6, (Matrix4 *)(0), (int)(value), scale, (Vector4 *)(0)));
   if(!p) return;
   *owner=p;
   p->x2848=owner;
   p->x276E=D_0029DBAC+8;
  }
  if(p->x2850<248 && (unsigned int)(D_0029DBAC-p->x276E)>=8) {
   __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)" : : "r"(v),"r"(D_0026E960),"r"(params->x40) : "memory");
   __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x2690(%0)" : : "r"(p),"r"(params->x10),"r"(v) : "memory");
   pos=p->x2690;
   p->x2726=D_0027BFC0.x186;
   p->x2720=D_0027BFC0.x180;
   p->x2630=pos;
   p->x2680=pos;
   p->x276E=D_0029DBAC;
  }
 }
}
