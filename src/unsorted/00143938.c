/* UNIT 0x00143938 0x001439A8 */
struct Obj {
    char pad0[0x10];
    char *x10;
    char pad14[0xC];
    float x20[4];
};

static inline void ApplyRot(float *dst, char *m, float *v)
{
    __asm__ volatile("lqc2 $vf11, 0(%0)" : : "r"(v) : "memory");
    __asm__ volatile("lqc2 $vf12, 0(%1)\n\tlqc2 $vf13, 16(%1)\n\tlqc2 $vf14, 32(%1)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(*dst) : "r"(m) : "memory");
}

static inline void AddVec(float *dst, char *a, float *b)
{
    __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*dst) : "r"(a), "r"(b) : "memory");
}

void func_00143938(struct Obj *p)
{
    float v[4] __attribute__((aligned(16)));

    v[0] = 0;
    v[1] = 20.0f;
    v[2] = 10.0f;
    ApplyRot(p->x20, p->x10 + 0x40, v);
    AddVec(p->x20, p->x10 + 0x10, p->x20);
}
