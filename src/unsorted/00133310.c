/* UNIT 0x00133310 0x001333C0 */
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

extern float D_0026DDE0[][4];
extern int D_0029C2F0;

extern void func_00232A80(Frame *frame, float *out, float *in);

static inline void ScaleVector(float *out, float *in, float scale)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(in), "f"(scale) : "$2", "memory");
}

static inline void ApplyMatrix(float *out, float *m, float *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(m), "r"(in) : "memory");
}

static inline void AddVector(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(a), "r"(b) : "memory");
}

void func_00133310(Effect *effect, float *pos, float *in, Frame *frame)
{
    float tmp[4] __attribute__((aligned(16)));

    func_00232A80(frame, pos, in);
    if (D_0029C2F0 != 0) {
        float *dir;

        dir = D_0026DDE0[effect->dir];
        ScaleVector(tmp, dir, effect->scale);
        ApplyMatrix(tmp, frame->matrix, tmp);
        AddVector(pos, pos, tmp);
    }
}
