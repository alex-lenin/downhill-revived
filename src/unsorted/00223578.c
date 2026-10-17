/* UNIT 0x00223578 0x002235B0 */
#include "vector_blend.h"
#if defined(__mips__)
void func_00223578(float *out, float *in, float scale)
#else
void SharedCall00223578(float * out, float * in, float scale)
#endif
{

    __asm__ volatile ("lqc2 $vf11, 0(%0)" : : "r"(in) : "memory");
    __asm__ volatile ("qmtc2 %0, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0(%1)" : : "r"(scale), "r"(out) : "memory");
    out[3] = (1.0f - scale) + scale * in[3];
}
