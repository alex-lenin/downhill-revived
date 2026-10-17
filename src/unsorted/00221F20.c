/* UNIT 0x00221F20 0x00221F50 */
#include "vector.h"
/* Sony libvu0 identity-matrix routine; its argument register is scratch. */
void func_00221F20(Matrix4 *out) {
    __asm__ volatile(
        "daddu $7, %0, $0\n\t"
        "qmfc2.i $2, $vf0\n\t"
        "vmr32.xyzw $vf11, $vf0\n\t"
        "qmfc2.i $3, $vf11\n\t"
        "prot3w %0, $3\n\t"
        "prot3w $5, %0\n\t"
        "sq $2, 0x30($7)\n\t"
        "sq $3, 0x20($7)\n\t"
        "sq %0, 0x10($7)\n\t"
        "sq $5, 0x00($7)"
        : "+r"(out) : : "$2", "$3", "$5", "$7", "memory");
}
