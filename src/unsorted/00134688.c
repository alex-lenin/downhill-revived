/* UNIT 0x00134688 0x001349E0 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00231E00_2_func_00231E00 func_00231E00
#else
#define CallScalarBridge00231E00_2_func_00231E00 ScalarBridge00231E00_2
#endif
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Rider {
    char pad0[0x38];
    Vec *x38;
    char pad3C[0x14];
    Vec x50;
    char pad60[0xF8];
    char *x158;
    char pad15C[0x353];
    unsigned char x4AF;
} Rider;

extern void func_00134640(Rider *, Vec *, float *, void **, Vec *);
extern void func_00221888(void *, Vec *, Vec *);

#if defined(__mips__)
extern void func_00231E00(Rider *, Rider *, Vec *, Vec *, float, float, float);
#else

#endif

extern void func_00136030(Rider *, int);
extern void func_00231A48(Rider *, Vec *);
extern void func_001B0AC0(char *, float);

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)
	lqc2 $vf12, 0x0(%2)
	vsub.xyz $vf11, $vf11, $vf12
	sqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}
#define VEC_SUB(out, a, b) VecSub(&(out), a, b)

static inline void VecMul(Vec *out, Vec *a, Vec *b)
{
    Vec *v = a;
    __asm__ volatile ("lqc2 $vf1, 0x0(%0)" : : "r"(v));
    v = b;
    __asm__ volatile ("lqc2 $vf2, 0x0(%0)
	vmul.xyz $vf1, $vf1, $vf2" : : "r"(v));
    v = out;
    __asm__ volatile ("sqc2 $vf1, 0x0(%0)" : : "r"(v));
}
#define VEC_MUL(out, a, b) VecMul(out, a, b)

static inline void VecScale(Vec *out, Vec *v, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(scale) : "$2");
}

static inline float VecDot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline float VecLengthSq(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, r;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(r) : "f"(x), "f"(y), "f"(z));
    return r;
}

static inline float LengthSq(float x, float y, float z)
{
    float r;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(r) : "f"(x), "f"(y), "f"(z));
    return r;
}

static inline float InvSqrt(float x)
{
    float r;
    __asm__ volatile ("rsqrt.s %0, %1, %2" : "=&f"(r) : "f"(1.0f), "f"(x));
    return r;
}

static inline float VecNormalize(Vec *out, float x, float y, float z)
{
    float length, inv;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=f"(length) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return length;
}

static inline float minf(float a, float b) { float r; __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
static inline float maxf(float a, float b) { float r; __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }

static inline float Reach(void *shape, Vec *from, Vec *to, Vec *weight)
{
    Vec tmp;

    VEC_SUB(tmp, from, to);
    func_00221888(shape, &tmp, &tmp);
    VEC_MUL(&tmp, &tmp, weight);
    return InvSqrt(LengthSq(tmp.x, tmp.y, tmp.z));
}

void func_00134688(Rider *a, Rider *b)
{
    Vec posA, posB, delta, weightA, weightB;
    float radiusA;
    void *shapeA;
    float radiusB;
    void *shapeB;
    float reach, push, length, depth, side;

    func_00134640(a, &posA, &radiusA, &shapeA, &weightA);
    func_00134640(b, &posB, &radiusB, &shapeB, &weightB);
    reach = radiusA + radiusB;
    VEC_SUB(delta, &posA, &posB);
    if (VecLengthSq(&delta) > reach * reach)
        return;
    push = Reach(shapeA, &posB, &posA, &weightA);
    push += Reach(shapeB, &posA, &posB, &weightB);
    if (push <= 1.0f)
        return;
    length = VecNormalize(&delta, delta.x, delta.y, delta.z);
    CallScalarBridge00231E00_2_func_00231E00(a, b, &a->x50, &delta, 0.0f, 0.0f, 0.0f);
    if (a->x4AF == 3)
        func_00136030(a, 1);
    if (b->x4AF == 3)
        func_00136030(b, 1);
    VecScale(&delta, &delta, length * (push - 1.0f) * 0.1f);
    func_00231A48(a, &delta);
    depth = length / reach;
    depth = minf(1.0f, depth);
    depth = maxf(0.0f, depth);
    depth = 1.0f - depth;
    side = VecDot(&delta, a->x38) < 0.0f ? -1.0f : 1.0f;
    depth *= side;
    func_001B0AC0(a->x158 + 0x4E54, depth);
    func_001B0AC0(b->x158 + 0x4E54, -depth);
    VecScale(&delta, &delta, -1.0f);
    func_00231A48(b, &delta);
}
