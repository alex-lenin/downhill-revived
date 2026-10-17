/* UNIT 0x00132CE0 0x001331E0 */
#include "impulse.h"
#include "object_registry.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

/* surface table, 32 classes */
typedef struct Surface { float friction; char pad4[0x1C]; } Surface;

typedef struct Event {
    union {
        unsigned long kind;
        struct {
            unsigned short x0, x2, id;
            unsigned short type : 5;
            unsigned short x6_5 : 11;
        } s;
    } h;
    char pad8[8];
    Vec position;
    Vec normal;
} Event;

typedef struct World {
    char pad0[0x7928];
    float x7928;
    char pad792C[0x7A5C - 0x792C];
    int x7A5C;
    char pad7A60[0x7B9E - 0x7A60];
    unsigned char x7B9E;
} World;

typedef struct Actor {
    char pad0[0x38];
    Vec *x38;
    char pad3C[0x14];
    Vec x50;
    char pad60[0x60];
    Vec xC0;
    Vec xD0;
    char padE0[0x78];
    World *x158;
    char pad15C[4];
    int x160;
    char pad164[0xEC];
    Vec x250;
    char pad260[0x1CC];
    float x42C;
    char pad430[4];
    float x434;
    char pad438[0x54];
    float x48C;
} Actor;

class Obj {
public:
    virtual void call(int, Event *);
    virtual void unused();
    virtual void vector(float *, float *);
    virtual void unused2();
    virtual float scale();
};

extern Surface D_00768098[];
extern float D_007684AC[];
extern float D_0029C324;
extern float D_0029C320;
extern float D_0029C310;
extern int D_0029C83C;
extern float func_0012DF48(World *);
extern void func_00130D80(Actor *, Vec *, Vec *, Vec *, Vec *, Vec *);
extern void func_0016AFC0(int *, Vec *, Vec *, Vec *);
extern void func_001331E0(Actor *, Vec *);



static inline void Copy(Vec *out, Vec *v) {
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(v) : "$2");
}

static inline Surface *GetSurface(int type) {
    if (type >= 32) return D_00768098;
    return &D_00768098[type];
}

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
static inline void Zero(Vec *out) {
    __asm__ volatile("sqc2 $vf0, %0" : "=m"(*out));
}
static inline float Normalize(Vec *out, Vec *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %2, %3\n\tmadda.s %4, %5\n\tmadd.s %1, %6, %6\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=f"(len) : "f"(x), "f"(x), "f"(y), "f"(y), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}
static inline float Max(float a, float b) {
    float r;
    __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

/* wheel friction: wheel 0 = front, 1 = rear */
void func_00132CE0(Actor *a, Event *e, Vec *p, int wheel) {
    Vec point, rel, arm, vel, front, tmp, side, dir, push, push2;
    float friction, limit;
    int sideways;
    Obj *o;
    Vec *pos, *axis, *up;

    if (wheel == 2) return;
    friction = a->x42C * GetSurface(e->h.s.type)->friction * a->x158->x7928 * 0.01f;
    if (a->x158->x7B9E) friction *= D_007684AC[0];
    pos = &a->x50;
    *(u128 *)&point = *(u128 *)pos;
    limit = friction;
    if (func_0012DF48(a->x158) != 0.0f) limit *= D_0029C324;
    Sub(&arm, &point, pos);
    Cross(&vel, &a->xD0, &arm);
    Add(&vel, &vel, &a->xC0);
    Sub(&rel, &vel, p);
    func_00130D80(a, &rel, &point, &e->normal, a->x38, &front);
    front.w *= a->x48C;
    front.w = Max(-limit, front.w);
    Scale(&front, &front, -front.w);
    Copy(&tmp, &front);
    func_0016AFC0(&a->x160, &e->normal, &tmp, &front);
    axis = a->x38;
    up = &axis[1];
    Normalize(&dir, &a->xC0);
    float along = Dot(up, &dir);
    Cross(&push, &dir, axis);
    sideways = Dot(&push, &e->normal) > 0.0f;
    if (func_0012DF48(a->x158) != 0.0f || sideways) {
        func_00130D80(a, &rel, &point, &e->normal, up, &side);
    } else {
        Zero(&side);
        side.w = 0.0f;
    }
    side.w *= a->x48C;
    side.w = Max(-friction, side.w);
    Scale(&side, &side, -side.w);
    if (func_0012DF48(a->x158) != 0.0f) Scale(&side, &side, D_0029C320 * (along * along));
    /* zero-byte labels (cf. 001733C0): keep total live past abs.s so |total| gets its own
       register ($f0, retail); an output operand avoids flushing CSE of the 0.0f */
    {
        float total = front.w + side.w, slip = __builtin_fabsf(total);
        __asm__ volatile ("L00132CE0_slip0:" : "+f"(slip) : "f"(total));
        a->x434 += Max(0.0f, slip - limit);
    }
    if (func_0012DF48(a->x158) != 0.0f) {
        float total = front.w + side.w, slip = __builtin_fabsf(total);
        __asm__ volatile ("L00132CE0_slip1:" : "+f"(slip) : "f"(total));
        a->x434 += Max(0.0f, slip - limit) * 2.0f;
    }
    if (D_0029C83C && a->x160 == 3) {
        func_001331E0(a, &front);
        func_001331E0(a, &side);
    } else {
        func_00231CC0((ImpulseBody *)(a), (Vector4 *)(&front), (Vector4 *)(&point));
        func_00231CC0((ImpulseBody *)(a), (Vector4 *)(&side), (Vector4 *)(&point));
    }
    /* written per arm (cross-jumped into one pair): keeps the 0.15f load below the lw (retail) */
    if (a->x158->x7A5C == 3) {
        Scale(&front, &front, 0.15f);
        Scale(&side, &side, 0.15f);
    } else {
        Scale(&front, &front, D_0029C310);
        Scale(&side, &side, D_0029C310);
    }
    Copy(&push, &front);
    Copy(&push2, &side);
    o = (Obj *)func_0017D3D0(e->h.s.id);
    if (o) {
        Scale(&push, &push, o->scale());
        Scale(&push2, &push2, o->scale());
    }
    Add(&a->x250, &a->x250, &push);
    Add(&a->x250, &a->x250, &push2);
}
