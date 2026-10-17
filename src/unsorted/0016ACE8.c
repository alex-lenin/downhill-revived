/* UNIT 0x0016ACE8 0x0016AD98 */
#include "impulse.h"
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Other {
    char pad0[0x10];
    Vec x10;
} Other;

typedef struct Body {
    char pad0[0x38];
    Other *x38;
    char pad3C[0xC0 - 0x3C];
    Vec xC0;
} Body;

typedef struct Obj {
    char pad0[0x4];
    Body *x4;
    char pad8[0x4];
    float xC;
} Obj;

extern char D_0026EC18[];
extern float func_0016AD98(void *table, float t);


static inline float InnerProduct(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

void func_0016ACE8(Obj *obj)
{
    Vec *dir;
    Vec v;

    dir = &obj->x4->x38->x10;
    if (InnerProduct(&obj->x4->xC0, dir) > 1.0f) {
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(v) : "r"(dir) : "$2");
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "mfc1 $2, %2\n\t"
                          "qmtc2.ni $2, $vf12\n\t"
                          "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(v) : "r"(&v), "f"(-func_0016AD98(D_0026EC18, obj->xC)) : "$2", "memory");
        func_00231C88((ImpulseBody *)(obj->x4), (Vector4 *)(&v));
    }
}
