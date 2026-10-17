/* UNIT 0x001E9250 0x001E95B8 */
#include "animation.h"
struct Entry { char p0[12]; int xC, x10; char p14[12]; float x20; char p24[8]; float x2C, x30, x34, x38; char p3C[24]; };
struct VTable { char pad[0x28]; short delta; short pad2A; void (*call)(void *); };
struct Obj { char *x0; struct VTable *x4; int x8, xC; unsigned char x10; char pad11[0x17]; int x28, x2C, x30, x34; char pad38[0x194]; int x1CC; char pad1D0[0x10]; float x1E0, x1E4; char pad1E8[8]; unsigned char x1F0; char pad1F1[3]; unsigned char x1F4, x1F5; };
extern struct Entry D_00272888[];
extern struct Entry D_00273D60[];
extern int func_001E5CC8(void *, int);

extern void func_0012FE38(void *, void *);
extern int func_001EA070(struct Obj *);
extern int func_001E7DA0(struct Obj *);
extern int func_001E51A8(void);
extern int func_00138560(void *);
extern void func_001E59F8(void *);


extern float func_001E5198(void *);
extern void func_001E9CC0(void *, int, float);

extern void func_001AE210(void *, float);
extern float func_00236088(float, float, float, float);
/* Empty loop, e.g. a compiled-out debug macro. Its loop notes stop gcc's scheduler
   from moving instructions across it; the code depends on that ordering. */
#define NOP_BLOCK() do { } while (0)

static __inline__ AnimationHandle *get_part(char *part, int index) { return (AnimationHandle *)(part + (index * 16 + 0x14)); }
void func_001E9250(struct Obj *obj) {
    AnimationHandle tmp;
    char *base;
    char *part;
    char *other;
    char *other2;
    struct Entry *entry;
    float gain;
    float t1;
    float inv;
    float t2;
    float f;
    float delta;
    int old;
    int idx;
    int busy;
    int stride;
    struct Entry *tbl;
    base = obj->x0;
    idx = obj->x2C;
    part = base + 0x1E40;
    NOP_BLOCK();
    stride = sizeof(struct Entry);
    NOP_BLOCK();
    tbl = D_00272888;
    entry = (struct Entry *)((char *)tbl + idx * stride);
    other = base + 0x6610;
    busy = *(int *)(base + 0x78D4);
    /* same address as other, kept in its own register (s6) */
    other2 = part + (0x6610 - 0x1E40);
    if (!busy) {
        /* one-pass loop: doubles the weight of these references in register allocation */
        do {
            old = func_001E5CC8(other, obj->x28);
        } while (0);
        if (!old) {
            obj->x10 = 1;
            obj->x1F4 = 1;
        }
    }
    tmp = *get_part(part, obj->x34);
    if (((func_00204230(&tmp) ^ 1) & 0xFF)) {
        func_0012FE38(base, base + 0x4ED0);
    } else if (func_001EA070(obj) && func_001E7DA0(obj)) {
        old = obj->x1CC;
        obj->x1F5 = 1;
        if ((func_001E51A8() || (func_00138560((char *)obj->x0 + 0x40C0) & 0xFF)) && old != 4) {
            func_001E59F8(other2);
        }
        obj->x4->call((char *)obj + obj->x4->delta);
    }
    obj->xC++;
    gain = entry->x20 * 0.01f;
    idx = obj->x30;
    if (idx) {
        gain = D_00273D60[idx].x20 * 0.01f;
    }
    tmp = *get_part(part, obj->x34);
    t1 = func_00203C28(&tmp);
    inv = 1.0f - gain;
    tmp = *get_part(part, obj->x34);
    t2 = func_00204280(&tmp);
    f = t2 * func_001E5198(other);
    if (inv < t1 && !obj->x1F0) {
        obj->xC = 0;
        obj->x1F0 = 1;
        delta = f * gain * 16.66667f;
        func_001E9CC0(obj, 0, delta);
        SelectAnimationTimeFirst((AnimationHandle *)(part + 0x304), delta, 0, 1);
        if (entry->x10) {
            func_00202E38((AnimationHandle *)(base + 0x2F0), 0, 1, delta);
            func_001AE210(base + 0x48B0, delta);
        }
    }
    if (entry->x34 != 0.0f) {
        obj->x1E0 = func_00236088(obj->x1E0, 0.5f, 0.05f, 0.01f);
    }
    if (entry->x38 != 0.0f) {
        obj->x1E4 = func_00236088(obj->x1E4, 0.5f, 0.05f, 0.01f);
    }
}
