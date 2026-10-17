/* UNIT 0x001326F8 0x00132918 */
#include "object_registry.h"
typedef struct Actor { char pad0[0x158]; int x158; int x15C; char x160[0x2D0]; float x430; char pad434[0x7C]; unsigned long flags; } Actor;
typedef struct Event { unsigned short x0,type,id,x6; int x8; unsigned long *xC; float position[4],normal[4]; } Event;
class Obj { public: virtual void call(int,Event *); virtual void unused(); virtual void vector(float *,float *); };

extern void func_002093C8(unsigned long *,float *,float *);
extern int func_00132918(Actor *,Event *,float *,int);
extern void func_00132CE0(Actor *,Event *,float *,int);
extern float func_0012E070(int);
extern void func_0016A488(void *,float *,float,float);
extern void func_0016AA58(void *),func_0016A9E0(void *);
int func_001326F8(Actor *a,Event *e,int mode,int update,int first,int second) {
    float vec[4] __attribute__((aligned(16)));
    Obj *o; void *p; int result;
    a->flags &= ~0x20;
    o=(Obj *)func_0017D3D0(e->id);
    if(o) o->call(a->x158,e);
    if(a->flags&0x20) return 1;
    __asm__ volatile("sqc2 $vf0, 0x0(%0)" : : "r"(vec) : "memory");
    if(e->xC && (*e->xC & ((unsigned long)0x8000<<43))) {
        func_002093C8(e->xC,e->position,vec);
    } else {
        o=(Obj *)func_0017D3D0(e->id);
        if(o) {
            o->vector(e->position,vec);
            unsigned int normal=e->type;
            if(normal==0x1E75) {
                normal=(unsigned int)e+0x20;
                float dot;
                __asm__("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(dot) : "f"(((float *)normal)[0]),"f"(vec[0]),"f"(((float *)normal)[1]),"f"(vec[1]),"f"(((float *)normal)[2]),"f"(vec[2]));
                dot=-dot;
                __asm__ volatile("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, 0x0(%1)" : : "r"(vec), "r"(normal) : "memory");
                __asm__ volatile("qmtc2 %1, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, 0x0(%0)" : : "r"(vec),"r"(dot) : "memory");
            }
        }
    }
    if(!func_00132918(a,e,vec,mode)) return 0;
    if(update && e->id==0) {
        void *sub=a->x160; float *normal=e->normal; float value=func_0012E070(a->x158);
        func_0016A488(sub,normal,a->x430,value);
    }
    if(first && mode==0) func_00132CE0(a,e,vec,0);
    if(second && mode==1) func_00132CE0(a,e,vec,1);
    if(update) { void *p=a->x160; func_0016AA58(p); func_0016A9E0(p); }
    return 1;
}
