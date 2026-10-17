/* UNIT 0x00131630 0x00131998 */
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
#include "object_registry.h"
typedef float Vector[4] __attribute__((aligned(16)));

typedef struct Item {
    unsigned short x0, type, id, x6;
    int x8;
    unsigned long *xC;
    float position[4];
    float normal[4];
} Item;

typedef struct Group {
    Vector *x0;
    Vector *x4;
    float x8, xC, x10;
    Vector *x14;
    Vector *x18;
    int x1C;
    Item *x20;
    int x24;
} Group;

typedef struct Body {
    float x0[4];
    float x10[4];
    char pad20[0x10];
    float x30[4];
    float x40[4];
    float x50[4];
} Body;

class Obj {
public:
    virtual void call(int, Item *);
    virtual void unused();
    virtual void vector(float *, float *);
};

extern float D_0029C300, D_0029C330;
extern "C" void *func_00134F58(char *, int);
extern "C" void func_00134D70(void *, void *, char *);

extern "C" Item *func_002116E8(Group *, int, Item *);
extern "C" void func_002093C8(unsigned long *, float *, float *);


#define SCALE(dst, a, s) __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "f"(s) : "$2")
/* TODO: the "$3" clobber below is not written by the asm: it only steers reload into $v0 for the
 * scaled-vector pointer. Without it 24 of 217 words differ (build-try/cand-review/func_00131630.cpp). */
#define ADD(dst, a, b) __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b) : "$3")
#define DOT(r, v) __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(r) : "f"((v)[0]), "f"((v)[1]), "f"((v)[2]))
#define ZERO(dst) __asm__ volatile ("sqc2 $vf0, %0" : "=m"(dst))

extern "C" int func_00131630(char *a)
{
    Vector pos;
    Group groups[2];
    /* Wheel probe segments: start points, then end points (bp + 2 below). */
    struct { Vector before[2]; Vector after[2]; } ends;
    Vector scaled;
    int numWheels = 2;
    int maxPasses = 10;
    int i;
    int count;
    int result;
    Item *hit;
    Obj *o;
    float d;

    float r = ((Body *)func_00134F58(a, 2))->x0[3] * 1.5f;
    func_00134D70(func_00134F58(a, 2), &pos, a);

    Item hits[maxPasses];
    count = QuerySpatialHitsFloatFirst((Vector4 *)(&pos), (Vector4 *)(&pos), r, 1.0f, 1.0f, (SpatialHit *)(hits), maxPasses);
    if (count != 0)
        return count;

    Item probeHits[numWheels][maxPasses];

    Vector *axis0 = (Vector *)&((Body *)(a + 0x2E0))->x50;
    Vector *axis1 = (Vector *)&((Body *)(a + 0x360))->x50;
    Vector *sp;

    Vector *bp = &ends.before[0];
    for (i = 0; i < numWheels; i++) {
        groups[i].x0 = &ends.before[i];
        groups[i].x4 = &ends.after[i];
        sp = &scaled;
        groups[i].x20 = probeHits[i];
        groups[i].x14 = 0;
        groups[i].x18 = 0;
        groups[i].x1C = maxPasses;
        groups[i].x10 = 0.0f;
        groups[i].x8 = 1.0f;
        groups[i].xC = 1.0f;
        func_00134D70(func_00134F58(a, i), &ends.before[i], a);
        d = ((Body *)func_00134F58(a, i))->x0[3] + D_0029C300 + D_0029C330;
        Vector *axis;
        if (i == 0) {
            d = -d;
            axis = axis0;
        } else {
            d = -d;
            axis = axis1;
        }
        SCALE(*sp, axis, d);
        ADD(bp[2], bp[0], scaled);
        bp++;
    }

    QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(groups), numWheels);
    hit = func_002116E8(groups, numWheels, 0);

    result = 0;
    if (hit == 0) {
        result = 1;
    } else {
        ZERO(scaled);
        if (hit->xC != 0 && (*hit->xC & ((unsigned long)0x8000 << 43)) != 0) {
            func_002093C8(hit->xC, hit->position, scaled);
        } else {
            o = (Obj *)func_0017D3D0(hit->id);
            if (o != 0)
                o->vector(hit->position, scaled);
        }
        float dot;
        DOT(dot, scaled);
        if (dot > 0.0001f)
            result = 1;
    }
    return result;
}
