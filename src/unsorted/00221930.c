/* UNIT 0x00221930 0x002219A8 */
#include "vector.h"
void func_00221930(Matrix4 *out, const Matrix4 *left, const Matrix4 *right) {
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
        "lqc2       $vf13, 0x30(%2)
	"
        "vmulax.xyzw ACC, $vf10, $vf14x
	"
        "vmadday.xyzw ACC, $vf11, $vf14y
	"
        "vmaddaz.xyzw ACC, $vf12, $vf14z
	"
        "vmaddw.xyzw $vf14, $vf13, $vf14w
	"
        "vmulax.xyzw ACC, $vf10, $vf15x
	"
        "vmadday.xyzw ACC, $vf11, $vf15y
	"
        "vmaddaz.xyzw ACC, $vf12, $vf15z
	"
        "vmaddw.xyzw $vf15, $vf13, $vf15w
	"
        "vmulax.xyzw ACC, $vf10, $vf16x
	"
        "vmadday.xyzw ACC, $vf11, $vf16y
	"
        "vmaddaz.xyzw ACC, $vf12, $vf16z
	"
        "vmaddw.xyzw $vf16, $vf13, $vf16w
	"
        "vmulax.xyzw ACC, $vf10, $vf17x
	"
        "vmadday.xyzw ACC, $vf11, $vf17y
	"
        "vmaddaz.xyzw ACC, $vf12, $vf17z
	"
        "vmaddw.xyzw $vf17, $vf13, $vf17w
	"
        "sqc2       $vf14, 0x0(%0)
	"
        "sqc2       $vf15, 0x10(%0)
	"
        "sqc2       $vf16, 0x20(%0)
	"
        "sqc2       $vf17, 0x30(%0)
	"
        : : "r"(out), "r"(left), "r"(right) : "memory");
}
