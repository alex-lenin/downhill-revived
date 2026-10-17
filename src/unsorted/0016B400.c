/* UNIT 0x0016B400 0x0016B5A8 */
#include "motion_sample.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vec { u128 q; float f[4]; } Vec;
typedef struct Rider { char pad0[0x793c]; void *x793c; } Rider;
typedef struct Entry { char pad0[0x2b4]; Rider *rider; char pad2b8[0x380-0x2b8]; } Entry;
typedef struct Obj { float x0,x4; char pad8[8]; Vec x10; char pad20[0x40]; float x60; char pad64[0x90-0x64]; float x90; } Obj;
extern char D_0037EA60[];
extern Vec *func_0023CF78(int);
extern void *func_0023D180(int);

extern void func_0021D758(void *,float);
extern void func_0023D1A0(int);
extern void func_0023D1C8(int);
extern void func_0023D2B0(int,float);
extern int func_0023D608(void);
void func_0016B400(Obj *obj) {
    Vec pos;
    float fraction, value, strength, base, a, b, first_base;
    Rider *rider;
    int i=0, result;
    void *target;
    float *field = (float *)((char *)obj+0x40);
    for (; i<func_0023D608(); i++) {
        rider=((Entry *)(D_0037EA60+i*0x380))->rider;
        pos.q=func_0023CF78(i)->q;
        target=func_0023D180(i);
        result=func_00167A00(&obj->x10,i,&fraction,(char *)target+0xb8,&value,pos.f[2]);
        func_0021D758(rider->x793c,value);
        if (result==0 || (result==1 && fraction<1.0f)) {
            first_base=field[8];
            strength=obj->x0-(pos.f[2]-first_base);
            func_0023D1A0(i);
            func_0023D2B0(i,strength*obj->x4);
        } else {
            func_0023D1C8(i);
            if (result==1 || (result==2 && fraction<1.0f)) {
                base=obj->x90;
                a=pos.f[2]-base;
                b=field[8]-base;
                func_0023D2B0(i,((obj->x0*a)/b)*obj->x4);
            } else func_0023D2B0(i,0.0f);
        }
    }
}
