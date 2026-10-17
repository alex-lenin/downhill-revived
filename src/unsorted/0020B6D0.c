/* UNIT 0x0020B6D0 0x0020B880 .rodata=0x297E00:0x297E20 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

extern Vec D_00279A70[];

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void AddScaled(Vec *out, Vec *a, Vec *b, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline void CopyVec(Vec *dst, Vec *src) {
    COPY_VECTOR(*dst, src);
}

void func_0020B6D0(Vec *pos, Vec *basis, float radius)
{
    int idx[3] = {1, 2, 0};
    int colors[3] = {0xFF202080, 0x20FF2080, 0x2020FF80};
    Vec *vecs[3];
    Vec pts[4];
    float c;
    float s;
    float nc;
    int i;
    int j;

    if (basis == 0) basis = D_00279A70;
    for (j = 0; j < 3; j++) {
        vecs[j] = &basis[j];
        AddScaled(&pts[j], pos, &basis[j], radius);
        AddScaled(&pts[3], pos, vecs[j], -radius);
    }
    c = 1.0f;
    s = 0.0f;
    for (i = 0; i < 20; i++) {
        nc = c * 0.9510565f + s * 0.309017f;
        s = s * 0.9510565f - c * 0.309017f;
        c = nc;
        for (j = 0; j < 3; j++) {
            AddScaled(&pts[3], pos, vecs[j], c * radius);
            AddScaled(&pts[3], &pts[3], vecs[idx[j]], s * radius);
            CopyVec(&pts[j], &pts[3]);
        }
    }
}
