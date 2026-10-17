/* UNIT 0x00165CE0 0x00165DE8 .sdata=0x29C814:0x29C818 */
#include "impulse.h"
float D_0029C814 = 1500.0f;

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Flags {
    unsigned long bit0 : 5;
    unsigned long bit5 : 1;
    unsigned long bit6 : 10;
    unsigned long x2 : 16;
    unsigned long rest : 32;
} Flags;

typedef struct Body {
    char pad0[0x1E0];
    Vec x1E0;
    char pad1F0[0x2C0];
    Flags x4B0;
} Body;

typedef struct Rider {
    char pad0[0x40C0];
    Body body;
} Rider;

typedef struct Model {
    unsigned long bit0 : 57;
    unsigned long bit57 : 1;
    unsigned long rest : 6;
} Model;

typedef struct Obj {
    char pad0[0x100];
    Model *x100;
    char pad104[0x8C];
    int state;
    char pad194[4];
    int x198;
    char pad19C[0x284];
    char x420[0x10];
    char x430[0x10];
} Obj;

extern int func_00165648(Obj *obj);
extern void func_00202E38(void *p, int a, int b, float value);


static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "mfc1 $2, %2\n"
        "qmtc2.ni $2, $vf12\n"
        "vmulx.xyz $vf11, $vf11, $vf12x\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

void func_00165CE0(Obj *obj, Rider *rider)
{
    Body *body = &rider->body;
    Vec vec;
    int on = 1;

    body->x4B0.bit5 = on;
    body->x4B0.x2 = on;
    if (obj->state == on) {
        obj->x198 = 0;
        obj->state = 2;
        if (func_00165648(obj)) {
            func_00202E38(obj->x420, 1, 1, 200.0f);
        } else {
            func_00202E38(obj->x430, 1, 1, 200.0f);
        }
    }
    if (obj->state == 2) {
        VecScale(&vec, &body->x1E0, D_0029C814);
        func_00231C88((ImpulseBody *)(body), (Vector4 *)(&vec));
        obj->x100->bit57 = 0;
    }
}
