/* UNIT 0x00222800 0x002228F0 */
typedef unsigned int u128 __attribute__((mode(TI)));

/* out = a x b (VU0 outer product) */
static __inline__ void CrossProduct(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\t"
                      "lqc2 $vf12, 0(%2)\n\t"
                      "vopmula.xyz ACC, $vf11, $vf12\n\t"
                      "vopmsub.xyz $vf11, $vf12, $vf11\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*(u128 *)out) : "r"(a), "r"(b));
}

static __inline__ void Normalize(float *v)
{
    float x, y, z;
    float len;
    float inv;

    x = v[0];
    y = v[1];
    z = v[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v[0] = x * inv;
    v[1] = y * inv;
    v[2] = z * inv;
}

/* re-orthonormalize the rotation rows of m */
void func_00222800(float m[4][4])
{
    CrossProduct(m[0], m[1], m[2]);
    Normalize(m[0]);
    CrossProduct(m[1], m[2], m[0]);
    Normalize(m[1]);
    CrossProduct(m[2], m[0], m[1]);
    Normalize(m[2]);
}
