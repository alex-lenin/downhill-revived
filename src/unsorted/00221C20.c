/* UNIT 0x00221C20 0x00221C58 */
void func_00221C20(void *result,void *matrix,float scale) {
 union { float f; unsigned int u; } bits;
 __asm__ volatile(
 "lqc2 $vf10, 0x0(%0)\n\t"
 "lqc2 $vf11, 0x10(%0)\n\t"
 "lqc2 $vf12, 0x20(%0)" : : "r"(matrix) : "memory");
 bits.f=scale;
 __asm__ volatile(
 "qmtc2.ni %1, $vf13\n\t"
 "vmulx.xyz $vf10, $vf10, $vf13x\n\t"
 "vmulx.xyz $vf11, $vf11, $vf13x\n\t"
 "vmulx.xyz $vf12, $vf12, $vf13x\n\t"
 "sqc2 $vf10, 0x0(%0)\n\t"
 "sqc2 $vf11, 0x10(%0)\n\t"
 "sqc2 $vf12, 0x20(%0)" : : "r"(result),"r"(bits.u) : "memory");
}
