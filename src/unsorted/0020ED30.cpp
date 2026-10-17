/* UNIT 0x0020ED30 0x0020EE10 */
#include "ui_region.h"
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Light {
    Vec4 x0;
    float x10[3];
    int x1C;
} Light;

extern "C" Light D_004F32E0[];
extern "C" float D_004F33B8[];
extern "C" float D_004F33D0[];

/* dst.xyzw += src * scale */
static inline void AddScaledVector(Vec4 *dst, Vec4 *src, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyzw ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyzw $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(dst), "r"(src), "f"(scale) : "$2", "memory");
}

/* dst.xyz += src * scale */
static inline void AddScaledVectorXYZ(Vec4 *dst, Vec4 *src, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(dst), "r"(src), "f"(scale) : "$2", "memory");
}

/* max.s comes from g++'s >? operator. */
#if defined(__mips__)
extern "C" void func_0020ED30(int index, int r, int g, int b, float weight, float x, float y, float w, float z)
#else
void SharedCall0020ED30(int index, int r, int g, int b, float weight, float x, float y, float w, float z)
#endif
{

    Vec4 pos;
    Vec4 color;

    D_004F33B8[index] = weight >? D_004F33B8[index];
    D_004F33D0[index] += weight;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    pos.w = w;
    color.x = r;
    color.y = g;
    color.z = b;
    AddScaledVector(&D_004F32E0[index].x0, &pos, weight);
    AddScaledVectorXYZ((Vec4 *)D_004F32E0[index].x10, &color, weight);
    D_004F32E0[index].x1C = 1;
}
