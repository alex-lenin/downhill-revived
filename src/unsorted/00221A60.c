/* UNIT 0x00221A60 0x00221AB8 */
#include "vector.h"
void func_00221A60(Matrix4 *out, const Matrix4 *a, const Matrix4 *b) {
    __asm__ volatile (
        "lqc2       $vf14, 0x0(%1)
	"
        "lqc2       $vf15, 0x10(%1)
	"
        "lqc2       $vf16, 0x20(%1)
	"
        "lqc2       $vf17, 0x30(%1)
	"
        "lqc2       $vf10, 0x0(%2)
	"
        "lqc2       $vf11, 0x10(%2)
	"
        "lqc2       $vf12, 0x20(%2)
	"
        "vmulax.xyzw ACC, $vf10, $vf14x
	"
        "vmadday.xyzw ACC, $vf11, $vf14y
	"
        "vmaddz.xyz $vf14, $vf12, $vf14z
	"
        "vmulax.xyzw ACC, $vf10, $vf15x
	"
        "vmadday.xyzw ACC, $vf11, $vf15y
	"
        "vmaddz.xyz $vf15, $vf12, $vf15z
	"
        "vmulax.xyzw ACC, $vf10, $vf16x
	"
        "vmadday.xyzw ACC, $vf11, $vf16y
	"
        "vmaddz.xyz $vf16, $vf12, $vf16z
	"
        "sqc2       $vf14, 0x0(%0)
	"
        "sqc2       $vf15, 0x10(%0)
	"
        "sqc2       $vf16, 0x20(%0)
	"
        "sqc2       $vf17, 0x30(%0)
	"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}
