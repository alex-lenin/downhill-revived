/* UNIT 0x00157590 0x00157658 */
#include "track_projection.h"
#if defined(__mips__)
#define CallTrackProjection func_00119180
#else
#define CallTrackProjection ProjectTrackDiscard
#endif
typedef struct Rider {
    char pad0[0x7978];
    int x7978;
} Rider;

typedef struct Owner {
    Rider *rider;
    char pad4[0x4C];
    TrackProjectionAngles x50;
} Owner;

extern char D_0029EA20[];
extern void func_00221F50(float *m, void *src);
#if defined(__mips__)
extern void func_00119180(void *a, float *b, float *c, void *d, int flag);
#endif

static inline void ApplyMatrix(float *out, float *m, float *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(in), "r"(m) : "memory");
}

static inline void AddVector(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(a), "r"(b) : "memory");
}

void func_00157590(Owner *owner, float *pos, void *src)
{
    float v[4] __attribute__((aligned(16)));
    float tmp[4] __attribute__((aligned(16)));
    float m[16] __attribute__((aligned(16)));

    func_00221F50(m, src);
    v[0] = 50.0f;
    v[1] = 0;
    v[2] = 0;
    if (owner->rider->x7978 == 0) {
        v[0] = -50.0f;
    }
    ApplyMatrix(tmp, m, v);
    AddVector(pos, pos, tmp);
    CallTrackProjection(D_0029EA20, pos, pos, owner->x50, 0);
}
