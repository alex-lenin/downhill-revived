/* UNIT 0x00231D48 0x00231D88 */
#include "impulse.h"

/* Rotate v by the 3x3 matrix at body->orientation and pass the result on */
void func_00231D48(ImpulseBody *body, const Vector4 *v)
{
    Vector4 rotated;

    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)
	"
        "lqc2 $vf12, 0x0(%2)
	"
        "lqc2 $vf13, 0x10(%2)
	"
        "lqc2 $vf14, 0x20(%2)
	"
        "vmulax.xyz ACC, $vf12, $vf11x
	"
        "vmadday.xyz ACC, $vf13, $vf11y
	"
        "vmaddz.xyz $vf11, $vf14, $vf11z
	"
        "sqc2 $vf11, %0"
        : "=m"(rotated) : "r"(v), "r"(body->orientation) : "memory");
    func_00231C88(body, &rotated);
}
