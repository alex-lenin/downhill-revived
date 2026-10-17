/* UNIT 0x0016AFC0 0x0016B060 .sdata=0x29C864:0x29C868 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

float D_0029C864 = 10.0f;

static inline void VecCopy(Vec *out, Vec *in)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(in) : "$2");
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

void func_0016AFC0(int *mode, Vec *normal, Vec *in, Vec *out)
{
    Vec down;
    Vec offset;
    Vec sum;

    VecCopy(out, in);
    if (*mode == 2) {
        down.x = 0.0f;
        down.y = 0.0f;
        down.z = -1.0f;
        VecScale(&offset, normal, -VecDot(normal, &down));
        VecAdd(&sum, &offset, &down);
        VecScale(out, &sum, D_0029C864);
    }
}
