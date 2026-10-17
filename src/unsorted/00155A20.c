/* UNIT 0x00155A20 0x00155BA8 */
/* func_00155A20: rider-lean update. Normalizes the rider heading, dots it
   against the body direction, derives xA4 (lean) and xA8 (steer), then drives
   the state machine (func_00155E78) from magnitude thresholds.
   Struct offsets match the promoted sibling src/unsorted/00155BA8.c
   (Rider.x4180/x4ECC/x792C). */
typedef struct Body {
    char pad0[0x50];
    float dir[3];
} Body;

typedef struct Rider {
    char pad0[0x4180];
    float dir[3];
    char pad418C[0x4ECC - 0x418C];
    void *x4ECC;
    char pad4ED0[0x5FF0 - 0x4ED0];
    char x5FF0[0x792C - 0x5FF0];
    Body *body;
} Rider;

typedef struct Obj {
    Rider *rider;
    char pad4[0x70 - 4];
    float x70;
    char pad74[0xA4 - 0x74];
    float xA4, xA8;
    char padAC[0xC8 - 0xAC];
    long flags; /* bit 2 set = skip update */
} Obj;

extern int func_001E51A8(void);
extern int func_001EA070(void *);
extern void func_00155E78(Obj *, int);
extern float fabsf(float);

/* EE FPU-extra sequences (mula/madda/madd/rsqrt); allowed inline asm. */
static inline void normalize(float *dir, float *p) {
    float x, y, z, len, inv;
    x = p[0]; y = p[1]; z = p[2];
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    dir[0] = x * inv; dir[1] = y * inv; dir[2] = z * inv;
}

static inline float dotProduct(float *dir, float *p) {
    float result;
    __asm__ volatile("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(result) : "f"(dir[0]), "f"(dir[1]), "f"(dir[2]), "f"(p[0]), "f"(p[1]), "f"(p[2]));
    return result;
}

/* Zero-byte label keeps the abs input live so the abs.s result gets its own
   FPU register ($f1) instead of reusing the input ($f0); same device as the
   promoted src/unsorted/001656A0.c (Lfabs_pre/post) and 0012DEB0.c (Lj00:).
   The volatile load reproduces retail's delay-slot scheduling of the
   final branch. */
static inline float absolute(volatile float *v) {
    float t;
    float m;
    t = *v;
    m = fabsf(t);
    __asm__ volatile ("Lfabs_post:" : : "f" (t));
    return m;
}

void func_00155A20(Obj *obj, float *arg1) {
    float dir[3];
    float dot;
    void *part;
    float a4;
    float c;
    if (obj->flags & 4)
        return;
    {
        normalize(dir, obj->rider->dir);
        dot = dotProduct(dir, obj->rider->body->dir);
        obj->xA8 = (*arg1 * 100.0f) / obj->x70;
        if (dot < 0.0f)
            obj->xA4 = dot * -333.3333f;
        else
            obj->xA4 = 0.0f;
        part = obj->rider->x5FF0;
        if (func_001E51A8())
            goto check;
        if (obj->rider->x4ECC != obj->rider->x5FF0)
            goto check;
        if (func_001EA070(part))
            goto check;
        func_00155E78(obj, 7);
        return;
    check:
        /* Named locals: one 100.0f quantity in $f2 across both compares, and
           the xA4 load hoisted into the branch delay slots as in retail. */
        a4 = obj->xA4;
        c = 100.0f;
        if (a4 > c) {
            func_00155E78(obj, 2);
            return;
        }
        if (absolute(&obj->xA8) > c)
            func_00155E78(obj, 2);
    }
}
