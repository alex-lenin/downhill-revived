/* UNIT 0x001E86E8 0x001E8B70 */
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
    int x0, x4;
    char pad8[0xC];
    float x14;
    char pad18[0x3C];
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

static __inline__ void Prepare(unsigned char *p, Obj *obj)
{
    if (!p[0x10E6]) {
        func_001E5C68(p, obj->x2C);
    }
}

void func_001E86E8(Obj *obj)
{
    Vec tmp;
    char *base = obj->x0;
    unsigned char *other;
    char *part;
    Entry *entry;
    Link *link;
    int done;
    int changed;
    int next;
    int saved;
    float scale;
    float volume;
    float gain;
    float level;

    obj->x1F6 = 1;
    func_001E9A48(base + 0x5FF0, obj->x28);
    other = (unsigned char *)base + 0x6610;
    entry = &D_00272888[obj->x2C];
    part = base + 0x1E40;
    if (!other[0x10E6]) {
        func_001E5C00(other, obj->x2C);
    }
    done = 1;
    tmp = *get_part(part, obj->x34);
    func_00204280(&tmp);
    func_001E5198(other);
    obj->xC++;
    if (*(int *)(base + 0x78D4)) {
        if (obj->x1E8 >= 2) {
            done = 0;
        }
        obj->x1E8++;
    } else {
        done = func_001E5CC8(other, obj->x2C);
    }
    if (obj->x1F4) {
        done = 0;
    }
    changed = 0;
    scale = 1.0f;
    next = obj->x30;
    if (entry->x40 >= 0 && obj->x1F1) {
        next = func_001E9208(obj);
    }
    if (next) {
        link = &D_00273D60[next];
        obj->x1CC = 3;
        changed = 1;
        obj->x34 = link->x4;
        scale = link->x14 * func_001E5198(other) * *(float *)(part + 0x211C);
        Prepare((unsigned char *)base + 0x6610, obj);
        obj->x30 = next;
        obj->x1F3 = changed;
    }
    if ((!done && entry->x40 < obj->xC) || (entry->x40 == -1 && obj->x30 == 0) || obj->x1F2) {
        obj->x1CC = 5;
        scale = entry->x1C * func_001E5198(other) * *(float *)(part + 0x211C);
        obj->xC = 0;
        if (entry->xC) {
            obj->x34 = entry->xC;
        } else {
            obj->x34 = entry->x4;
            if (obj->x34) {
                tmp = *get_part(part, obj->x34);
                func_00204080(&tmp, 0);
            }
        }
        changed = 1;
    } else {
        saved = obj->x2C;
        if (func_001E5D90((unsigned char *)base + 0x6610, saved)) {
            obj->x1F4 = 1;
        }
        if (func_001E7DA0(obj)) {
            obj->x1CC = 5;
            scale = entry->x1C * func_001E5198(other) * *(float *)(part + 0x211C);
            obj->xC = 0;
            if (entry->xC) {
                obj->x34 = entry->xC;
            } else {
                obj->x34 = entry->x4;
                if (obj->x34) {
                    tmp = *get_part(part, obj->x34);
                    func_00204080(&tmp, 0);
                }
            }
            changed = 1;
        } else {
            obj->x2C = saved;
        }
    }
    if (changed) {
        func_001E9A48(base + 0x5FF0, obj->x28);
        tmp = *get_part(part, obj->x34);
        gain = entry->x28 * 0.01f;
        level = func_00204280(&tmp);
        volume = gain * level * func_001E5198(other) * 16.66667f;
        tmp = *get_part(part, obj->x34);
        func_00203E10(&tmp, scale);
        SelectAnimationTimeFirst(get_part(part, obj->x34), volume, 1, 1);
        if (entry->x10) {
            if (!entry->xC) {
                tmp = *get_base(base, obj->x34);
                func_00204080(&tmp, 0);
            }
            tmp = *get_base(base, obj->x34);
            func_00203E10(&tmp, scale);
            func_00202E38(get_base(base, obj->x34), 1, 1, volume);
        }
    }
    if (func_001E7BD8(obj)) {
        func_001EA7C8(obj);
    }
}
