/* UNIT 0x0017CF98 0x0017D110 */
#include "particle.h"
#include "timer.h"
typedef struct Vec { float v[4]; } Vec;
typedef struct Effect { char pad[0x272c]; float x272c; char pad2730[0x20]; float x2750,x2754; } Effect;
typedef struct Params { char pad[8]; float x8; char padc[0x14]; float x20; char pad24[0x3c]; Vec x60; char pad70[0x30]; Vec xa0; } Params;
typedef struct Obj { char pad[0x14]; char *x14; char pad18[8]; Params *x20; char pad24[4]; float x28; } Obj;
extern char D_00759780[];
typedef struct Global { char pad[0x240]; int x240; } Global;
extern Global D_00398C40;
extern float D_003B7754[];
extern void func_00261E18(void);
extern int func_0017CD70(Obj *);
extern void func_001D4670(void *,void *);
extern Effect *func_0022DD88(int);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define SCALE(dst,src,s) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(src), "f"(s) : "$2")
static __inline__ void setup(char *base, char *p) { func_001D4670(base+0x240,p+0x10); }
void func_0017CF98(Obj *obj,char *arg1,char *arg2) {
    Vec vec;
    Params *params;
    Effect *effect;
    float dot,old,scale,z;
    if (!obj->x14 && func_0017CD70(obj)) {
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00261E18), obj, 1);
        setup((char *)&D_00398C40,obj->x14);
        params=obj->x20;
        if(params) {
            effect=func_0022DD88(func_0022CDC0((Vector4 *)(&params->xa0), 0, (Matrix4 *)(&params->x60), (int)(0xc0807060), 1.0f, (Vector4 *)(0)));
            if(effect) {
                scale=params->x8*0.5f;
                effect->x2754=scale;
                effect->x2750=scale;
                effect->x272c=effect->x272c*0.00041666668f*(params->x8*params->x20);
            }
        }
    }
    if(arg1) {
        volatile float *normal;
        float nx,ax,ay;
        COPY_VECTOR(vec,arg1+0x4180);
        normal=(volatile float *)(arg2+0x20);
        old=obj->x28;
        nx=*(float *)(arg2+0x20);
        z=vec.v[2]-old;
        ax=vec.v[0];
        ay=vec.v[1];
        *(volatile float *)&vec.v[2]=z;
        dot=ax;
        __asm__ volatile("mula.s %0,%1\n\tmadda.s %2,%3\n\tmadd.s %0,%4,%5" : "+f"(dot) : "f"(nx),"f"(ay),"f"(normal[1]),"f"(z),"f"(normal[2]));
        SCALE(vec,normal,dot);
        obj->x28=old+vec.v[2]*D_003B7754[0];
    }
}
