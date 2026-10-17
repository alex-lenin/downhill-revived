/* UNIT 0x00164FF0 0x00165408 .sdata=0x29C800:0x29C80C */
#include "impulse.h"
#include "object_registry.h"
float D_0029C800 = 0.5f;
float D_0029C804 = -2.0f;
float D_0029C808 = 10.0f;

struct Vec { float x, y, z, w; } __attribute__((aligned(16)));

class Target {
public:
    virtual void v0();
    virtual void v1();
    virtual void position(Vec *, Vec *);
};

typedef struct Flags {
    unsigned long bit0 : 5;
    unsigned long bit5 : 1;
    unsigned long bit6 : 10;
    unsigned long x2 : 16;
    unsigned long rest : 32;
} Flags;

typedef struct Model {
    unsigned long bit0 : 57;
    unsigned long bit57 : 1;
    unsigned long rest : 6;
} Model;

struct Body {
    char pad0[0x34];
    Vec *pos;
    Vec *direction;
    char pad3C[0x474];
    Flags x4B0;
};

struct Frame {
    char pad0[0x20];
    Vec x20;
};

struct Entry {
    float x0;
    char pad4[0x18];
    float x1C;
    float x20;
    char pad24[0x9C];
};

struct Obj {
    char pad0[0x34];
    Vec *pos;
    char pad38[0xC8];
    Model *x100;
    char pad104[0x1C];
    int x120;
    char pad124[0x32C];
    char x450[4];
};

struct Hit {
    char pad0[4];
    unsigned short id;
    char pad6[10];
    Vec pos;
};

extern Entry D_003960A0[];

extern void func_00232A80(Body *, Vec *, Vec *);
extern Frame *func_00138558(Body *);


extern void func_001B0AC0(void *, float);
extern void func_001CF720(void *, Vec *, int);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void sub(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline void cross(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline void scale(Vec *out, Vec *v, float s) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void addScale(Vec *out, Vec *a, Vec *b, float s) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline float norm(Vec *out, Vec *v) {
    float len, inv, x = v->x, y = v->y, z = v->z;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline void unit(Vec *v) {
    float len, inv, x = v->x, y = v->y, z = v->z;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
}

static inline float dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline float minf(float a, float b) { float r; __asm__ volatile("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
static inline float maxf(float a, float b) { float r; __asm__ volatile("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }

/* uses VU0 / EE FPU inline asm */
void func_00164FF0(Obj *obj, char *rider, Hit *hit)
{
    Body *body = (Body *)(rider + 0x40C0);
    Vec *input = &hit->pos;
    Entry *entry = &D_003960A0[obj->x120];
    Vec side, delta, dir, push, diff, saved, target, spin, lift;
    Target *t;
    float length, sign, steer;

    int on = 1;

    body->x4B0.bit5 = on;
    body->x4B0.x2 = on;
    obj->x100->bit57 = 0;
    func_00232A80(body, &diff, input);
    COPY_VECTOR(saved, &diff);
    t = (Target *)func_0017D3D0(hit->id);
    if (t) {
        t->position(input, &target);
        sub(&diff, &diff, &target);
    }
    length = norm(&dir, &diff);
    cross(&side, &func_00138558(body)->x20, &dir);
    unit(&side);
    sub(&delta, obj->pos, body->pos);
    if (dot(&delta, &side) < 0.0f) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    addScale(&push, &dir, &side, sign);
    unit(&push);
    scale(&push, &push, entry->x0 * D_0029C800 * length);
    if (obj->x120 == 0) {
        scale(&target, &push, 0.95f);
        scale(&spin, &push, 0.05f);
    } else {
        scale(&target, &push, 0.9f);
        scale(&spin, &push, 0.1f);
    }
    addScale(&target, &target, &saved, entry->x20);
    func_00231C88((ImpulseBody *)(obj), (Vector4 *)(&target));
    func_00231CC0((ImpulseBody *)(obj), (Vector4 *)(&spin), (Vector4 *)(&hit->pos));
    scale(&push, &push, D_0029C804);
    func_00231C88((ImpulseBody *)(body), (Vector4 *)(&push));
    lift.x = 0.0f;
    lift.y = 0.0f;
    lift.z = entry->x1C;
    lift.w = 0.0f;
    func_00231C88((ImpulseBody *)(obj), (Vector4 *)(&lift));
    steer = maxf(-1.0f, minf(1.0f, dot(&diff, body->direction) / D_0029C808));
    func_001B0AC0(rider + 0x4E54, steer);
    func_001CF720(obj->x450, obj->pos, obj->x120);
}
