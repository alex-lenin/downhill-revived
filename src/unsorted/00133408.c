/* UNIT 0x00133408 0x00133518 .sdata=0x29C2C4:0x29C2C8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef union Vec4 {
    u128 q;
    float f[4];
} Vec4;

typedef struct Effect {
    char pad0[0x60];
    float scale;
    char pad64[0x11];
    unsigned char dir;
} Effect;

typedef struct Frame {
    char pad0[0x38];
    float *matrix;
} Frame;

float D_0029C2C4 = 0.005f;

extern float D_0026DDE0[][4];
extern int D_0029C2F0;
extern float D_0029C2F8;

extern u128 *func_001333C0(u128 *out, Effect *effect, u128 *in, int arg, Frame *frame);

static inline void ApplyMatrix(float *out, float *m, float *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(m), "r"(in) : "memory");
}

static inline float Dot(float *a, float *b)
{
    float result;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
             : "=f"(result) : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]) : "memory");
    return result;
}

u128 *func_00133408(u128 *out, Effect *effect, Vec4 *in, int arg, Frame *frame)
{
    Vec4 dir;
    Vec4 v;
    Vec4 result;
    float d;

    ApplyMatrix(dir.f, frame->matrix, D_0026DDE0[effect->dir]);
    effect->scale += Dot(in->f, dir.f) * D_0029C2C4;
    __asm__ volatile ("lq $2, %1\n\tsq $2, %0" : "=m"(v.q) : "m"(in->q) : "$2");
    if (D_0029C2F0 != 0) {
        d = -Dot(v.f, dir.f) * D_0029C2F8;
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0"
                          : "=m"(v) : "r"(&v), "r"(&dir), "f"(d) : "$2");
    }
    func_001333C0(&result.q, effect, &v.q, arg, frame);
    *out = v.q;
    return out;
}
