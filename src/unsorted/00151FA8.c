/* UNIT 0x00151FA8 0x00152088 */
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

typedef struct Entry {
    int x0;
    int x4;
    Target *x8;
    char padC[4];
    Vec x10;
    u128 x20;
} Entry;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern int D_0029E4B0;
extern Entry D_003830B0[50];

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



void func_00151FA8(Target *target, int unused, u128 *data)
{
    Entry *e;

    if (D_0029E4B0 < 50 && target->type == 4) {
        e = &D_003830B0[D_0029E4B0++];
        e->x4 = target->x0_7;
        e->x8 = target;
        target->x0_7 = RegisterObjectRetail((void *)(e), (unsigned int)(target));
        VecScale(&e->x10, (Vec *)target->x20, 1.0f);
        COPY_VECTOR(e->x20, data);
    }
}
