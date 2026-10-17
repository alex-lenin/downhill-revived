/* UNIT 0x001DDBA0 0x001DDC70 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Track {
    int x0;
    Vec *prev;
    Vec *cur;
    int xC;
    Vec pos;
    Vec x20;
    char pad30[0x70 - 0x30];
    Vec offset;
    float x80;
    unsigned char x84;
} Track;

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecCross(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vopmula.xyz ACC, $vf11, $vf12\n\t"
                      "vopmsub.xyz $vf11, $vf12, $vf11\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void MatApply(Vec *out, Vec *m, Vec *v)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%2)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "lqc2 $vf13, 0x10(%1)\n\t"
                      "lqc2 $vf14, 0x20(%1)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\t"
                      "vmadday.xyz ACC, $vf13, $vf11y\n\t"
                      "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(m), "r"(v));
}

void func_001DDBA0(Track *track)
{
    Vec m[4];
    Vec *dir;
    Vec *up;

    if (track->cur == 0 || track->prev == 0) {
        return;
    }
    track->pos = track->cur[0];
    track->x20 = track->cur[1];
    if (track->x84 == 0) {
        return;
    }
    VecSub(&m[1], track->cur, track->prev);
    dir = &m[1];
    up = &m[2];
    m[2].x = 0.0f;
    m[2].y = 0.0f;
    up->z = 1.0f;
    VecCross(&m[0], dir, up);
    VecCross(up, &m[0], dir);
    MatApply(&track->pos, m, &track->offset);
    VecAdd(&track->pos, &track->pos, track->cur);
}
