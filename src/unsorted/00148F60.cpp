/* UNIT 0x00148F60 0x0014DD58 .sdata=0x29C5B0:0x29C5C0 .sbss=0x29DD60:0x29DD68 */
#include "vector_blend.h"
#if defined(__mips__)
#define CallShared00223578_0 func_00223578
#else
#define CallShared00223578_0 Forward00223578_0
#endif
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
#include "include_asm.h"

extern "C" float fabsf(float);

int D_0029C5B0 = 1;
int D_0029C5B4 = 0;

namespace u148F60 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec x10;
    char pad20[0x20];
    Vec x40[4];
} Body;

typedef struct Rider {
    char pad0[0x4308];
    float x4308;
    char pad430C[0x4564 - 0x430C];
    short x4564;
    char pad4566[0x792C - 0x4566];
    Body *x792C;
    char pad7930[0x7A69 - 0x7930];
    unsigned char x7A69;
    char pad7A6A[0x7A78 - 0x7A6A];
    float x7A78;
} Rider;

typedef struct Obj {
    char pad0[0x40];
    Vec x40[4];
    Vec x80;
    Vec x90;
    Vec xA0;
    char padB0[0x140 - 0xB0];
    Vec x140;
    char pad150[0x2B0 - 0x150];
    Body *x2B0;
    Rider *x2B4;
    char pad2B8[0x2CC - 0x2B8];
    float x2CC, x2D0, x2D4;
    char pad2D8[0x358 - 0x2D8];
    int x358;
    char pad35C[0x36E - 0x35C];
    unsigned char x36E, x36F;
} Obj;

typedef struct Scratch {
    Vec x0, x10, x20, x30;
    Vec x40[4];
} Scratch;

extern "C" int D_0029C5B0;
extern void func_00149EC0(Obj *, Vec *);
extern void func_0014A830(Obj *, Vec *);
extern void func_00149418(Obj *);
extern void func_00221ED0(void *, void *);
extern void func_001496D0(Obj *);
extern void func_00149798(Obj *);
extern void func_00149888(Obj *);
extern float func_0012DF48(Rider *);
extern float func_00134E40(void *);
extern int func_00138560(void *);
extern float func_00235EA8(float, float, float, float);
extern void func_00221F50(void *, Vec *);

extern void func_00223348(void *, void *);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Add(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
            : "=m"(*out) : "r"(a), "r"(b) : "memory");
}

static inline void Apply(Vec *out, Vec *v, Vec *matrix) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0"
            : "=m"(*out) : "r"(v), "r"(matrix) : "memory");
}

/* The game's Abs helper (docs/findings/013). */
static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

void func_00148F60(Obj *obj) {
    Scratch tmp;
    Rider *rider;
    Vec *body;
    float delta;
    float coefficient;
    float threshold;
    short speed;

    if (obj->x358) {
        func_00149EC0(obj, &tmp.x0);
        Add(&tmp.x10, &obj->x2B4->x792C->x10, &tmp.x0);
        func_0014A830(obj, &tmp.x10);
        return;
    }

    if (D_0029C5B0) {
        func_00149418(obj);
    } else {
        func_00221ED0(obj->x40, obj->x2B4->x792C->x40);
    }

    COPY_VECTOR(tmp.x10, &obj->xA0);
    rider = obj->x2B4;
    coefficient = 42.971836f;
    delta = rider->x4308 * coefficient;
    if (Abs(delta) > 1.5f) {
        if (delta > 0.0f) delta = 1.5f;
        else delta = -1.5f;
    }
    tmp.x10.x += delta;

    rider = obj->x2B4;
    if (rider->x7A69) {
        tmp.x10.y += 0.4f;
    } else if (rider->x7A78 != 0.0f) {
        tmp.x10.y += 0.2f;
    } else if (func_0012DF48(rider) != 0.0f) {
        tmp.x10.y += 0.3f;
    }

    tmp.x10.z += func_00134E40((char *)obj->x2B4 + 0x43A0) * 0.5f;
    if (func_00138560((char *)obj->x2B4 + 0x40C0) & 0xFF) {
        speed = obj->x2B4->x4564;
        if (speed >= 31) tmp.x10.z += 0.5f;
        else tmp.x10.z += (float)speed * 0.016666668f;
    }

    obj->x140.x = func_00235EA8(obj->x140.x, tmp.x10.x, 0.07f, 0.001f);
    obj->x140.y = func_00235EA8(obj->x140.y, tmp.x10.y, 0.07f, 0.001f);
    obj->x140.z = func_00235EA8(obj->x140.z, tmp.x10.z, 0.15f, 0.001f);

    if (D_0029C5B0) {
        Vec *axis = &tmp.x20;
        tmp.x20.x = 0.0f;
        axis->y = 1.0f;
        tmp.x20.z = 0.0f;
        Apply(&tmp.x30, &tmp.x20, obj->x2B4->x792C->x40);
        Apply(&tmp.x0, &obj->x140, obj->x40);
        Add(&tmp.x0, &tmp.x0, &tmp.x30);
    } else {
        body = obj->x2B4->x792C->x40;
        Apply(&tmp.x0, &obj->x140, body);
    }
    body = &obj->x2B4->x792C->x10;
    Add(&obj->x80, body, &tmp.x0);
    func_001496D0(obj);
    do {
    if (obj->x36E) func_00149798(obj);
    else if (obj->x36F) func_00149888(obj);

    delta = fabsf(obj->x2CC);
    } while (0);
    threshold = 0.001f;
    if (delta > threshold ||
        (delta = fabsf(obj->x2D0)) > threshold ||
        (delta = fabsf(obj->x2D4)) > threshold) {
        int flag = func_00138560((char *)obj->x2B4 + 0x40C0) & 0xFF;
        float current = obj->x2CC;
        if (flag) {
            obj->x2CC = func_00235EA8(current, obj->x2D4, 0.07f, 0.001f);
        } else {
            obj->x2CC = func_00235EA8(current, obj->x2D0, 0.2f, 0.001f);
        }
        tmp.x20.x = obj->x2CC;
        tmp.x20.y = 0.0f;
        tmp.x20.z = 0.0f;
        func_00221F50(tmp.x40, &tmp.x20);
        func_00221A60((Matrix4 *)((char *)obj->x2B0 + 0x40), (const Matrix4 *)(tmp.x40), (const Matrix4 *)(obj->x40));
        func_00221ED0(obj->x40, (char *)obj->x2B0 + 0x40);
        func_00223348((char *)obj + 0xE0, obj->x40);
    }
    func_00221ED0((char *)obj->x2B0 + 0x40, obj->x40);
}
}

/* ---- merged from src/unsorted/00149418.c ---- */
namespace u149418 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

extern int func_00138560(char *);
extern float func_001385A8(char *);
extern void func_00223348(char *, char *);
extern float func_00236088(float, float, float, float);
extern void func_0014A3A0(char *, void *);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_00149418(char *obj) {
    Vec dir;
    Vec tmp;
    float rate, r;

    if (*(unsigned char *)(obj + 0x36E)) {
        return;
    }
    if (*(unsigned char *)(obj + 0x36F)) {
        return;
    }
    rate = 0.2f;
    if (func_00138560(*(char **)(obj + 0x2B4) + 0x40C0) & 0xFF) {
        COPY_VECTOR(dir, *(char **)(*(char **)(obj + 0x2B4) + 0x792C) + 0x50);
        if (*(float *)(*(char **)(obj + 0x2B4) + 0x4300) > 0.0f) {
            rate = 1.0f;
        }
    } else if (func_001385A8(*(char **)(obj + 0x2B4) + 0x40C0) < 0.1f) {
        char *p = *(char **)(obj + 0x2B4);
        void *cur = *(void **)(p + 0x4ECC);
        if (cur == p + 0x62B4) {
            float *base = (float *)(p + 0x7AA0);
            float *v1 = (float *)(*(char **)(p + 0x792C) + 0x50);
            float bx, vx, vz, vy, by, bz, dot;
            float x, y, z, len, inv;
            bx = base[0];
            vx = v1[0];
            vy = v1[1];
            vz = v1[2];
            by = base[1];
            bz = base[2];
            dot = vx;
            __asm__ volatile ("mula.s %0,%1\n\tmadda.s %2,%3\n\tmadd.s %0,%4,%5"
                : "+f"(dot)
                : "f"(bx), "f"(vy), "f"(by), "f"(vz), "f"(bz));
            __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                : "=m"(tmp) : "r"(base), "f"(dot) : "$2", "memory");
            __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                : "=m"(dir) : "r"(*(char **)(p + 0x792C) + 0x50), "r"(&tmp) : "memory");
            x = dir.x; y = dir.y; z = dir.z;
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                : "=f"(len) : "f"(x), "f"(y), "f"(z));
            __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
            dir.x = x * inv;
            dir.y = y * inv;
            dir.z = z * inv;
        } else {
            COPY_VECTOR(dir, *(char **)(p + 0x792C) + 0x50);
        }
    } else {
        float *src = (float *)(*(char **)(obj + 0x2B4) + 0x4180);
        float x, y, z, len, inv;
        x = src[0];
        y = src[1];
        z = src[2];
        __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
            : "=f"(len) : "f"(x), "f"(y), "f"(z));
        __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
        dir.x = x * inv;
        dir.y = y * inv;
        dir.z = z * inv;
        func_00223348(obj + 0xE0, obj + 0x40);
    }
    r = func_00236088(*(float *)(obj + 0x120), rate, 0.1f, 0.001f);
    *(float *)(obj + 0x120) = r;
    *(float *)(obj + 0x128) = r;
    *(float *)(obj + 0x124) = r;
    *(float *)(obj + 0xF0) = func_00236088(*(float *)(obj + 0xF0), dir.x, r, 0.0001f);
    *(float *)(obj + 0xF4) = func_00236088(*(float *)(obj + 0xF4), dir.y, *(float *)(obj + 0x124), 0.0001f);
    *(float *)(obj + 0xF8) = func_00236088(*(float *)(obj + 0xF8), dir.z, *(float *)(obj + 0x128), 0.0001f);
    {
        float *src = (float *)(obj + 0xF0);
        float x, y, z, len, inv;
        x = src[0];
        y = src[1];
        z = src[2];
        __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
            : "=f"(len) : "f"(x), "f"(y), "f"(z));
        __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
        src[0] = x * inv;
        src[1] = y * inv;
        src[2] = z * inv;
        func_0014A3A0(obj, src);
    }
}
}

/* ---- merged from src/unsorted/001496D0.c ---- */
namespace u1496D0 {
typedef struct Owner { char pad0[0x4ECC]; void *x4ECC; char pad1[0x7A68-0x4ED0]; unsigned char x7A68; } Owner;
typedef struct { char pad0[0x2B4]; Owner *owner; char pad1[0x38]; float x2F0; char pad2[0x7A]; unsigned char x36E, x36F; } Object;
extern "C" Owner *D_0043EEC0[];
extern int func_001E64C8(void *);
extern void func_00223348(void *, void *);
void func_001496D0(Object *obj)
{
    int active = 0;
    Owner *owner = obj->owner;
    if (owner->x4ECC == (char *)owner + 0x5E90 || func_001E64C8((char *)owner + 0x6610)) active = 1;
    else {
        owner = obj->owner;
        if (owner->x7A68) active = owner == D_0043EEC0[0];
    }
    if (active && !obj->x36E) func_00223348((char *)obj + 0xE0, (char *)obj + 0x40);
    if ((obj->x36E = active) == 0) {
        float v = obj->x2F0;
        float c;
        float m;
        m = u148F60::Abs(v);
        c = 0.01f;
        if (m > c) obj->x36F = 1;
    }
}
}

/* ---- merged from src/unsorted/00149798.c ---- */
namespace u149798 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct Obj {
    char pad0[0x80];
    Vec x80;
    char pad90[0x10];
    Vec xA0;
    char padB0[0x2BC - 0xB0];
    int x2BC;
    char pad2C0[0x2F0 - 0x2C0];
    float x2F0;
    float x2F4;
    char pad2F8[0x36C - 0x2F8];
    unsigned char x36C;
} Obj;

extern "C" int D_0029C5B0;
extern void func_0014D798(Obj *, Vec *, Vec *);
extern void func_00149EC0(Obj *, Vec *);

static inline float GetScale(Obj *obj)
{
    float scale;
    if (obj->x2BC == 0) {
        unsigned char reversed = obj->x36C;
        scale = obj->x2F0;
        if (reversed != 0) {
            scale = -scale;
        }
    } else {
        scale = *(volatile float *)&obj->x2F0;
    }
    return scale;
}

void func_00149798(Obj *obj)
{
    Vec step;
    Vec saved;
    Vec extra;
    float progress;

    COPY_VECTOR(saved, &obj->xA0);
    func_0014D798(obj, &obj->xA0, &extra);
    if (D_0029C5B0) {
        obj->xA0.y *= 0.8f;
        obj->xA0.z *= 0.5f;
    }
    func_00149EC0(obj, &step);
    COPY_VECTOR(obj->xA0, &saved);

    progress = obj->x2F0 + obj->x2F4;
    obj->x2F0 = progress;
    if (progress > 1.0f) {
        obj->x2F0 = 1.0f;
    }
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0($sp)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(obj->x80) : "r"(&obj->x80), "f"(GetScale(obj)) : "$2");
}
}

/* ---- merged from src/unsorted/00149888.c ---- */
namespace u149888 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct Obj {
    char pad0[0x40];
    Vec x40;
    char pad50[0x30];
    Vec x80;
    char pad90[0x10];
    Vec xA0;
    char padB0[0xE0 - 0xB0];
    Vec xE0;
    char padF0[0x2BC - 0xF0];
    unsigned int x2BC;
    char pad2C0[0x2F0 - 0x2C0];
    float x2F0;
    float x2F4;
    float x2F8;
    char pad2FC[0x36C - 0x2FC];
    unsigned char x36C;
    char pad36D[2];
    unsigned char x36F;
} Obj;

extern "C" int D_0029C5B0;
extern void func_0014D798(Obj *, Vec *, Vec *);
extern void func_00149EC0(Obj *, Vec *);
extern void func_00223348(Vec *, Vec *);

static inline void AddScaled(Vec *out, Vec *a, Vec *b, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}

void func_00149888(Obj *obj)
{
    float progress;
    Vec step;
    Vec saved;
    Vec extra;

    COPY_VECTOR(saved, &obj->xA0);
    func_0014D798(obj, &obj->xA0, &extra);
    if (D_0029C5B0) {
        obj->xA0.y *= 0.8f;
        obj->xA0.z *= 0.5f;
    }
    func_00149EC0(obj, &step);
    COPY_VECTOR(obj->xA0, &saved);

    if (obj->x2BC == 0) {
        float scale = obj->x2F0;
        if (obj->x36C) {
            scale = -scale;
        }
        AddScaled(&obj->x80, &obj->x80, &step, scale);
    } else {
        AddScaled(&obj->x80, &obj->x80, &step, obj->x2F0);
    }

    progress = (obj->x2F0 -= obj->x2F8);
    if (progress < 0.1f) {
        obj->x2F0 = 0.0f;
        obj->x36F = 0;
        func_00223348(&obj->xE0, &obj->x40);
    }
}
}

/* ---- merged from src/unsorted/00149988.cpp ---- */
namespace u149988 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Matrix {
    Vec rows[4];
} Matrix;

typedef struct Camera {
    char pad0[0x10];
    Vec x10;
} Camera;

typedef struct World {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    Camera *x792C;
} World;

typedef struct Parent {
    char pad0[0x40];
    Vec x40;
} Parent;

typedef struct Obj {
    char pad0[0x40];
    Vec x40;
    char pad50[0x30];
    Vec x80;
    char pad90[0xF8 - 0x90];
    float xF8;
    char padFC[0x2B0 - 0xFC];
    Parent *x2B0;
    World *x2B4;
    char pad2B8[4];
    int x2BC;
    char pad2C0[0x2CC - 0x2C0];
    float x2CC;
    float x2D0;
    float x2D4;
    char pad2D8[0x308 - 0x2D8];
    float x308;
    char pad30C[0x358 - 0x30C];
    int x358;
    char pad35C[0x36E - 0x35C];
    unsigned char x36E;
    unsigned char x36F;
} Obj;

extern "C" {
extern void func_00149EC0(Obj *obj, Vec *out);
extern void func_0014A830(Obj *obj, Vec *pos);
extern void func_00149BD0(Obj *obj);
extern void func_00149CD0(Obj *obj, Vec *offset);
extern void func_00149DB8(Obj *obj, Vec *offset);
extern int func_00138560(void *p);
extern float func_00235EA8(float cur, float target, float rate, float eps);
extern void func_00221F50(Matrix *m, Vec *angles);

extern void func_00221ED0(Vec *dst, Vec *src);
extern void func_0014A9A0(Obj *obj);

}

static inline void AddVector(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}


extern "C" void func_00149988(Obj *obj)
{
    Vec offset;
    Vec angles;
    Matrix matrix;
    float target;
    float step;
    float s;
    float v;
    float a;
    float m;
    float eps;
    Vec *transform;

    func_00149EC0(obj, &offset);
    if (obj->x358) {
        AddVector(&angles, &obj->x2B4->x792C->x10, &offset);
        func_0014A830(obj, &angles);
    } else {
        AddVector(&obj->x80, &obj->x2B4->x792C->x10, &offset);
        if (obj->x2BC < 2) {
            func_00149BD0(obj);
            if (obj->x36E) {
                func_00149CD0(obj, &offset);
            } else if (obj->x36F) {
                func_00149DB8(obj, &offset);
            }
        }
    }

    /* Zero-byte labels keep each loaded value live past abs.s, so the result gets
       its own register (retail allocation); eps after the first one keeps its
       li.s after the first abs.s. */
    a = obj->x2CC;
    m = __builtin_fabsf(a);
    __asm__ volatile ("L00149988_abs0:" : : "f"(a));
    eps = 0.001f;
    if (m > eps) goto active;
    a = obj->x2D0;
    m = __builtin_fabsf(a);
    __asm__ volatile ("L00149988_abs1:" : : "f"(a));
    if (m > eps) goto active;
    a = obj->x2D4;
    m = __builtin_fabsf(a);
    __asm__ volatile ("L00149988_abs2:" : : "f"(a));
    if (m > eps) {
    active:
        if (func_00138560(obj->x2B4->x40C0) & 0xFF) {
            s = obj->xF8;
            if (s > 0.0f) {
                target = obj->x2D4;
            } else {
                target = obj->x2D4 * (s + 1.0f);
            }
        } else {
            target = obj->x2D0;
        }
        step = obj->x308;
        if (step != 0.0f) {
            v = obj->x2CC;
            if (v > target) {
                v = (v - step) >? target;
            } else if (v < target) {
                v = (v + step) <? target;
            } else {
                goto skip;
            }
        } else {
            v = func_00235EA8(obj->x2CC, target, 0.05f, 0.0001f);
        }
        obj->x2CC = v;
    skip:
        angles.x = obj->x2CC;
        angles.y = 0.0f;
        angles.z = 0.0f;
        transform = &obj->x40;
        func_00221F50(&matrix, &angles);
        func_00221A60((Matrix4 *)(&obj->x2B0->x40), (const Matrix4 *)(&matrix), (const Matrix4 *)(transform));
        func_00221ED0(transform, &obj->x2B0->x40);
    }
    func_0014A9A0(obj);
}
}

/* ---- merged from src/unsorted/00149BD0.c ---- */
namespace u149BD0 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Rider {
    char pad0[0x4ECC];
    char *x4ECC;
    char pad4ED0[0x6610 - 0x4ED0];
    int x6610;
    char pad6614[0x7A68 - 0x6614];
    unsigned char x7A68;
} Rider;

typedef struct Obj {
    char pad0[0x40];
    Vec x40;
    char pad50[0xE0 - 0x50];
    Vec xE0;
    char padF0[0x2B4 - 0xF0];
    Rider *x2B4;
    char pad2B8[0x2F0 - 0x2B8];
    float x2F0;
    char pad2F4[0x36E - 0x2F4];
    unsigned char x36E;
    unsigned char x36F;
} Obj;

extern int func_001E64C8(void *);
extern void func_00223348(Vec *, Vec *);
extern "C" Rider *D_0043EEC0[];

void func_00149BD0(Obj *obj)
{
    int flag = 0;

    if (obj->x2B4->x4ECC == (char *)obj->x2B4 + 0x5E90) {
        flag = 1;
    } else if (func_001E64C8((char *)obj->x2B4 + 0x6610)) {
        int state = obj->x2B4->x6610;
        if (state == 0x17 || state == 0x11 || state == 8 ||
            state == 6 || state == 0xB || state == 0x19) {
            flag = 1;
        }
    } else if (obj->x2B4->x7A68) {
        flag = obj->x2B4 == D_0043EEC0[0];
    }

    if (flag && obj->x36E == 0) {
        func_00223348(&obj->xE0, &obj->x40);
    }
    if ((obj->x36E = flag) == 0) {
        float v = obj->x2F0;
        float c;
        float m;
        m = u148F60::Abs(v);
        c = 0.01f;
        if (m > c) {
            obj->x36F = 1;
        }
    }
}
}

/* ---- merged from src/unsorted/00149CD0.c ---- */
namespace u149CD0 {
struct Obj {
    char pad0[0x80];
    float pos[4];
    char pad90[0x2BC - 0x90];
    int x2BC;
    char pad2C0[0x2F0 - 0x2C0];
    float x2F0;
    float x2F4;
    char pad2F8[0x36C - 0x2F8];
    unsigned char x36C;
};

extern void func_0014D798(struct Obj *, float *, float *);

void func_00149CD0(struct Obj *obj, float *dir)
{
    float a[4] __attribute__((aligned(16)));
    float b[4] __attribute__((aligned(16)));
    float lenA, lenB;
    float scale;

    func_0014D798(obj, a, b);
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(lenA) : "f"(a[0]), "f"(a[1]), "f"(a[2]));
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(lenB) : "f"(dir[0]), "f"(dir[1]), "f"(dir[2]));
    obj->x2F0 += obj->x2F4;
    if (obj->x2F0 > 1.0f) {
        obj->x2F0 = 1.0f;
    }
    scale = (1.0f - lenB / lenA) * obj->x2F0;
    if (obj->x2BC == 0 && obj->x36C != 0) {
        scale = -scale;
    }
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, 0x80(%3)"
                      : : "r"(obj->pos), "r"(dir), "f"(scale), "r"(obj) : "$2", "memory");
}
}

/* ---- merged from src/unsorted/00149DB8.c ---- */
namespace u149DB8 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x40];
    Vec x40;
    char pad50[0x30];
    Vec x80;
    char pad90[0x50];
    Vec xE0;
    char padF0[0x2BC - 0xF0];
    int x2BC;
    char pad2C0[0x30];
    float x2F0;
    float x2F4;
    float x2F8;
    char pad2FC[0x36C - 0x2FC];
    unsigned char x36C;
    char pad36D[2];
    unsigned char x36F;
} Obj;

extern void func_0014D798(Obj *obj, Vec *a, Vec *b);
extern void func_00223348(Vec *dst, Vec *src);

static inline float VecLength(Vec *v)
{
    float length;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(length) : "f"(v->x), "f"(v->y), "f"(v->z));
    return length;
}

void func_00149DB8(Obj *obj, Vec *v)
{
    Vec a;
    Vec b;
    float len;
    float vlen;
    float scale;

    func_0014D798(obj, &a, &b);
    len = VecLength(&a);
    vlen = VecLength(v);
    obj->x2F0 -= obj->x2F8;
    if (obj->x2F0 < 0.1f) {
        obj->x2F0 = 0.0f;
        obj->x36F = 0;
        func_00223348(&obj->xE0, &obj->x40);
        return;
    }
    scale = (1.0f - vlen / len) * obj->x2F0;
    if (obj->x2BC == 0 && obj->x36C != 0) {
        scale = -scale;
    }
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(obj->x80) : "r"(&obj->x80), "r"(v), "f"(scale) : "$2");
}
}

namespace u149EC0_new {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x40];
    Vec x40;
    Vec x50;
    char pad60[0xA0 - 0x60];
    Vec xA0;
    char padB0[0xE0 - 0xB0];
    Vec xE0;
    Vec xF0;
    char pad100[0x120 - 0x100];
    float x120, x124, x128;
    char pad12C[0x130 - 0x12C];
    Vec x130;
    char pad140[0x2B0 - 0x140];
    char *x2B0;
    char *x2B4;
    char pad2B8[0x35C - 0x2B8];
    int x35C;
    char pad360[0x371 - 0x360];
    unsigned char x371;
} Obj;

extern float func_001385A8(char *) __asm__("func_001385A8");
extern int func_00138560(char *) __asm__("func_00138560");
extern float func_00236088(float, float, float, float) __asm__("func_00236088");
extern void func_0014A580(char *, Vec *, Vec *) __asm__("func_0014A580");
extern void func_0014A3A0(char *, Vec *) __asm__("func_0014A3A0");
extern int func_0014A470(char *) __asm__("func_0014A470");

extern void func_00223348(char *, char *) __asm__("func_00223348");
extern void func_00239858(int, Vec *, Vec *) __asm__("func_00239858");
extern char *D_0043EEC0[] __asm__("D_0043EEC0");

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VSUB(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b))
#define VADD(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b))
#define VMUL(dst, a, m) \
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 16(%2)\n\tlqc2 $vf14, 32(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(m))

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

static inline void Normalize(Vec *out, Vec *src) {
    float x, y, z, len, inv;
    x = src->x; y = src->y; z = src->z;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
        : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
}

static inline float Dot(Vec *normal, Vec *body)
{
    float dot;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
        : "=f"(dot)
        : "f"(body->x), "f"(normal->x), "f"(body->y), "f"(normal->y), "f"(body->z), "f"(normal->z));
    return dot;
}

static inline char *InitFrame(char *p, Vec *dir)
{
    func_002226C8((Matrix4 *)(p), (const Vector4 *)(dir));
    return p;
}

void func_00149EC0(char *, Vec *) __asm__("func_00149EC0");
void func_00149EC0(char *obj, Vec *dir) {
    Vec tmp;
    Vec in;
    char *parent;
    char *pos;
    Vec *frame;
    float rate, r;

    if (func_001385A8(*(char **)(obj + 0x2B4) + 0x40C0) < 0.1f) {
        char *p = *(char **)(obj + 0x2B4);
        void *cur = *(void **)(p + 0x4ECC);
        if (cur == p + 0x62B4) {
            float *base = (float *)(p + 0x7AA0);
            float *v = (float *)(*(char **)(p + 0x792C) + 0x50);
            float dot = Dot((Vec *)base, (Vec *)v);
            __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                : "=m"(tmp) : "r"(base), "f"(dot) : "$2");
            VSUB(*dir, *(char **)(p + 0x792C) + 0x50, &tmp);
            Normalize(dir, dir);
        } else {
            COPY_VECTOR(*dir, *(char **)(p + 0x792C) + 0x50);
        }
    } else {
        char *p = *(char **)(obj + 0x2B4);
        char *other = *(char **)(p + 0x7BA4);
        /* Keep these calls separate: SN merges their tails after register allocation. */
        if (other) {
            Normalize(dir, (Vec *)(other + 0x10));
        } else {
            Normalize(dir, (Vec *)(p + 0x4180));
        }
    }

    frame = &((Obj *)obj)->xF0;
    rate = 0.1f;
    {
    char *p = *(char **)(obj + 0x2B4);
    if (p + 0x5E40 != *(char **)(p + 0x4ECC)) {
        float x, y, z, maxXY, maxXYZ, result;
        VSUB(tmp, frame, dir);
        x = Abs(tmp.x);
        y = Abs(tmp.y);
        __asm__ volatile ("max.s %0,%1,%2" : "=&f"(maxXY) : "f"(x), "f"(y));
        z = Abs(tmp.z);
        __asm__ volatile ("max.s %0,%1,%2" : "=&f"(maxXYZ) : "f"(z), "f"(maxXY));
        __asm__ volatile ("min.s %0,%1,%2" : "=&f"(result) : "f"(rate), "f"(maxXYZ));
        rate = result;
        __asm__ volatile ("max.s %0,%1,%2" : "=&f"(result) : "f"(0.01f), "f"(rate));
        rate = result;
    }
    }
    {
        float check = dir->z > 0.9f ? 1.0f : 0.0f;
        if (Abs(check) != 0.0f) {
            rate *= 1.0f - __builtin_fabsf(dir->z);
        }
    }
    r = func_00236088(((Obj *)obj)->x120, rate, 0.1f, 0.001f);
    ((Obj *)obj)->x120 = r;
    ((Obj *)obj)->x128 = r;
    ((Obj *)obj)->x124 = r;
    ((Obj *)obj)->xF0.x = func_00236088(((Obj *)obj)->xF0.x, dir->x, r, 0.0001f);
    ((Obj *)obj)->xF0.y = func_00236088(((Obj *)obj)->xF0.y, dir->y, ((Obj *)obj)->x124, 0.0001f);
    ((Obj *)obj)->xF0.z = func_00236088(((Obj *)obj)->xF0.z, dir->z, ((Obj *)obj)->x128, 0.0001f);
    Normalize(frame, frame);

    parent = *(char **)(obj + 0x2B4);
    if (*(unsigned char *)(parent + 0x7A68) && parent == D_0043EEC0[0]) {
        func_0014A580(obj, dir, frame);
        pos = InitFrame((char *)&((Obj *)obj)->x40, dir);
    } else if (((Obj *)obj)->x371) {
        char *p;
        func_0014A3A0(obj, frame);
        p = ((Obj *)obj)->x2B4;
        pos = obj + 0x40;
        if (*(float *)(*(char **)(p + 0x792C) + 0x68) > 0.2f &&
            *(float *)(((Obj *)obj)->x2B0 + 0x68) > 0.4f &&
            !*(unsigned char *)(p + 0x7B9F)) {
            *(unsigned char *)(obj + 0x371) = 0;
            COPY_VECTOR(*(Vec *)(obj + 0xF0), obj + 0x50);
        }
    } else {
        pos = InitFrame(obj + 0x40, frame);
        if (!(func_00138560(*(char **)(obj + 0x2B4) + 0x40C0) & 0xFF)) {
            float check;
            char *p = *(char **)(obj + 0x2B4);
            check = *(float *)(*(char **)(p + 0x792C) + 0x68) < 0.1f ? 1.0f : 0.0f;
            if (Abs(check) != 0.0f && *(unsigned char *)(p + 0x7B9F)) {
                *(unsigned char *)(obj + 0x371) = 1;
                func_00223348(obj + 0xE0, pos);
            }
        }
    }
    func_0014A470(obj);
    VADD(tmp, obj + 0xA0, obj + 0x130);
    VMUL(*dir, &tmp, pos);
    *(u128 *)&in = *(u128 *)(obj + 0xF0);
    func_00239858(*(int *)(obj + 0x35C), &in, dir);
}

}
#undef COPY_VECTOR
#undef VSUB
#undef VADD
#undef VMUL


/* ---- merged from src/unsorted/0014A3A0.c ---- */
float D_0029C5B8 = 0.1f;
float D_0029C5BC = 0.1f;
namespace u14A3A0 {
typedef struct { float v[4]; } Vec;
typedef struct { char pad[0x792C]; char *x792C; } Parent;
typedef struct { char pad[0x2B4]; Parent *x2B4; } Obj;
extern void func_00222F60(Vec *, void *, void *);
extern void func_00223578(Vec *, Vec *, float);
extern void func_00223210(Vec *, Vec *, Vec *);
extern void func_002230B0(Vec *, Vec *);
extern void func_00223248(Vec *, Vec *);
extern void func_002232C8(void *, Vec *);
void func_0014A3A0(Obj *obj, void *arg) {
    Vec a, b;
    Vec *v;
    func_00222F60(&a, (char *)obj + 0x50, arg);
    v = (Vec *)((char *)obj + 0xE0);
    func_00223578(&a, &a, D_0029C5B8);
    func_00223210(v, v, &a);
    func_002230B0(v, v);
    func_00223248(&b, v);
    func_00222F60(&a, &b, obj->x2B4->x792C + 0x60);
    func_00223578(&a, &a, D_0029C5BC);
    func_00223210(v, v, &a);
    func_002230B0(v, v);
    func_002232C8((char *)obj + 0x40, v);
}
}

/* ---- merged from src/unsorted/0014A470.c ---- */
namespace u14A470 {
struct State { char pad0[0x7A69]; unsigned char x7A69; };
struct Obj {
    char pad0[0xA4];
    float xA4;
    char padA8[0x134 - 0xA8];
    float x134;
    char pad138[0x2B4 - 0x138];
    struct State *x2B4;
    int x2B8;
    int x2BC;
    char pad2C0[0x2FC - 0x2C0];
    float x2FC;
    float x300;
};

extern float func_001385A8(void *);

void func_0014A470(struct Obj *obj)
{
    float target;
    float step;

    if (obj->x2B4->x7A69) {
        target = obj->x2FC * func_001385A8((char *)obj->x2B4 + 0x40C0);
        if (obj->x2BC == 1) target *= 3.0f;
        if (target < obj->x134) {
            obj->x134 += obj->xA4 * obj->x300;
            if (!(obj->x134 < target)) return;
        } else if (obj->x134 < target) {
            obj->x134 -= obj->xA4 * obj->x300;
            if (!(target < obj->x134)) return;
        } else {
            return;
        }
    } else {
        target = 0.0f;
        if (obj->x134 == target) return;
        step = obj->xA4 * obj->x300;
        obj->x134 -= step;
        if (step > target) {
            if (!(obj->x134 < target)) return;
        } else {
            if (!(obj->x134 > target)) return;
        }
    }
    obj->x134 = target;
}
}

/* ---- merged from src/unsorted/0014A580.c ---- */
namespace u14A580 {
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Matrix { Vec row[4]; } Matrix;
typedef struct Flags { char pad0[0x106]; unsigned char x106,x107; } Flags;
typedef struct Rider { char pad0[0x78D0]; Flags *x78D0; } Rider;
typedef struct Obj { char pad0[0xA4]; float xA4,xA8; char padAC[0x208]; Rider *x2B4; int x2B8,x2BC; char pad2C0[0xA4]; int x364; } Obj;

extern void func_00221F50(Matrix *,Vec *);

extern int func_00238F60(void);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void Normalize(Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(len) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
void func_0014A580(Obj *obj,Vec *out,Vec *input) {
 Matrix basis;
 Vec angles;
 Matrix rotation,result;
 Flags *flags=obj->x2B4->x78D0;
 float angle,limit,zero,step,t;
 if(flags) {
 if(flags->x106) {
 obj->x364+=func_00238F60();
 func_002226C8((Matrix4 *)(&basis), (const Vector4 *)(input));
 angle=(float)obj->x364*0.017453294f;
 if(obj->x364>=181) angle=3.1415927f;
 if(flags->x107) angle=-angle;
 zero=0.0f;
 angles.x=zero; angles.y=zero; angles.z=angle;
 func_00221F50(&rotation,&angles);
 func_00221A60((Matrix4 *)(&result), (const Matrix4 *)(&basis), (const Matrix4 *)(&rotation));
 COPY_VECTOR(*out,&result.row[1]);
 limit=zero;
 __asm__ volatile ("Lsw19_1:" : : "f"(limit));
 t=fabsf(angle);
 __asm__ volatile ("Lsw19_2:" : : "f"(t));
 if(t<1.5707964f) { __asm__ volatile ("Lsw19_3:" : "+f"(angle)); limit=1.0f-fabsf(angle)*0.63661975f; }
 if(limit<fabsf(out->z)) { if(out->z>zero) out->z=limit; else out->z=-limit; }
 Normalize(out);
 if(obj->xA8>0.0f) {
 step=0.03f;
 if(obj->x2BC==1) step=0.015f; else if(obj->x2BC==3) step=0.05f;
 obj->xA8-=step;
 }
 if(obj->xA4 < -7.0f) {
 float step2=0.05f;
 if(obj->x2BC==1) step2=0.025f; else if(obj->x2BC==3) step2=0.07f;
 obj->xA4+=step2;
 }
 } else { COPY_VECTOR(*out,input); obj->x364=0; }
 } else { COPY_VECTOR(*out,input); }
}
}

/* ---- merged from src/unsorted/0014A830.c ---- */
namespace u14A830 {
struct Obj { char pad[0x80]; float pos[4]; char pad90[0x22c]; int x2bc; char pad2c0[0x98]; int x358; };
extern "C" int D_0043F348[];
extern float func_00236088(float,float,float,float);
extern void func_0014DAA8(struct Obj *, int);
#define SUB(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a), "r"(b) : "memory")
void func_0014A830(struct Obj *p, float *target) {
 float diff[4] __attribute__((aligned(16)));
 float distance, factor, rate, threshold, x, y, z;
 float *pos = p->pos;
 SUB(diff,target,pos);
 x=diff[0]; y=diff[1];
 __asm__ volatile ("mula.s %1,%1\n\tmadd.s %0,%2,%2\n\tsqrt.s %0,%0" : "=&f"(distance) : "f"(x), "f"(y));
 factor = (75.0f-distance)*0.004f;
 rate = 0.01f;
 rate += factor*factor;
 p->pos[0]=func_00236088(p->pos[0],target[0],rate,0.01f);
 p->pos[1]=func_00236088(p->pos[1],target[1],rate,0.01f);
 p->pos[2]=func_00236088(p->pos[2],target[2],rate,0.01f);
 SUB(diff,target,pos);
 x=diff[0]; y=diff[1]; z=diff[2];
 __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(distance) : "f"(x),"f"(y),"f"(z));
 threshold = 0.1f;
 if (!p->x2bc) threshold=2.0f;
 if (distance < threshold || (unsigned)(D_0043F348[0]-2)<3) {
  p->x358=0;
  func_0014DAA8(p,p->x2bc);
 }
}
}

/* ---- merged from src/unsorted/0014A9A0.c ---- */
namespace u14A9A0 {
typedef struct Target { unsigned long flags; char pad8[8]; float pos[4]; } Target;
typedef struct Child { char pad[0x792c]; Target *x792c; } Child;
typedef struct Obj { char pad0[0x80]; float pos[4]; char pad90[0x224]; Child *x2b4; char pad2b8[0x54]; float x30c,x310; char pad314[0x30]; int x344; char pad348[0x20]; int x368; char pad36c[9]; unsigned char x375; } Obj;
extern void func_0014AB80(Obj *);
extern void func_0014AF20(Obj *);
static __inline__ void normalize(float *dir) { float x=dir[0],y=dir[1],z=dir[2],len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(len):"f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(inv):"f"(1.0f),"f"(len));
 dir[0]=x*inv; dir[1]=y*inv; dir[2]=z*inv;
}
void func_0014A9A0(Obj *obj) {
 float vec[4] __attribute__((aligned(16))); float x,y,z,len,limit,inv,min; unsigned long bit; unsigned long flags; Target *target; Child *child;
 if(!obj->x344 || !obj->x2b4) return;
 flags=obj->x2b4->x792c->flags; obj->x2b4->x792c->flags=flags&0xfdffffffffffffffUL; bit=(flags>>57)&1;
 if(obj->x368>=11) func_0014AB80(obj); else func_0014AF20(obj);
 obj->x2b4->x792c->flags=(obj->x2b4->x792c->flags&0xfdffffffffffffffUL)|(bit<<57);
 if(!obj->x375 && obj->x368<240) {
 child=obj->x2b4;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(vec):"r"(obj->pos),"r"(child->x792c->pos):"memory");
 x=vec[0]; y=vec[1]; z=vec[2];
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0":"=&f"(len):"f"(x),"f"(y),"f"(z));
 limit=obj->x30c+obj->x310;
 if(limit<len) {
 normalize(vec);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(obj->pos):"r"(child->x792c->pos),"r"(vec),"f"(limit):"$2","memory");
 obj->x30c=limit;
 len=obj->x310+0.002f;
 __asm__ volatile("min.s %0,%1,%2":"=&f"(min):"f"(0.2f),"f"(len)); obj->x310=min;
 } else { obj->x30c=len; obj->x310=0.002f; }
 }
}
}

/* ---- merged from src/unsorted/0014AB80.c ---- */
namespace u14AB80 {
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

typedef struct Hit {
    unsigned short x0;
    char pad2[6];
    float x8;
    char padC[0x10];
    float x1C;
    Vec x20;
} Hit __attribute__((aligned(16)));

typedef struct Contact { unsigned short found; char pad2[14]; Vec pos, normal; } Contact;
typedef struct Body { char pad0[0x10]; Vec pos; } Body;
typedef struct Rider { char pad0[0x792C]; Body *x792C; } Rider;

typedef struct Obj {
    char pad0[0x80];
    Vec x80;
    char pad90[0xC0];
    Contact x150;
    char pad180[0x134];
    Rider *x2B4;
    char pad2B8[4];
    int x2BC;
    char pad2C0[0x9C];
    int x35C;
    char pad360[8];
    int x368;
    char pad36C[6];
    unsigned char x372;
    unsigned char x373;
} Obj;

extern "C" int D_0029C5B4;
extern QueryFlags *func_00210940(QueryFlags *);
extern int func_00212EA8(Vec *, Vec *, Contact *, int);

extern Hit *func_00211780(Hit *, int, Hit *);
extern void func_0014D208(Obj *, int);
extern void func_00148BE8(Obj *, int);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Init(QueryFlags *f) { f->x0 = -1; f->x4.x4 = 0xffff; f->x4.bits.a = 0; f->x4.bits.b = 0; f->x4.bits.c = 0; }
static inline void Enable(QueryFlags *f, int value) { f->x0 = 0; f->x4.bits.b = value; }

static __inline__ void AddScaled(Vec *out, Vec *a, Vec *b, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}

static __inline__ void Sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static __inline__ float Normalize(Vec *v)
{
    float length, scale;
    /* the unused 4th input (z again) is needed for the retail register allocation */
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %0, %4, %4\n\tsqrt.s %0, %0\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %1\n\tnop\n\tdiv.s %1, %1, %0"
                      : "=&f"(length), "=&f"(scale) : "f"(v->x), "f"(v->y), "f"(v->z), "f"(v->z) : "$2");
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;
    return length;
}

/* called from both branches; gcc cross-jumps the two copies into one */
static inline void Hold(Obj *obj)
{
    obj->x373 = 1;
    func_0014D208(obj, 1);
    func_00148BE8(obj, obj->x35C);
}

/* uses VU0/FPU inline asm */
void func_0014AB80(Obj *obj)
{
    QueryFlags flags, *old;
    Hit hits[8];
    QueryFlags unused;
    Vec target, dir;
    Vec *goal;
    Hit *hit;
    int n, i;
    float length;

    Init(&flags);
    Enable(&flags, 1);
    old = func_00210940(&flags);
    if (!(obj->x372 = func_00212EA8(&obj->x2B4->x792C->pos, &obj->x80, &obj->x150, 1) != 0) && obj->x150.found) {
        obj->x368 = 0;
        if (D_0029C5B4 && obj->x2BC) {
            Hold(obj);
        } else {
            Vec *pos = &obj->x80;
            COPY_VECTOR(obj->x80, &obj->x150.pos);
            AddScaled(&obj->x80, pos, &obj->x150.normal, 1.0f);
        }
    } else {
        Init(&unused);
        Enable(&unused, 1);
        hit = 0;
        n = QuerySpatialHitsCountLast((Vector4 *)(&obj->x80), (Vector4 *)(&obj->x80), (SpatialHit *)(hits), 1.0f, 1.0f, 1.0f, 8);
        if (n) {
            obj->x368 = 0;
            if (D_0029C5B4 && obj->x2BC) {
                Hold(obj);
            } else if (n == 1) {
                hit = func_00211780(hits, 1, 0);
                AddScaled(&obj->x80, &obj->x80, &hit->x20, hit->x8);
            } else {
                COPY_VECTOR(target, &obj->x80);
                goal = &target;
                for (i = 0; i < n; i++) {
                    hit = func_00211780(hits, n, hit);
                    if (hit && hit->x0)
                        AddScaled(&obj->x80, &obj->x80, &hit->x20, hit->x8);
                }
                n = func_00214A18((Vector4 *)(&obj->x80), (Vector4 *)(goal), (SpatialHit *)(hits), 8, 1.0f, 1.0f, 1.0f);
                if (n >= 2) {
                    hit = func_00211780(hits, n, 0);
                    if (hit && hit->x0 && 0.0f <= hit->x1C) {
                        Vec *step = &dir;
                        Sub(&dir, goal, &obj->x80);
                        length = Normalize(&dir);
                        AddScaled(&obj->x80, &obj->x80, step, length * hit->x1C);
                    }
                }
            }
        }
    }
    func_00210940(old);
}
}

/* ---- merged from src/unsorted/0014AF20.cpp ---- */
/* One translation unit: func_0014AF20 and func_0014DD00 read D_0029C5B4 through $gp outside a
   delay slot, so the variable (and D_0029C5B0 before it) is defined here. The functions in
   between were promoted as separate files; each keeps its own types in a namespace. */

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

typedef struct Hit { char pad0[0x1C]; float x1C; char pad20[0x10]; } Hit;
typedef struct Body { char pad0[0x10]; Vec pos; } Body;
typedef struct Rider { char pad0[0x792C]; Body *x792C; } Rider;

typedef struct Obj {
    char pad0[0x80];
    Vec pos;
    char pad90[0x224];
    Rider *x2B4;
    char pad2B8[4];
    int x2BC;
    char pad2C0[0x9C];
    int x35C;
    char pad360[8];
    int x368;
    char pad36C[7];
    unsigned char x373;
} Obj;

extern "C" float fabsf(float);
volatile int D_0029DD60 __attribute__((section(".sbss"))) = 0;
int pad_D_0029DD64 __attribute__((section(".sbss"))) = 0;

extern QueryFlags *func_00210940(QueryFlags *);
extern Hit *func_00211780(Hit *, int, Hit *);
extern void func_0014D208(Obj *, int);
extern void func_00148BE8(Obj *, int);
extern void func_0014B0E8(Obj *, Rider *);

static inline void Init(QueryFlags *f) { f->x4.x4 = 0xffff; f->x4.bits.a = 0; f->x4.bits.b = 0; f->x4.bits.c = 0; }
static inline void Enable(QueryFlags *f, int value) { f->x0 = 0; f->x4.bits.b = value; }

static inline void Sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Scale(Vec *out, Vec *v, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v), "f"(scale) : "$2");
}

static inline void Add(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline float Length(Vec *v)
{
    float r;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(r) : "f"(v->x), "f"(v->y), "f"(v->z));
    return r;
}

/* pull the object back toward its rider when the path between them is blocked */
void func_0014AF20(Obj *obj)
{
    QueryFlags flags, *old;
    Hit hits[8] __attribute__((aligned(16)));
    Vec delta;
    Hit *hit;
    int count;
    Rider *rider;

    Init(&flags);
    Enable(&flags, 1);
    old = func_00210940(&flags);
    count = QuerySpatialHitsCountLast((Vector4 *)(&obj->x2B4->x792C->pos), (Vector4 *)(&obj->pos), (SpatialHit *)(hits), 1.0f, 1.0f, 1.0f, 8);
    if (count) {
        obj->x368 = 0;
        if (D_0029C5B4 && obj->x2BC) {
            obj->x373 = 1;
            func_0014D208(obj, 1);
            func_00148BE8(obj, obj->x35C);
        } else {
            hit = func_00211780(hits, count, 0);
            if (hit && hit->x1C > 0.0f) {
                rider = obj->x2B4;
                Sub(&delta, &obj->pos, &rider->x792C->pos);
                Scale(&delta, &delta, hit->x1C);
                if (Length(&delta) < 3.0f)
                    func_0014B0E8(obj, rider);
                else
                    Add(&obj->pos, &rider->x792C->pos, &delta);
            }
        }
    }
    func_00210940(old);
}


/* ---- 0014B0E8.c ---- */
namespace u14B0E8 {
typedef struct Vec { float x, y, z, w; } Vec;
struct State { char pad0[0x792C]; char *x792C; };
struct Obj {
    char pad0[0x80];
    Vec x80;
    char pad90[0x140 - 0x90];
    float x140[4];
    char pad150[0x2B0 - 0x150];
    char *x2B0;
    struct State *x2B4;
    char pad2B8[0x375 - 0x2B8];
    unsigned char x375;
};

extern "C" int D_0029C5B0;
extern void func_00221ED0(void *, void *);

static inline void ApplyMatrix(float *out, char *m, float *v)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)
	"
                      "lqc2 $vf12, 0x0(%1)
	"
                      "lqc2 $vf13, 0x10(%1)
	"
                      "lqc2 $vf14, 0x20(%1)
	"
                      "vmulax.xyz ACC, $vf12, $vf11x
	"
                      "vmadday.xyz ACC, $vf13, $vf11y
	"
                      "vmaddz.xyz $vf11, $vf14, $vf11z
	"
                      "sqc2 $vf11, 0x0(%2)"
                      : : "r"(v), "r"(m), "r"(out) : "memory");
}

void func_0014B0E8(struct Obj *obj)
{
    float rotated[4];
    float local[4];
    float *v;
    struct State *state;

    if (!obj->x2B0) return;
    state = obj->x2B4;
    if (!state) return;
    if (D_0029C5B0) {
        v = local;
        v[0] = 0.0f;
        v[1] = -1.0f;
        v[2] = 1.6f;
        ApplyMatrix(rotated, state->x792C + 0x40, v);
    } else {
        ApplyMatrix(rotated, state->x792C + 0x40, obj->x140);
    }
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)
	"
                      "lqc2 $vf12, 0x0(%1)
	"
                      "vadd.xyz $vf11, $vf11, $vf12
	"
                      "sqc2 $vf11, %2"
                      : : "r"(obj->x2B4->x792C + 0x10), "r"(rotated), "m"(obj->x80) : "memory");
    func_00221ED0(obj->x2B0 + 0x40, obj->x2B4->x792C + 0x40);
    obj->x375 = 1;
}
}

/* ---- 0014B1B8.c ---- */
namespace u14B1B8 {
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Mtx {
    Vec row[4];
} Mtx;

typedef struct Body {
    char pad0[0x10];
    Vec pos;
    char pad20[0x40 - 0x20];
    Mtx mtx;
} Body;

typedef struct Obj {
    char pad0[0xA0];
    Vec xA0;
    char padB0[0x2B0 - 0xB0];
    Body *body;
} Obj;

extern void func_00221F50(Mtx *out, Vec *rot);

extern void func_00221ED0(Mtx *dst, Mtx *src);

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void ApplyMatrix(Vec *out, Mtx *m, Vec *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(in), "r"(m));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

void func_0014B1B8(Obj *obj)
{
    Mtx m0;
    Mtx m1;
    Vec rot;
    Vec v;
    Vec d;
    Vec *r;

    rot.x = 0.0f;
    rot.y = 0.0f;
    r = &rot;
    r->z = 3.1415927f;
    func_00221F50(&m1, r);
    func_00221A60((Matrix4 *)(&m0), (const Matrix4 *)(&m1), (const Matrix4 *)(&obj->body->mtx));
    func_00221ED0(&obj->body->mtx, &m0);
    VecScale(&v, &obj->xA0, 2.0f);
    v.z = 0.0f;
    ApplyMatrix(&d, &obj->body->mtx, &v);
    VecAdd(&obj->body->pos, &obj->body->pos, &d);
}
}

/* ---- 0014B298.c ---- */
namespace u14B298 {
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Mtx {
    Vec row[4];
} Mtx;

typedef struct Body {
    char pad0[0x10];
    Vec pos;
    char pad20[0x40 - 0x20];
    Mtx mtx;
} Body;

typedef struct Obj {
    char pad0[0x98];
    float x98;
    char pad9C[0x2B0 - 0x9C];
    Body *body;
    char pad2B4[0x2E4 - 0x2B4];
    float x2E4;
    char pad2E8[0x360 - 0x2E8];
    int x360;
} Obj;

extern void func_00221F50(Mtx *out, Vec *rot);

extern void func_00221ED0(Mtx *dst, Mtx *src);
extern void func_0020ED30(int, int, int, int, float, float, float, float, float);

static inline void ApplyMatrix(Vec *out, Mtx *m, Vec *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(in), "r"(m));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

void func_0014B298(Obj *obj, int arg1)
{
    Mtx m0;
    Mtx m1;
    Vec rot;
    Vec v;
    Vec d;

    *(u128 *)&v = 0;
    v.y = -6.5f;
    v.z = 1.5f;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = obj->x98 + obj->x2E4 * 0.017453292f;
    if (obj->x360 == 3) {
        v.y = -8.0f;
        v.z = 2.0f;
    } else if (obj->x360 == 4) {
        v.y = -5.0f;
    } else if (obj->x360 == 5) {
        rot.x = 1.5707964f;
        v.y = -10.0f;
        rot.z = 0.0f;
        v.z = -10.0f;
    }
    func_00221F50(&m1, &rot);
    func_00221A60((Matrix4 *)(&m0), (const Matrix4 *)(&m1), (const Matrix4 *)(&obj->body->mtx));
    func_00221ED0(&obj->body->mtx, &m0);
    ApplyMatrix(&d, &obj->body->mtx, &v);
    VecAdd(&obj->body->pos, &obj->body->pos, &d);
    func_0020ED30(arg1, 0x80, 0x80, 0x80, obj->x360 == 3 ? 1.0f : 0.0f, 250.0f, 500.0f, 220.0f, 1000.0f);
}
}

/* ---- 0014B450.c ---- */
namespace u14B450 {

void func_0014B450(char *obj) {
 if (*(int *)(obj + 0x348) == 0) {
  float *dir = (float *)(obj + 0xc0); float x,y,z,len,inv;
  char *pos = *(char **)(*(char **)(obj + 0x2b4) + 0x792c) + 0x10;
  __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0xc0(%2)" : : "r"(pos), "r"(obj+0x80), "r"(obj) : "memory");
  x=dir[0]; y=dir[1]; z=dir[2];
  __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
  __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
  dir[0]=x*inv; dir[1]=y*inv; dir[2]=z*inv;
  func_002226C8((Matrix4 *)(obj), (const Vector4 *)(dir));
 }
}
}

/* ---- 0014B4E8.c ---- */
namespace u14B4E8 {

void func_0014B4E8(char *obj, float *pos) {
 float *dir = (float *)(obj + 0xc0); float x,y,z,len,inv;
 __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0xc0(%2)" : : "r"(pos), "r"(obj+0x80), "r"(obj) : "memory");
 x=dir[0]; y=dir[1]; z=dir[2];
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
 dir[0]=x*inv; dir[1]=y*inv; dir[2]=z*inv;
 func_002226C8((Matrix4 *)(obj), (const Vector4 *)(dir));
}
}
INCLUDE_ASM("asm/nonmatchings/text", func_0014B568);

/* ---- 0014B570.c ---- */
namespace u14B570 {
typedef struct { char bytes[0x380]; } Element;
typedef struct { Element elements[4]; int count; } Obj;
extern void func_0023DDE0(int, int);
extern void func_0014D740(Element *);
void func_0014B570(Obj *obj) {
    int i;
    func_0023DDE0(0, 0);
    for (i = 0; i < obj->count; i++) {
        func_0014D740(&obj->elements[i]);
    }
}
}
namespace u14B5E0 {
typedef float Vector[4] __attribute__((aligned(16)));
typedef float Matrix[16] __attribute__((aligned(16)));
typedef struct Node { char pad0[0x10]; Vector x10; char pad20[0x70]; Vector x90; } Node;
typedef struct Other { char pad0[0x3514]; Node *x3514; char pad3518[0x4414]; Node *x792C; } Other;
typedef struct Obj { char pad0[0x10]; Vector x10; char pad20[0x60]; Vector x80; char pad90[0x30]; Vector xC0; char padD0[0x1E0]; int x2B0; Other *x2B4; int x2B8,x2BC; } Obj;
extern float fabsf(float);
#if defined(__mips__)
extern float func_00211980(Vector,Vector,int,float);
#endif

extern void func_0014B828(Obj *,Vector,Matrix);
extern int func_00238F30(void);
extern void func_00209248(Node *,Vector,void *,float);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define SUB(dst,a,b) __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*(Vector *)(dst)) : "r"(a), "r"(b) : "memory")
#define ADD(dst,a,b) __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*(Vector *)(dst)) : "r"(a), "r"(b) : "memory")
static inline void ScaleVector(Vector *out, Vector *in, float factor) {
 __asm__ volatile("lqc2 $vf11, 0(%1)
	mfc1 $2,%2
	qmtc2.ni $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0" : "=m"(*out) : "r"(in),"f"(factor) : "$2");
}
static inline Vector *Advance(Vector *delta, Vector *hit, Obj *obj, float factor) {
 Vector *pos; float height,limit;
 pos=&obj->x80;
 height=CallGroundProbe(*pos,*hit,0,7.0f);
 limit=5.0f;
 if(obj->x2BC==0) limit=10.0f;
 if(height<limit && __builtin_fabsf(height)>1e-10f) { height=limit-height; (*delta)[2]+=height; }
 ADD(obj->x80,*pos,*delta);
 return pos;
}
static inline float Factor(int state) {
 if(state==0) return 0.2f;
 if(state==1) return 0.67f;
 if(state==3) return 0.95f;
 return 0.8f;
}
void func_0014B5E0(Obj *obj) {
 Vector old,delta,hit; Matrix matrix;
 Node *node; Vector *pos,*np; float factor,height,limit,x,y,z,len,inv;
 if(!obj->x2B0) return;
 if(!obj->x2B4) return;
 node=obj->x2B4->x3514;
 if(node) {
  COPY_VECTOR(old,obj->x10);
  SUB(delta,node->x10,node->x90);
  if(obj->x2BC==0) ScaleVector(&delta,&delta,0.2f);
  else if(obj->x2BC==1) ScaleVector(&delta,&delta,0.67f);
  else if(obj->x2BC==3) ScaleVector(&delta,&delta,0.95f);
  else ScaleVector(&delta,&delta,0.8f);
  pos=&obj->x80;
  height=CallGroundProbe(*pos,hit,0,7.0f);
  limit=5.0f;
  if(obj->x2BC==0) limit=10.0f;
  if(height<limit && __builtin_fabsf(height)>1e-10f) { height=limit-height; delta[2]+=height; }
  ADD(obj->x80,*pos,delta);
  if(obj->x2BC==0) {
   Vector *dir=&obj->xC0;
   SUB(*dir,obj->x2B4->x792C->x10,*pos);
   y=(*dir)[1];z=(*dir)[2];x=obj->xC0[0];
   __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
   __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv):"f"(1.0f),"f"(len));
   obj->xC0[0]=x*inv;(*dir)[1]=y*inv;(*dir)[2]=z*inv;
   func_002226C8((Matrix4 *)(matrix), (const Vector4 *)(*dir));
   func_0014B828(obj,old,matrix);
  }
  np=&node->x10;
  func_00209248(node,*np,(char *)node+0x40,1.0f/(float)func_00238F30());
 }
}

#undef COPY_VECTOR
#undef SUB
#undef ADD
}

namespace u14B828 {
typedef float Vec[4];
extern void func_00222F60(void *,void *,void *);
extern void func_002230B0(void *,void *);
extern void func_00223210(void *,void *,void *);
extern void func_00223248(void *,void *);
extern void func_002232C8(void *,void *);
extern void func_00223578(void *,void *,float);
extern float func_00235EA8(float,float,float,float);
extern float func_00138580(void *);
#define F(p,o) (*(float *)((char *)(p)+(o)))
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(char **)((char *)(p)+(o)))
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VADD(d,a,b,op) __asm__ volatile("lqc2 $vf11, 0(%2)\n\tlqc2 $vf12, 0(%3)\n\t" op ".xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(d),"r"(a),"r"(b) : "memory")
#define CROSS(d,a,b) __asm__ volatile("lqc2 $vf11,0(%3)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(d),"r"(b),"r"(a) : "memory")
#define CROSS_UP(d,a,b) __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(b),"r"(a) : "memory")
#define VADD_TARGET(d,a,b,op) __asm__ volatile("lqc2 $vf11, 0(%2)\n\tlqc2 $vf12, 0(%1)\n\t" op ".xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(b),"r"(a) : "memory")
void func_0014B828(char *p,void *arg1,void *matrix) {
    Vec rotation, offset, delta, pos, target, transformed, axis, final;
    float time,speed,result,x,y,z,sum,inverse,dot;
    char *q,*up,*right,*forward;
    COPY_VECTOR(pos,p+0x80);
    COPY_VECTOR(target,P(P(p,0x2B4),0x3514)+0x10);
    offset[0]=0;offset[1]=0;offset[2]=F(p,0x2D0);
    __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%3)\n\tlqc2 $vf13,16(%3)\n\tlqc2 $vf14,32(%3)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(transformed) : "r"(transformed),"r"(offset),"r"(matrix) : "memory");
    VADD_TARGET(target,target,transformed,"vadd");
    VADD(delta,target,pos,"vsub");
    speed=0.1f;
    func_00222F60(rotation,arg1,delta);
    if(I(p,0x358))speed=0.01f;
    time=func_00138580(P(p,0x2B4)+0x40C0);
    if(29.333334f>time) {speed=speed*time*0.03409091f;if(speed<0.01f)speed=0.01f;}
    result=func_00235EA8(F(p,0x2C4),speed,0.03f,0.001f);
    F(p,0x2C4)=result;
    if(!I(p,0x354))func_00223578(rotation,rotation,result);
    q=p+0xE0;
    func_00223210(q,q,rotation);
    up=p+0x60;
    func_002230B0(q,q);func_002232C8(p,q);
    COPY_VECTOR(*(Vec *)(p+0x60),p+0x20);
    if(F(p,0x68)<0.0f) {
        float scale=-1.0f;
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)up) : "r"(up),"f"(scale) : "$2","memory");
    }
    right=p+0x40;
    CROSS(right,p+0x10,up);
    forward=p+0x50;
    CROSS(forward,up,right);
    F(p,0x48)=0.0f;
    x=F(p,0x40);y=F(right,4);z=F(right,8);
    __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %0,%1,%1" : "=&f"(sum) : "f"(z),"f"(x),"f"(y));
    speed=1.0f;
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inverse) : "f"(speed),"f"(sum));
    *(volatile float *)(p+0x40)=x*inverse;*(volatile float *)(right+8)=z*inverse;*(volatile float *)(right+4)=y*inverse;
    CROSS_UP(up,right,forward);CROSS(right,forward,up);CROSS(forward,up,right);
    func_00223248(axis,q);
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(F(p,0x60)),"f"(axis[0]),"f"(F(up,4)),"f"(axis[1]),"f"(F(up,8)),"f"(axis[2]));
    speed-=dot;
    if(1e-10f<speed) {func_00222F60(final,axis,up);func_00223210(q,q,final);}
    func_002230B0(q,q);func_002232C8(p,q);
}

}
#undef F
#undef I
#undef P
#undef COPY_VECTOR
#undef VADD
#undef CROSS
/* func_0014BB70: uses the allowed vector-copy, VU0 and EE FPU inline asm, plus one zero-byte
   asm label (cf. src/unsorted/00111D98.cpp, 0010E9B0.c) right after abs.s. It is a scheduling
   barrier only (no bytes): it keeps the 1.0f constant and the rotation/up addresses below abs.s
   instead of being hoisted above the first call, and keeping z live through it gives the load
   its own register ($f0, abs into $f12). That placement also gives up ($s3) priority over
   right ($s4), as in retail. */
namespace u14BB70 {
struct Obj;
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Target { char pad[0x3514]; struct Obj *x3514; } Target;
typedef struct Obj { Vec x0,x10,x20,x30,x40,x50,x60,x70,x80; char pad90[0xE0-0x90]; Vec xE0; char padF0[0x2B0-0xF0]; int x2B0; Target *x2B4; char pad2B8[0x18]; float x2D0; } Obj;
extern void func_00222F60(Vec*,Vec*,Vec*),func_00223578(float,Vec*,Vec*),func_00223210(Vec*,Vec*,Vec*),func_002230B0(Vec*,Vec*),func_002232C8(Obj*,Vec*),func_00223248(Vec*,Vec*);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2")
static inline void Rotate(Vec *dst,Vec *src,Vec *basis) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(*dst):"r"(src),"r"(basis):"memory");
}
static inline void Add(Vec *dst,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*dst):"r"(a),"r"(b):"memory");
}
static inline void Sub(Vec *dst,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*dst):"r"(a),"r"(b):"memory");
}
static inline void Cross(Vec *dst,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0":"=m"(*dst):"r"(a),"r"(b):"memory");
}
static inline void Scale(Vec *dst,Vec *a,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0":"=m"(*dst):"r"(a),"f"(scale):"$2","memory");
}
extern "C" float fabsf(float);
void func_0014BB70(Obj *obj) {
 struct { Vec x0,x10,x20,x30,x40,x50,x60,x70,x80; } local; Vec *forward,*right,*up,*rotation; 
 if(!obj->x2B0) return;
 if(!obj->x2B4) return;
 forward=&obj->x10;
 COPY_VECTOR(local.x0,forward); COPY_VECTOR(local.x40,&obj->x80); COPY_VECTOR(local.x50,&obj->x2B4->x3514->x10);
 right=&obj->x40;
 local.x20.x=0; local.x20.z=obj->x2D0; local.x20.y=0;
 Rotate(&local.x60,&local.x20,right); Add(&local.x50,&local.x50,&local.x60); Sub(&local.x30,&local.x50,&local.x40);
 func_00222F60(&local.x10,&local.x0,&local.x30);
 { float z=obj->x20.z,speed=__builtin_fabsf(z); __asm__ volatile("Lbb70_abs:" : "+f"(z)); CallShared00223578_0((1.0f-speed)*0.01f,&local.x10,&local.x10); }
 rotation=&obj->xE0; up=&obj->x60;
 func_00223210(rotation,rotation,&local.x10); func_002230B0(rotation,rotation); func_002232C8(obj,rotation);
 COPY_VECTOR(obj->x60,&obj->x20);
 if(obj->x60.z<0.0f) Scale(&obj->x60,up,-1.0f);
 Cross(&obj->x40,forward,up); Cross(&obj->x50,up,right);
 obj->x40.z=0.0f;
 { float x=obj->x40.x,y=right->y,z=right->z,sum,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(sum):"f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(inv):"f"(1.0f),"f"(sum));
 obj->x40.x=x*inv; right->y=y*inv; right->z=z*inv;
 }
 Cross(&obj->x60,right,&obj->x50); Cross(&obj->x40,&obj->x50,up); Cross(&obj->x50,up,right);
 func_00223248(&local.x70,rotation);
 { float dot=obj->x60.x;
 __asm__ volatile("mula.s %0,%1\n\tmadda.s %2,%3\n\tmadd.s %0,%4,%5":"+f"(dot):"f"(local.x70.x),"f"(up->y),"f"(local.x70.y),"f"(up->z),"f"(local.x70.z));
 if(1.0f-dot>1e-10f) { func_00222F60(&local.x80,&local.x70,up); func_00223210(rotation,rotation,&local.x80); }
 }
 func_002230B0(rotation,rotation); func_002232C8(obj,rotation);
}

}
#undef COPY_VECTOR

/* ---- 0014BE48.c ---- */
namespace u14BE48 {
typedef struct Obj {
    char pad0[0x2B0];
    int x2B0;
    int x2B4;
    char pad2B8[0x2C4 - 0x2B8];
    int x2C4;
    char pad2C8[0x358 - 0x2C8];
    int x358;
} Obj;

void func_0014BE48(Obj *self)
{
    if (self->x2B0 != 0 && self->x2B4 != 0) {
        self->x2C4 = 0;
        self->x358 = 1;
    }
}
}

/* ---- 0014BE70.c ---- */
namespace u14BE70 {
struct Obj { char pad[0x2B0]; int x2B0, x2B4, x2B8; char pad2[0x9C]; int x358; };
extern void func_00148860(struct Obj *);
void func_0014BE70(struct Obj *obj) {
    if (obj->x2B0 && obj->x2B4) {
        obj->x358 = 0;
        func_00148860(obj);
        obj->x2B8 = 1;
    }
}
}

/* ---- 0014BEB8.c ---- */
namespace u14BEB8 {
struct Obj {
    char pad0[0x314];
    int x314;
    int x318;
    int x31c;
    int x320;
    int x324;
    int x328;
    int x32c;
    int x330;
    int x334;
    int x338;
    int x33c;
    int x340;
};

void func_0014BEB8(struct Obj *obj, int mode)
{
    if (mode == 0) {
        obj->x314 = 0x8000;
        obj->x318 = 0x2000;
        obj->x31c = 0x4000;
        obj->x320 = 0x1000;
        obj->x324 = 0x200;
        obj->x328 = 0x800;
        obj->x32c = 0x80;
        obj->x330 = 0x20;
        obj->x334 = 0x10;
        obj->x338 = 0x40;
        obj->x33c = 0x400;
        obj->x340 = 0x100;
    } else {
        obj->x314 = 0x80;
        obj->x318 = 0x20;
        obj->x31c = 0x40;
        obj->x320 = 0x10;
        obj->x324 = 0x200;
        obj->x328 = 0x800;
        obj->x32c = 0x8000;
        obj->x330 = 0x2000;
        obj->x334 = 0x1000;
        obj->x338 = 0x4000;
        obj->x33c = 0x400;
        obj->x340 = 0x100;
    }
}
}
namespace u14BF50 {
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
struct Obj {
    char pad0[0x80]; Vec pos; Vec rot;
    char padA0[0x2B0-0xA0]; int x2B0;
    char pad2B4[0x314-0x2B4];
    int x314,x318,x31C,x320,x324,x328,x32C,x330,x334,x338,x33C,x340;
};
extern int func_00220618(int,int);
extern int func_00220AE0(int,int);
extern void func_00208788(Vec *);
extern void func_00221F50(Obj *,Vec *);
extern "C" int abs(int);
void func_0014BF50(Obj *obj) {
    Vec local, rotated;
    int value;
    if (!obj->x2B0) return;
    value=func_00220618(obj->x318,0);
    if(value) {
        if((value<0 ? -value : value)>=2) obj->rot.z += (float)value * 0.00020614125f;
        else obj->rot.z += 0.017453292f;
    } else if((value=func_00220618(obj->x314,0))!=0) {
        if((value<0 ? -value : value)>=2) obj->rot.z -= (float)value * 0.00020614125f;
        else obj->rot.z -= 0.017453292f;
    } else if((value=func_00220AE0(2,0))!=0) {
        if(value>=128) { value=127-value; obj->rot.z -= (float)value*0.000274855f; }
        else obj->rot.z += (float)value*0.000274855f;
    }
    value=func_00220618(obj->x31C,0);
    if(value) {
        if((value<0 ? -value : value)>=2) obj->rot.x += (float)value * 0.00020614125f;
        else obj->rot.x += 0.017453292f;
    } else if((value=func_00220618(obj->x320,0))!=0) {
        if((value<0 ? -value : value)>=2) obj->rot.x -= (float)value * 0.00020614125f;
        else obj->rot.x -= 0.017453292f;
    } else if((value=func_00220AE0(3,0))!=0) {
        if(value>=128) { value=127-value; obj->rot.x -= (float)value*0.000274855f; }
        else obj->rot.x += (float)value*0.000274855f;
    }
    if(func_00220618(obj->x328,0)) obj->rot.y -= 0.017453292f;
    else if(func_00220618(obj->x324,0)) obj->rot.y += 0.017453292f;
    else if(func_00220618(1,0)) obj->rot.y = 0.0f;
    func_00208788(&obj->rot);
    func_00221F50(obj,&obj->rot);
    local.x=0.0f; local.y=0.0f; local.z=0.0f;
    value=func_00220618(obj->x334,0);
    if(value) {
        if((value<0 ? -value : value)>=2) {
            local.y += (float)value*0.007874016f;
            if(value>=64) local.y += (2.0f*(float)value-63.0f)*0.007874016f;
        } else local.y += 0.5f;
    } else if((value=func_00220618(obj->x338,0))!=0) {
        if((value<0 ? -value : value)>=2) {
            local.y -= (float)value*0.007874016f;
            if(value>=64) local.y -= (2.0f*(float)value-63.0f)*0.007874016f;
        } else local.y -= 0.5f;
    } else if((value=func_00220AE0(1,0))!=0) {
        if(value>=128) {
            value=127-value;
            local.y += (float)value*0.007874016f;
            if(value>=64) local.y += (2.0f*(float)value-63.0f)*0.007874016f;
        } else {
            local.y -= (float)value*0.007874016f;
            if(value>=64) local.y -= (2.0f*(float)value-63.0f)*0.007874016f;
        }
        if(func_00220618(4,0)) local.y *= 3.0f;
    }
    value=func_00220618(obj->x330,0);
    if(value) {
        if((value<0 ? -value : value)>=2) {
            local.x += (float)value*0.007874016f;
            if(value>=64) local.x += (2.0f*(float)value-63.0f)*0.007874016f;
        } else local.x += 0.5f;
    } else if((value=func_00220618(obj->x32C,0))!=0) {
        if((value<0 ? -value : value)>=2) {
            local.x -= (float)value*0.007874016f;
            if(value>=64) local.x -= (2.0f*(float)value-63.0f)*0.007874016f;
        } else local.x -= 0.5f;
    } else if((value=func_00220AE0(0,0))!=0) {
        if(value>=128) {
            value=127-value;
            local.x -= (float)value*0.007874016f;
            if(value>=64) local.x -= (2.0f*(float)value-63.0f)*0.007874016f;
        } else {
            local.x += (float)value*0.007874016f;
            if(value>=64) local.x += (2.0f*(float)value-63.0f)*0.007874016f;
        }
        if(func_00220618(4,0)) local.x *= 3.0f;
    }
    value=func_00220618(obj->x33C,0);
    if(value) {
        if((value<0 ? -value : value)>=2) {
            local.z += (float)value*0.007874016f;
            if(value>=64) local.z += (2.0f*(float)value-63.0f)*0.007874016f;
        } else local.z += 0.5f;
    } else if((value=func_00220618(obj->x340,0))!=0) {
        if((value<0 ? -value : value)>=2) {
            local.z -= (float)value*0.007874016f;
            if(value>=64) local.z -= (2.0f*(float)value-63.0f)*0.007874016f;
        } else local.z -= 0.5f;
    }
    __asm__ volatile ("lqc2 $vf11,%1\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0"
        : "=m"(rotated) : "m"(local), "r"(obj));
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
        : "=m"(obj->pos) : "r"(&obj->pos), "r"(&rotated));
}
}

#ifdef NONMATCHING
/* Non-matching: saved matrix / up-axis pointers get s1/s3 swapped by global-alloc and the
   matrix copy (daddu s3,s1) is gone (built 0x434 vs 0x438); best diff 14 aligned rows */
namespace u14C800 {
struct Vec { float x, y, z, w; } __attribute__((aligned(16)));
struct Body { char pad0[0x10]; Vec pos; };
struct Rider { char pad0[0x792C]; Body *body; };
/* Orbit camera: x0/x10/x20 = right/forward/up basis, x80 = position. */
struct Obj {
    Vec x0, x10, x20;
    char pad30[0x50];
    Vec x80;
    char pad90[0x70];
    Vec x100;
    char pad110[0x1A0];
    void *x2B0;
    Rider *x2B4;
    char pad2B8[0x30];
    float x2E8, x2EC; /* min / max orbit distance */
    char pad2F0[0x24];
    int x314, x318, x31C, x320; /* input ids: yaw -, yaw +, pitch +, pitch - */
    char pad324[0x10];
    int x334, x338;             /* input ids: zoom out, zoom in */
};
extern void func_00221F50(Vec *, Vec *);
extern int func_00220618(int, int);
extern void func_00222E80(Vec *, Vec *, float);
extern void func_002232C8(Vec *, Vec *);
extern void func_00221888(Vec *, Vec *, Vec *);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* out.xyz = in.xyz / |in.xyz| */
static inline void Normalize(Vec *out, Vec *in) {
    float length,inv,x=in->x,y=in->y,z=in->z;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x=x*inv;out->y=y*inv;out->z=z*inv;
}
/* Same as Normalize (z passed twice: register-allocation form only). */
static inline void NormalizeCross(Vec *out, Vec *in) {
    float length,inv,x=in->x,y=in->y,z=in->z;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%5\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    out->x=x*inv;out->y=y*inv;out->z=z*inv;
}
/* Scales x and y by 1/|in.xyz|; z is left untouched. */
static inline void NormalizeXY(Vec *out, Vec *in) {
    float length,inv,x=in->x,y=in->y,z=in->z;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z) : "$2");
    out->x=x*inv;out->y=y*inv;
}
/* out.xyz = in.xyz * scale (VU0) */
static inline void Scale(Vec *out, Vec *in, float scale) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(in), "f"(scale) : "$2");
}
/* The game's Abs helper (docs/findings/013). */
static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}
static inline float Sqrt(float value) {
    float result;
    __asm__ volatile ("sqrt.s %0,%1" : "=f"(result) : "f"(value));
    return result;
}
/* out.xyz = a.xyz - b.xyz (VU0) */
#define VSUB(dst, a, b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a), "r"(b))
/* out.xyz = a.xyz + b.xyz (VU0) */
#define VADD(dst, a, b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a), "r"(b))
/* out.xyz = a.xyz x b.xyz (VU0 outer product) */
#define VCROSS(dst, a, b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a), "r"(b))

/* Orbit camera update: yaw/pitch the offset to the rider with the input
   buttons, zoom the distance (clamped to [x2E8, x2EC]), keep the view
   direction away from straight up/down, rebuild the basis and place the
   camera at body->pos - distance * forward. */
void func_0014C800(Obj *obj) {
    Vec quat;
    Vec matrix[4];
    Vec delta;
    Vec limited;
    Body *body;
    Vec *pos, *zeroVec, *matrixOrAxis, *savedMatrix, *forward;
    float distance, angle, z, dy, shifted;
    if (!obj->x2B0 || !obj->x2B4) return;
    dy = 0.0f;
    zeroVec = &obj->x100;
    zeroVec->x = dy;
    zeroVec->y = dy;
    zeroVec->z = dy;
    func_00221F50(matrix, zeroVec);
    body = obj->x2B4->body;
    pos = &obj->x80;
    VSUB(delta, &body->pos, pos);
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
        : "=&f"(distance) : "f"(delta.x), "f"(delta.y), "f"(delta.z));
    matrixOrAxis = matrix;
    savedMatrix = matrixOrAxis;
    angle = dy;
    obj->x2EC = 40.0f;
    obj->x2E8 = 5.0f;
    /* yaw around the up axis (x20), 2 degrees per frame */
    if (func_00220618(obj->x318, 0)) angle = 0.034906585f;
    else if (func_00220618(obj->x314, 0)) angle = -0.034906585f;
    matrixOrAxis = &obj->x20;
    if (angle != 0.0f) {
        func_00222E80(&quat, matrixOrAxis, angle);
        func_002232C8(savedMatrix, &quat);
        func_00221888(savedMatrix, &delta, &delta);
    }
    /* pitch around the right axis (x0) */
    angle = 0.0f;
    if (func_00220618(obj->x31C, 0)) angle = 0.034906585f;
    else if (func_00220618(obj->x320, 0)) angle = -0.034906585f;
    if (angle != 0.0f) {
        func_00222E80(&quat, &obj->x0, angle);
        func_002232C8(savedMatrix, &quat);
        func_00221888(savedMatrix, &delta, &delta);
    }
    /* zoom */
    if (func_00220618(obj->x334, 0)) dy = 0.5f;
    else if (func_00220618(obj->x338, 0)) dy = -0.5f;
    shifted = distance + dy;
    shifted = shifted >? obj->x2E8;
    distance = shifted <? obj->x2EC;
    Normalize(&obj->x80, &delta);
    /* keep |forward.z| <= 0.9 */
    z = obj->x80.z;
    if (Abs(z) > 0.9f) {
        float maxed, clamped, scale, root;
        maxed = z >? -0.9f;
        clamped = maxed <? 0.9f;
        scale = 1.0f - clamped * clamped;
        root = Sqrt(scale);
        if (z == 1.0f) {
            limited.x = 0.0f;
            limited.y = root;
            limited.z = 0.0f;
        } else {
            COPY_VECTOR(limited, &obj->x80);
            limited.z = 0.0f;
            NormalizeXY(&limited, &limited);
            Scale(&limited, &limited, root);
        }
        limited.z = clamped;
        COPY_VECTOR(obj->x80, &limited);
    }
    forward = &obj->x10;
    COPY_VECTOR(*forward, &obj->x80);
    float unitZ = 1.0f;
    obj->x20.x = 0.0f;
    matrixOrAxis->z = unitZ;
    matrixOrAxis->y = 0.0f;
    VCROSS(obj->x0, forward, matrixOrAxis);
    NormalizeCross(&obj->x0, &obj->x0);
    VCROSS(obj->x20, &obj->x0, forward);
    float negativeDistance = -distance;
    Scale(&obj->x80, &obj->x80, negativeDistance);
    VADD(obj->x80, &obj->x80, &body->pos);
}
#undef VSUB
#undef VADD
#undef VCROSS
#undef COPY_VECTOR
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0014C800);
#endif

/* ---- 0014CC38.c ---- */
namespace u14CC38 {
typedef struct Rider {
 char pad0[0x117C]; int x117C; char pad1180[0x18]; int x1198,x119C;
 char pad11A0[0xA4]; int x1244; char pad1248[0x18]; int x1260,x1264;
 char pad1268[0x16A]; unsigned char x13D2; char pad13D3[6]; unsigned char x13D9,x13DA;
 char pad13DB[0x30]; unsigned char x140B; volatile unsigned char x140C;
 char pad140D[0x656B]; int x7978;
} Rider;
typedef struct Object {
 char pad0[0x2B4]; Rider *x2B4; int x2B8,x2BC,x2C0; char pad2C4[0x18]; float x2DC,x2E0;
 char pad2E4[0x68]; int x34C; char pad350[0x18]; int x368;
 unsigned char x36C; char pad36D[6]; unsigned char x373;
} Object;
extern "C" unsigned char D_0043FA34[];
extern void func_0014CFB0(Object *),func_0014D208(Object *,int),func_0014DAA8(Object *,int);
extern int func_002205A0(int),func_00220AE0(int,int),func_00238F60(void);
void func_0014CC38(Object *s)
{
    Rider *r;
    int timer, state, value, axis;
    float raised, lowered, up, down;

    if (D_0043FA34[0])
        return;
    timer = s->x368 + func_00238F60();
    s->x368 = timer;
    if (s->x373) {
        state = s->x2BC;
        if ((state == 0 && timer >= 61) || (state != 0 && timer >= 6)) {
            if (state != s->x2C0) {
                func_0014D208(s, 0);
                if (s->x2BC == s->x2C0) {
                    s->x2DC = s->x2E0;
                    func_0014CFB0(s);
                    s->x373 = 0;
                }
            }
        }
    }
    r = s->x2B4;
    if (!r)
        return;
    if (s->x34C) {
        value = r->x13D2 ? r->x1244 : r->x117C;
        if (!value || s->x36C)
            return;
        if (r->x140B || r->x140C) {
            axis = func_00220AE0(3, func_002205A0(r->x7978));
            if (axis > 0) {
                raised = s->x2DC + 0.03f;
                s->x373 = 0;
                s->x2DC = raised;
                if (raised < -0.4f)
                    s->x2DC = -0.4f;
                func_0014CFB0(s);
                up = s->x2DC;
                if (up > 2.0f) {
                    s->x2DC = 2.0f;
                    func_0014DAA8(s, 3);
                } else if (up > 1.0f)
                    func_0014DAA8(s, 2);
                else
                    func_0014DAA8(s, 1);
                s->x2C0 = s->x2BC;
                s->x2E0 = s->x2DC;
            } else if (axis < 0) {
                lowered = s->x2DC - 0.03f;
                s->x373 = 0;
                s->x2DC = lowered;
                if (lowered < -0.4f)
                    s->x2DC = -1.1f;
                func_0014CFB0(s);
                down = s->x2DC;
                if (down < -1.0f) {
                    s->x2DC = -1.0f;
                    func_0014DAA8(s, 0);
                } else if (down < 1.0f)
                    func_0014DAA8(s, 1);
                else if (down < 2.0f)
                    func_0014DAA8(s, 2);
                else
                    func_0014DAA8(s, 3);
                s->x2C0 = s->x2BC;
                s->x2E0 = s->x2DC;
            } else
                return;
        } else {
            value = r->x13D9 ? r->x1260 : r->x1198;
            if (value) {
                s->x34C = 0;
                s->x373 = 0;
                func_0014D208(s, 0);
                return;
            }
            value = r->x13DA ? r->x1264 : r->x119C;
            if (value) {
                s->x34C = 0;
                s->x373 = 0;
                func_0014D208(s, 1);
            }
        }
    } else {
        value = r->x13D9 ? r->x1260 : r->x1198;
        if (value)
            return;
        value = r->x13DA ? r->x1264 : r->x119C;
        if (value == 0)
            s->x34C = 1;
    }
}
}

/* ---- 0014CF80.c ---- */
namespace u14CF80 {
struct Obj {
    char pad0[0x140];
    char x140[0x17C];
    int x2BC;
};
extern void func_0014D9C0(struct Obj *, void *, void *);
void func_0014CF80(struct Obj *arg0, int arg1) {
    char local[16];
    if (arg1 != 0) {
        arg0->x2BC = 0;
        func_0014D9C0(arg0, arg0->x140, local);
    }
}
}

/* ---- 0014CFB0.c ---- */
namespace u14CFB0 {
typedef struct { float x,y,z,w; } Vec;
typedef struct { char pad0[0xA0]; Vec xA0; char padB0[0x21C]; float x2CC,x2D0,x2D4,x2D8,x2DC; } Obj;
extern void func_0014D798(Obj *,Vec *,float *);
extern void func_0014D948(Obj *,Vec *,float *);
extern void func_0014D988(Obj *,Vec *,float *);
extern void func_0014D9C0(Obj *,Vec *,float *);
extern void func_0014DA80(Obj *,float,float,float);
extern int func_0023DED0(void);
#define COPY(d,s) __asm__ volatile("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(d) : "r"(s) : "$2")
void func_0014CFB0(Obj *o) {
 struct { Vec a,b; float c,d; } local;
 float blend;
 float oc,oz,oy,ox,c,z,y,x;
 func_0014D798(o,&o->xA0,&local.c);
 blend=o->x2DC; o->x2CC=local.c;
 if (blend<0.0f) {
 float wb1,wa1;
wb1=1.0f; wa1=blend+wb1; wb1-=wa1;
 COPY(local.a,&o->xA0); func_0014D948(o,&local.a,&local.c);
 COPY(local.b,&o->xA0); func_0014D9C0(o,&local.b,&local.d);
 o->xA0.x=local.b.x*wb1+local.a.x*wa1;
 o->xA0.y=local.b.y*wb1+local.a.y*wa1;
 o->xA0.z=local.b.z*wb1+local.a.z*wa1;
 o->x2D0=local.d*wb1+local.c*wa1;
 } else {
 if (blend<1.0f) {
 float wb2;
wb2=1.0f-blend; COPY(local.a,&o->xA0); func_0014D948(o,&local.a,&local.c);
 o->xA0.x=o->xA0.x*blend+local.a.x*wb2; o->xA0.y=o->xA0.y*blend+local.a.y*wb2; o->xA0.z=o->xA0.z*blend+local.a.z*wb2; o->x2D0=o->x2D0*blend+local.c*wb2;
 } else {
 float wa3,wb3;
wa3=blend-1.0f; wb3=2.0f; wb3-=blend;
 COPY(local.a,&o->xA0); func_0014D988(o,&local.a,&local.c);
 o->xA0.x=o->xA0.x*wb3+local.a.x*wa3; o->xA0.y=o->xA0.y*wb3+local.a.y*wa3; o->xA0.z=o->xA0.z*wb3+local.a.z*wa3; o->x2D0=o->x2D0*wb3+local.c*wa3;
 }
 
 }
 if (func_0023DED0()==2) func_0014DA80(o,1.0f,1.5f,1.5f);
}
}
#undef COPY

/* ---- 0014D208.c ---- */
namespace u14D208 {
typedef struct Obj { char pad0[0xa0]; float xA0,xA4,xA8; char padac[0x2bc-0xac]; int x2BC,x2C0; char pad2c4[0x18]; float x2DC,x2E0,x2E4; char pad2e8[0x78]; int x360; char pad364[0xf]; unsigned char x373; } Obj;
extern "C" unsigned char D_0043FA34[];
extern "C" int D_004B26AC[];
extern void func_0014D3B8(Obj *);
extern void func_0014D538(Obj *);
extern void func_0014D740(Obj *);
void func_0014D208(Obj *obj,int arg) {
    if(D_0043FA34[0] || D_004B26AC[0]==10) {
        if(obj->x360==0) {
            func_0014D3B8(obj);
            if(obj->x2BC==0) {
                obj->x360=1;
                obj->x2E4=45.0f;
                func_0014D3B8(obj);
            }
        } else if(obj->x360==1) {
            obj->x360=2; obj->x2E4=-45.0f;
        } else if(obj->x360==2) {
            obj->x360=3; obj->x2E4=180.0f;
        } else if(obj->x360==3) {
            obj->x360=4; obj->x2E4=75.0f;
        } else if(obj->x360==4) {
            obj->x360=0;
        }
    } else if(arg) func_0014D538(obj);
    else func_0014D3B8(obj);
    func_0014D740(obj);
    switch(obj->x2BC) {
    case 0: obj->x2DC=-1.0f; break;
    case 1: obj->x2DC=0.0f; break;
    case 3: obj->x2DC=2.0f; break;
    case 2: default: obj->x2DC=1.0f; break;
    }
    if(obj->x360) { obj->xA0=0; obj->xA4=0; obj->xA8=0; }
    if(!obj->x373) { obj->x2C0=obj->x2BC; obj->x2E0=obj->x2DC; }
}
}

/* ---- 0014D3B8.c ---- */
namespace u14D3B8 {
typedef struct { char pad0[0x2BC]; int x2BC; char pad2C0[0x1C]; float x2DC; float x2E0; char pad2E4[0x28]; float x30C; char pad310[0x5D]; unsigned char x36D; } Obj;
extern void func_0014DAA8(Obj *, int);
void func_0014D3B8(Obj *obj) {
    float t;
    float frac;
    float a;
    t = obj->x2DC;
    frac = t - (float)(int)t;
    a = fabsf(frac);
    /* Zero-byte label: scheduling barrier so the 0.001f load stays after abs.s;
       keeping frac live here puts t/abs in $f3/$f2 (cf. src/unsorted/00176ED8.c). */
    __asm__ volatile ("Labsdiff_14D3B8:" : : "f" (a), "f" (frac));
    if (a > 0.001f) {
        if (t < 0.0f) {
            obj->x2DC = 0.0f;
            func_0014DAA8(obj, 1);
        } else if (t < 1.0f) {
            obj->x2DC = 1.0f;
            func_0014DAA8(obj, 2);
        } else {
            obj->x2DC = 2.0f;
            func_0014DAA8(obj, 3);
        }
        obj->x2E0 = obj->x2DC;
    } else {
        switch (obj->x2BC) {
        case 0:
            func_0014DAA8(obj, 1);
            break;
        case 1:
            func_0014DAA8(obj, 2);
            break;
        case 2:
            func_0014DAA8(obj, 3);
            break;
        case 3:
            if (obj->x36D != 0)
                func_0014DAA8(obj, 0);
            else
                func_0014DAA8(obj, 1);
            break;
        }
    }
    obj->x30C = 1000.0f;
}
}

/* ---- 0014D538.c ---- */
namespace u14D538 {
typedef struct { char pad0[0x2BC]; int x2BC; char pad2C0[0x1C]; float x2DC; float x2E0; char pad2E4[0x28]; float x30C; char pad310[0x5D]; unsigned char x36D; } Obj;
extern void func_0014DAA8(Obj *, int);
void func_0014D538(Obj *obj) {
    float t;
    float frac;
    float a;
    t = obj->x2DC;
    frac = t - (float)(int)t;
    a = fabsf(frac);
    /* Zero-byte label: scheduling barrier so the 0.001f load stays after abs.s;
       keeping frac live here puts t/abs in $f3/$f2 (cf. src/unsorted/0014D3B8.c). */
    __asm__ volatile ("Labsdiff_14D538:" : : "f" (a), "f" (frac));
    if (a > 0.001f) {
        if (t < 0.0f) {
            obj->x2DC = -1.0f;
            func_0014DAA8(obj, 0);
        } else if (t < 1.0f) {
            obj->x2DC = 0.0f;
            func_0014DAA8(obj, 1);
        } else {
            obj->x2DC = 1.0f;
            func_0014DAA8(obj, 2);
        }
        obj->x2E0 = obj->x2DC;
    } else {
        switch (obj->x2BC) {
        case 1:
            if (obj->x36D != 0) {
                func_0014DAA8(obj, 0);
                break;
            }
        case 0:
            func_0014DAA8(obj, 3);
            break;
        case 2:
            func_0014DAA8(obj, 1);
            break;
        case 3:
            func_0014DAA8(obj, 2);
            break;
        }
    }
    obj->x30C = 1000.0f;
}
}

/* ---- 0014D6A8.c ---- */
namespace u14D6A8 {
struct State { char pad0[0x117c]; int x117c; char pad1180[0x14]; int x1194; char pad1198[0xac]; int x1244; char pad1248[0x14]; int x125c; char pad1260[0x172]; unsigned char x13d2; char pad13d3[5]; unsigned char x13d8; };
struct Obj { char pad0[0x2b4]; struct State *x2b4; char pad2b8[0x98]; int x350; };
extern "C" char D_0037EA60[];
extern void func_0014B570(void *);
void func_0014D6A8(struct Obj *obj) {
 struct State *state = obj->x2b4;
 int value;
 if (state) {
  if (obj->x350) {
   if (state->x13d2) value=state->x1244; else value=state->x117c;
   if (value == 0) return;
   state = obj->x2b4;
   if (state->x13d8) value=state->x125c; else value=state->x1194;
   if (value) { func_0014B570(D_0037EA60); obj->x350=0; }
  } else {
   if (state->x13d8) value=state->x125c; else value=state->x1194;
   if (!value) obj->x350=1;
  }
 }
}
}

/* ---- 0014D740.cpp ---- */
namespace u14D740 {
struct Vec4 { float x, y, z, w; } __attribute__((aligned(16)));

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

struct Obj {
    char pad0[0xA0];
    Vec4 xA0;
    char padB0[0x2CC - 0xB0];
    float x2CC;

    void func_0014D740();
    void func_0014D798(Vec4 *vec, float *f);
    void func_0014D8B8(Vec4 *vec, float *f);
};

void Obj::func_0014D740() {
    struct {
        Vec4 vec;
        float f;
    } tmp;
    Obj *self = this;

    func_0014D798(&tmp.vec, &tmp.f);
    func_0014D8B8(&tmp.vec, &tmp.f);
    COPY_VECTOR(self->xA0, &tmp.vec);
    self->x2CC = tmp.f;
}
}
#undef COPY_VECTOR

/* ---- 0014D798.c ---- */
namespace u14D798 {
typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

typedef struct Obj {
    char pad0[0x2BC];
    int x2BC;
    char pad2C0[0x2D0 - 0x2C0];
    float x2D0;
    float x2D4;
} Obj;

typedef struct Game {
    char pad0[0x288];
    int mode;
    char pad28C[0x2A0 - 0x28C];
    int x2A0;
} Game;

extern "C" Game D_004B25A0;
extern int func_0023DED0(void);
extern int func_00182290(int);

void func_0014D798(Obj *obj, Vec3 *ofs, float *out)
{
    int mode;

    *out = 0.0f;
    mode = D_004B25A0.mode;
    if (mode == 2) {
        if (func_0023DED0() == mode) {
            ofs->y = -17.7f;
            ofs->z = 2.7f;
        } else {
            ofs->y = -12.0f;
            ofs->z = 5.0f;
        }
        ofs->x = 0.0f;
        obj->x2D4 = 0.0f;
        return;
    }
    if (func_00182290(D_004B25A0.x2A0) == 0) {
        ofs->y = -11.0f;
        ofs->z = 4.3f;
        ofs->x = 0.0f;
        if (obj->x2BC != 0) {
            obj->x2D0 = obj->x2D4 = -0.163f;
        } else {
            obj->x2D4 = 0.0f;
        }
        return;
    } else {
        ofs->y = -10.0f;
        ofs->z = 3.7f;
        ofs->x = 0.0f;
        obj->x2D4 = 0.0f;
        obj->x2D0 = 0.0f;
    }
}
}

/* ---- 0014D8B8.c ---- */
namespace u14D8B8 {
typedef struct Obj {
    char pad0[0x2BC];
    int state;
} Obj;

extern void func_0014D9C0(Obj *, void *, float *);
extern void func_0014D948(Obj *, void *, float *);
extern void func_0014D980(Obj *, void *, float *);
extern void func_0014D988(Obj *, void *, float *);

void func_0014D8B8(Obj *obj, void *ofs, float *out)
{
    switch (obj->state) {
    case 0:
        func_0014D9C0(obj, ofs, out);
        break;
    case 1:
        func_0014D948(obj, ofs, out);
        break;
    case 2:
        func_0014D980(obj, ofs, out);
        break;
    case 3:
        func_0014D988(obj, ofs, out);
        break;
    }
}
}

/* ---- 0014D948.c ---- */
namespace u14D948 {
void func_0014D948(void *arg0, float *values) {
    values[1] *= 0.6f;
    values[2] *= 0.7f;
}
}

/* ---- 0014D980.c ---- */
namespace u14D980 {
struct S_0014D980 {
    int x0;
};

void func_0014D980(int a0, int a1, struct S_0014D980 *s) {
    s->x0 = 0;
}
}

/* ---- 0014D988.c ---- */
namespace u14D988 {
typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

void func_0014D988(void *unused, Vec3 *v)
{
    v->y = v->y * 1.82f;
    v->z = v->z * 2.41f;
}
}

/* ---- 0014D9C0.c ---- */
namespace u14D9C0 {
extern "C" int D_004B2828[];
extern int func_0023DED0(void);
struct Obj { char pad[0x2d0]; int x2d0, x2d4; };
struct Range { float x0,x4,x8; };
void func_0014D9C0(struct Obj *obj, struct Range *range) {
 int state;
 state=D_004B2828[0];
 if(state==2) {
  if(func_0023DED0()==state) {range->x4=-0.5f;range->x8=0.5f;}
  else { range->x4=-0.5f; range->x8=1.0f; }
 } else {range->x4=-1.0f; range->x8=1.0f;}
 obj->x2d4=0;obj->x2d0=0;
 if(D_0029C5B0) range->x4-=1.0f;
}
}

/* ---- 0014DA80.c ---- */
namespace u14DA80 {
typedef struct Obj { char pad[0xA0]; float xA0, xA4, xA8; } Obj;
void func_0014DA80(Obj *obj, float x, float y, float z) {
    obj->xA0 *= x;
    obj->xA4 *= y;
    obj->xA8 *= z;
}
}

/* ---- 0014DAA8.c ---- */
namespace u14DAA8 {
typedef struct Obj { char pad0[0x2BC]; int x2BC; char pad2C0[0x1C]; float x2DC; char pad2E0[0x7C]; int x35C; } Obj;
typedef struct State { char pad0[0x12220]; int x12220[4]; float x12230[4]; } State;
extern "C" State D_004225A0;
extern "C" unsigned char D_0043FA34[];
extern "C" int D_004B26AC[];
extern void func_0014CF80(Obj *, int);
void func_0014DAA8(Obj *obj, int value) {
 obj->x2BC=value;
 func_0014CF80(obj,value==0);
 if (!D_0043FA34[0] && D_004B26AC[0] != 10) {
  D_004225A0.x12220[obj->x35C]=value;
  D_004225A0.x12230[obj->x35C]=obj->x2DC;
 }
}
}
namespace u14DB38 {
typedef struct Entry { char pad0[0x2B4]; void *x2B4; int x2B8; int x2BC; char pad2C0[0x98]; int x358; char pad35C[0x18]; unsigned char x374; char pad375[0xB]; } Entry;
typedef struct Entries { Entry entries[4]; int count; } Entries;
typedef struct Net { char pad0[0x5BE8]; int x5BE8; } Net;
typedef struct Local { char pad0[0x130]; int x130; } Local;
typedef struct Game { char pad0[0x288]; int x288; char pad28C[0xE4]; int x370; } Game;
extern "C" Entries D_0037EA60;
extern "C" Net D_00398C40;
extern "C" Local D_003F52D0;
extern "C" Game D_004B25A0;
extern "C" char D_002AF2C0[];
extern "C" void func_0012DE68(void *, int);
extern "C" void func_0011C540(char *, int);
static inline long Enabled(Entry *e) { long r = 0; if (e->x374) { int t = e->x358 == 0; r = t; } return (int)r; }
void func_0014DB38(int index) {
 int i;
 for (i = 0; i < D_0037EA60.count; i++) {
  Entry *e = &D_0037EA60.entries[i];
  Game *g = &D_004B25A0;
  void *target = e->x2B4;
  int sel;
  if (target) {
   if (e->x2BC == 0) {
    if (g->x370) {
     if (i == D_003F52D0.x130 && Enabled(e)) func_0012DE68(target, 1);
     else func_0012DE68(target, 0);
    } else {
     sel = i == index;
     if (sel && Enabled(&D_0037EA60.entries[index])) func_0012DE68(target, 1);
     else func_0012DE68(target, 0);
    }
   } else if (D_00398C40.x5BE8) {
    func_0012DE68(target, 0);
   } else {
     sel = i == index;
     if (sel && Enabled(&D_0037EA60.entries[index])) func_0012DE68(target, 1);
     else func_0012DE68(target, 0);
   }
  }
 }
 if (D_004B25A0.x288 >= 2) func_0011C540(D_002AF2C0, index);
}
}
struct OwnerObj { char pad[0x180]; void *x180; char pad184[0x1fc]; };
extern struct OwnerObj D_0037EA60[];
extern char D_00285B58[];
void func_0014DD00(int enabled, int code) {
 struct OwnerObj *obj;
 int count;
 if (code == 0xffff && enabled != 0) {
  obj = D_0037EA60;
  count = 3;
  do { obj->x180 = D_00285B58; count--; obj++; } while (count != -1);
  D_0029DD60 = D_0029C5B4;
 }
}
