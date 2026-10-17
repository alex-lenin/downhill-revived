/* UNIT 0x00221B48 0x00221B80 */
void func_00221B48(void *out, const void *a, const void *b)
{
    __asm__ volatile (
        "lqc2 $vf14, 0(%1)\n\t"
        "lqc2 $vf15, 16(%1)\n\t"
        "lqc2 $vf16, 32(%1)\n\t"
        "lqc2 $vf10, 0(%2)\n\t"
        "lqc2 $vf11, 16(%2)\n\t"
        "lqc2 $vf12, 32(%2)\n\t"
        "vadd.xyz $vf14, $vf10, $vf14\n\t"
        "vadd.xyz $vf15, $vf11, $vf15\n\t"
        "vadd.xyz $vf16, $vf12, $vf16\n\t"
        "sqc2 $vf14, 0(%0)\n\t"
        "sqc2 $vf15, 16(%0)\n\t"
        "sqc2 $vf16, 32(%0)"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}
