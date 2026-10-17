/* UNIT 0x0015B1A0 0x0015B380 */
#include "rider_geometry.h"
typedef unsigned int u128 __attribute__((mode(TI)));

struct Vec4 {
    union {
        u128 q;
        float f[4];
    };
};

struct Body {
    char pad0[0x10];
    float x10;
    float x14;
    float x18;
    char pad1C[0x40 - 0x1C];
    char x40[0x10];
    Vec4 x50;
    Vec4 x60;
};

class Item {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Body *GetBody();
    virtual void v4();
    virtual void *GetTarget();
};

struct Obj : public Item {
    int x4;
    Body *x8;
    char padC[0x10 - 0xC];
    Vec4 rot;
    char pad20[0x140 - 0x20];
    float x140[2];
    char pad148[0x150 - 0x148];
    Vec4 x150;
    Vec4 x160;
    char pad170[0x1A0 - 0x170];
    Vec4 x1A0;
    char pad1B0[0x1E8 - 0x1B0];
    short x1E8;
};


extern "C" float func_00235EA8(float, float, float, float);
extern "C" void func_00222F60(Vec4 *, Vec4 *, Vec4 *);
extern "C" void func_00223578(Vec4 *, Vec4 *, float);
extern "C" void func_00223210(Vec4 *, Vec4 *, Vec4 *);
extern "C" void func_002230B0(Vec4 *, Vec4 *);
extern "C" void func_002232C8(void *, Vec4 *);
extern "C" void func_00159CE0(Obj *, int);

static inline float Dot(Vec4 *a, Vec4 *b)
{
    float dot;

    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(dot)
                      : "f"(a->f[0]), "f"(b->f[0]), "f"(a->f[1]), "f"(b->f[1]), "f"(a->f[2]), "f"(b->f[2]));
    return dot;
}

/* 128-bit vector copy (lq/sq through $2) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"((dst).q) : "r"(&(src)) : "$2")

/* copies xyz, keeps dst's w */
static inline void SetXYZ(Vec4 *dst, Vec4 *src)
{
    float w = dst->f[3];

    COPY_VEC(*dst, *src);
    dst->f[3] = w;
}

/* same, but keeps the source's z component live for the caller */
static inline void SetXYZ_Z(Vec4 *dst, Vec4 *src, float z)
{
    float w = dst->f[3];

    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst->q) : "r"(src), "f"(z) : "$2");
    dst->f[3] = w;
}

extern "C" void func_0015B1A0(Obj *obj)
{
    Vec4 q;
    Vec4 dir;
    float height;
    float lenSq;
    float rinv;
    Vec4 *target;
    Vec4 *fwd;
    float *p;
    float x, y, z;

    height = func_0015B910((GroundNpc *)(obj), (void *)(obj->GetTarget()), 0);
    obj->x8->x18 = func_00235EA8(obj->x8->x18, obj->x8->x18 - (height - 3.306f), 0.04f, 0.001f);
    target = &obj->x1A0;
    fwd = &obj->x8->x60;
    if (Dot(target, fwd) < 0.99f) {
        func_00222F60(&q, fwd, target);
        func_00223578(&q, &q, 0.1f);
        func_00223210(&obj->rot, &obj->rot, &q);
        func_002230B0(&obj->rot, &obj->rot);
        func_002232C8(obj->GetBody()->x40, &obj->rot);
        return;
    }
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(q));
    COPY_VEC(dir, obj->x8->x50);
    dir.f[2] = 0.0f;
    x = dir.f[0];
    y = dir.f[1];
    z = dir.f[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(lenSq) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(rinv) : "f"(1.0f), "f"(lenSq));
    dir.f[0] = x * rinv;
    dir.f[1] = y * rinv;
    p = obj->x140;
    SetXYZ(&obj->x150, &q);
    SetXYZ_Z(&obj->x160, &dir, z);
    p[0] = obj->x8->x10;
    p[1] = obj->x8->x14;
    obj->x1E8 = 0;
    func_00159CE0(obj, 6);
}
