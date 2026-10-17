/* UNIT 0x00151DF0 0x00151EE0 */
#include "object_registry.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Target {
    unsigned int type : 6;
    unsigned int bit6 : 1;
    unsigned int x0_7 : 11;
    unsigned int hi : 14;
    char pad4[0x1C];
    float x20[4];
} Target;

typedef struct Source {
    char pad0[0x10];
    float x10[4];
} Source;

typedef struct Entry {
    int x0;
    int x4;
    Target *x8;
    char padC[4];
    Vec x10;
    u128 x20;
} Entry;

extern int D_0029E4AC;
extern Entry D_00381DF0[100];

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)
"
        "mfc1 $2, %2
"
        "qmtc2.ni $2, $vf12
"
        "vmulx.xyz $vf11, $vf11, $vf12x
"
        "sqc2 $vf11, %0
"
        : "=m"(*out) : "r"(v), "f"(s) : "$2");
}



void func_00151DF0(Target *target, Source *src, u128 *data)
{
    Entry *e;

    if (D_0029E4AC < 100) {
        e = &D_00381DF0[D_0029E4AC++];
        e->x4 = target->x0_7;
        e->x8 = target;
        target->x0_7 = RegisterObjectRetail((void *)(e), (unsigned int)(target));
        if (target->type == 4) {
            VecScale(&e->x10, (Vec *)target->x20, e->x4);
        } else {
            VecScale(&e->x10, (Vec *)src->x10, e->x4);
        }
        e->x20 = *data;
    }
}
