/* UNIT 0x001B16C8 0x001B1748 */
typedef struct Vec {
    float x;
    float y;
    float z;
    float w;
} Vec __attribute__((aligned(16)));
void func_001B16C8(Vec *out, Vec *from, Vec *to, Vec *up)
{
    float len;
    float inv;
    float x;
    float y;
    float z;
    int bits;

    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(out[0]) : "r"(to), "r"(from));
    x = out[0].x;
    y = out[0].y;
    z = out[0].z;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits)
                      : "f"(x), "f"(y), "f"(z));
    out[0].x = x * inv;
    out[0].y = y * inv;
    out[0].z = z * inv;
    out[2] = *up;
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0"
                      : "=m"(out[1]) : "r"(&out[2]), "r"(&out[0]) : "memory");
}
