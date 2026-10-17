/* UNIT 0x00192268 0x00192460 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Mtx { Vec row[4]; } Mtx;
struct Body { char pad0[0x10]; Vec pos; char pad20[0x20]; Mtx mtx; char pad80[0x20]; Vec rot; };
struct Other { char pad0[0x792C]; struct Body *x792C; };
struct Obj { char pad0[8]; struct Body *x8; char padC[8]; struct Other *x14; char pad18[0x38]; Vec x50; float x60,x64; };
extern void func_00191BC0(struct Obj *,int,Vec *);
extern void func_00222578(Vec *,Vec *,Vec *);
extern float func_00235F38(float,float,float,float);
extern float func_00235EA8(float,float,float,float);
extern void func_00221F50(Mtx *,Vec *);
#if defined(__mips__)
extern float func_00211980(Vec *,Vec *,int,float);
#endif
extern float func_00222330(Vec *,Vec *);
extern void func_001928A0(struct Obj *,struct Other *);
extern void func_00211280(struct Body *);
static inline void ApplyMatrix(Vec *out,Mtx *m,Vec *in) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(in),"r"(m));
}
static inline void VecAdd(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void adjust(struct Obj *o,float current,float target,float distance) {
 if(distance<5.0f) o->x8->pos.z=func_00235EA8(current,target+5.0f,0.03f,0.001f);
}
void func_00192268(struct Obj *o) {
 Vec v; float d,y,t; Vec *target; struct Body *body;
 body=o->x8; target=&o->x14->x792C->pos;
 if(body->pos.z<target->z) {
 func_00222578(&v,&body->pos,target);
 o->x8->rot.x=func_00235F38(o->x8->rot.x,v.x,0.15f,0.001f);
 o->x8->rot.y=0.0f;
 o->x8->rot.z=func_00235F38(o->x8->rot.z,v.z,0.15f,0.001f);
 func_00221F50(&o->x8->mtx,&o->x8->rot);
 } else func_00191BC0(o,3,target);
 t=func_00235EA8(o->x60,o->x64,0.05f,0.001f); o->x60=t; v.y=t; v.x=0.0f; v.z=0.0f;
 ApplyMatrix(&v,&o->x8->mtx,&v); VecAdd(&o->x8->pos,&o->x8->pos,&v);
 d=CallGroundProbe(&o->x8->pos,&o->x50,0,15.0f); y=o->x8->pos.z;
 adjust(o,y,y-d,d);
 if(func_00222330(&o->x8->pos,&o->x14->x792C->pos)<10.0f) func_001928A0(o,o->x14);
 func_00211280(o->x8);
}
