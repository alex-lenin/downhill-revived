/* UNIT 0x0016AA58 0x0016ABC8 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Parent {
    char pad0[0x38];
    Vec *x38;
    char pad3C[0x84];
    Vec xC0;
} Parent;

typedef struct Obj {
    int x0;
    Parent *x4;
    char pad8[8];
    Vec normal;
    Vec x20;
    Vec x30;
    Vec x40;
} Obj;

extern void func_0016ABC8(Obj *);
extern void func_0016ACE8(Obj *);

static inline void VecCopy(Vec *dst, Vec *src)
{
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
}

static inline float VecDot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float scale)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "mfc1 $2, %3\n"
        "qmtc2.ni $2, $vf13\n"
        "vaddax.xyz ACC, $vf11, $vf0x\n"
        "vmaddx.xyz $vf11, $vf12, $vf13x\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}

static inline void VecNormalize(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3"
        : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
}
/* Projects two direction vectors onto the plane of normal and renormalizes them. */
void func_0016AA58(Obj *o)
{
    if (o->x0) {
        Vec *forward = &o->x20;
        Vec *normal;
        Vec *side;

        VecCopy(forward, o->x4->x38);
        normal = &o->normal;
        VecAddScaled(forward, forward, normal, -VecDot(forward, normal));
        VecNormalize(forward);

        side = &o->x40;
        VecCopy(side, &o->x4->xC0);
        VecAddScaled(side, side, normal, -VecDot(side, normal));
        VecNormalize(side);
    }
    if (o->x0 == 1) {
        func_0016ABC8(o);
        func_0016ACE8(o);
    }
}
