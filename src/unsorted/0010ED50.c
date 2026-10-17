/* UNIT 0x0010ED50 0x0010F068 */
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
#include "vector.h"
typedef Vector4 Vec;

typedef struct Hit {
    char pad0[0x10];
    Vec x10;
    Vec x20;
} Hit __attribute__((aligned(16)));

typedef struct Physics {
    char pad0[0x34];
    Vec *pos;
    char pad38[0xC0 - 0x38];
    Vec velocity;
    char padD0[0x100 - 0xD0];
    Vec force;
    char pad110[0x150 - 0x110];
    float mass;
    char pad154[0x490 - 0x154];
    float drag;
} Physics;

typedef struct Obj {
    char *x0;
    char pad4[0x1FC];
    Vec x200;
    Vec x210;
    char pad220[0x9C];
    float x2BC;
} Obj;

typedef struct Global {
    char pad0[0x5BB4];
    int x5BB4;
    char pad5BB8[0x24];
    int x5BDC;
} Global;

extern Global D_00398C40;
extern float D_0026DD00[];


#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline float Normalize(Vec *out, Vec *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline void Scale(Vec *out, Vec *v, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void Add(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void AddScaled(Vec *out, Vec *a, Vec *b, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

void func_0010ED50(Obj *obj) {
    Vec start[4];
    Vec end[4];
    Vec accel;
    Vec pos;
    Vec next;
    Vec vel;
    Vec dir;
    Hit hit;
    Physics *phys;
    float scale;
    float t;
    float dt;
    float len;
    int value;
    int i;

    if (D_00398C40.x5BB4 == 0) value = D_00398C40.x5BDC / 2;
    else if (D_00398C40.x5BB4 == 2) value = D_00398C40.x5BDC / 2 + 50;
    else value = D_00398C40.x5BDC / 2 + 25;
    scale = (float)value * 0.07f + 1.0f;

    phys = (Physics *)(obj->x0 + 0x40C0);
    COPY_VECTOR(pos, phys->pos);
    COPY_VECTOR(vel, &phys->velocity);

    i = 0;
    for (t = D_0026DD00[0]; t <= 3.0f; t += dt) {
        dt = D_0026DD00[i];
        len = Normalize(&dir, &vel);
        Scale(&dir, &dir, -(len * len) * phys->mass * phys->drag);
        Add(&accel, &phys->force, &dir);
        Scale(&accel, &accel, 1.0f / phys->mass);
        AddScaled(&next, &pos, &vel, dt);
        AddScaled(&next, &next, &accel, dt * dt * 0.5f);
        AddScaled(&vel, &vel, &accel, dt);
        COPY_VECTOR(start[i], &pos);
        COPY_VECTOR(end[i], &next);
        i++;
        COPY_VECTOR(pos, &next);
    }

    for (i = 0, obj->x2BC = 0.0f; i < 4 && !QuerySpatialHitsFloatFirst((Vector4 *)(&start[i]), (Vector4 *)(&end[i]), scale, 0.0f, 1.0f, (SpatialHit *)(&hit), 1); i++) {
        obj->x2BC += D_0026DD00[i];
    }
    if (i < 4) {
        obj->x2BC += hit.x10.w * D_0026DD00[i];
        COPY_VECTOR(obj->x200, &hit.x20);
        COPY_VECTOR(obj->x210, &hit.x10);
    }
}
