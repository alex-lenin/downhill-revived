/* UNIT 0x00165408 0x00165560 */
#include "impulse.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Entry {
    char pad0[0x1C];
    float x1C;
    char pad20[0x4];
    float x24;
    float x28;
    char pad2C[0x4];
    float x30;
    char pad34[0x8C];
} Entry;

typedef struct Obj {
    char pad0[0x120];
    int x120;
} Obj;

extern Entry D_003960A0[];
extern float func_00222B68(float min, float max);



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

/* uses VU0 inline asm */
void func_00165408(Obj *obj, char *rider, Vec *dir)
{
    Entry *entry = &D_003960A0[obj->x120];
    Vec in;
    Vec pos;
    Vec rnd;
    Vec force;
    Vec lift;
    Vec push;

    rnd.x = func_00222B68(-0.4f, 0.4f);
    rnd.y = func_00222B68(-0.4f, 0.4f);
    rnd.z = func_00222B68(-1.0f, 1.0f) + entry->x30;
    /* vector copy (as sceVu0CopyVector) */
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(in) : "r"(&rnd) : "$2");
    func_001668C8((ImpulseBody *)obj, (Vector4 *)&pos, (const Vector4 *)&in);
    VecScale(&force, dir, entry->x28);
    func_00231CC0((ImpulseBody *)(obj), (Vector4 *)(&force), (Vector4 *)(&pos));
    lift.x = 0.0f;
    lift.y = 0.0f;
    lift.z = entry->x1C;
    lift.w = 0.0f;
    func_00231C88((ImpulseBody *)(obj), (Vector4 *)(&lift));
    VecScale(&push, (Vec *)(rider + 0x4180), entry->x24);
    func_00231C88((ImpulseBody *)(obj), (Vector4 *)(&push));
}
