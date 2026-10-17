/* UNIT 0x00157950 0x00157AB8 */
#include "timer.h"
typedef struct { float x,y,z,w; } Vec;
typedef struct { char *x0; char pad4[12]; char x10[256]; int x110; int x114[2]; int x11c; float x120,x124; char pad128[8]; Vec x130; } Obj;
extern Vec D_00768570[], D_0076E6F0[];
extern char D_00759780[], D_00261A38[];
extern void func_001E9998(void *);
extern int func_001B1C08(void *),func_00238F60(void);
extern void func_00231A08(void *,Vec *),func_00231BD8(void *),func_00232AD0(void *,Vec *),func_00231B70(void *,float,float),func_00218498(int,int);
static inline void clear(Vec *v) { v->x=0; v->y=0; v->z=0; v->w=0; }
void func_00157950(Obj *obj) {
    Vec diff;
    Vec *start,*end;
    int index,i;
    int *item;
    void *state;
    float scale;
    if(obj->x11c) {
        if(obj->x0) func_001E9998(obj->x0+0x5ff0);
        state=obj->x10;
        index=func_001B1C08(obj->x0+0x1e40);
        start=&D_00768570[index];
        func_00231A08(state,start);
        func_00231BD8(state);
        end=&D_0076E6F0[index];
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff):"r"(start),"r"(end));
        scale=(60.0f/(float)func_00238F60())*0.5f;
        __asm__ volatile("lqc2 $vf11,%0\n\tmfc1 $2,%1\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "+m"(diff):"f"(scale):"$2");
        func_00232AD0(state,&diff);
        i=0;
        clear(&obj->x130);
        func_00231B70(state,obj->x124,obj->x120);
        for (;i<obj->x110;i++) func_00218498(obj->x114[i],1);
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(D_00261A38), obj, 1);
    }
}
