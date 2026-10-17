/* UNIT 0x00242D08 0x00242D50 */
void func_00242D08(float *plane, float *a, float *b, float *out) {
    __asm__ volatile (
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "lqc2 $vf3, 0(%2)\n\t"
        "vaddw.xyz $vf6, $vf0, $vf0w\n\t"
        "vmul.xyz $vf10, $vf1, $vf2\n\t"
        "vmul.xyz $vf11, $vf1, $vf3\n\t"
        "vmulax.x ACC, $vf1, $vf2x\n\t"
        "vmulay.y ACC, $vf1, $vf3y\n\t"
        "vmadday.x ACC, $vf6, $vf10y\n\t"
        "vmaddax.y ACC, $vf6, $vf11x\n\t"
        "vmaddaz.x ACC, $vf6, $vf10z\n\t"
        "vmaddaz.y ACC, $vf6, $vf11z\n\t"
        "vmsubw.x $vf12, $vf6, $vf1w\n\t"
        "vmsubw.y $vf12, $vf6, $vf1w\n\t"
        "sqc2 $vf12, 0(%3)"
        : : "r"(plane), "r"(a), "r"(b), "r"(out) : "memory");
}
