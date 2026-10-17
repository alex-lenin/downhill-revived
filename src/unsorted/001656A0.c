/* UNIT 0x001656A0 0x001657F8 */
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Sub { char pad[0x10]; Vec v; } Sub;
typedef struct Entry { char pad0[0x18]; float x18; char pad1C[0x1C]; int x38; char pad3C[0x84]; } Entry;
typedef struct Obj {
    char pad0[0x38];
    Sub *x38;
    char pad3C[0x84];
    Vec xC0;
    Vec xD0;
    char padE0[0x40];
    int x120;
    char pad124[0xC];
    Vec x130;
    unsigned char x140;
    char pad141[0x6B];
    int x1AC;
} Obj;
extern Entry D_003960A0[];
int func_001656A0(Obj *obj) {
    int idx;
    Entry *e;
    float len2;
    float len1;
    float d;
    float dot;
    float tmp;
    int ok;
    Vec *pC0;
    Vec *pD0;
    Vec *a;
    Vec *b;
    idx = obj->x120;
    e = &D_003960A0[idx];
    ok = 0;
    pC0 = &obj->xC0;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len1) : "f"(pC0->x), "f"(pC0->y), "f"(pC0->z));
    pD0 = &obj->xD0;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len2) : "f"(pD0->x), "f"(pD0->y), "f"(pD0->z));
    d = 0.0f;
    if (obj->x140) {
        a = &obj->x38->v;
        b = &obj->x130;
        __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
        /* Zero-byte labels (cf. 0010FF38/001100D0): keep dot/tmp separate so fabs is abs+mov via $f0 (retail). */
        __asm__ volatile ("Lfabs_pre:" : "=f"(tmp));
        tmp = __builtin_fabsf(dot);
        __asm__ volatile ("Lfabs_post:" : : "f"(tmp), "f"(dot));
        d = tmp;
    }
    if (idx == 0) {
        if (d > 0.9f && len1 < e->x18 && len2 < e->x18)
            ok = 1;
        else if (d > 0.92f)
            ok = 1;
    } else if (idx == 1) {
        if (len1 < e->x18 && len2 < e->x18)
            ok = 1;
    }
    return (obj->x1AC > e->x38) ? 1 : ok;
}
