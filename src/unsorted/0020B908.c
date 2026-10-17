/* UNIT 0x0020B908 0x0020B930 */
#include "scalar_api.h"
void func_0020B908(const void *a, const void *b, float scale)
{
    int temp __attribute__((mode(TI)));
    union { float f; int i; } bits;
    __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)" : : "r"(a), "r"(b));
    bits.f = scale;
    __asm__ volatile("qmtc2 %0, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %1" : : "r"(bits.i), "m"(temp) : "memory");
}
