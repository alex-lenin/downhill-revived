/* UNIT 0x001DD888 0x001DDA50 */
#include "path_motion.h"
typedef Vector4 Vec;

typedef PathNode Node;

typedef struct Obj {
    Vec pos;
    Vec vel;
    Vec dir;
    char pad30[0x58 - 0x30];
    float speed;
} Obj;

/* x30.w = segment length, x50.w / x5C = position along the track */
typedef PathTrack Track;



#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

#define VEC_ADD_SCALED(out, a, b, s)     __asm__ volatile ("lqc2 $vf11, 0x0(%1)
	lqc2 $vf12, 0x0(%2)
	mfc1 $2, %3
	qmtc2.ni $2, $vf13
	"                       "vaddax.xyz ACC, $vf11, $vf0x
	vmaddx.xyz $vf11, $vf12, $vf13x
	sqc2 $vf11, %0"                       : "=m"(out) : "r"(a), "r"(b), "f"(s) : "$2")

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline float VecDot(Vec *a, Vec *b)
{
    float dot;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
             : "=f"(dot) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return dot;
}

static inline float VecNormalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\t"
                      "lui $3,0x3f80\n\tmtc1 $3,%0\n\tnop\n\tdiv.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(len)
                      : "f"(x), "f"(y), "f"(z)
                      : "$3");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

void func_001DD888(Obj *obj, Track *track)
{
    Vec delta;
    Vec proj;
    Vec *dir;
    float scale;
    float ratio;
    Node *prev;
    Node *cur;
    float dot;

    if (obj->speed > 1e-10f) {
        scale = track->x80;
        dir = &obj->vel;
    } else {
        scale = track->x80;
        dir = &obj->dir;
    }
    VEC_ADD_SCALED(track->x60, &obj->pos, dir, scale);
    VecSub(&delta, &track->x60, &track->prev->pos);
    dot = VecDot(&track->x30, &delta);
    if (dot > track->x30.w) {
        dot = func_001DDA50(obj, track, &delta);
    }
    if (dot < 0.0f) {
        COPY_VECTOR(track->x50, &delta);
        track->x50.w = track->prev->x10;
    } else {
        VecScale(&proj, &track->x30, dot);
        VecSub(&track->x50, &delta, &proj);
        ratio = dot / track->x30.w;
        prev = track->prev;
        cur = track->cur;
        track->x50.w = prev->x10 + ratio * (cur->x10 - prev->x10);
    }
    track->x50.w = VecNormalize(&track->x50, &track->x50);
}
