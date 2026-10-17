/* UNIT 0x00221D58 0x00221DA8 */
/* Sony libvu0 matrix transpose assembly. */
void func_00221D58(void *out, const void *in) {
 __asm__ volatile("daddu      $t0, $a0, $zero
	daddu      $a3, $a1, $zero
	lq         $v0, 0x0($a3)
	lq         $v1, 0x10($a3)
	lq         $a0, 0x20($a3)
	pextlw     $a2, $v1, $v0
	lq         $a1, 0x30($a3)
	pextuw     $v0, $v1, $v0
	pextlw     $v1, $a1, $a0
	pextuw     $a0, $a1, $a0
	pcpyld     $a1, $v1, $a2
	pcpyud     $a2, $a2, $v1
	sq         $a1, 0x0($t0)
	pcpyld     $v1, $a0, $v0
	sq         $a2, 0x10($t0)
	pcpyud     $a0, $v0, $a0
	sq         $v1, 0x20($t0)
	sq         $a0, 0x30($t0)" : : : "memory");
}
