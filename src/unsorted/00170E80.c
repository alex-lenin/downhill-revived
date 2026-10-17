/* UNIT 0x00170E80 0x00171050 */
#include "impulse.h"
#include "tween.h"
#include "animation.h"
typedef float Vector[4];
typedef struct Health { char pad0[0xC4]; int xC4; } Health;
typedef struct Obj { char *x0; int x4, x8, xC; } Obj;
extern void func_001ACFA0(void *,int), func_001AD700(void *,float), func_00136030(void *,int);
extern void func_001AE1D0(void *,float);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void scale_vector(Vector *out, Vector *in, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(in), "f"(scale) : "$2");
}
static inline float as_float(int *value) { return *value; }
void func_00170E80(Obj *obj) {
    char *base = obj->x0;
    Health *health = (Health *)(base + 0x3FD0);
    char *a = base + 0x4580;
    char *b = base + 0x40C0;
    char *c = base + 0x1E40;
    Vector value;
    float duration;
    unsigned int frames = 96;
    obj->x8 = health->xC4;
    func_001ACFA0(a,1);
    func_001AD700(a,0.0f);
    *(int *)(base + 0x48B0) = 1;
    func_00136030(b,1);
    COPY_VECTOR(value, base + 0x4120);
    scale_vector(&value,&value, (float)health->xC4 * -0.005f);
    func_00231C88((ImpulseBody *)(b), (Vector4 *)(&value));
    if (obj->x8 >= 66) obj->xC = 231;
    else if (obj->x8 >= 33) obj->xC = 233;
    else obj->xC = 234;
    duration = (float)(unsigned int)frames;
    func_001AE1D0(base + 0x48B0,duration);
    SelectAnimationTimeFirst((AnimationHandle *)(c + (obj->xC * 16 + 20)), duration, 1, 1);
    if (obj->xC == 231) func_00202E38((AnimationHandle *)(base + 0xE70), 1, 1, duration);
    { char *component = c + 0x1CB0;
    func_001B3170((RiderSecondaryBlend *)(c + 0x16F0), frames, 0.0f);
    func_001AE630((RiderBodyBlend *)(base + 0x47F0), frames, 0.0f);
    func_001B1698((RiderBlendController *)(c + 0x1DC0), frames, 0.0f);
    func_001B1698((RiderBlendController *)(component), frames, 0.0f);
    func_001B1698((RiderBlendController *)(c + 0x1ED0), frames, 0.0f);
    func_001B1698((RiderBlendController *)(c + 0x1FE0), frames, 0.0f);
    }
    *(int *)(base + 0x4094) = 0;
}
