/* UNIT 0x00211930 0x00211980 */
void func_00211930(float *matrix, float *a, float *b, float *c, float *out_a, float *out_b, float *out_c) {
    __asm__ volatile (
        "lqc2 $vf12, 0x0(%0)\n\t"
        "lqc2 $vf13, 0x10(%0)\n\t"
        "lqc2 $vf14, 0x20(%0)\n\t"
        "lqc2 $vf11, 0x0(%1)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, 0x0(%4)\n\t"
        "lqc2 $vf11, 0x0(%2)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, 0x0(%5)\n\t"
        "lqc2 $vf11, 0x0(%3)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, 0x0(%6)"
        : : "r"(matrix), "r"(a), "r"(b), "r"(c), "r"(out_a), "r"(out_b), "r"(out_c) : "memory");
}
