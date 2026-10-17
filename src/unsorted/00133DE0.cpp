/* UNIT 0x00133DE0 0x001341D8 .sdata=0x29C2D0:0x29C2D4 */
#include "spatial_batch.h"
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Hit { char pad0[0x1c]; float x1c; char pad20[0x10]; } Hit;

typedef struct Group {
    Vec *x0;
    Vec *x4;
    int x8;
    float xc;
    float x10;
    int x14;
    int x18;
    int x1c;
    Hit *x20;
    int x24;
} Group;

typedef struct Saved { char pad[0xe0]; } Saved;

typedef struct Obj {
    char pad0[8];
    void *x8;
    char padc[4];
    char x10[0x12c - 0x10];
    int x12c;
    char pad130[0x150 - 0x130];
    Vec x150[1];
    char pad160[0x1d0 - 0x160];
    int x1d0;
} Obj;

int D_0029C2D0 = 0;

extern void func_001345A8(Obj *, Vec *, Vec *);
extern void func_0020B6D0(Vec *, int, float);
extern int func_00238F30(void);
extern void func_00232B60(void *, Saved *);
extern void func_00232BF8(void *, Saved *);
extern void func_002327B0(void *, float);
extern void func_00231C10(void *);
extern Hit *func_002116E8(Group *, int, Hit *);
extern void func_001341F8(Obj *, Group *);
extern int func_001341D8(Obj *, Hit *, int);
extern void func_00211280(void *);

void func_00133DE0(Obj *obj)
{
    Vec tmp;
    Saved saved;

    obj->x1d0++;
    if (D_0029C2D0 != 0) {
        for (int i = 0; i < obj->x12c; i++) {
            func_001345A8(obj, &tmp, &obj->x150[i]);
            func_0020B6D0(&tmp, 0, obj->x150[i].w);
        }
    }
    int n = obj->x12c;
    float inv = 1.0f / (float)func_00238F30();
    int count = obj->x12c;
    Group groups[n];
    Hit hits[n][8];
    Vec before[n];
    Vec after[n];
    float t;
    int pass;
    float scale;
    Hit *hit;
    Group *g;
    int j;
    int k;

    for (int i = 0; i < count; i++) {
        groups[i].x0 = &before[i];
        groups[i].x4 = &after[i];
        groups[i].x20 = hits[i];
        groups[i].x10 = obj->x150[i].w;
        groups[i].x8 = 0;
        groups[i].xc = 1.0f;
        groups[i].x1c = 8;
        groups[i].x14 = 0;
        groups[i].x18 = 0;
    }

    t = 1.0f;
    pass = 0;
    do {
        func_00232B60(obj->x10, &saved);
        scale = inv * t;
        for (int i = 0; i < count; i++)
            func_001345A8(obj, &before[i], &obj->x150[i]);
        func_002327B0(obj->x10, scale);
        func_00231C10(obj->x10);
        for (int i = 0; i < count; i++)
            func_001345A8(obj, &after[i], &obj->x150[i]);
        QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(groups), count);
        hit = func_002116E8(groups, count, 0);
        if (hit == 0)
            break;
        if (pass + 1 == 2)
            hit->x1c = -1.0f;
        else
            func_00232BF8(obj->x10, &saved);
        if (hit->x1c < 0.0f) {
            func_001341F8(obj, groups);
        } else {
            for (;;) {
                if (hit != 0) {
                    for (j = 0; ; j++) {
                        g = &groups[j];
                        k = hit - g->x20;
                        if (k >= 0 && k < g->x1c)
                            break;
                    }
                    func_002327B0(obj->x10, scale * hit->x1c);
                    if (func_001341D8(obj, hit, j)) {
                        float d = t - t * hit->x1c;
                        __asm__ volatile ("max.s %0,%1,%2" : "=&f"(t) : "f"(0.0f), "f"(d));
                        break;
                    }
                    func_00232BF8(obj->x10, &saved);
                } else {
                    func_002327B0(obj->x10, scale);
                    break;
                }
                hit = func_002116E8(groups, count, hit);
            }
        }
        pass++;
    } while (t > 0.0f && pass < 2);
    func_00211280(obj->x8);
}
