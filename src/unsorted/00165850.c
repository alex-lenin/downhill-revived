/* UNIT 0x00165850 0x00165B90 .sdata=0x29C80C:0x29C810 */
typedef struct { float v[4]; } Vector;
typedef struct { char pad[0x2c]; float x2c; char tail[0x90]; } Settings;
typedef struct Obj {
    char p0[0x34];
    Vector *x34;
    char *x38;
    char p3c[0xc4];
    char *x100;
    char p104[0x1c];
    int x120;
    char p124[0xc];
    Vector x130;
    unsigned char x140;
    char p141[0x4f];
    int x190;
    char p194[8];
    unsigned char x19c;
    char p19d[0xb];
    int x1a8, x1ac, x1b0, x1b4;
    char p1b8[0x258];
    char x410[0x30];
    Vector x440;
} Obj;

extern Settings D_003960A0[];
float D_0029C80C = 12.0f;
extern int func_001656A0(Obj *);
extern void func_00165DE8(Obj *);
extern void func_00165E68(Obj *);
extern void func_00166B30(Obj *, float);
extern void func_00166BB0(Obj *, Vector *, float);
extern void func_00166D28(Obj *, Vector *, float);
extern void func_00202E38(void *, int, int, float);
extern void func_00203E10(Vector *, float);
extern void func_00221888(void *, Vector *, Vector *);
extern void func_00232A80(Obj *, Vector *, Vector *);

static inline void Scale(Vector *dst, Vector *src, float k) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(src), "f"(k) : "$2");
}

static inline void AddScaled(Vector *dst, Vector *a, Vector *b, float k) {
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b), "f"(k) : "$2");
}

static inline float Dot(Vector *a, Vector *b) {
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->v[0]), "f"(b->v[0]), "f"(a->v[1]), "f"(b->v[1]), "f"(a->v[2]), "f"(b->v[2]));
    return r;
}

void func_00165850(Obj *o) {
    Vector a, b, c;
    Settings *s;
    int limit;
    float force, sign, tmp, z, absZ, lateral;

    s = &D_003960A0[o->x120];
    limit = o->x120 == 1 ? 6 : 2;
    o->x1ac++;
    if (o->x19c) {
        if (!o->x140 && o->x1b4 >= limit) {
            AddScaled(&a, o->x34, (Vector *)(o->x38 + 0x20), 6.0f);
            func_00232A80(o, &b, &a);
            func_00221888(o->x38, &b, &c);
            func_00166BB0(o, &c, s->x2c);
            func_00232A80(o, &b, o->x34);
            func_00221888(o->x38, &b, &c);
            func_00166D28(o, &c, s->x2c);
        } else if (o->x120 == 0) {
            /* tmp first holds |dot| and later the scaled force: being one
               variable, it is dead across the call, which keeps the sub.s
               in the call's delay slot, and its second life gives it $f20. */
            tmp = __builtin_fabsf(Dot((Vector *)(o->x100 + 0x50), &o->x130));
            sign = 1.0f;
            force = 1.0f - tmp;
            func_00221888(o->x100 + 0x40, &o->x130, &a);
            tmp = force * 400.0f;
            tmp = -tmp;
            b.v[0] = tmp * a.v[0];
            z = a.v[2];
            /* Zero-byte labels (cf. src/unsorted/0010E9B0.c): the first keeps
               abs.s after the b.x store; the second keeps the a.y load and the
               0.0f after abs.s, and its two uses of absZ give it allocation
               priority over tmp ($f1 vs $f20, retail). */
            __asm__ volatile ("L165850_a:");
            absZ = __builtin_fabsf(z);
            __asm__ volatile ("L165850_b:" : : "f"(absZ), "f"(absZ));
            if (a.v[1] < 0.0f)
                sign = -1.0f;
            lateral = tmp * absZ;
            b.v[1] = lateral * sign;
            b.v[2] = 0.0f;
            func_00166BB0(o, &b, 0.05f);
            b.v[0] = -b.v[0];
            func_00166D28(o, &b, 0.05f);
            c = o->x440;
            func_00203E10(&c, 0.1f);
        } else {
            Scale(&b, &o->x130, D_0029C80C);
            func_00166BB0(o, &b, s->x2c);
            func_00232A80(o, &a, o->x34);
            func_00221888(o->x38, &a, &b);
            func_00166D28(o, &b, s->x2c);
            c = o->x440;
            func_00203E10(&c, 2.0f);
        }
    }
    func_00165DE8(o);
    func_00165E68(o);
    if (func_001656A0(o)) {
        o->x190 = 1;
        o->x1a8 = 0;
        if (o->x19c)
            func_00166B30(o, 100.0f);
        func_00202E38(o->x410, 1, 1, 200.0f);
    }
}
