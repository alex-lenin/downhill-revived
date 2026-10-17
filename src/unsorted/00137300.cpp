/* UNIT 0x00137300 0x001377A8 .sdata=0x29C3B0:0x29C3B4 */
/* Uses the game's VU0 vector and EE FPU helpers (inline asm). */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef union Vec4 {
    u128 q;
    float f[4];
} Vec4;

typedef struct Body {
    Vec4 x0;
    Vec4 x10;
} Body;

typedef struct Obj {
    char pad0[0x38];
    Body *x38;
    char pad3C[0xC0 - 0x3C];
    Vec4 xC0;
    char padD0[0x158 - 0xD0];
    unsigned char *x158;
    char pad15C[0x160 - 0x15C];
    int x160;
    char pad164[0x250 - 0x164];
    Vec4 x250;
    Vec4 x260;
    Vec4 x270;
    Vec4 x280;
} Obj;

extern "C" float D_0029C3B0 = 0.9825000167f;

extern "C" char *func_00138558(char *p);
extern "C" float func_001385A8(void *p);
extern "C" float func_001ADA20(void *p);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecScale(Vec4 *dst, Vec4 *a, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "f"(s) : "$2");
}

static inline void VecAdd(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VecAddScaled(Vec4 *dst, Vec4 *a, Vec4 *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline void VecCross(Vec4 *dst, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void Set(Vec4 *out, float x, float y, float z)
{
    out->f[0]=x;
    out->f[1]=y;
    out->f[2]=z;
}

static inline float Dot(Vec4 *a, Vec4 *b)
{
    float d;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                      : "=f"(d)
                      : "f"(a->f[0]), "f"(b->f[0]),
                        "f"(a->f[1]), "f"(b->f[1]),
                        "f"(a->f[2]), "f"(b->f[2]));
    return d;
}

/* normalizes v into out (1/sqrt + div); z goes in twice, like the Dot(v, v) helpers */
static inline Vec4 *Normalize(Vec4 *out, Vec4 *v)
{
    float a = v->f[0];
    float b = v->f[1];
    float c = v->f[2];
    float inverse, length;
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %5\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inverse), "=f"(length) : "f"(a), "f"(b), "f"(c), "f"(c) : "$2");
    out->f[0] = a * inverse;
    out->f[1] = b * inverse;
    out->f[2] = c * inverse;
    return out;
}

/* same, with x passed twice */
static inline Vec4 *NormalizeX(Vec4 *out, Vec4 *v)
{
    float a = v->f[0];
    float b = v->f[1];
    float c = v->f[2];
    float inverse, length;
    __asm__ volatile ("mula.s %2, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inverse), "=f"(length) : "f"(a), "f"(a), "f"(b), "f"(c) : "$2");
    out->f[0] = a * inverse;
    out->f[1] = b * inverse;
    out->f[2] = c * inverse;
    return out;
}

static inline float Rsqrt(float one, float len)
{
    float inv;
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(one), "f"(len));
    return inv;
}

static inline void Unit(Vec4 *v)
{
    float length;
    float inverse;
    float a = v->f[0], b = v->f[1], c = v->f[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                      : "=&f"(length) : "f"(a), "f"(b), "f"(c));
    inverse = Rsqrt(1.0f, length);
    v->f[0] = a * inverse;
    v->f[1] = b * inverse;
    v->f[2] = c * inverse;
}

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

static inline float MinF(float a, float b)
{
    return a <? b;
}

extern "C" void func_00137300(Obj *obj)
{
    Vec4 v0;
    Vec4 v1;
    Vec4 v2;
    Vec4 v3;
    Vec4 v4;
    Vec4 *dir;
    Vec4 *bodyDir;
    Vec4 *out;
    Vec4 *target;
    Vec4 *clear2;
    Vec4 *clear250;
    float scale;
    float one;
    float one2;
    float amount;
    float dot;

    if (obj->x160 == 2) {
        Vec4 *specialDir;
        float specialDot;
        float specialOne;
        float specialScale;

        specialDir = &obj->x270;
        VecScale(specialDir, specialDir, D_0029C3B0);
        COPY_VECTOR(v4, func_00138558((char *)obj) + 0x20);
        Set(&v0,0.0f,0.0f,1.0f);
        specialDot = Dot(&v4, &v0);
        VecScale(&v1, &v4, specialDot);
        VecAdd(&v2, &v1, &v0);
        specialOne = 1.0f;
        VecScale(&v3, &v2, specialOne);
        specialScale = specialOne - D_0029C3B0;
        VecAddScaled(specialDir, specialDir, &v3, specialScale);
        COPY_VECTOR(obj->x280, specialDir);
        return;
    }

    scale = 0.9825000167f;
    if (obj->x158[0x7B9F] != 0) {
        scale = 0.85f;
    }
    dir = &obj->x270;
    VecScale(dir, dir, scale);
    one = 1.0f;
    scale = one - scale;
    target = &obj->x250;
    VecAddScaled(dir, dir, target, scale);

    clear250 = target;
    bodyDir = &obj->x38->x10;
    amount = MinF(func_001ADA20(obj->x158 + 0x48B0) * 0.5f, one);
    target = dir;
    if (0.0f < amount) {
        VecCross(&v0, bodyDir, target);
    } else {
        VecCross(&v0, target, bodyDir);
    }

    Normalize(&v0, &v0);

    out = NormalizeX(&obj->x280, &obj->x270);

    amount = Abs(amount);
    VecScale(&v0, &v0, amount);

    COPY_VECTOR(v1, &obj->x38->x0);
    VecAddScaled(&v1, &v1, out, -Dot(&v1, out));
    Unit(&v1);

    COPY_VECTOR(v2, &obj->xC0);
    VecAddScaled(&v2, &v2, out, -Dot(&v2, out));
    Unit(&v2);

    dot = Dot(&v1, &v2);
    one2 = 1.0f;
    amount = MinF(one2, func_001385A8(obj));
    amount *= Abs(dot);
    amount *= 0.75f;
    VecAddScaled(&v0, &v0, &v2, -amount);
    VecScale(out, out, one2 - amount);
    VecAdd(out, out, &v0);
    Normalize(out, out);

    obj->x250.f[0] = 0.0f;
    clear250->f[1] = 0.0f;
    clear250->f[2] = 0.0f;
    clear2 = &obj->x260;
    obj->x260.f[0] = 0.0f;
    clear2->f[1] = 0.0f;
    clear2->f[2] = 0.0f;
}
