/* UNIT 0x00170C28 0x00170E60 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Matrix { Vec r[4]; } Matrix;
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst, src) __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" : "=m"((dst).r[0]), "=m"((dst).r[1]), "=m"((dst).r[2]), "=m"((dst).r[3]) : "r"(src) : "$8", "$9", "$10", "$11")
extern float fabsf(float);

static inline void Cross(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz $ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline float Normalize(Vec *v)
{
    float x, y, z, len, inv;
    x = v->x;
    y = v->y;
    z = v->z;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
    return len;
}

/* Build an orthonormal basis into *m from a and b, with fallbacks when
   the inputs are near-vertical (|z| >= 0.995): a x up, a x b, a x m->r[2],
   else reuse m->r[0]. The row added last is cleared, as are the w terms.
   First arg is unused (caller passes its cam). Labels Lc28_*: zero-byte
   scheduling pins (cf. src/unsorted/0010E9B0.c): LB/a keep the up stores
   above the first load in prologue order, b/c/d keep each z value live
   past its abs.s so it keeps its own register. */
void func_00170C28(void *p0, Matrix *m, Vec *a, Vec *b)
{
    Vec up;
    Matrix local;
    float t, at, cut;
    float one = 1.0f;
    (void)p0;
    up.x = 0;
    up.y = 0;
    __asm__ volatile ("Lc28_LB:");
    up.z = one;
    __asm__ volatile ("Lc28_a:");
    t = a->z;
    at = fabsf(t);
    __asm__ volatile ("Lc28_b:" : : "f"(t), "f"(at));
    cut = 0.995f;
    if (at < cut) {
        Cross(&local.r[0], a, &up);
    } else {
        t = b->z;
        at = fabsf(t);
        __asm__ volatile ("Lc28_c:" : : "f"(t), "f"(t));
        if (at < cut) {
            Cross(&local.r[0], a, b);
        } else {
            t = m->r[2].z;
            at = fabsf(t);
            __asm__ volatile ("Lc28_d:" : : "f"(t), "f"(t));
            if (at < cut) {
                Cross(&local.r[0], a, &m->r[2]);
            } else {
                COPY_VECTOR(local.r[0], &m->r[0]);
            }
        }
    }
    Normalize(&local.r[0]);
    Cross(&local.r[1], b, &local.r[0]);
    if (Normalize(&local.r[1]) < 0.005f) {
        Cross(&local.r[1], &up, b);
        Normalize(&local.r[1]);
    }
    Cross(&local.r[2], &local.r[0], &local.r[1]);
    Normalize(&local.r[2]);
    COPY_MATRIX(*m, &local);
    __asm__ volatile ("sqc2 $vf0,%0" : "=m"(m->r[3]));
    {
        float zero = 0.0f;
        m->r[2].w = zero;
        m->r[1].w = zero;
        m->r[0].w = zero;
    }
}
