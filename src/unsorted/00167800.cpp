/* UNIT 0x00167800 0x001678E0 */
/* Interpolates between two 48-byte keys; VU0 parts use inline asm.
   x0 is lerped, x10 is scaled by each key's x2C and lerped, x28 is lerped on the FPU. */
typedef struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

typedef struct Key {
    Vec4 x0;
    Vec4 x10;
    char pad20[0x8];
    float x28;
    float x2C;
} Key;

void func_00167800(char *base, Key *out, Key *prev, Key *next, float t) {
    Vec4 w = { t, prev->x2C, next->x2C, 1.0f };
    float diff;

    if (t == 1.0f) {
        out->x0 = next->x0;
        out->x28 = next->x28;
        __asm__ volatile (
            "lqc2 $vf11, 0x0(%1)\n\t"
            "mfc1 $2, %2\n\t"
            "qmtc2.ni $2, $vf12\n\t"
            "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
            "sqc2 $vf11, 0x10(%0)"
            : : "r"(out), "r"(&next->x10), "f"(next->x2C) : "$2", "memory");
    } else {
        __asm__ volatile ("lqc2 $vf11, 0x0(%0)" : : "r"(&w));
        __asm__ volatile ("lqc2 $vf12, 0x0(%0)" : : "r"(&prev->x0));
        __asm__ volatile ("lqc2 $vf13, 0x0(%0)" : : "r"(&next->x0));
        __asm__ volatile ("vaddax.xyzw ACC, $vf12, $vf0x");
        __asm__ volatile ("lqc2 $vf14, 0x0(%0)" : : "r"(&prev->x10));
        __asm__ volatile ("vmaddax.xyzw ACC, $vf13, $vf11x");
        __asm__ volatile ("lqc2 $vf15, 0x0(%0)" : : "r"(&next->x10));
        __asm__ volatile ("vmsubx.xyzw $vf12, $vf12, $vf11x");
        __asm__ volatile ("vmuly.xyz $vf14, $vf14, $vf11y");
        __asm__ volatile ("vmulz.xyz $vf15, $vf15, $vf11z");
        diff = next->x28 - prev->x28;
        __asm__ volatile ("vaddax.xyzw ACC, $vf14, $vf0x");
        __asm__ volatile ("vmaddax.xyzw ACC, $vf15, $vf11x");
        diff *= t;
        __asm__ volatile ("vmsubx.xyzw $vf13, $vf14, $vf11x");
        __asm__ volatile ("sqc2 $vf12, 0x0(%0)" : : "r"(&out->x0) : "memory");
        out->x28 = prev->x28 + diff;
        __asm__ volatile ("sqc2 $vf13, 0x0(%0)" : : "r"(&out->x10) : "memory");
    }
}
