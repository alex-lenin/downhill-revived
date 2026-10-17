/* UNIT 0x002092B8 0x00209300 */
#include "spatial.h"
void func_002092B8(SpatialMotion *obj, Vector4 *out) {
 float scale;
 __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%2)" : : "r"((char *)obj + 0x10), "r"((char *)obj + 0x90), "r"(out) : "memory");
 scale = 1.0f / obj->xB0;
 __asm__ volatile("lqc2 $vf11, 0(%0)" : : "r"(out) : "memory");
 __asm__ volatile("qmtc2 %0, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0(%1)" : : "r"(*(int *)&scale), "r"(out) : "memory");
}
