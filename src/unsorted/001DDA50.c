/* UNIT 0x001DDA50 0x001DDB58 */
#include "path_motion.h"
typedef Vector4 Vec;

typedef PathNode Node;

typedef PathTrack Track;

extern void func_001DDBA0(Track *track);
extern Node *func_00163DA0(int a, Node *node, int b);
extern void func_001DD7E8(void *obj, Vec *a, Vec *b, Node *cur, Node *next, float scale);

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecCopy(Vec *out, Vec *v)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(v) : "$2", "memory");
}

static inline float VecDot(Vec *a, Vec *b)
{
    float dot;
    __asm__ ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
             : "=f"(dot) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return dot;
}

float func_001DDA50(void *obj, Track *track, Vec *out)
{
    float result = 0.0f;
    int x0;

    track->prev = track->cur;
    track->cur = track->next;
    func_001DDBA0(track);
    x0 = track->x0;
    if (track->cur->x16 == 0) {
        __asm__ ("sqc2 $vf0, %0" : "=m"(track->x30));
        __asm__ ("sqc2 $vf0, %0" : "=m"(track->x50));
        track->x50.w = 0.0f;
    } else {
        track->next = func_00163DA0(x0, track->next, 0);
        VecCopy(&track->x30, &track->x40);
        func_001DD7E8(obj, &track->x30, &track->x40, track->cur, track->next, 1.0f);
        if (out != 0) {
            VecSub(out, &track->x60, (Vec *)track->prev);
            result = VecDot(&track->x30, out);
        }
    }
    return result;
}
