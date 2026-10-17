/* UNIT 0x00223210 0x00223248 */
void func_00223210(void *out, const void *a, const void *b)
{
    __asm__ volatile (
        "lqc2 $vf1, 0(%2)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "vmul.xyzw $vf3, $vf1, $vf2\n\t"
        "vopmula.xyz ACC, $vf1, $vf2\n\t"
        "vmaddaw.xyz ACC, $vf1, $vf2w\n\t"
        "vmaddaw.xyz ACC, $vf2, $vf1w\n\t"
        "vopmsub.xyz $vf1, $vf2, $vf1\n\t"
        "vsubaz.w ACC, $vf3, $vf3z\n\t"
        "vmsubay.w ACC, $vf0, $vf3y\n\t"
        "vmsubx.w $vf1, $vf0, $vf3x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}
