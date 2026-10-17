/* UNIT 0x0016A860 0x0016A990 */
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Basis { Vec x0, x10; } Basis;
typedef struct Part {
    char pad0[0x36];
    unsigned short type : 5;
    unsigned short x36_5 : 11;
    char pad38[0x48];
} Part;
typedef struct Rider {
    char pad0[0x38];
    Basis *x38;
    char pad3C[0x2E0 - 0x3C];
    Part x2E0;
    Part x360;
    long x3E0;
} Rider;
typedef struct Obj {
    int x0;
    Rider *x4;
    float x8, xC;
    Vec x10, x20, x30, x40;
} Obj;

extern float D_0029C86C, D_0029C878, D_0029C87C;
__asm__(".extern D_0029C86C, 4\n\t.extern D_0029C878, 4\n\t.extern D_0029C87C, 4");
extern float func_00248B90(float);
extern void func_00169F48(Obj *);

static inline float dot(Vec *a, Vec *b) {
    float out;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(out):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return out;
}

static inline float GetTilt(Obj *obj) {
    Vec c;
    float one = 1.0f;
    c.x = 0.0f; c.y = 0.0f; c.z = one;
    return (1.0f - dot(&c, &obj->x10)) * 90.0f;
}

int func_0016A860(Obj *obj) {
    Rider *r = obj->x4;
    float angle, t, lean;   /* t is reused for the last limit: keeps it out of lean's register */
    int sa, sb;
    sa = r->x2E0.type;
    sb = r->x360.type;
    if (sa == 0x17 || sb == 0x17) return 2;
    do {
        t = func_00248B90(dot(&obj->x10, &obj->x4->x38->x0)) * 0.31830988618379067153f - 0.5f;
        t += t;
        lean = __builtin_fabsf(t);
    } while (0);
    angle = GetTilt(obj);
    if ((lean < D_0029C86C && angle < D_0029C87C) || angle < (t = D_0029C878)) {
        func_00169F48(obj);
        return 2;
    }
    return 0;
}
