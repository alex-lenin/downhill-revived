/* UNIT 0x0016A788 0x0016A860 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Other {
    char pad0[0x10];
    Vec x10;
} Other;

typedef struct Body {
    char pad0[0x38];
    Other *x38;
    char pad3C[0xC0 - 0x3C];
    Vec xC0;
} Body;

typedef struct Obj {
    char pad0[0x4];
    Body *x4;
    char pad8[0x4];
    float xC;
} Obj;

extern float D_0026EC00[];
extern float func_00248B90(float);
extern float func_0016AD98(float *values, float t);
extern void func_00169F48(void *);

static inline float InnerProduct(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline void Normalize(Vec *dst, Vec *src)
{
    float x = src->x, y = src->y, z = src->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    dst->x = x * inv;
    dst->y = y * inv;
    dst->z = z * inv;
}

int func_0016A788(Obj *obj)
{
    Body *body = obj->x4;
    Vec v;
    float angle;

    Normalize(&v, &body->xC0);
    angle = func_00248B90(InnerProduct(&v, &body->x38->x10)) * 57.29578f;
    if (angle < func_0016AD98(D_0026EC00, obj->xC)) {
        func_00169F48(obj);
        return 1;
    }
    return 0;
}
