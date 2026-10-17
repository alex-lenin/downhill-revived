/* UNIT 0x00231A78 0x00231AF8 */
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec4;

typedef struct Obj {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x14];
    Vec4 *x34;
    Vec4 *x38;
    char pad3C[0x14];
    Vec4 x50;
} Obj;

extern void func_00232950(Obj *);

void func_00231A78(Obj *obj, Vec4 *matrix)
{
    Vec4 tmp;

    /* 64-byte matrix copy (as sceVu0CopyMatrix) */
    __asm__ volatile ("lq $8, 0x0(%1)\n\t"
                      "lq $9, 0x10(%1)\n\t"
                      "lq $10, 0x20(%1)\n\t"
                      "lq $11, 0x30(%1)\n\t"
                      "sq $8, 0x0(%0)\n\t"
                      "sq $9, 0x10(%0)\n\t"
                      "sq $10, 0x20(%0)\n\t"
                      "sq $11, 0x30(%0)"
                      : : "r"(obj->x38), "r"(matrix) : "$8", "$9", "$10", "$11", "memory");
    /* tmp.xyz = x38 (rotation part) applied to x10 */
    __asm__ volatile ("lqc2 $vf11, 0x0(%2)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "lqc2 $vf13, 0x10(%1)\n\t"
                      "lqc2 $vf14, 0x20(%1)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\t"
                      "vmadday.xyz ACC, $vf13, $vf11y\n\t"
                      "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(tmp) : "r"(obj->x38), "r"(&obj->x10) : "memory");
    /* x34.xyz = x50 - tmp */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, %2\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(obj->x34), "r"(&obj->x50), "m"(tmp) : "memory");
    func_00232950(obj);
}
