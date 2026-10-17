/* UNIT 0x001AB418 0x001AB768 */
#include "spatial_batch.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Hit {
    char pad0[0x1c];
    float x1c;
    char pad20[0x10];
} Hit;

typedef struct Group {
    Vec *x0;
    Vec *x4;
    int x8;
    float xC;
    float x10;
    int x14;
    int x18;
    int x1C;
    Hit *x20;
    int x24;
} Group;

typedef struct State {
    char pad0[0x1c];
    int x1C;
    char pad20[8];
    Vec *x28;
    char pad2C[0x14];
    char x40[0x100];
} State;

typedef union Body {
    unsigned long x0;
    struct { unsigned long pad : 57; unsigned long flag : 1; unsigned long rest : 6; } bits;
} Body;

typedef struct Obj {
    char pad0[0x20];
    State *x20;
    Body *x24;
} Obj;

#define BIT57 ((unsigned long)1 << 57)

extern "C" int func_00238F30(void);
extern "C" void func_00232B60(void *obj, Vec *state);
extern "C" void func_00232BF8(void *obj, Vec *state);
extern "C" void func_002327B0(void *obj, float dt);
extern "C" void func_00231C10(void *obj);
extern "C" Hit *func_002116E8(Group *groups, int count, Hit *after);
extern "C" void func_001AAFC0(Obj *obj, Group *groups);
extern "C" int func_001AB0D0(Obj *obj, Hit *hit);
extern "C" void func_001AB768(Obj *obj, Vec *out, Vec *in);
extern "C" void func_00211280(Body *body);

static inline float maxf(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

extern "C" void func_001AB418(Obj *obj)
{
    Vec state[14];
    Group groups[8];
    Hit items[8][8];
    Vec posA[8];
    Vec posB[8];
    void *m;
    Hit *hit;
    float inv;
    float remaining;
    float step;
    int count;
    bool saved;
    int iter;
    int next;
    int i;
    int j;
    int k;

    inv = 1.0f / func_00238F30();
    count = obj->x20->x1C;
    m = obj->x20->x40;
    for (j = 0; j < count; j++) {
        groups[j].x8 = 0;
        groups[j].xC = 1.0f;
        groups[j].x0 = &posA[j];
        groups[j].x4 = &posB[j];
        groups[j].x20 = items[j];
        groups[j].x1C = 8;
        groups[j].x10 = obj->x20->x28[j].w;
        groups[j].x14 = 0;
        groups[j].x18 = 0;
    }
    saved = ((unsigned int)(obj->x24->x0 >> 57) & 1) != 0;
    obj->x24->x0 &= ~BIT57;
    remaining = 1.0f;
    iter = 0;
    do {
        func_00232B60(m, state);
        i = 0;
        bool active = i < count;
        step = inv * remaining;
        if (active) {
            do {
                func_001AB768(obj, &posA[i], &obj->x20->x28[i]);
                i++;
            } while (i < count);
        }
        func_002327B0(m, step);
        func_00231C10(m);
        for (k = 0; k < count; k++)
            func_001AB768(obj, &posB[k], &obj->x20->x28[k]);
        QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(groups), count);
        hit = func_002116E8(groups, count, 0);
        if (hit == 0)
            break;
        next = iter + 1;
        if (next == 3)
            hit->x1c = -1.0f;
        else
            func_00232BF8(m, state);
        if (hit->x1c < 0.0f) {
            func_001AAFC0(obj, groups);
        } else {
            for (;; hit = func_002116E8(groups, count, hit)) {
                if (hit != 0) {
                    func_002327B0(m, step * hit->x1c);
                    if (func_001AB0D0(obj, hit)) {
                        remaining = maxf(0.0f, remaining - remaining * hit->x1c);
                        break;
                    }
                    func_00232BF8(m, state);
                } else {
                    func_002327B0(m, step);
                    break;
                }
            }
        }
        iter = next;
    } while (remaining > 0.0f && next < 3);
    obj->x24->bits.flag = saved;
    func_00211280(obj->x24);
}
