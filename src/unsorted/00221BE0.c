/* UNIT 0x00221BE0 0x00221C20 */
/* out[i] = b[i] - a[i] for three rows (xyz) */
void func_00221BE0(void *out, const void *a, const void *b)
{
    __asm__ volatile (
        "lqc2 $vf14, 0x0(%1)\n\t"
        "lqc2 $vf15, 0x10(%1)\n\t"
        "lqc2 $vf16, 0x20(%1)\n\t"
        "lqc2 $vf10, 0x0(%2)\n\t"
        "lqc2 $vf11, 0x10(%2)\n\t"
        "lqc2 $vf12, 0x20(%2)\n\t"
        "vsub.xyz $vf14, $vf10, $vf14\n\t"
        "vsub.xyz $vf15, $vf11, $vf15\n\t"
        "vsub.xyz $vf16, $vf12, $vf16\n\t"
        "sqc2 $vf14, 0x0(%0)\n\t"
        "sqc2 $vf15, 0x10(%0)\n\t"
        "sqc2 $vf16, 0x20(%0)"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}
