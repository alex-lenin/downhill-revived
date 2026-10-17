/* UNIT 0x00118048 0x00118138 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "mfc1 $2, %2\n"
        "qmtc2.ni $2, $vf12\n"
        "vmulx.xyz $vf11, $vf11, $vf12x\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline float VecDot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile (
        "mula.s %1, %2\n"
        "madda.s %3, %4\n"
        "madd.s %0, %5, %6\n"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}


/* Distance from pos to its projection on the line origin->dir, with the
   height difference weighted 4x more. */
float func_00118048(Vec *origin, Vec *dir, Vec *pos)
{
    Vec d;
    Vec p;
    Vec proj;
    Vec diff;
    float result;
    float len;
    float t;
    float x, y, z, m;

    result = 0.0f;
    VecSub(&d, dir, origin);
    VecSub(&p, pos, origin);
    t = VecDot(&d, &p);
    len = VecDot(&d, &d);
    if (len != 0.0f) {
        VecScale(&proj, &d, t / len);
        VecAdd(&proj, &proj, origin);
        VecSub(&diff, pos, &proj);
        x = diff.x;
        y = diff.y;
        z = diff.z;
        __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0" : "=&f"(result) : "f"(x), "f"(y), "f"(z));
        m = __builtin_fabsf(z) * 4.0f;
        /* zero-byte label keeps z live past abs.s, so abs.s does not reuse z's register */
        __asm__ volatile ("L00118048_abs:" : : "f"(z));
        result += m;
    }
    return result;
}
