/* UNIT 0x002239B0 0x00223A38 */
#include "matrix_packets.h"
void func_002239B0(Matrix4 *out, const Matrix4 *a, const Matrix4 *b) {
 __asm__ volatile (
 "lqc2       $vf14, 0x0(%2)
	"
 "lqc2       $vf15, 0x10(%2)
	"
 "lqc2       $vf16, 0x20(%2)
	"
 "lqc2       $vf11, 0x0(%1)
	"
 "lqc2       $vf12, 0x10(%1)
	"
 "lqc2       $vf13, 0x20(%1)
	"
 "vmove.xyzw $vf20, $vf14
	"
 "vaddx.z    $vf14, $vf0, $vf16x
	"
 "vmove.xyzw $vf21, $vf15
	"
 "vaddy.z    $vf15, $vf0, $vf16y
	"
 "vaddz.x    $vf16, $vf0, $vf20z
	"
 "vnop
	"
 "vaddx.y    $vf14, $vf0, $vf21x
	"
 "vaddy.x    $vf15, $vf0, $vf20y
	"
 "vaddz.y    $vf16, $vf0, $vf21z
	"
 "vnop
	"
 "vmulax.xyzw ACC, $vf11, $vf14x
	"
 "vmadday.xyzw ACC, $vf12, $vf14y
	"
 "vmaddaz.xyzw ACC, $vf13, $vf14z
	"
 "vmaddx.xyzw $vf17, $vf0, $vf0x
	"
 "vmulax.xyzw ACC, $vf11, $vf15x
	"
 "vmadday.xyzw ACC, $vf12, $vf15y
	"
 "vmaddaz.xyzw ACC, $vf13, $vf15z
	"
 "vmaddx.xyzw $vf18, $vf0, $vf0x
	"
 "vmulax.xyzw ACC, $vf11, $vf16x
	"
 "vmadday.xyzw ACC, $vf12, $vf16y
	"
 "vmaddaz.xyzw ACC, $vf13, $vf16z
	"
 "vmaddx.xyzw $vf19, $vf0, $vf0x
	"
 "sqc2       $vf17, 0x0(%0)
	"
 "sqc2       $vf18, 0x10(%0)
	"
 "sqc2       $vf19, 0x20(%0)
	"
 "sqc2       $vf0, 0x30(%0)
	"
 : : "r"(out), "r"(a), "r"(b) : "memory");
}
