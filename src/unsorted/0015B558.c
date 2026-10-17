/* UNIT 0x0015B558 0x0015B798 */
typedef float Vec4[4] __attribute__((aligned(16)));
typedef struct Frame { char pad[0x40]; Vec4 mat[4]; } Frame;
typedef struct Obj { char pad[8]; Frame *x8; } Obj;
typedef struct Quad { unsigned int w[4]; } Quad __attribute__((aligned(16)));

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

static inline void Cross(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0"
                      : "=m"(*(Quad *)out) : "r"(a), "r"(b));
}

static inline void Copy(float *out, float *src)
{
    __asm__ volatile ("lq $2,0(%1)\n\tsq $2,%0" : "=m"(*(Quad *)out) : "r"(src) : "$2");
}

static inline float Normalize(float *v)
{
    float x, y, z, len, inv;
    x = v[0]; y = v[1]; z = v[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                      : "=&f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v[0] = x * inv; v[1] = y * inv; v[2] = z * inv;
    return len;
}

void func_0015B558(Obj *obj, float *a, float *b)
{
    Vec4 up;
    Vec4 m[4];
    float *dst;
    Frame *frame;
    float az, absaz, zero, limit;

    up[0] = 0.0f; up[2] = 1.0f; up[1] = 0.0f;
    frame = obj->x8;
    dst = frame->mat[0];
    az = a[2];
    absaz = Abs(az);
    limit = 0.995f;
    if (absaz < limit) Cross(m[0], a, up);
    else if (Abs(b[2]) < limit) Cross(m[0], a, b);
    else if (Abs(dst[10]) < limit) Cross(m[0], a, frame->mat[2]);
    else Copy(m[0], dst);
    Normalize(m[0]);
    Cross(m[1], b, m[0]);
    if (Normalize(m[1]) < 0.005f) { Cross(m[1], up, b); Normalize(m[1]); }
    Cross(m[2], m[0], m[1]);
    Normalize(m[2]);

    __asm__ volatile ("lq $8,0(%1)\n\tlq $9,16(%1)\n\tlq $10,32(%1)\n\tlq $11,48(%1)\n\tsq $8,0(%0)\n\tsq $9,16(%0)\n\tsq $10,32(%0)\n\tsq $11,48(%0)"
                      : : "r"(dst), "r"(m) : "$8", "$9", "$10", "$11", "memory");
    __asm__ volatile ("sqc2 $vf0,0x30(%0)" : : "r"(dst) : "memory");
    zero = 0.0f;
    dst[11] = zero;
    dst[7] = zero;
    dst[3] = zero;
}
