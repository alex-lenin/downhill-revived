/* UNIT 0x00157F80 0x00158058 */
#include "impulse.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    char x10[0x118];
    float x128;
    float x12C;
} Body;

typedef struct Input {
    char pad0[0x10];
    char x10[0x10];
    char x20[0x10];
} Input;

extern void func_002325E0(void *, Vec4 *, Vec4 *, void *, void *);

extern void func_00232450(void *, Vec4 *, Vec4 *, void *, void *, float);

int func_00157F80(Body *body, Input *in) {
    Vec4 a;
    Vec4 b;
    float scale = body->x12C;

    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(b));
    func_002325E0(body->x10, &a, &b, in->x10, in->x20);
    __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tmfc1 $2, %1\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0(%0)" : : "r"(&a), "f"(scale) : "$2", "memory");
    func_00231CC0((ImpulseBody *)(body->x10), (Vector4 *)(&a), (Vector4 *)(in->x10));
    func_00232450(body->x10, &a, &b, in->x10, in->x20, body->x128);
    func_00231CC0((ImpulseBody *)(body->x10), (Vector4 *)(&a), (Vector4 *)(in->x10));
    return 1;
}
