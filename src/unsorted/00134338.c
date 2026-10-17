/* UNIT 0x00134338 0x001345A8 */
#include "impulse.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef unsigned int U128 __attribute__((mode(TI)));
typedef struct Body { unsigned long flags; char pad8[8]; Vec x10,x20; char pad30[0x10]; Vec x40; } Body;
typedef struct Info { char pad0[0x281C]; void *x281C; } Info;
typedef struct Rider { char pad0[0x792C]; Body *x792C; char pad7930[0x114]; Info *x7A44; } Rider;
typedef struct Physics { char pad0[0x38]; void *x38; } Physics;
typedef struct Obj { int x0; Rider *x4; Body *x8; int xC; Physics x10; char pad4C[0xC8]; float x114,x118; char pad11C[0xB4]; int x1D0; } Obj;
extern int D_0029C2C8;
extern void func_002325E0(Physics *,Vec *,Vec *,Vec *,Vec *);
extern void func_00232450(Physics *,Vec *,Vec *,Vec *,Vec *,float);

extern void func_001CC520(void *,float);
extern void func_001B5DE8(Vec *,Vec *,int,Vec *,void *,void *,Rider *,float);
static inline void Scale(Vec *dst,Vec *src,float scale) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(src),"f"(scale) : "$2"); }
static inline void Cross(Vec *dst,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b)); }
static inline float Normalize(Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(len) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
 return len;
}
static inline float Length(Vec *v) { float len; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(v->x),"f"(v->y),"f"(v->z)); return len; }
static inline float Dot(Vec *a,Vec *b) { float d; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(d) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return d; }
static inline void Project(Vec *dst,Vec *a,Vec *b,float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmsubx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b),"f"(f) : "$2"); }
int func_00134338(Obj *obj,Body *body,int mode) {
 Vec impulse,zero,cross,a,b;
 Physics *physics;
 Vec *pos,*normal;
 float strength=obj->x118;
 float length;
 if(mode==2) strength*=1.25f;
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(zero));
 physics=&obj->x10; pos=&body->x10; normal=&body->x20;
 func_002325E0(physics,&impulse,&zero,pos,normal);
 Scale(&impulse,&impulse,strength);
 Cross(&cross,normal,&obj->x8->x40);
 if(Normalize(&cross)>0.5f) Project(&impulse,&impulse,&cross,Dot(&impulse,&cross));
 func_00231CC0((ImpulseBody *)(physics), (Vector4 *)(&impulse), (Vector4 *)(pos));
 func_00232450(physics,&impulse,&zero,pos,normal,obj->x114);
 func_00231CC0((ImpulseBody *)(physics), (Vector4 *)(&impulse), (Vector4 *)(pos));
 length=Length(&impulse);
 if(length>400.0f && obj->x1D0>=D_0029C2C8) {
 func_001CC520((char *)obj->x4+0x7720,length); obj->x1D0=0;
 Scale(&impulse,&impulse,0.1f);
 if(*((unsigned char *)obj->x4->x792C+5)) {
 *(U128 *)&a=*(U128 *)&body->x20; *(U128 *)&b=*(U128 *)&body->x10;
 func_001B5DE8(&a,&b,(unsigned short)(body->flags>>53)&63,&impulse,physics->x38,obj->x4->x7A44->x281C,obj->x4,length*0.0005f);
 }
 }
 return 1;
}
