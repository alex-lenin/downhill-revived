/* UNIT 0x00236438 0x00236558 */
typedef float Vector[4];

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static __inline__ float dot(float *a, float *b)
{
    float r;
    __asm__ volatile ("lwc1 $f5,0(%1)\n\tlwc1 $f4,0(%2)\n\tlwc1 $f3,4(%1)\n\tlwc1 $f2,4(%2)\n\t"
                      "lwc1 $f1,8(%1)\n\tlwc1 $f0,8(%2)\n\t"
                      "mula.s $f5,$f4\n\tmadda.s $f3,$f2\n\tmadd.s %0,$f1,$f0"
                      : "=f"(r) : "r"(a), "r"(b) : "$f0", "$f1", "$f2", "$f3", "$f4", "$f5");
    return r;
}

static __inline__ float root(float a)
{
    float r;
    __asm__ volatile ("sqrt.s %0,%1" : "=f"(r) : "f"(a));
    return r;
}

static __inline__ void scale(float *dst, float *src, float s)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\t"
                      "vmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"
                      : : "r"(dst), "r"(src), "f"(s) : "$2", "memory");
}

/* Splits a into its component along b (x) and the perpendicular rest (y), rescales them
   (x by positive/negative depending on sign, y by tangent) and writes a scaled down to
   the new length, or a unchanged. Uses inline asm for VU0 and EE FPU extras. */
void func_00236438(float *a, float *b, float *out, float positive, float tangent, float negative)
{
    Vector v;
    float x;
    float y;
    float length;
    float target;

    x = dot(b, a);
    scale(v, b, x);
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%0)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"
                      : : "r"(v), "r"(a) : "memory");
    __asm__ volatile ("lwc1 $f3,0(%1)\n\tlwc1 $f2,4(%1)\n\tlwc1 $f1,8(%1)\n\t"
                      "mula.s $f3,$f3\n\tmadda.s $f2,$f2\n\tmadd.s %0,$f1,$f1"
                      : "=f"(y) : "r"(v) : "$f1", "$f2", "$f3");
    y = root(y);
    length = root(y * y + x * x);
    if (0.0f < length) {
        y = y * tangent / length;
        x = (0.0f < x ? x * positive : x * negative) / length;
    }
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(target) : "f"(y * y + x * x));
    if (target < length) {
        scale(out, a, target / length);
    } else {
        COPY_VECTOR(*(Vector *)out, a);
    }
}
