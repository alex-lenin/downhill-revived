/* UNIT 0x002320B0 0x00232448 */
#include "impulse.h"
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Obj {
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
static inline float Normalize(Vec *v) {
    float length, inv, x = v->x, y = v->y, z = v->z;
    __asm__ volatile("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %0, %4, %4\n\tsqrt.s %0, %0\n\tlui $2, 0x3F80\n\tmtc1 $2, %1\n\tdiv.s %1, %1, %0" : "=f"(length), "=&f"(inv) : "f"(x), "f"(y), "f"(z) : "$2");
    v->x = x * inv; v->y = y * inv; v->z = z * inv;
    return length;
}
void func_002320B0(Obj *obj, Vec *point, Vec *normal, float restitution, float lowFriction, float highFriction) {
    Vec delta, velocity, impulse, transformed, cross, transformed2;
    float dot, magnitude, denominator, length, friction;
    void *matrix;
    Sub(&delta, point, &obj->x50);
    Cross(&velocity, &obj->xD0, &delta);
    Add(&velocity, &velocity, &obj->xC0);
    dot = Dot(normal, &velocity);
    if (dot > 0.001f) return;
    if (dot > -0.001f) return;
    restitution += 1.0f;
    restitution = -restitution;
    magnitude = restitution * dot;
    denominator = obj->x0;
    Cross(&impulse, &delta, normal);
    matrix = obj->x80;
    func_00221888((Matrix4 *)(matrix), (Vector4 *)(&impulse), (Vector4 *)(&transformed));
    Cross(&impulse, &transformed, &delta);
    denominator += Dot(normal, &impulse);
    Scale(&impulse, normal, magnitude / denominator);
    func_00231CC0((ImpulseBody *)(obj), (Vector4 *)(&impulse), (Vector4 *)(point));
    Sub(&delta, point, &obj->x50);
    Cross(&velocity, &obj->xD0, &delta);
    Add(&velocity, &velocity, &obj->xC0);
    Scale(&transformed, normal, Dot(normal, &velocity));
    Sub(&transformed, &velocity, &transformed);
    length = Normalize(&transformed);
    magnitude = -1.0f;
    Scale(&transformed, &transformed, magnitude);
    friction = lowFriction;
    if (length > 6.5f) friction = highFriction;
    restitution = friction + magnitude;
    restitution += 1.0f;
    restitution = -restitution;
    restitution *= length;
    denominator = obj->x0;
    Cross(&cross, &delta, &transformed);
    func_00221888((Matrix4 *)(matrix), (Vector4 *)(&cross), (Vector4 *)(&transformed2));
    Cross(&cross, &transformed2, &delta);
    denominator += Dot(&transformed, &cross);
    Scale(&impulse, &transformed, -(restitution / denominator));
    func_00231CC0((ImpulseBody *)(obj), (Vector4 *)(&impulse), (Vector4 *)(point));
}
