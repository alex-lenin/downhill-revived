/* UNIT 0x001103B0 0x001105F8 */
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
    char pad78[0xD0 - 0x78];
    Vec xD0;
    char padDC[0x240 - 0xDC];
    float x240;
    char pad244[0x250 - 0x244];
    float x250;
    float x254;
    char pad258[0x278 - 0x258];
    float x278;
    char pad27C[0x3BC - 0x27C];
    int x3BC;
} Obj;
static inline float Dot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}
extern int D_004B26AC[];
extern void func_0010B2B8(Obj *, int);
extern float func_001385A8(void *);
extern void func_00155E78(void *, int);
void func_001103B0(Obj *obj, float limit)
{
    float add;
    float x240;
    Frame *f;
    Vec *r1;
    Vec *r0;
    Vec *d;
    float d1;
    float d0;
    float w;
    float k785;
    add = 0.0f;
    obj->x3BC++;
    f = obj->x0->x792C;
    r1 = &f->x50;
    r0 = &f->x40;
    d = &obj->xD0;
    d1 = Dot(r1, d);
    d0 = Dot(r0, d);
    w = obj->x250;
    if (d0 * d1 * w < 0.0f) {
        float ax = obj->x278;
        add = __builtin_fabsf(ax);
        __asm__ volatile ("Lb30_k1:" : : "f" (ax));
        k785 = 0.7853982f;
        __asm__ volatile ("Lb30_k1u:" : : "f" (k785));
        if (w < k785) {
            float m = add * __builtin_fabsf(w);
            __asm__ volatile ("Lb30_k2:");
            add = m * 1.2732395f;
        }
    }
    if (limit < 5.0f)
        limit = 5.0f;
    x240 = obj->x240;
    if (obj->x0->x4564 >= 61) {
        float w2 = obj->x250;
        float a2 = __builtin_fabsf(w2);
        float k349;
        __asm__ volatile ("Lb30_m1:" : : "f" (w2));
        k349 = 0.34906584f;
        __asm__ volatile ("Lb30_m1u:" : : "f" (k349));
        if (a2 > k349 && x240 < 70.0f)
            limit = x240 + add + 1.0f;
    }
    if (x240 < limit + add) {
        func_0010B2B8(obj, 0);
    } else if (obj->x3BC > 180) {
        float v = obj->x254;
        float m;
        float c;
        float b;
        __asm__ volatile ("Lb30_c0:" : "=f" (c));
        __asm__ volatile ("Lb30_m0:" : "=f" (m));
        m = __builtin_fabsf(v);
        __asm__ volatile ("Lb30_fm1:" : : "f" (m));
        __asm__ volatile ("Lb30_b0:" : "=f" (b));
        c = 0.2617994f;
        b = obj->x250;
        __asm__ volatile ("Lb30_v1:" : : "f" (v));
        if (m + c < __builtin_fabsf(b)) {
            func_0010B2B8(obj, 0);
        } else if (D_004B26AC[0] != 15) {
            if (func_001385A8((char *)obj->x0 + 0x40C0) < 0.1f || obj->x3BC > 480) {
                if (!((obj->x74->x14 >> 5) & 1))
                    func_00155E78((char *)obj->x0 + 0x3FD0, 1);
                obj->x3BC = 0;
            }
        }
    }
}
