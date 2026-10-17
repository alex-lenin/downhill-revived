/* UNIT 0x00134AF8 0x00134CC8 */
#include "particle.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Effect Effect;

typedef struct Holder {
    Effect *effect;
} Holder;

struct Effect {
    char pad0[0x2630];
    u128 x2630;
    char pad2640[0x40];
    u128 x2680;
    u128 x2690;
    char pad26A0[0x86];
    unsigned short x2726;
    char pad2728[4];
    float x272C;
    char pad2730[0x14];
    float x2744;
    char pad2748[0x10];
    unsigned int x2758;
    char pad275C[0x10];
    unsigned short x276C;
    char pad276E[0xDA];
    Holder *x2848;
};

typedef struct Obj {
    char pad0[0x10];
    Vec4 x10;
} Obj;

typedef struct Tune {
    char pad0[0x3C6];
    unsigned short x3C6;
    char pad3C8[4];
    float x3CC;
    char pad3D0[0x14];
    float x3E4;
    char pad3E8[0xA6C];
    Vec4 *xE54;
} Tune;

extern unsigned int D_0027CE38[];
extern Tune D_0027BFC0;

extern void func_0022DDD8(Effect *);

extern Effect *func_0022DD88(void *);

static inline float Normalize(Vec4 *out, Vec4 *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3
	madda.s %4, %4
	madd.s %1, %5, %5
	sqrt.s %1, %1
	"
                      "lui %2, 0x3f80
	mtc1 %2, %0
	nop
	div.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

void func_00134AF8(Holder *holder, Obj *obj, Vec4 *dirIn)
{
    Vec4 dir;
    Vec4 n;
    Vec4 m[4];
    float *d;
    Effect *e;
    float len;

    d = (float *)&dir;
    dir = *dirIn;
    e = holder->effect;
    if (e != 0 && e->x2758 < D_0027CE38[0]) {
        func_0022DDD8(e);
        holder->effect = 0;
        e = 0;
    }
    len = Normalize(&n, (Vec4 *)d);
    if (e == 0) {
        func_002226C8(m, &n);
        {
        Vec4 rot = { -0.9659258f, 0.25881904f };

        __asm__ volatile ("lqc2 $vf11, 0(%0)" : : "r" (&m[1]));
        __asm__ volatile ("lqc2 $vf12, 0(%0)" : : "r" (&m[2]));
        __asm__ volatile ("lqc2 $vf13, 0(%0)" : : "r" (&rot));
        __asm__ volatile ("vmove.w $vf14, $vf11
	"
                          "vmulax.xyz ACC, $vf11, $vf13x
	"
                          "vmaddy.xyz $vf14, $vf12, $vf13y
	"
                          "vmulax.xyz ACC, $vf12, $vf13x
	"
                          "vmsuby.xyz $vf12, $vf11, $vf13y");
        __asm__ volatile ("sqc2 $vf14, 0(%0)" : : "r" (&m[1]));
        __asm__ volatile ("sqc2 $vf12, 0(%0)" : : "r" (&m[2]));
        }
        e = func_0022DD88(func_0022CDC0((Vector4 *)(&obj->x10), 0x39, (Matrix4 *)(m), (int)(0), 1.0f, (Vector4 *)(0)));
        if (e == 0) {
            return;
        }
        holder->effect = e;
        e->x2848 = holder;
    }
    if (e->x276C == 0x39) {
        Tune *t = &D_0027BFC0;

        e->x2744 = t->x3E4 * len;
        e->x272C = t->x3CC * len;
        e->x2726 = t->x3C6;
        e->x2680 = e->x2630;
        __asm__ volatile ("lqc2 $vf11, 0(%1)\n\t"
                          "lqc2 $vf12, 0(%2)\n\t"
                          "vadd.xyz $vf11, $vf11, $vf12\n\t"
                          "sqc2 $vf11, %0"
            : "=m" (e->x2690) : "r" (&obj->x10), "r" (t->xE54));
        e->x2630 = e->x2690;
    }
}
