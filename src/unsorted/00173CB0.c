/* UNIT 0x00173CB0 0x00173E58 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundHeightFirst
#endif
typedef struct Part { char pad[0x10]; float pos[4]; } Part;
typedef struct Obj { int x0,x4,x8; Part *xC; int x10; Part *x14; char pad18[0x78]; float x90[4]; char padA0[8]; float xA8; char padAC[12]; float xB8; } Obj;
extern float func_00235EA8(float,float,float,float);
extern void func_00173E58(Obj*,float),func_00174098(Obj*),func_00173A18(Obj*);
#if defined(__mips__)
extern float func_00211980(float*,float*,float,int);
#endif
void func_00173CB0(Obj *obj) {
    float hit[4],dist,amount,clamped;
    float *v;
    obj->xB8 = func_00235EA8(obj->xB8,0.0f,0.05f,0.001f);
    v=obj->x90;
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(obj->x90) : "r"(obj->x14->pos),"r"(obj->xC->pos), "r"(v));
    dist=v[0];
    __asm__ volatile("mula.s %0,%0
	madda.s %1,%1
	madd.s %0,%2,%2" : "+f"(dist) : "f"(v[1]),"f"(v[2]));
    if(dist>90000.0f) { func_00173E58(obj,1.57079637f); func_00174098(obj); }
    if(obj->xB8<0.24444444f && obj->x4!=3) {
        amount=CallGroundProbe(obj->xC->pos,hit,0.0f,0);
        if(amount!=0.0f) {
            __asm__ volatile("max.s %0,%1,%2" : "=&f"(clamped) : "f"(0.3f),"f"(hit[2]));
            amount*=clamped;
            if(amount<60.0f) obj->xC->pos[2]+=0.5f;
            else if(obj->xC->pos[2]-obj->xA8>60.0f) obj->xC->pos[2]-=0.5f;
        }
    }
    func_00173A18(obj);
}
