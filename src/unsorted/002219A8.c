/* UNIT 0x002219A8 0x00221A60 */
#include "vector.h"
/* Hand-written VU0 assembly (libvu0 style): transposes the 4x4 matrix rot and multiplies each
   row of in by it, writing the result to out. */
void func_002219A8(Matrix4 *out, const Matrix4 *in, const Matrix4 *rot) {
 __asm__ volatile("daddu      $t1, $a0, $zero
	daddu      $t0, $a1, $zero
	daddu      $v0, $a2, $zero
	lq         $v1, 0x0($v0)
	lq         $a0, 0x10($v0)
	lq         $a1, 0x20($v0)
	pextlw     $a3, $a0, $v1
	lq         $a2, 0x30($v0)
	pextuw     $v1, $a0, $v1
	lqc2       $vf10, 0x0($t0)
	pextlw     $a0, $a2, $a1
	lqc2       $vf11, 0x10($t0)
	pextuw     $a1, $a2, $a1
	lqc2       $vf12, 0x20($t0)
	pcpyld     $a2, $a0, $a3
	lqc2       $vf13, 0x30($t0)
	pcpyud     $a3, $a3, $a0
	qmtc2.ni   $a2, $vf14
	pcpyld     $a0, $a1, $v1
	qmtc2.ni   $a3, $vf15
	pcpyud     $a1, $v1, $a1
	qmtc2.ni   $a0, $vf16
	qmtc2.ni   $a1, $vf17
	vmulax.xyzw ACC, $vf14, $vf10x
	vmadday.xyzw ACC, $vf15, $vf10y
	vmaddaz.xyzw ACC, $vf16, $vf10z
	vmaddw.xyzw $vf10, $vf17, $vf10w
	vmulax.xyzw ACC, $vf14, $vf11x
	vmadday.xyzw ACC, $vf15, $vf11y
	vmaddaz.xyzw ACC, $vf16, $vf11z
	vmaddw.xyzw $vf11, $vf17, $vf11w
	sqc2       $vf10, 0x0($t1)
	vmulax.xyzw ACC, $vf14, $vf12x
	vmadday.xyzw ACC, $vf15, $vf12y
	vmaddaz.xyzw ACC, $vf16, $vf12z
	vmaddw.xyzw $vf12, $vf17, $vf12w
	sqc2       $vf11, 0x10($t1)
	vmulax.xyzw ACC, $vf14, $vf13x
	vmadday.xyzw ACC, $vf15, $vf13y
	vmaddaz.xyzw ACC, $vf16, $vf13z
	vmaddw.xyzw $vf13, $vf17, $vf13w
	sqc2       $vf12, 0x20($t1)
	sqc2       $vf13, 0x30($t1)" : : : "memory");
}
