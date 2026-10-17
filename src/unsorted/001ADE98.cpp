/* UNIT 0x001ADE98 0x001AE0A8 */
#include "rider.h"
typedef Vector4 Vec;
typedef RiderPhysics Physics;
typedef struct Obj { char pad0[8]; float x8; char padc[0xec]; Rider *xf8; } Obj;
extern "C" float func_001385A8(Physics *);
extern "C" float func_00236088(float,float,float,float);
static inline void Scale(Vec *out, Vec *v, float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v),"f"(scale) : "$2");
}
static inline void Add(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void Normalize(Vec *v) {
 float length,inv,x=v->x,y=v->y,z=v->z;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(length) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(length));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
static inline float Dot(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static inline float DotSecond(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %3,%2\n\tmadda.s %1,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->y),"f"(b->x),"f"(a->x),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static inline void ScaledAdd(Vec *out,Vec *a,Vec *b,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b),"f"(scale) : "$2");
}
static inline float Min(float a,float b) { return a <? b; }
static inline float Max(float a,float b) { return a >? b; }
extern "C" void func_001ADE98(Obj *obj,float factor) {
 Vec a,b;
 SceneNode *transform = obj->xf8->bodyNode;
 Rider *rider;
 Physics *physics;
 float blend, target, decay, up, down, dot, current;
 blend=func_001385A8(&obj->xf8->physics);
 rider=obj->xf8;
 physics=&rider->physics;
 Scale(&a,&physics->velocity,blend);
 Vec *source=&physics->node->position;
 blend=1.0f-blend;
 Scale(&b,source,blend);
 Add(&a,&a,&b);
 Normalize(&a);
 dot=-Dot(&a,&transform->matrix.row[2]);
 ScaledAdd(&a,&a,&transform->matrix.row[2],dot);
 dot=Dot(&a,&transform->matrix.row[0]);
 decay=1.0f-Min((float)(physics->x4A4/60),1.0f);
 target = -dot;
 {
  float t = target * (factor * -4.0f);
  float speed = Max(Min((rider->x7AC0-5.0f)*0.05f,1.0f),0.0f);
  t=Max(t,-1.0f);
  target=Min(t,1.0f);
  target *= speed;
 }
 current=obj->x8;
 if(current<=target) { up=0.3f; down=0.2f; up=decay*up; down=decay*down; }
 else { up=0.3f; down=0.2f; }
 obj->x8=func_00236088(current,target,up,down);
}
