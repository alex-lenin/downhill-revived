/* UNIT 0x00222C38 0x00222D48 .sdata=0x29DAF8:0x29DAFC */
int D_0029DAF8 = 0x0379EA54;

extern float D_00279AB0[];
extern float D_00279AC0[];

/* Random unit vector: v[0], v[1] from an LCG as floats in [2,4). */
void func_00222C38(float *v, float scale)
{
    unsigned int *bits = (unsigned int *)v;

    D_0029DAF8 = D_0029DAF8 * 0x41C64E6D + 12345;
    ((unsigned int *)v)[0] = (D_0029DAF8 & 0x7FFFFF) | 0x40000000;
    D_0029DAF8 = D_0029DAF8 * 0x41C64E6D + 12345;
    ((unsigned int *)v)[1] = (D_0029DAF8 & 0x7FFFFF) | 0x40000000;
    v[1] = (v[1] - D_00279AB0[0]) * scale + 1.0f;

    __asm__ volatile (
        "lqc2 $vf1, 0x0(%0)\n\t"
        "vaddaw.y ACC, $vf0, $vf0w\n\t"
        "vmsub.y $vf2, $vf1, $vf1\n\t"
        "vsqrt Q, $vf2y"
        : : "r"(v));

    if (*bits < 0x40400000) {
        __asm__ volatile (
            "lqc2 $vf3, 0x0(%0)\n\t"
            "vaddw.z $vf1, $vf0, $vf0w"
            : : "r"(D_00279AB0));
    } else {
        __asm__ volatile (
            "lqc2 $vf3, 0x0(%0)\n\t"
            "vsubw.z $vf1, $vf0, $vf0w"
            : : "r"(D_00279AC0));
    }

    __asm__ volatile (
        "vmul.x $vf2, $vf1, $vf1\n\t"
        "vaddaw.x ACC, $vf0, $vf3w\n\t"
        "vmaddaz.x ACC, $vf1, $vf3z\n\t"
        "vmul.x $vf1, $vf2, $vf1\n\t"
        "vmadday.x ACC, $vf2, $vf3y\n\t"
        "vmove.w $vf1, $vf0\n\t"
        "vmaddx.x $vf1, $vf1, $vf3x\n\t"
        "vaddaw.x ACC, $vf0, $vf0w\n\t"
        "vmsub.x $vf2, $vf1, $vf1\n\t"
        "vmulq.xz $vf1, $vf1, Q\n\t"
        "vsqrt Q, $vf2x\n\t"
        "vwaitq\n\t"
        "vmulq.z $vf1, $vf1, Q\n\t"
        "sqc2 $vf1, 0x0(%0)"
        : : "r"(v));
}
