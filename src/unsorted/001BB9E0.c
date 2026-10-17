/* UNIT 0x001BB9E0 0x001BBD50 */
#include "timer.h"
#include "tween.h"
/* State machine update for a rider object (states in obj->x18):
   0: count to 19, then go to 1; 1: count to 39 (reset), test sample, else go
   to 2; 3: test sample, else advance rider state; any state with a trigger
   condition set goes to 3 first. */
typedef struct Value { int x[4]; } Value;

/* Mode call: vtable pointer at +4; slot at +0x20 holds a 16-bit `this`
   displacement and the function (same device as src/unsorted/00170850.c). */
struct Vtable { char pad0[0x20]; short delta; short pad22; int (*call)(); };
struct Mode { int x0; struct Vtable *vt; };
#define VCALL(o) ((o)->vt->call((char *)(o) + (o)->vt->delta))

typedef struct Rider {
    char pad0[0x1190];
    int x1190;
    int x1194;
    int x1198;
    int x119C;
    char pad11A0[0x1258 - 0x11A0];
    int x1258;
    int x125C;
    int x1260;
    int x1264;
    char pad1268[0x13D7 - 0x1268];
    unsigned char x13D7;
    unsigned char x13D8;
    unsigned char x13D9;
    unsigned char x13DA;
    char pad13DB[0x1E40 - 0x13DB];
    char x1E40[0x40C0 - 0x1E40];
    char x40C0[0x4580 - 0x40C0];
    char x4580[0x47F0 - 0x4580];
    char x47F0[0x48B0 - 0x47F0];
    int x48B0;
    char pad48B4[0x4ED0 - 0x48B4];
    char x4ED0[0x5E40 - 0x4ED0];
    struct Mode x5E40;
    char pad5E48[0x5E90 - 0x5E48];
    struct Mode x5E90;
    char pad5E98[0x62E0 - 0x5E98];
    struct Mode x62E0;
    char pad62E8[0x7A78 - 0x62E8];
    float x7A78;
} Rider;

typedef struct Obj {
    Rider *x0;
    int x4;
    int x8;
    int xC;
    float x10;
    float x14;
    int x18;
} Obj;

extern void func_001AC578(void *p);
extern void func_0012FE38(Rider *r, void *p);
extern void func_001BBD80(Obj *obj);
extern void func_001ACFA0(void *p, int flag);



extern void func_001AD580(void *p, int v, float f1, float f2);
extern void func_00136030(void *p, int v);
extern unsigned char func_00204230(Value *v);
extern char D_00759780[];
extern void func_002625D0(void *);

void func_001BB9E0(Obj *obj) {
    Rider *rider;
    char *sub;
    struct Mode *modeA;
    struct Mode *modeB;
    char *part;
    char *p3530;
    Value sample;
    int condA;
    int condB;
    int sel;
    int flags;

    rider = obj->x0;
    sub = rider->x1E40;
    modeA = &rider->x62E0;
    func_001AC578(sub);
    if (VCALL(modeA) != 0) {
        func_0012FE38(rider, modeA);
        return;
    }
    /* Each byte at 0x13D7-0x13DA selects between the 0x1190 and 0x1258 value. */
    condA = 0;
    if ((rider->x13D8 ? rider->x125C : rider->x1194) ||
        (rider->x13D7 ? rider->x1258 : rider->x1190) ||
        rider->x7A78 != 0.0f ||
        (rider->x13D9 ? rider->x1260 : rider->x1198) ||
        (sel = rider->x13DA ? rider->x1264 : rider->x119C, sel))
        condA = 1;
    modeB = &rider->x5E40;
    condB = 0;
    if (VCALL(modeB) != 0 || (modeB = &rider->x5E90, VCALL(modeB) != 0))
        condB = 1;
    flags = condA | condB;
    if (obj->x18 != 3) {
        if (flags != 0) {
            part = rider->x4580;
            obj->x18 = 3;
            func_001BBD80(obj);
            func_001ACFA0(part, 0);
            rider->x48B0 = 0;
            part[4] = 1;
            /* sub+0x1FE0 is rider+0x3E20. */
            SetRiderBlendDiscard((RiderBlendController *)(sub + 0x1FE0), (int)obj->x10, 1.0f);
            /* 1/(ms per frame at 60fps); folds to 0x3D75C28C. */
            func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_002625D0), obj, obj->x10 * (1.0f / 16.66667f));
            func_00136030(rider->x40C0, 1);
        }
    }
    if (obj->x18 == 0) {
        float zero;
        obj->xC++;
        if (obj->xC >= 0x13) {
            obj->x18 = 1;
            /* sub+0x16F0 is rider+0x3530. */
            p3530 = sub + 0x16F0;
            func_001BBD80(obj);
            zero = 0.0f;
            func_001B1698((RiderBlendController *)(sub + 0x1FE0), (int)obj->x10, zero);
            func_001B3170((RiderSecondaryBlend *)(p3530), (int)obj->x10, zero);
            func_001AE630((RiderBodyBlend *)(rider->x47F0), (int)obj->x10, zero);
        }
    }
    if (obj->x18 == 1) {
        int off;
        obj->xC++;
        if (obj->xC >= 0x27) {
            func_001AD580(rider->x4580, 0, 1.0f, 0.8f);
            obj->xC = 0;
        }
        off = obj->x8 * 16 + 20;
        sample = *(Value *)(sub + off);
        /* True unless the test returned 1. */
        if ((func_00204230(&sample) ^ 1) & 0xFF) {
            obj->x18 = 2;
            func_001BBD80(obj);
        }
    } else if (obj->x18 == 3) {
        int off;
        off = obj->x8 * 16 + 20;
        sample = *(Value *)(sub + off);
        if ((func_00204230(&sample) ^ 1) & 0xFF)
            func_0012FE38(rider, rider->x4ED0);
    }
}
