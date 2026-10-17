/* UNIT 0x0010F8E8 0x0010FB00 */
#include "vector.h"
typedef Vector4 Vec4;

typedef struct Body {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x50 - 0x20];
    Vec4 x50;
} Body;

typedef struct World {
    char pad0[0x40C0];
    char x40C0[0x4180 - 0x40C0];
    Vec4 x4180;
    char pad4190[0x792C - 0x4190];
    Body *x792C;
} World;

typedef struct Obj {
    World *x0;
    char pad4[0x30 - 0x4];
    Vec4 x30;
    char pad40[0x50 - 0x40];
    Vec4 x50;
    char pad60[0x78 - 0x60];
    int x78;
    char pad7C[0xD0 - 0x7C];
    Vec4 xD0;
    Vec4 xE0;
    char padF0[0x240 - 0xF0];
    float x240;
    float x244;
    float x248;
    float x24C;
    float x250;
    float x254;
    float x258;
    float x25C;
    float x260;
    char pad264[0x280 - 0x264];
    float x280;
} Obj;

extern float func_001385A8(void *p);
extern float func_00222640(Vec4 *v);
extern void func_002087C0(float *angle);

static inline void Sub(Vec4 *out, Vec4 *a, Vec4 *b) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

/* normalizes v into out, returns the length (sqrt + div) */
static inline float Normalize(Vec4 *out, Vec4 *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=f"(len) : "f"(x), "f"(y), "f"(z) : "$2");
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
    out->z = z * inv;
}

static inline float Length(Vec4 *v) {
    float len;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(len) : "f"(v->x), "f"(v->y), "f"(v->z));
    return len;
}

void func_0010F8E8(Obj *obj)
{
    float matrix[16] __attribute__((aligned(16)));
    Vec4 forward;
    Vec4 delta;
    Vec4 local;
    Vec4 *v;

    Sub(&delta, &obj->x30, &obj->x0->x792C->x10);
    NormalizeFast(&obj->xD0, &delta);
    obj->x248 = obj->x240;
    if (func_001385A8(obj->x0->x40C0) < 0.05f) {
        NormalizeFast(&forward, &obj->x0->x792C->x50);
    } else {
        NormalizeFast(&forward, &obj->x0->x4180);
    }
    func_002226C8(matrix, &forward);
    func_00221888(matrix, &delta, &local);
    local.z = 0.0f;
    obj->x240 = Length(&local);
    obj->x280 = func_00222640(&obj->x0->x792C->x50);
    obj->x24C = func_00222640(&obj->xD0);
    obj->x250 = obj->x24C - obj->x280;
    func_002087C0(&obj->x250);
    if (obj->x78) {
        v = &obj->xE0;
        Sub(v, &obj->x50, &obj->x0->x792C->x10);
        obj->x258 = Normalize(v, v);
        obj->x25C = func_00222640(v);
        obj->x260 = obj->x25C - obj->x280;
        func_002087C0(&obj->x260);
    }
}
