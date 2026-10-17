/* UNIT 0x001EA7C8 0x001EA918 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Rider {
    char pad0[0x4180];
    Vec x4180;
    char pad1[0xF0];
    Vec x4280;
    char pad2[0x10];
    Vec x42A0;
} Rider;

typedef struct Obj {
    Rider *x0;
    char pad4[0x1DC];
    float x1E0;
    float x1E4;
} Obj;

static inline void Normalize(Vec *dst, Vec *src) {
    float x = src->x, y = src->y, z = src->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1"
        : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    dst->x = x * inv;
    dst->y = y * inv;
    dst->z = z * inv;
}

static inline float Dot(Vec *a, Vec *b) {
    float d;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
        : "=f"(d) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return d;
}

extern float func_00236088(float cur, float target, float rate, float min);

void func_001EA7C8(Obj *obj) {
    Rider *base = obj->x0;
    Vec normal;
    Vec *u = &base->x4280;
    float dotA, dotB, a, b;

    Normalize(&normal, &base->x4180);
    dotA = Dot(&normal, &base->x42A0);
    dotB = Dot(&normal, u);
    a = 1.0f - (dotA + 1.0f) * 0.5f - 0.2f;
    b = 1.0f - (dotB + 1.0f) * 0.5f;
    if (a < 0.0f) a = 0.0f;
    obj->x1E0 = func_00236088(obj->x1E0, b, 0.05f, 0.01f);
    obj->x1E4 = func_00236088(obj->x1E4, a, 0.05f, 0.01f);
}
