/* UNIT 0x001AD098 0x001AD1C0 */
#include "rider.h"
typedef SceneNode Body;
typedef RiderComponent Component;

typedef struct Obj { char pad[0xDC]; char xDC[0x64]; Rider *x140; } Obj;
extern "C" float D_00768590[][4];

extern "C" void func_001ACAE0(void*,float);
extern "C" float func_00223850(float),func_00248EB8(float,float);
extern "C" void func_001AD098(Obj *obj) {
 float v[4] __attribute__((aligned(16))); Component *component; Body *body; float length,inv,angle,clamped,zero; int index;
 component=&obj->x140->component; body=obj->x140->bodyNode;
 index=func_001B1C08(component);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v),"r"(&component->node->position),"r"(D_00768590[index]) : "memory");
 func_00221888(&body->matrix,(Vector4 *)v,(Vector4 *)v); v[0]=0.0f; zero=v[0];
 __asm__ volatile ("mula.s %2,%2
	madda.s %3,%3
	madd.s %0,%4,%4
	sqrt.s %0,%0
	lui $2,0x3f80
	mtc1 $2,%1
	nop
	div.s %1,%1,%0" : "=&f"(length),"=&f"(inv) : "f"(zero),"f"(v[1]),"f"(v[2]),"f"(v[2]) : "$2");
 v[2]*=inv; v[1]*=inv;
 clamped=(v[2] <? 1.0f) >? -1.0f;
 angle=func_00223850(clamped); if(v[1]<zero) angle=-angle;
 angle=func_00248EB8(angle*0.591549f+1.0f,1.0f);
 func_001ACAE0(obj->xDC,angle);
}
