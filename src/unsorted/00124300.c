/* UNIT 0x00124300 0x001246D0 */
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

/* collision filter pushed with func_00210940 */
typedef struct QueryFlags {
    unsigned int x0;
    union {
        unsigned int word;
        unsigned short x4;
        struct { unsigned int low : 16, a : 1, b : 1, c : 1, other : 13; } bits;
    } x4;
    int pad8[2];
} QueryFlags;

typedef struct Hit { unsigned short found; char pad2[6]; float x8; char padC[4]; Vec pos, normal; } Hit;

typedef struct Obj {
    char pad0[0x1C];
    int x1C;
    char pad20[0x8];
    int x28;
    char pad2C[0xC4];
    Vec plane;      /* normal xyz, distance w */
    char pad100[0x20];
    Vec x120;
    char pad130[0xB0];
    Vec x1E0;
    Vec x1F0;
} Obj;

extern QueryFlags *func_00210940(QueryFlags *);


extern int func_00238F60(void);
extern void func_00123F60(Obj *, int);
extern void func_001246D0(Obj *);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Init(QueryFlags *f) { f->x0 = -1; f->x4.x4 = 0xffff; f->x4.bits.a = 0; f->x4.bits.b = 0; f->x4.bits.c = 0; }
static inline float Dot(Vec *a, Vec *b)
{
    float out;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(out) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return out;
}
static inline float Min(float a, float b) { float r; __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
/* dst = a - b * s (xyz) */
static inline void SubScaled(Vec *dst, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmsubx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b), "f"(s) : "$2");
}
/* dst = a + b * s (xyz) */
static inline void AddScaled(Vec *dst, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b), "f"(s) : "$2");
}
static inline void CopyXYZ(Vec *dst, Vec *src) { float w = dst->w; COPY_VECTOR(*dst, src); dst->w = w; }
/* removes the velocity component going into the plane */
static inline void ClipVelocity(Obj *obj)
{
    Vec v;
    SubScaled(&v, &obj->x1F0, &obj->plane, Min(0.0f, Dot(&obj->plane, &obj->x1F0)));
    CopyXYZ(&obj->x1F0, &v);
}

/* uses VU0/FPU inline asm */
void func_00124300(Obj *obj)
{
    Vec pos, target;
    Hit hits[8];
    QueryFlags flags, *old;
    Hit *hit;
    float d;
    int n;

    if (obj->x28 == 0) {
        COPY_VECTOR(pos, &obj->x1E0);
        pos.z += 4.0f;
        d = Dot(&pos, &obj->plane) - obj->plane.w - 2.0f;
        if (d < 0.0f) {
            SubScaled(&target, &obj->x1F0, &obj->plane, Min(0.0f, Dot(&obj->plane, &obj->x1F0)));
            CopyXYZ(&obj->x1F0, &target);
            SubScaled(&pos, &pos, &obj->plane, Min(0.0f, d));
            /* volatile accesses keep the w load after the z store (retail order) */
            ((volatile Vec *)&pos)->z -= 4.0f;
            {
                Vec *dst = &obj->x1E0;
                float w = ((volatile Vec *)dst)->w;
                COPY_VECTOR(*dst, &pos);
                dst->w = w;
            }
        }
        func_00123F60(obj, 0);
        return;
    } else {
        COPY_VECTOR(pos, &obj->x120);
        COPY_VECTOR(target, &obj->x1E0);
        pos.z += 4.0f;
        target.z += 4.0f;
        Init(&flags);
        flags.x0 = 0;
        old = func_00210940(&flags);
        n = QuerySpatialHitsCountLast((Vector4 *)(&pos), (Vector4 *)(&target), (SpatialHit *)(hits), 2.0f, 1.0f, 1.0f, 8);
        func_00210940(old);
        hit = func_00211780(hits, n, 0);
        if (hit) {
            Vec *plane;
            obj->x1C += func_00238F60();
            plane = &obj->plane;
            COPY_VECTOR(obj->plane, &hit->normal);
            obj->plane.w = Dot(&hit->normal, &hit->pos);
            if (hit->pos.w >= 0.0f)
                SubScaled(&obj->x120, &target, plane, Dot(&target, plane) - obj->plane.w - 2.0f);
            else
                AddScaled(&obj->x120, &pos, plane, hit->x8);
            obj->x120.z -= 4.0f;
            ClipVelocity(obj);
            CopyXYZ(&obj->x1E0, &obj->x120);
        } else {
            obj->x1C = 0;
            __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->plane));
            func_001246D0(obj);
        }
    }
    func_00123F60(obj, 0);
}
