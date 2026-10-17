/* UNIT 0x001AEED0 0x001AF090 */
#include "camera.h"
#include "rider.h"
typedef Vector4 Vec;
typedef RiderCamera Obj;
extern void func_001B3F48(Obj *,Vec *);

#define SUB(out,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out) : "r"(a),"r"(b),"r"(&(out)),"m"(*obj))
static __inline__ void normalize(Vec *v) {
 float length,scale;
 __asm__ volatile("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(scale),"=&f"(length) : "f"(v->x),"f"(v->y),"f"(v->z),"f"(v->z) : "$2");
 v->x*=scale; v->y*=scale; v->z*=scale;
}
static __inline__ void normalize_second(Vec *v) {
 float length,scale;
 __asm__ volatile("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(scale),"=&f"(length) : "f"(v->x),"f"(v->y),"f"(v->z),"f"(v->x),"f"(v->y),"f"(v->z),"f"(v->x) : "$2");
 v->x*=scale; v->y*=scale; v->z*=scale;
}
void func_001AEED0(char *arg0,Obj *obj) {
 Vec a,b; Matrix4 matrix; Vec pos;
 SceneNode *body=obj->xA10->node; SceneNode *rbody=obj->xA10->rider->bodyNode;
 float dot;
 func_001B3F48(obj,&a); SUB(a,&body->position,&a); normalize(&a);
 func_001B3E98(obj,&pos); SUB(b,&obj->x90,&pos); normalize_second(&b);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "0"(b.x),"f"(a.x),"f"(b.y),"f"(a.y),"f"(b.z),"f"(a.z));
 if(__builtin_fabsf(dot)>0.95f) { SUB(b,&obj->x60,&pos); }
 func_00221888(&rbody->matrix,&a,&a); func_00221888(&rbody->matrix,&b,&b);
 func_001AE8A8(rbody,&a,&b,&matrix); func_002219A8((Matrix4 *)(arg0+0x60),&matrix,(Matrix4 *)(arg0+0x20));
}
