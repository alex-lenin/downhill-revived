/* UNIT 0x00118DD8 0x00118EB0 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

struct Track {
    char pad0[0x1004C];
    int count;
};

extern float D_0029C124[];

extern Vec *func_001177F8(struct Track *track, int index);

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
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

/* Selects the point nearest to pos. */
int func_00118DD8(struct Track *track, Vec *pos)
{
    Vec delta;
    float best;
    float dist;
    int bestIndex;
    int i;

    best = D_0029C124[0];
    bestIndex = 0;
    for (i = 0; i < track->count; i++) {
        VecSub(&delta, pos, func_001177F8(track, i));
        dist = VecDot(&delta, &delta);
        if (dist < best) {
            best = dist;
            bestIndex = i;
        }
    }
    return func_001177F8(track, bestIndex);
}
