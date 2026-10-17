/* UNIT 0x00134E88 0x00134F00 */
typedef struct Obj {
    float x0, x4, x8, xC;
    char pad10[0x70 - 0x10];
    float x70;
    unsigned char x74;
    unsigned char x75;
} Obj;

extern float D_0026DDA0[][4];
extern float D_0026DDE0[][4];

void func_00134E88(Obj *obj, float t)
{
    float *from = D_0026DDA0[obj->x75];
    float *to = D_0026DDE0[obj->x75];
    float saved = obj->xC;
    float lo;
    float clamped;

    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(lo) : "f"(0.0f), "f"(t));
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(clamped) : "f"(1.0f), "f"(lo));
    /* obj.xyz = from + to * clamped */
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, 0x0(%3)"
                      : : "r"(from), "r"(to), "f"(clamped), "r"(obj) : "$2", "memory");
    obj->xC = saved;
    obj->x8 += obj->x70;
}
