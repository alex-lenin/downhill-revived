/* UNIT 0x001B31A0 0x001B3300 */
#include "rider.h"
typedef Vector4 Vec4;

typedef SceneNode Target;

typedef Rider World;

typedef RiderComponent Owner;

typedef struct Obj {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x40 - 0x20];
    Vec4 x40;
    char pad50[0x60 - 0x50];
    Vec4 x60;
    char pad70[0x90 - 0x70];
    Vec4 x90;
    char padA0[0xC0 - 0xA0];
    Owner *xC0;
    char padC4[0xD0 - 0xC4];
    Vec4 xD0;
    Vec4 xE0;
    char padF0[0x5A0 - 0xF0];
    float x5A0;
} Obj;

extern Vec4 D_007684C0[4][3];



static inline void VecSub(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecAdd(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecScale(Vec4 *dst, Vec4 *a, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "f"(s) : "$2");
}

static inline float VecLength(Vec4 *v)
{
    float len;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(len) : "f"(v->x), "f"(v->y), "f"(v->z));
    return len;
}

void func_001B31A0(Obj *obj)
{
    Vec4 tmp;
    Target *target;
    int idx;

    idx = func_001B1C08(obj->xC0);
    target = obj->xC0->rider->bodyNode;

    VecSub(&tmp, &obj->x10, &obj->x40);
    obj->x5A0 = VecLength(&tmp);

    VecAdd(&obj->x60, &D_007684C0[0][idx], &D_007684C0[1][idx]);
    VecScale(&obj->x60, &obj->x60, 0.5f);
    VecSub(&tmp, &obj->x60, &target->position);
    func_00221888(&target->matrix, &tmp, &obj->xD0);

    VecAdd(&obj->x90, &D_007684C0[2][idx], &D_007684C0[3][idx]);
    VecScale(&obj->x90, &obj->x90, 0.5f);
    VecSub(&tmp, &obj->x90, &target->position);
    func_00221888(&target->matrix, &tmp, &obj->xE0);
}
