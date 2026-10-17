/* UNIT 0x001B3440 0x001B3858 */
#include "camera.h"
#include "rider.h"
typedef Vector4 Vec;

typedef SceneNode Target;

typedef RiderFade Fade;
typedef Rider World;

typedef RiderComponent Owner;

typedef struct Obj {
    char pad0[0x10];
    Vec x10;
    Vec x20;
    char pad30[0xC0 - 0x30];
    Owner *xC0;
    char padC4[0x110 - 0xC4];
    float x110;
    char pad114[0x598 - 0x114];
    float x598;
} Obj;

extern Vec D_00768590[];



extern float func_001385A8(void *p);
extern float func_001ADAC8(void *p);
extern float func_001DC0F8(void *p);
extern float func_002487A0(float);
extern float func_00248990(float);


#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecSub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecAdd(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecScale(Vec *dst, Vec *a, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "f"(s) : "$2");
}

static inline void VecCross(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecApply(Vec *dst, Vec *m, Vec *v)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(v), "r"(m));
}

static inline void VecNormalize(Vec *v)
{
    float len, inv, x = v->x, y = v->y, z = v->z;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
}

/* normalizes using all three components but only rescales x and z */
static inline void VecNormalizeXZ(Vec *v)
{
    float len, inv;
    __asm__ volatile ("mula.s %2, %3\n\tmadda.s %4, %5\n\tmadd.s %0, %6, %6\n\tsqrt.s %0, %0\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %1\n\tnop\n\tdiv.s %1, %1, %0"
                      : "=&f"(len), "=&f"(inv) : "f"(v->x), "f"(v->x), "f"(v->y), "f"(v->y), "f"(v->z) : "$2");
    v->x *= inv;
    v->z *= inv;
}

static inline void VecSet(Vec *v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

static inline float minf(float a, float b) { float r; __asm__ ("min.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b)); return r; }
static inline float maxf(float a, float b) { float r; __asm__ ("max.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b)); return r; }

/* uses VU0 / EE FPU inline asm */
void func_001B3440(Obj *obj)
{
    Vec tmp;
    Vec scaled;
    Vec start;
    Vec end;
    Vec basis[4];
    Vec m[4];
    Vec up;
    World *world;
    Fade *fade;
    Target *target;
    Vec *pos;
    Vec *mat;
    int idx;
    float k, d, limit, angle, c, s, rx, rz;

    world = (World *)obj->xC0->rider;
    target = world->bodyNode;
    idx = func_001B1C08(obj->xC0);
    pos = &target->position;
    VecSub(&tmp, &D_00768590[idx], pos);
    mat = target->matrix.row;
    func_00221888((Matrix4 *)mat, &tmp, &start);
    VecSub(&tmp, &D_00768590[idx + 1], pos);
    func_00221888((Matrix4 *)mat, &tmp, &end);
    VecSub(&basis[1], &end, &start);
    VecNormalize(&basis[1]);
    VecSub(&basis[2], &obj->x10, &start);
    VecCross(&basis[0], &basis[2], &basis[1]);
    VecNormalize(&basis[0]);
    VecCross(&basis[2], &basis[1], &basis[0]);
    func_00221930((Matrix4 *)m, (Matrix4 *)basis, (Matrix4 *)mat);
    VecSet(&up, 0.0f, 0.0f, 1.0f);
    COPY_VECTOR(tmp, &up);
    func_00221888((Matrix4 *)m, &tmp, &tmp);
    limit = obj->x598 * 0.1f;
    k = func_001385A8(&world->physics);
    d = 1.0f - m[2].z;
    limit = minf(k * limit, 1.0f);
    k = maxf(minf(maxf(d * 0.3f, 0.0f), 1.0f), limit);
    VecScale(&tmp, &tmp, 1.0f - k);
    VecScale(&scaled, &up, k);
    VecAdd(&tmp, &tmp, &scaled);
    angle = func_001ADAC8(world->x48B0) + world->x4D28 * 0.1f;
    angle += func_001DC0F8(world->x621C);
    fade = &world->x4D40;
    angle += fade->x4 * fade->xC;
    tmp.y = 0.0f;
    VecNormalizeXZ(&tmp);
    c = func_002487A0(angle);
    s = func_00248990(angle);
    rz = c * tmp.x + s * tmp.z;
    rx = -s * tmp.x + c * tmp.z;
    VecSub(&tmp, &obj->x10, &start);
    func_00221888((Matrix4 *)basis, &tmp, &scaled);
    tmp.x = rx * scaled.x + rz * scaled.z;
    tmp.y = scaled.y;
    tmp.z = -rz * scaled.x + rx * scaled.z;
    VecApply(&scaled, basis, &tmp);
    VecAdd(&tmp, &scaled, &start);
    VecApply(&scaled, mat, &tmp);
    VecAdd(&obj->x20, &scaled, pos);
    obj->x110 = rz;
    func_001B3028((RiderCamera *)(obj), 7, rz);
}
