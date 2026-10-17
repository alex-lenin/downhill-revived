/* UNIT 0x00231E00 0x002320B0 */
#include "scalar_api.h"
#include "impulse.h"
typedef struct Scalar00231E00Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Scalar00231E00Obj {
    float x0;
    char pad4[0x4C];
    Vec x50;
    char pad60[0x20];
    float x80[4][4];
    Vec xC0;
    Vec xD0;
} Obj;


static inline void Sub(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}
static inline void Add(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}
static inline void Cross(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}
static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}
static inline void Scale(Vec *out, Vec *v, float scale) {
    __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v), "f"(scale) : "$2");
}
void func_00231E00(Obj *a, Obj *b, Vec *point, Vec *normal, float restitution) {
    Vec armA, velA, armB, velB, work, temp;
    float speed, impulse, denom, dot;
    Sub(&armA, point, &a->x50);
    Cross(&velA, &a->xD0, &armA);
    Add(&velA, &velA, &a->xC0);
    Sub(&armB, point, &b->x50);
    Cross(&velB, &b->xD0, &armB);
    Add(&velB, &velB, &b->xC0);
    Sub(&work, &velA, &velB);
    speed = Dot(normal, &work);
    if (speed > 0.001f) return;
    if (speed > -0.001f) return;
    impulse = -(restitution + 1.0f) * speed;
    denom = a->x0 + b->x0;
    Cross(&work, &armA, normal);
    func_00221888((Matrix4 *)(&a->x80), (Vector4 *)(&work), (Vector4 *)(&temp));
    Cross(&work, &temp, &armA);
    denom += Dot(normal, &work);
    Cross(&work, &armB, normal);
    func_00221888((Matrix4 *)(&b->x80), (Vector4 *)(&work), (Vector4 *)(&temp));
    Cross(&work, &temp, &armB);
    denom += Dot(normal, &work);
    impulse /= denom;
    Scale(&work, normal, impulse);
    func_00231CC0((ImpulseBody *)(a), (Vector4 *)(&work), (Vector4 *)(point));
    Scale(&work, &work, -1.0f);
    func_00231CC0((ImpulseBody *)(b), (Vector4 *)(&work), (Vector4 *)(point));
}
