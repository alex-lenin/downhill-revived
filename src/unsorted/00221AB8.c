/* UNIT 0x00221AB8 0x00221B48 */
/* Hand-written VU0 assembly (libvu0 style): transposes the 3x3 rotation part of rot, applies it
   to rows 0-2 of in (xyz) and copies row 3, writing the result to out. */
void func_00221AB8(void *out, const void *in, const void *rot) {
 __asm__ volatile("daddu      $t2, $a0, $zero
	daddu      $t1, $a1, $zero
	daddu      $v0, $a2, $zero
	lq         $v1, 0x0($v0)
	lq         $a0, 0x10($v0)
	lq         $a1, 0x20($v0)
	pextlw     $a2, $a0, $v1
	lqc2       $vf10, 0x0($t1)
	pcpyld     $a3, $a1, $a2
	pextlw     $t0, $v1, $a1
	qmtc2.ni   $a3, $vf14
	lqc2       $vf11, 0x10($t1)
	pcpyud     $t0, $a2, $t0
	pextuw     $a3, $a1, $v1
	qmtc2.ni   $t0, $vf15
	lqc2       $vf12, 0x20($t1)
	pextuw     $t0, $a0, $a0
	lqc2       $vf13, 0x30($t1)
	pextlw     $t0, $t0, $a3
	qmtc2.ni   $t0, $vf16
	nop
	vmulax.xyzw ACC, $vf14, $vf10x
	vmadday.xyzw ACC, $vf15, $vf10y
	vmaddz.xyz $vf10, $vf16, $vf10z
	vmulax.xyzw ACC, $vf14, $vf11x
	vmadday.xyzw ACC, $vf15, $vf11y
	vmaddz.xyz $vf11, $vf16, $vf11z
	vmulax.xyzw ACC, $vf14, $vf12x
	vmadday.xyzw ACC, $vf15, $vf12y
	vmaddz.xyz $vf12, $vf16, $vf12z
	sqc2       $vf10, 0x0($t2)
	sqc2       $vf11, 0x10($t2)
	sqc2       $vf12, 0x20($t2)
	sqc2       $vf13, 0x30($t2)" : : : "memory");
}
