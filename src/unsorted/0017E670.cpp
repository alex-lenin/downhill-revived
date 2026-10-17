/* UNIT 0x0017E670 0x0017E7F0 */
#include "object_registry.h"
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
struct Body { char pad[16]; Vec pos; char pad20[32]; Vec x40; };
struct Rider { char pad[0x792c]; Body *body; };
struct Obj { Rider *rider; };
struct Node { unsigned *flags; char pad[12]; Vec pos; };
class Target { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual Body *v3(); };
extern "C" int func_00211390(Vec *, int, int, Node **, int, float);

extern "C" void func_00221888(Vec *, Vec *, int);
int func_0017E670(Obj *obj, int arg1) {
 Node *nodes[10]; Vec diff; Target *best; int result; float distance;
 distance=99999.0f; best=0; result=0;
 int total = func_00211390(&obj->rider->body->pos,1200,4499,nodes,10,40.0f);
 int count;
 if(total>0) { count=total; Node **p=nodes; do {
 Target *candidate=(Target *)func_0017D3D0((*(*p)->flags >> 7)&0x7ff);
 if(candidate) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(&(*p)->pos), "r"(&obj->rider->body->pos));
 float length;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(diff.x),"f"(diff.y),"f"(diff.z));
 if(length<distance) { distance=length; best=candidate; }
 }
 --count; ++p;
 }while(count); }
 if(best) {
 Body *body=best->v3();
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(&body->pos), "r"(&obj->rider->body->pos));
 func_00221888(&obj->rider->body->x40,&diff,arg1); result=1;
 }
 return result;
}
