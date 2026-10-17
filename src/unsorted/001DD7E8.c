/* UNIT 0x001DD7E8 0x001DD888 */
void func_001DD7E8(void *obj, float *a, float *out, float *start, float *end, float scale)
{
    float x, y, z, len, inv, dot, res;

    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(out), "r"(end), "r"(start) : "memory");
    x = out[0];
    y = out[1];
    z = out[2];
    __asm__ volatile ("mula.s %2,%2\n\t"
                      "madda.s %3,%3\n\t"
                      "madd.s %1,%4,%4\n\t"
                      "sqrt.s %1,%1\n\t"
                      "lui $2,0x3f80\n\t"
                      "mtc1 $2,%0\n\t"
                      "nop\n\t"
                      "div.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    x *= inv;
    out[0] = x;
    y *= inv;
    out[1] = y;
    z *= inv;
    out[2] = z;
    out[3] = len;
    __asm__ volatile ("mula.s %1,%2\n\t"
                      "madda.s %3,%4\n\t"
                      "madd.s %0,%5,%6"
                      : "=f"(dot) : "f"(a[0]), "f"(x), "f"(a[1]), "f"(y), "f"(a[2]), "f"(z));
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(res) : "f"(1.0f - dot * dot));
    a[3] -= res * scale;
}
