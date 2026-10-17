/* UNIT 0x001246D0 0x00124960 */
#include "spatial_primitives.h"
#if defined(__mips__)
#define CallShared00211390_0 func_00211390
#else
#define CallShared00211390_0 Forward00211390_0
#endif
struct Vec {
    float v[4];
} __attribute__((aligned(16)));

struct Body {
    int *x0;
    char pad4[12];
    Vec position;
};

class Actor {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual Body *v4();
};

struct Obj {
    char pad0[0x20];
    int x20;
    char pad24[0x10C];
    Actor *actor;
    char pad134[0xAC];
    Vec position;
};

extern "C" int func_00211390(Vec *, int, int, Body **, float, int);
extern "C" float func_00236088(float, float, float, float);

static inline Vec *Position(Obj *obj)
{
    return obj->actor ? &obj->actor->v4()->position : 0;
}

static inline void Sub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0(%1)\n\t"
        "lqc2 $vf12, 0(%2)\n\t"
        "vsub.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void Normalize(Vec *v)
{
    float inverseLength;
    float length;

    __asm__ volatile (
        "mula.s %2, %2\n\t"
        "madda.s %3, %3\n\t"
        "madd.s %1, %4, %5\n\t"
        "sqrt.s %1, %1\n\t"
        "lui $2, 0x3f80\n\t"
        "mtc1 $2, %0\n\t"
        "nop\n\t"
        "div.s %0, %0, %1"
        : "=&f"(inverseLength), "=&f"(length)
        : "f"(v->v[0]), "f"(v->v[1]), "f"(v->v[2]), "f"(v->v[2])
        : "$2");
    v->v[0] *= inverseLength;
    v->v[1] *= inverseLength;
    v->v[2] *= inverseLength;
}

static inline void Scale(Vec *dst, Vec *v, float scale)
{
    __asm__ volatile (
        "lqc2 $vf11, 0(%1)\n\t"
        "mfc1 $2, %2\n\t"
        "qmtc2.ni $2, $vf12\n\t"
        "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*dst) : "r"(v), "f"(scale) : "$2");
}

static inline void Add(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0(%1)\n\t"
        "lqc2 $vf12, 0(%2)\n\t"
        "vadd.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void CopyKeepW(Obj *dst, Vec *src, Vec *position)
{
    float w = position->v[3];
    __asm__ volatile (
        "lq $2, 0(%1)\n\t"
        "sq $2, %0"
        : "=m"(dst->position) : "r"(src) : "$2");
    position->v[3] = w;
}

extern "C" int func_001246D0(Obj *obj)
{
    Body *nearby[10];
    Vec difference;
    Vec scaled;
    Vec target;
    int count;
    int remaining;
    Body **current;
    Body *body;
    int flags;
    int mask;
    Vec *copyDst;
    Vec *differencePtr;
    Vec *scaledPtr;
    int result;

    count = CallShared00211390_0(Position(obj), 4000, 0x1193, nearby, 4.0f, 10);
    if (count > 0) {
        mask = 0x3FF80;
        scaledPtr = &scaled;
        differencePtr = &difference;
        copyDst = &obj->position;
        current = nearby;
        remaining = count;
        do {
            flags = *(*current)->x0 & mask;
            if (flags != (*obj->actor->v4()->x0 & mask)) {
                body = obj->actor->v4();
                obj->x20++;
                Sub(&difference, Position(obj), &(*current)->position);
                Normalize(&difference);
                Scale(&scaled, differencePtr, 4.0f);
                Add(&target, &(*current)->position, scaledPtr);
                body->position.v[0] = func_00236088(
                    body->position.v[0], target.v[0], 0.15f, 0.001f);
                body->position.v[1] = func_00236088(
                    body->position.v[1], target.v[1], 0.15f, 0.001f);
                body->position.v[2] = func_00236088(
                    body->position.v[2], target.v[2], 0.15f, 0.001f);
                CopyKeepW(obj, &body->position, copyDst);
            }
            remaining--;
            current++;
        } while (remaining);
    }
    if (count < 2) {
        result = 0;
        obj->x20 = 0;
    } else {
        result = 1;
    }
    return result;
}
