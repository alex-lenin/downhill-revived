/* UNIT 0x00132918 0x00132CE0 .sdata=0x29C2C0:0x29C2C4 */
#include "object_registry.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef float Vector[4] __attribute__((aligned(16)));
typedef union Vec4 { u128 q; float f[4]; } Vec4;

typedef struct Event {
    union {
        unsigned long kind;     /* bits 59..63 = surface type */
        struct { unsigned short x0, x2, id, x6; } s;
    } h;
    char pad8[8];
    u128 position;
    u128 normal;
} Event;

typedef struct Contact {
    char pad0[0x30];
    u128 x30, x40, x50;
    char pad60[0x14];
    char x74;
} Contact;

class Body {
public:
    char pad0[0x20];
    virtual Vec4 velocity(float *, float *, struct Actor *);
    virtual void position(float *, float *, struct Actor *);
};

typedef struct Actor {
    char pad0[0x38];
    float *x38;
    char pad3C[0x11C];
    char *x158;
    char pad15C[0xA4];
    u128 x200, x210, x220;
    char pad230[0x20];
    Vector x250;
    Vector x260;
    char pad270[0x70];
    Contact c0;
    Contact c1;
    char pad3E0[0x50];
    float x430;
    char pad434[0x70];
    short x4A4;
    char pad4A6[8];
    unsigned char x4AE;
} Actor;

class Obj {
public:
    virtual void call(int, Event *);
    virtual void unused();
    virtual void vector(float *, float *);
    virtual void unused2();
    virtual float scale();
};

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

float D_0029C2C0 = 30.0f;

static inline void SetContact(Contact *c, Event *e)
{
    c->x30 = ((u128 *)e)[0];
    c->x40 = ((u128 *)e)[1];
    c->x50 = ((u128 *)e)[2];
    c->x74 = 0;
}

static inline float Max(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float Min(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

extern Body *func_00134F58(Actor *, int);
extern void func_00134D70(Body *, float *, Actor *);
extern float func_00130C08(Actor *, float *, float *, float *, float *, float *, float);

extern void func_001C99B0(char *, int, float);
extern void func_001C9A00(char *, int, float);
extern void func_001D1150(char *, int, float);
extern void func_00232A80(Actor *, float *, float *);
extern void func_001B0AC0(char *, float);

int func_00132918(Actor *a, Event *e, float *vec, int mode) {
    Vector start;
    Vector point;
    Vector before;
    Vector after;
    Vector impulse;
    float depth;
    float strength;
    float *normal;
    Body *body;
    Obj *o;
    float d;

    if (a->x4AE > 1) depth = a->x4AE * 0.1f;
    else depth = 0.0f;
    normal = (float *)&e->normal;
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0"
                      : "=m"(start) : "r"(vec), "r"(normal), "f"(depth) : "$2");
    a->x4A4 = 0;
    a->x200 = ((u128 *)e)[0];
    a->x210 = ((u128 *)e)[1];
    a->x220 = ((u128 *)e)[2];
    body = func_00134F58(a, mode);
    func_00134D70(body, point, a);
    body->position(before, point, a);
    strength = func_00130C08(a, before, start, point, normal, impulse, 0.0f);
    Vec4 push = body->velocity(impulse, point, a);
    body->position(after, point, a);
    if (before[0] == after[0] && before[1] == after[1] && before[2] == after[2]) return 0;
    a->x430 += strength;
    Vec4 scaled;
    float *sv = scaled.f;
    COPY_VECTOR(scaled, &push);
    Obj *src = (Obj *)func_0017D3D0(e->h.s.id);
    if (src) {
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                          : "=m"(scaled) : "r"(sv), "f"(src->scale()) : "$2");
    }
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(a->x250) : "r"(a->x250), "r"(sv) : "memory");
    switch (mode) {
    case 0:
        func_001C99B0(a->x158 + 0x7720, (unsigned short)(e->h.kind >> 59), strength);
        SetContact(&a->c0, e);
        break;
    case 1:
        func_001C9A00(a->x158 + 0x7720, (unsigned short)(e->h.kind >> 59), strength);
        SetContact(&a->c1, e);
        break;
    case 2: {
        Vector offset;
        Vector target;
        func_001D1150(a->x158 + 0x3FA8, (unsigned short)(e->h.kind >> 59), strength);
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                          : "=m"(a->x260) : "r"(a->x260), "r"(impulse) : "memory");
        func_00232A80(a, offset, (float *)&e->position);
        o = (Obj *)func_0017D3D0(e->h.s.id);
        if (o) {
            o->vector((float *)&e->position, target);
            __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                              : "=m"(offset) : "r"(offset), "r"(target) : "memory");
        }
        __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                          : "=f"(d) : "f"(offset[0]), "f"(a->x38[0]), "f"(offset[1]), "f"(a->x38[1]), "f"(offset[2]), "f"(a->x38[2]));
        d = Max(-1.0f, Min(1.0f, d / D_0029C2C0));
        func_001B0AC0(a->x158 + 0x4E54, d);
        break;
    }
    }
    return 1;
}
