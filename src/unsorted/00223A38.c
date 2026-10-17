/* UNIT 0x00223A38 0x00223A68 */
#include "matrix_packets.h"
/* Sony libvu0 matrix copy routine. */
void func_00223A38(Matrix4 *out, const Matrix4 *in) {
#if defined(__mips__)
    __asm__ volatile(
        "daddu      $a3, $a0, $zero\n\t"
        "daddu      $a2, $a1, $zero\n\t"
        "lq         $v0, 0x0($a2)\n\t"
        "lq         $v1, 0x10($a2)\n\t"
        "lq         $a0, 0x20($a2)\n\t"
        "lq         $a1, 0x30($a2)\n\t"
        "sq         $v0, 0x0($a3)\n\t"
        "sq         $v1, 0x10($a3)\n\t"
        "sq         $a0, 0x20($a3)\n\t"
        "sq         $a1, 0x30($a3)"
        : : : "memory");
#else
    __builtin_memmove(out, in, sizeof(Matrix4));
#endif
}
