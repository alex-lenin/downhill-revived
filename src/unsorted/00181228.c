/* UNIT 0x00181228 0x001812F8 */
typedef float Vec4[4] __attribute__((aligned(16)));

typedef struct Frame {
    char pad0[0x40];
    Vec4 mat[4];
} Frame;

typedef struct Obj {
    char pad0[8];
    Frame *x8;
} Obj;

typedef struct Quad {
    unsigned int w[4];
} Quad __attribute__((aligned(16)));

static inline void Cross(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz $ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0"
                      : "=m"(*(Quad *)out) : "r"(a), "r"(b));
}

static inline void Normalize(float *v)
{
    float x, y, z, len, inv;

    x = v[0];
    y = v[1];
    z = v[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v[0] = x * inv;
    v[1] = y * inv;
    v[2] = z * inv;
}

void func_00181228(Obj *obj, float *a, float *b)
{
    Frame *frame;
    Vec4 *mat;

    frame = obj->x8;
    mat = frame->mat;
    Cross(mat[0], a, b);
    Normalize(mat[0]);
    Cross(mat[1], b, mat[0]);
    Normalize(mat[1]);
    *(Quad *)frame->mat[2] = *(Quad *)b;
    mat[2][3] = 0.0f;
    mat[3][0] = 0.0f;
    mat[3][1] = 0.0f;
    mat[3][2] = 0.0f;
    mat[0][3] = 0.0f;
    mat[1][3] = 0.0f;
    mat[3][3] = 1.0f;
}
