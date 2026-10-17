/* UNIT 0x002227A8 0x002227F8 */
void func_002227A8(const float *a, const float *b, float *out, float *projection) {
    float dot;
    int scalar;
    __asm__ volatile (
        "mula.s %1, %2\n\t"
        "madda.s %3, %4\n\t"
        "madd.s %0, %5, %6"
        : "=f"(dot)
        : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    /* SDK-style vector scale block includes the scalar FPU transfer. */
    __asm__ volatile (
        "lqc2 $vf11, 0(%1)\n\t"
        "mfc1 %0, %2\n\t"
        "qmtc2 %0, $vf12\n\t"
        "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
        "sqc2 $vf11, 0(%3)"
        : "=&r"(scalar) : "r"(b), "f"(dot), "r"(projection) : "memory");
    __asm__ volatile (
        "lqc2 $vf11, 0(%0)\n\t"
        "lqc2 $vf12, 0(%1)\n\t"
        "vsub.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, 0(%2)"
        : : "r"(a), "r"(projection), "r"(out) : "memory");
}
