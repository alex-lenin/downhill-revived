/* UNIT 0x00236260 0x002362E8 */
#include "rider_geometry.h"
typedef Vector4 Vec;

/* in-place normalize: out = normalize(v), returns |v| */
static inline float norm(Vec *out, Vec *v)
{
    float len, inv, x = v->x, y = v->y, z = v->z;
    /* SDK vector length and reciprocal sequence. */
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %0,%4,%4\n\tsqrt.s %0,%0\n\tlui $2,0x3f80\n\tmtc1 $2,%1\n\tnop\n\tdiv.s %1,%1,%0"
                      : "=&f"(len), "=&f"(inv) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

/* Returns |b.xyz - a.xyz|; out.xyz = normalize(b.xyz - a.xyz) * a.w - c.xyz; out.w = b.w */
float func_00236260(Vec *a, Vec *b, Vec *c, Vec *out)
{
    Vec d __attribute__((aligned(16)));
    float len;

    __asm__ volatile ("lqc2 $vf11,0x0(%1)\n\tlqc2 $vf12,0x0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(d) : "r"(b), "r"(a));
    len = norm(&d, &d);
    __asm__ volatile ("lqc2 $vf11,%1\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                      : "=m"(d) : "m"(d), "f"(a->w) : "$2");
    __asm__ volatile ("lqc2 $vf11,%1\n\tlqc2 $vf12,0x0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "m"(d), "r"(c));
    return len;
}
