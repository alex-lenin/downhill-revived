/* UNIT 0x00239858 0x002398C0 */
typedef struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

extern int D_0027EC68[];
extern float D_0075A6A0[];

void func_00239858(unsigned int index, Vec4 *v, Vec4 *out)
{
    Vec4 local = *v;
    float scale;

    if (index < 4 && D_0027EC68[index] < 3) {
        scale = D_0075A6A0[index];
        /* out.xyz += local.xyz * scale */
        __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                          "lqc2 $vf12, %1\n\t"
                          "mfc1 $2, %2\n\t"
                          "qmtc2.ni $2, $vf13\n\t"
                          "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                          "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                          "sqc2 $vf11, 0x0(%0)"
                          : : "r"(out), "m"(local), "f"(scale) : "$2", "memory");
    }
}
