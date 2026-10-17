/* UNIT 0x001349E0 0x00134A20 */
typedef float Vec4[4] __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x120];
    Vec4 x120;
} Obj;

void func_001349E0(Obj *obj, float *a, float *b, float value)
{
    float *out = obj->x120;

    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(obj->x120) : "r"(a), "r"(b) : "memory");
    /* scale by 0.5, written like libvu0's sceVu0ScaleVector (mfc1 inside the asm) */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(obj->x120) : "r"(out), "f"(0.5f) : "$2", "memory");
    obj->x120[2] -= value;
}
