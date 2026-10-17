/* UNIT 0x001B3F48 0x001B4000 */
#include "rider.h"
typedef SceneNode Frame;

typedef Rider Body;

typedef RiderComponent Component;

typedef struct Obj {
    char pad0[0x30];
    float x30[4];
    float x40[4];
    char pad50[0xC0 - 0x50];
    Component *rider;
    char padC4[0xF1 - 0xC4];
    unsigned char xF1;
    char padF2[2];
    float xF4;
} Obj;

static inline void ScaleVector(float *out, float *in, float scale)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"
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

/* out = x30 * t + (frame matrix * x40 + frame pos) * (1 - t) */
void func_001B3F48(Obj *obj, float *out)
{
    float tmp[4] __attribute__((aligned(16)));
    Frame *frame;
    float t;

    frame = obj->rider->rider->bodyNode;
    t = obj->xF4;
    if (obj->xF1) {
        t = 0.0f;
    }
    ApplyMatrix(tmp, &frame->matrix.row[0].x, obj->x40);
    AddVector(tmp, tmp, &frame->position.x);
    ScaleVector(tmp, tmp, 1.0f - t);
    ScaleVector(out, obj->x30, t);
    AddVector(out, out, tmp);
}
