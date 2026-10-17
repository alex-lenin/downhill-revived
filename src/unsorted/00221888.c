/* UNIT 0x00221888 0x002218D0 */
#include "vector.h"
void func_00221888(const Matrix4 *matrix,const Vector4 *vector,Vector4 *result) {
 __asm__ volatile(
 "lqc2 $vf11, 0x0(%1)\n\t"
 "lqc2 $vf12, 0x0(%0)\n\t"
 "lqc2 $vf13, 0x10(%0)\n\t"
 "lqc2 $vf14, 0x20(%0)\n\t"
 "vaddw.xyz $vf15, $vf0, $vf0w\n\t"
 "vmul.xyz $vf12, $vf12, $vf11\n\t"
 "vmul.xyz $vf13, $vf13, $vf11\n\t"
 "vmul.xyz $vf14, $vf14, $vf11\n\t"
 "vadday.x ACC, $vf12, $vf12y\n\t"
 "vmaddz.x $vf11, $vf15, $vf12z\n\t"
 "vaddaz.y ACC, $vf13, $vf13z\n\t"
 "vmaddx.y $vf11, $vf15, $vf13x\n\t"
 "vaddax.z ACC, $vf14, $vf14x\n\t"
 "vmaddy.z $vf11, $vf15, $vf14y\n\t"
 "sqc2 $vf11, 0x0(%2)" : : "r"(matrix),"r"(vector),"r"(result) : "memory");
}
