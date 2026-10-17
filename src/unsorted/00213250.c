/* UNIT 0x00213250 0x00213658 */
#include "spatial_primitives.h"
#if defined(__mips__)
#define CallShared00213098_0 func_00213098
#else
#define CallShared00213098_0 Forward00213098_3
#endif
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Rec { float x, y, z, t; } Rec;
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VSUB(dst, a, b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a), "r"(b))
#define VMADD(dst, a, b, f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a), "r"(b), "f"(f) : "$2")
extern int func_00213098(Vec *, Vec *, Vec *, float, float, Vec *, float, Rec *);
static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}
static inline float Len2(Vec *v) {
    float r;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(r) : "f"(v->x), "f"(v->y), "f"(v->z));
    return r;
}
static inline float Min(float a, float b) {
    float r;
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}
static inline float Abs(float x) {
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}
int func_00213250(Vec *origin, Vec *dir, float *bmin, float *bmax, Vec *segmentStart, Vec *segmentEnd, Rec *out, float radius, float tmax) {
    Vec offset, edge, hit;
    float offsetEdge, edgeLength2, edgeDirection, offsetDirection, offsetLength2, directionLength2;
    float radius2, b2, c1, c2, a, c, b, disc, sq, nb, aa2, t1, t2, t, s, aa;
    float *u, *v;
    int overlap;
    int axis;
    axis = 0;
    v = (float *)segmentEnd;
    u = (float *)segmentStart;
    while (axis < 3) {
        float x = *u;
        float y = *v;
        float hi = *bmax;
        if (x < y) {
            if (hi < x || y < *bmin) { overlap = 0; goto done; }
        } else {
            if (hi < y || x < *bmin) { overlap = 0; goto done; }
        }
        axis++;
        bmin++;
        bmax++;
        v++;
        u++;
    }
    overlap = 1;
done:
    if ((overlap & 0xFF) == 0) return 0;
    VSUB(offset, origin, segmentStart);
    VSUB(edge, segmentEnd, segmentStart);
    offsetEdge = Dot(&offset, &edge);
    edgeLength2 = Len2(&edge);
    if (CallShared00213098_0(segmentStart, &edge, &offset, edgeLength2, offsetEdge, origin, radius, out) == 0) {
        edgeDirection = Dot(&edge, dir);
        offsetDirection = Dot(&offset, dir);
        offsetLength2 = Len2(&offset);
        directionLength2 = Len2(dir);
        b2 = offsetEdge * edgeDirection - offsetDirection * edgeLength2;
        c1 = offsetEdge * offsetEdge + (radius * radius) * edgeLength2;
        a = edgeDirection * edgeDirection - edgeLength2 * directionLength2;
        c2 = offsetLength2 * edgeLength2;
        b = b2 + b2;
        t = c1 - c2;
        aa = Abs(a);
        if (0.0001f < aa) {
            float nb, aa2, t1, t2;
            disc = b * b - a * (t * 4.0f);
            if (disc <= 0.0f) return 0;
            __asm__ volatile ("sqrt.s %0,%1" : "=&f"(t) : "f"(disc));
            nb = -b;
            aa2 = a + a;
            t2 = (nb + t) / aa2;
            t1 = (nb - t) / aa2;
            t = t2;
            t = Min(t, t1);
            if (t < 0.0f) return 0;
            if (tmax < t) return 0;
            VMADD(hit, origin, dir, t);
            VSUB(hit, &hit, segmentStart);
            s = Dot(&hit, &edge) / edgeLength2;
            if (s >= 0.0f && s <= 1.0f) {
                VMADD(*out, segmentStart, &edge, s);
                out->t = t;
                return 1;
            }
        }
        {
        float nb, aa2, t1, t2;
        t = offsetLength2 - radius * radius;
        b = offsetDirection + offsetDirection;
        if (Abs(directionLength2) < 0.0001f) return 0;
        disc = b * b - directionLength2 * (t * 4.0f);
        if (disc <= 0.0f) return 0;
        __asm__ volatile ("sqrt.s %0,%1" : "=&f"(t) : "f"(disc));
        nb = -b;
        aa2 = directionLength2 + directionLength2;
        t2 = (nb + t) / aa2;
        t1 = (nb - t) / aa2;
        t = t2;
        t = Min(t, t1);
        if (t < 0.0f) return 0;
        if (tmax < t) return 0;
        COPY_VECTOR(*out, segmentStart);
        out->t = t;
        return 1;
        }
    }
    return 1;
}
