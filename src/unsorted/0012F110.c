/* UNIT 0x0012F110 0x0012F328 */
#include "impulse.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Vtable { char pad[0x20]; short delta,pad2; char *(*call)(void *); };
struct Virtual { struct Vtable *vt; };
struct Transform { char pad[0x10]; Vec x10,x20; };
struct Obj { char pad[0x3FD0]; char x3FD0[0xF0]; char x40C0[0xE0C]; void *x4ECC; char x4ED0[0x20]; char x4EF0[0x13A8]; char x6298[0x1694]; struct Transform *x792C; };
extern void func_0012FE38(struct Obj *,void *);
extern struct Transform *func_00138558(void *);
extern float func_00222B68(float,float);

extern void func_00155E78(void *,int);
extern void func_001A77E0(void *,Vec *);
static inline void Sub(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline float Dot(Vec *a,Vec *b) {
 float r; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r;
}
static inline void Normalize(Vec *out,Vec *v,float one) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(one),"f"(len));
 out->x=x*inv; out->y=y*inv; out->z=z*inv;
}
static inline float NormalizeFirst(Vec *out,Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv,one;
 __asm__ volatile("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
 one=1.0f;
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(one),"f"(len));
 x*=inv; y*=inv; z*=inv; out->x=x; out->y=y; out->z=z;
 return one;
}
static inline void AddScale(Vec *out,Vec *a,Vec *b,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b),"f"(scale) : "$2");
}
static inline void Scale(Vec *out,Vec *v,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v),"f"(scale) : "$2");
}
void func_0012F110(struct Obj *obj,struct Virtual *other,float a,float b,float blend) {
 Vec delta,dir; Vec *pos; char *target; void *state; float dot,scale,limit,one;
 if(obj->x4ECC==obj->x6298) func_0012FE38(obj,obj->x4ED0);
 pos=&obj->x792C->x10;
 target=other->vt->call((char *)other+other->vt->delta);
 Sub(&delta,pos,(Vec *)(target+0x10));
 one=NormalizeFirst(&dir,&delta);
 state=obj->x40C0;
 dot=-Dot(&func_00138558(state)->x20,&dir);
 AddScale(&dir,&dir,&func_00138558(state)->x20,dot);
 Normalize(&dir,&dir,one);
 dir.z+=0.5f;
 Normalize(&dir,&dir,one);
 scale=func_00222B68(a,b);
 limit=b-(b-a)*blend;
 Scale(&dir,&dir,scale);
 func_00231C88((ImpulseBody *)(state), (Vector4 *)(&dir));
 if(limit<scale) func_00155E78(obj->x3FD0,2);
 func_001A77E0(obj->x4EF0,&dir);
}
