/* UNIT 0x001668C8 0x00166920 */
#include "impulse.h"

/* out = body->orientation (3x3 rotation) * in + body->translation; uses VU0 inline asm */
void func_001668C8(ImpulseBody *body, Vector4 *out, const Vector4 *in) {
    float x = in->x;
    float y = in->y;
    float z = in->z;

    out->x = x;
    out->y = y;
    out->z = z;
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%0)\n\t"
        "lqc2 $vf12, 0x0(%1)\n\t"
        "lqc2 $vf13, 0x10(%1)\n\t"
        "lqc2 $vf14, 0x20(%1)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, 0x0(%0)"
        : : "r"(out), "r"(body->orientation) : "memory");
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%0)\n\t"
        "lqc2 $vf12, 0x0(%1)\n\t"
        "vadd.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, 0x0(%0)"
        : : "r"(out), "r"(body->translation) : "memory");
}
