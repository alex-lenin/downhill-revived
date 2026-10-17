/* UNIT 0x0010F710 0x0010F8E8 */
#include "vector.h"
typedef Vector4 Vec4;

typedef struct Body {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x40 - 0x20];
    Vec4 x40;
    Vec4 x50;
} Body;

typedef struct World {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    Body *x792C;
    char pad7930[0x796C - 0x7930];
    int x796C;
} World;

typedef struct Obj {
    World *x0;
    char pad4[0x8 - 0x4];
    World *x8;
    char padC[0x294 - 0xC];
    float x294;
} Obj;

extern float func_001385A8(void *p);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Sub(Vec4 *out, Vec4 *a, Vec4 *b) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

/* normalizes v into out, returns the length (sqrt + div). Same asm as func_0010ED50's
   Normalize; x and y are passed twice (Dot(v, v) form), which the register allocation needs. */
static inline float Normalize(Vec4 *out, Vec4 *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %2, %3\n\tmadda.s %4, %5\n\tmadd.s %1, %6, %6\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=f"(len) : "f"(x), "f"(x), "f"(y), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

/* fast normalize (rsqrt) */
static inline void NormalizeFast(Vec4 *out, Vec4 *v) {
    float x, y, z, len, inv;
    x = v->x; y = v->y; z = v->z;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z;
}

static inline float Dot(Vec4 *a, Vec4 *b) {
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

int func_0010F710(Obj *obj, float *out)
{
    Vec4 diff;
    Vec4 dir;
    Vec4 side;
    Vec4 local;
    float speed;
    float len;
    float proj;
    int result;

    result = 0;
    if (obj->x8 != 0 && obj->x0->x796C != 0xA0 && obj->x294 < 50.0f) {
        speed = func_001385A8(obj->x0->x40C0);
        if (func_001385A8(obj->x8->x40C0) * 1.1f < speed) {
            Sub(&diff, &obj->x8->x792C->x10, &obj->x0->x792C->x10);
            if (diff.z < 0.0f) {
                COPY_VECTOR(dir, &obj->x0->x792C->x50);
                dir.z = 0.0f;
                diff.z = 0.0f;
                len = Normalize(&side, &diff);
                side.z = 0.0f;
                NormalizeFast(&dir, &dir);
                proj = len * Dot(&dir, &side);
                if (len * len - proj * proj < 9.0f) {
                    result = 1;
                    func_00221888(&obj->x0->x792C->x40, &diff, &local);
                    out[0] = local.x;
                }
            }
        }
    }
    return result;
}
