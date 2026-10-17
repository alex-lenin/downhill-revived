/* UNIT 0x001E8B70 0x001E9208 */
#include "animation.h"
typedef AnimationHandle Vec;

typedef struct Obj {
    char *x0;
    int x4, x8, xC;
    char pad10[0x18];
    int x28, x2C, x30, x34;
    char pad38[0x194];
    int x1CC;
    char pad1D0[0x18];
    int x1E8;
    char pad1EC[5];
    unsigned char x1F1;
    unsigned char x1F2;
    unsigned char x1F3;
    unsigned char x1F4;
    char pad1F5;
    unsigned char x1F6;
} Obj;

typedef struct Entry {
    int x0, x4, x8, xC, x10, x14;
    float x18;
    float x1C;
    char pad20[8];
    float x28;
    char pad2C[0x14];
    int x40;
    char pad44[0x10];
} Entry;

typedef struct Link {
    int x0, x4, x8, xC;
    char pad10[4];
    float x14;
    char pad18[0x28];
    int x40;
    char pad44[0x10];
} Link;

extern Entry D_00272888[];
extern Link D_00273D60[];
extern void func_001E9A48(void *, int);
extern void func_001E5C00(void *, int);
extern void func_001E5C68(void *, int);
extern int func_001E5CC8(void *, int);
extern int func_001E5D90(void *, int);
extern float func_001E5198(void *);
extern int func_001E9208(Obj *);
extern int func_001E7DA0(Obj *);
extern int func_001E7BD8(Obj *);
extern void func_001EA7C8(Obj *);





static __inline__ Vec *get_part(char *part, int index) { return (Vec *)(part + (index * 16 + 0x14)); }
static __inline__ Vec *get_base(char *base, int index) { return (Vec *)base + index; }

static __inline__ void Prepare(unsigned char *p, Obj *obj, char *base)
{
    if (!p[0x10E6]) {
        func_001E5C68(p, obj->x2C);
        func_001E9A48(base + 0x5FF0, obj->x28);
    }
}

extern void func_001E5C40(void *, int);
extern void func_001E9250(Obj *);



void func_001E8B70(Obj *obj)
{
    Vec first, second, third;
    char *base = obj->x0;
    char *setup = base + 0x5FF0;
    unsigned char *other = (unsigned char *)base + 0x6610;
    char *part = base + 0x1E40;
    Entry *entry;
    Link *link;
    int done;
    int saved;
    float scale, gain, level, volume;
    struct { unsigned char v; } stopped;
    func_001E9A48(setup, obj->x28);
    entry = &D_00272888[obj->x2C];
    link = &D_00273D60[obj->x30];
    if (!other[0x10E6]) func_001E5C40(other, obj->x2C);
    if (link->x40 == -1 || obj->x34 == link->xC) {
        func_001E9250(obj);
        return;
    }
    obj->xC++;
    func_001E9A48(setup, obj->x28);
    done = 1;
    if (*(int *)(base + 0x78D4)) {
        if (obj->x1E8 >= 2) done = 0;
        obj->x1E8++;
    } else done = func_001E5CC8(other, obj->x2C);
    if (done) {
        first = *get_part(part, obj->x34);
        stopped.v = (func_00204230(&first) ^ 1);
        if (stopped.v) {
            float scale;
            if (entry->x10) {
                second = *get_base(base, obj->x34);
                func_00203CB8(&second);
            }
            obj->x34 = link->x8;
            scale = link->x14 * func_001E5198(other) * *(float *)(part + 0x211C);
            second = *get_part(part, obj->x34);
            func_00203E10(&second, scale);
            SelectAnimationTimeFirst(get_part(part, obj->x34), 1.0f, 1, 1);
            if (entry->x10) {
                second = *get_base(base, obj->x34);
                func_00203E10(&second, scale);
                func_00202E38(get_base(base, obj->x34), 1, 1, 1.0f);
            }
        } else goto active;
    } else {
        if (obj->x34 == link->x4) {
            second = *get_part(part, obj->x34);
            stopped.v = (func_00204230(&second) ^ 1);
            if (!stopped.v) goto finish;
        }

        float scale, volume, level, gain;
        obj->x34 = link->xC;
        scale = link->x14 * func_001E5198(other) * *(float *)(part + 0x211C);
        Prepare((unsigned char *)base + 0x6610, obj, base);
        third = *get_part(part, obj->x34);
        gain = entry->x28 * 0.01f;
        level = func_00204280(&third);
        volume = gain * level * func_001E5198(other) * 16.66667f;
        third = *get_part(part, obj->x34);
        func_00203E10(&third, scale);
        SelectAnimationTimeFirst(get_part(part, obj->x34), volume, 1, 1);
        if (entry->x10) {
            if (!entry->xC) {
                third = *get_base(base, obj->x34);
                func_00204080(&third, 0);
            }
            third = *get_base(base, obj->x34);
            func_00203E10(&third, scale);
            func_00202E38(get_base(base, obj->x34), 1, 1, volume);
        }

    }
    goto finish;
active:
    {
            saved = obj->x2C;
            unsigned char *nextOther = (unsigned char *)base + 0x6610;
            if (func_001E5D90(nextOther, saved)) obj->x1F4 = 1;
            if (func_001E7DA0(obj) && obj->x34 != link->x4) {

        float scale, volume, level, gain;
        obj->x34 = link->xC;
        scale = link->x14 * func_001E5198(other) * *(float *)(part + 0x211C);
        if (!nextOther[0x10E6]) {
            func_001E5C68(nextOther, obj->x2C);
            func_001E9A48(base + 0x5FF0, obj->x28);
        }
        second = *get_part(part, obj->x34);
        gain = entry->x28 * 0.01f;
        level = func_00204280(&second);
        volume = gain * level * func_001E5198(other) * 16.66667f;
        second = *get_part(part, obj->x34);
        func_00203E10(&second, scale);
        SelectAnimationTimeFirst(get_part(part, obj->x34), volume, 1, 1);
        if (entry->x10) {
            if (!entry->xC) {
                second = *get_base(base, obj->x34);
                func_00204080(&second, 0);
            }
            second = *get_base(base, obj->x34);
            func_00203E10(&second, scale);
            func_00202E38(get_base(base, obj->x34), 1, 1, volume);
        }

            } else obj->x2C = saved;
    }

finish:
    if (func_001E7BD8(obj)) func_001EA7C8(obj);
}
