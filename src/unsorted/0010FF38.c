/* UNIT 0x0010FF38 0x001100D0 */
typedef struct Vec { float x, y, z; } Vec;

typedef struct Frame {
    char pad0[0x40];
    Vec x40;
    char pad4C[0x50 - 0x4C];
    Vec x50;
} Frame;

typedef struct Owner {
    char pad0[0x4564];
    short x4564;
    char pad4566[0x792C - 0x4566];
    Frame *x792C;
} Owner;

typedef struct Sub { char pad[0x14]; unsigned short x14; } Sub;

typedef struct Obj {
    Owner *x0;
    char pad4[0x70];
    Sub *x74;
    char pad78[0x1E0 - 0x78];
    Vec x1E0;
    char pad1EC[0x250 - 0x1EC];
    float x250;
    char pad254[0x278 - 0x254];
    float x278;
    char pad27C[0x284 - 0x27C];
    float x284;
    float x288;
} Obj;

static inline float Dot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

extern void func_001122D8(Obj *, void *);
extern float func_00138580(void *);
extern void func_001101B8(Obj *, float);
extern void func_001100D0(Obj *, float);

void func_0010FF38(Obj *obj)
{
    float t;
    float add;
    float limit;
    float e;
    float k416;
    float k785;
    Frame *f;
    Vec *r1;
    Vec *r0;
    Vec *d;
    float d1;
    float d0;
    float w;
    float result;

    func_001122D8(obj, (char *)obj->x0->x792C + 0x10);
    t = func_00138580((char *)obj->x0 + 0x40C0) * obj->x288;
    limit = t + 5.0f;
    if (obj->x0->x4564 >= 121) {
        /* Define k416 here too (zero bytes) so it is defined on both paths
           and spans the branch; the allocator then colors e first ($f0)
           and k416 takes $f1 (retail allocation). */
        __asm__ volatile ("Lff38_kd:" : "=f" (k416));
        e = obj->x284;
    }
    else {
        float fs = (float)obj->x0->x4564;
        /* Zero-byte label: scheduling barrier pinning the constant load
           below the conversion (retail order). */
        __asm__ volatile ("Lff38_e0:");
        k416 = 0.4166667f;
        e = fs * k416;
    }
    limit += e;
    /* Second use of k416 past the join keeps it live across it. */
    __asm__ volatile ("Lff38_e2:" : : "f" (k416));
    if (limit < 5.0f)
        limit = 5.0f;
    add = 0.0f;
    f = obj->x0->x792C;
    r1 = &f->x50;
    r0 = &f->x40;
    d = &obj->x1E0;
    d1 = Dot(r1, d);
    d0 = Dot(r0, d);
    w = obj->x250;
    if (d0 * d1 * w < 0.0f) {
        float ax = obj->x278;
        add = __builtin_fabsf(ax);
        /* Zero-byte label (cf. src/unsorted/0012F8D8.c): scheduling barrier
           so the constant load stays below abs.s (retail order); using ax
           extends its live range over the load. */
        __asm__ volatile ("Lff38_k1:" : : "f" (ax));
        k785 = 0.7853982f;
        /* Extra use of k785 within its live range (no extension) to tie its
           coloring with ax so ax takes $f0 and k785 takes $f1 (retail). */
        __asm__ volatile ("Lff38_k1u:" : : "f" (k785));
        if (w < k785) {
            float m = add * __builtin_fabsf(w);
            /* Zero-byte label: scheduling barrier so the 1.2732395f
               constant loads stay below the first mul.s (retail order). */
            __asm__ volatile ("Lff38_k2:");
            add = m * 1.2732395f;
        }
    }
    result = limit + add;
    func_001101B8(obj, result);
    if (!((obj->x74->x14 >> 5) & 1))
        func_001100D0(obj, result);
}
