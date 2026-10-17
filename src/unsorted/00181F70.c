/* UNIT 0x00181F70 0x00181FE8 */
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Rider {
    char pad0[0x4270];
    Vec pos;
    Vec dir;
} Rider;

typedef struct Obj {
    Rider *x0;
    int x4;
    int x8;
    Rider *xC;
} Obj;

static inline void SubVector(float *dst, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "vsub.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, 0x0(%0)"
        : : "r"(dst), "r"(a), "r"(b) : "memory");
}

static inline float InnerProduct(Vec *a, float *b)
{
    float result;

    __asm__ volatile (
        "mula.s %1, %2\n\t"
        "madda.s %3, %4\n\t"
        "madd.s %0, %5, %6"
        : "=f"(result)
        : "f"(a->x), "f"(b[0]), "f"(a->y), "f"(b[1]), "f"(a->z), "f"(b[2]));
    return result;
}

void func_00181F70(Obj *o)
{
    float delta[4] __attribute__((aligned(16)));
    Rider *r = o->x0;

    SubVector(delta, &o->xC->pos, &r->pos);
    if (InnerProduct(&r->dir, delta) >= 0.0f) {
        o->x8 = 50;
    } else {
        o->x8 = 51;
    }
}
