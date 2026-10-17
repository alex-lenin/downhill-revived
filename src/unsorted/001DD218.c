/* UNIT 0x001DD218 0x001DD350 */
#include "motion_interpolation.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    Vec pos;
    Vec vel;
    Vec dir;
    Vec force;
    float x40;
    float maxSpeed;
    float x48;
    float x4C;
    float maxForce;
    char pad54[0x58 - 0x54];
    float speed;
} Obj;

typedef struct Target {
    char pad0[0x30];
    Vec x30;
    char pad40[0x50 - 0x40];
    float x50[3];
    float x5C;
    char pad60[0x6C - 0x60];
    float x6C;
} Target;

extern void func_001DD888(Obj *obj, Target *target);
extern void func_001DD350(Obj *obj, Vec *v);

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

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

#if defined(__mips__)
void func_001DD218(Obj *obj, Vec *out, Target *target, float gain, float scale)
#else
void SharedCall001DD218(struct Shared001DD218_Obj * obj_view, struct Vector4 * out_view, struct Shared001DD218_Target * target_view, float gain, float scale)
#endif
{
#if !defined(__mips__)
    Obj * obj = (Obj *)obj_view;
    Vec * out = (Vec *)out_view;
    Target * target = (Target *)target_view;
#endif

    float d, lo, cur;

    func_001DD888(obj, target);
    lo = target->x5C;
    cur = target->x6C;
    if (cur < lo) {
        /* The do/while (0) blocks (likely macros in the original) are needed to match:
           their loop notes stop the scheduler hoisting the x4C load above the abs. */
        do {
            cur -= lo;
            d = cur * gain;
            lo = __builtin_fabsf(d);
        } while (0);
        do {
            if (lo > obj->x4C) {
                if (d > 0.0f) {
                    d = obj->x4C;
                } else {
                    d = -obj->x4C;
                }
            }
            VecScale(out, (Vec *)target->x50, d);
        } while (0);
        if (obj->x48 < scale) {
            scale = obj->x48;
        }
        VecAddScaled(out, out, &target->x30, scale);
        func_001DD350(obj, out);
    } else if (obj->speed < obj->maxSpeed) {
        VecScale(out, &obj->dir, obj->x48);
    }
}
