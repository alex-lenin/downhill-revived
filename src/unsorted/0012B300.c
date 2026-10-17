/* UNIT 0x0012B300 0x0012B448 */
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

typedef struct Matrix { Vec4 mat[4]; } Matrix;
extern void *memcpy(void *, const void *, unsigned int);
void func_0012B300(Obj *obj, float *a, float *b)
{
    Matrix local;
    __asm__ volatile ("lq $2,0(%1)
	sq $2,%0" : "=m"(local.mat[2]) : "r"(b) : "$2");
    Normalize(local.mat[2]);
    Cross(local.mat[0], a, local.mat[2]);
    Normalize(local.mat[0]);
    Cross(local.mat[1], local.mat[2], local.mat[0]);
    Normalize(local.mat[1]);
    if (obj->x8) {
        __asm__ volatile ("lq $8,0(%1)
	lq $9,16(%1)
	lq $10,32(%1)
	lq $11,48(%1)
	sq $8,64(%0)
	sq $9,80(%0)
	sq $10,96(%0)
	sq $11,112(%0)" : : "r"(obj->x8), "r"(&local) : "$8", "$9", "$10", "$11", "memory");
        obj->x8->mat[3][0] = 0.0f;
        obj->x8->mat[3][1] = 0.0f;
        obj->x8->mat[3][2] = 0.0f;
        obj->x8->mat[3][3] = 1.0f;
        obj->x8->mat[0][3] = 0.0f;
        obj->x8->mat[1][3] = 0.0f;
        obj->x8->mat[2][3] = 0.0f;
    }
}
