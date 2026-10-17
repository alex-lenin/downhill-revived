/* UNIT 0x00192460 0x001926E0 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct { char pad0[0x10]; Vec pos; char pad20[0x20]; Vec matrix[4]; char pad80[0x20]; Vec direction; } Body;
typedef struct { char pad0[0x4098]; unsigned long flags; char pad40a0[0x388c]; Body *body; } Target;
typedef struct { char pad0[8]; Body *body; char padc[8]; Target *target; char pad18[0x38]; Vec x50; char pad60[0x14]; int timer; char pad78[0x18]; Vec x90; } Obj;
extern int func_00222BD8(int,int);
extern void func_00191BC0(Obj *,int);
extern void func_00222578(float *,float *,float *);
extern float func_00235F38(float,float,float,float);
extern float func_00235EA8(float,float,float,float);
extern void func_00221F50(float *,float *);
#if defined(__mips__)
extern float func_00211980(float *,float *,int,float);
#endif
extern float func_00222330(float *,float *);
extern void func_001928A0(Obj *,Target *);
extern void func_00211280(Body *);
#define TRANSFORM(d,a,m) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(d) : "r"(a),"r"(m) : "memory")
#define ADD(d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(d) : "r"(a),"r"(b) : "memory")
void func_00192460(Obj *o) {
 Vec v; int timer; float height,z,desired; Target *t;
 timer=o->timer++;
 if(timer < func_00222BD8(30,60)) {
  func_00222578(v,o->body->pos,o->target->body->pos);
  o->body->direction[0]=func_00235F38(o->body->direction[0],v[0],0.08f,0.001f);
  o->body->direction[1]=0;
  o->body->direction[2]=func_00235F38(o->body->direction[2],v[2],0.08f,0.001f);
  func_00221F50(o->body->matrix[0],o->body->direction);
  t=o->target;
  TRANSFORM(v,o->x90,t->body->matrix[0]);
  ADD(v,t->body->pos,v);
  o->body->pos[0]=func_00235EA8(o->body->pos[0],v[0],0.15f,0.001f);
  o->body->pos[1]=func_00235EA8(o->body->pos[1],v[1],0.15f,0.001f);
  o->body->pos[2]=func_00235EA8(o->body->pos[2],v[2],0.15f,0.001f);
  if((unsigned char)(o->target->flags>>2)&1) func_00191BC0(o,4);
 } else func_00191BC0(o,4);
 height=CallGroundProbe(o->body->pos,o->x50,0,15.0f);
 z=o->body->pos[2]; desired=z-height;
 if(height<5.0f) o->body->pos[2]=func_00235EA8(z,desired+5.0f,0.15f,0.001f);
 if(func_00222330(o->body->pos,o->target->body->pos)<10.0f) func_001928A0(o,o->target);
 func_00211280(o->body);
}
