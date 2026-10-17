/* UNIT 0x00111160 0x00111368 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Transform { char pad0[0x40]; Vec x40, x50; } Transform;
typedef struct Owner { char pad0[0x40C0]; char x40C0[0xC0]; Vec x4180; char pad4190[0x379C]; Transform *x792C; } Owner;
typedef struct Params { char pad0[0x78]; float x78, x7C; } Params;
typedef struct Rider {
    Owner *x0;
    char pad4[0x1FC];
    Vec x200;
    char pad210[0x40];
    float x250;
    char pad254[0x28];
    float x27C;
    char pad280[0x3C];
    float x2BC;
    char pad2C0[0x14];
    Params x2D4;
    char pad354[0xEC];
    unsigned long x440;
} Rider;
typedef struct Control {
    char pad0[0x1B8];
    int x1B8, x1BC;
    char pad1C0[0xC0];
    int x280, x284;
    char pad288[0x171];
    unsigned char x3F9, x3FA;
} Control;

extern int func_00138560(void *);
extern float fabsf(float);

static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}
/* Same dot product with $f2 clobbered: retail keeps $f2 out of the operands
   of both second-half dot products (they use $f6 instead). */
static inline float DotF2(Vec *a, Vec *b) {
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z) : "$f2");
    return r;
}
static inline void Normalize(Vec *out, Vec *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    out->x = x * inv; out->y = y * inv; out->z = z * inv;
}

void func_00111160(Rider *rider, Control *control) {
    float projection;
    Params *params;

    if (!(func_00138560(rider->x0->x40C0) & 255)) return;
    if (rider->x440 & (0xC000UL << 37)) return;
    projection = Dot(&rider->x0->x792C->x50, &rider->x200);
    params = &rider->x2D4;
    if (projection < params->x7C) {
        int one = 1;
        if (control->x3F9) control->x280 = one;
        else { control->x1B8 = one; control->x280 = 0; }
    } else if (params->x78 < projection) {
        int one = 1;
        if (control->x3FA) control->x284 = one;
        else { control->x1BC = one; control->x284 = 0; }
    }
    if (rider->x2BC < 0.6f) {
        Vec tmp;
        int active;
        float value, absolute;

        active = 0;
        if (rider->x2BC > 0.5f) {
            Normalize(&tmp, &rider->x0->x4180);
            value = DotF2(&rider->x0->x792C->x40, &tmp);
        } else {
            value = DotF2(&rider->x0->x792C->x40, &rider->x200);
        }
        /* Zero-byte label: keeps abs.s out of the delay slot of the jump
           from the first branch (retail has b + nop). */
        __asm__ volatile("L00111160_abs:");
        absolute = fabsf(value);
        if (absolute > 0.2f) active = 1;
        /* Keeps |value| live into the next block so it is allocated
           globally ($f2) with value in $f1, as retail. */
        __asm__ volatile("L00111160_live:" : : "f"(absolute));
        if (active) { rider->x250 = value; rider->x27C = value; }
    }
}
