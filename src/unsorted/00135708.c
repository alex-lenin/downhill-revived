/* UNIT 0x00135708 0x001357D8 .sdata=0x29C398:0x29C3A0 */
extern void *func_00134F58(char *obj, int index);
extern void func_00134D70(void *src, void *dst, char *obj);

float D_0029C398 = -1.4f;

void func_00135708(char *obj, float *out)
{
    float a[4];
    float b[4];
    struct { float v[4]; } diff;
    float *d;

    func_00134D70(func_00134F58(obj, 0), a, obj);
    func_00134D70(func_00134F58(obj, 1), b, obj);
    d = diff.v;
    /* diff = a - b */
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %2"
                      : : "r"(a), "r"(b), "m"(diff) : "memory");
    /* out = b + diff * 0.7 */
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, 0x0(%3)"
                      : : "r"(b), "r"(d), "f"(0.7f), "r"(out) : "$2", "memory");
    /* out += obj->x1E0 * -1.4 */
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(out), "r"(obj + 0x1E0), "f"(D_0029C398) : "$2", "memory");
}
