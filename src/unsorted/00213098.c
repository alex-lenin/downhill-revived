/* UNIT 0x00213098 0x00213250 */
#include "spatial_primitives.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Hit {
    Vec pos;
    Vec normal;
    float depth;
} Hit;

extern float D_0029DA6C[];

static inline float VecDot(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, result;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3"
                      : "=&f"(result) : "f"(x), "f"(y), "f"(z));
    return result;
}

static inline float VecNormalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, length, inv;
    int bits;

    /* length and 1/length (libvu0 style) */
    __asm__ volatile ("mula.s %3, %3
	madda.s %4, %4
	madd.s %1, %5, %5
	sqrt.s %1, %1
	"
                      "lui %2, 0x3f80
	mtc1 %2, %0
	nop
	div.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(length), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return length;
}

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

#if defined(__mips__)
int func_00213098(Vec *start, Vec *edge, Vec *delta, Vec *center, Hit *hit,
                  float t0, float t1, float radius)
#else
int SharedCall00213098(struct Vector4 * start_view, struct Vector4 * edge_view, struct Vector4 * delta_view, struct Vector4 * center_view, struct Shared00213098_Hit * hit_view, float t0, float t1, float radius)
#endif
{
#if !defined(__mips__)
    Vec * start = (Vec *)start_view;
    Vec * edge = (Vec *)edge_view;
    Vec * delta = (Vec *)delta_view;
    Vec * center = (Vec *)center_view;
    Hit * hit = (Hit *)hit_view;
#endif

    Vec temp;
    float length;
    float sq;

    if (t0 <= t1)
        return 0;
    sq = radius * radius;
    if (t1 < 0.0f) {
        if (sq < VecDot(delta))
            return 0;
        length = VecNormalize(&hit->normal, delta);
        hit->depth = (radius - length) + 0.01f;
        VecAddScaled(&hit->pos, start, &hit->normal, 0.01f);
        hit->pos.w = D_0029DA6C[0];
        return 1;
    } else {
        VecAddScaled(&temp, start, edge, t1 / t0);
        VecSub(&hit->normal, center, &temp);
        if (sq < VecDot(&hit->normal))
            return 0;
        length = VecNormalize(&hit->normal, &hit->normal);
        hit->depth = (radius - length) + 0.01f;
        VecAddScaled(&hit->pos, &temp, &hit->normal, 0.01f);
        hit->pos.w = D_0029DA6C[0];
        return 1;
    }
}
