/* UNIT 0x00126B20 0x00126D30 */
#include "animation.h"
typedef AnimationHandle Sound;

typedef struct Body {
    char pad0[0x40];
    char x40[0x10];
    char pad50[0xA0 - 0x50];
    char xA0[0x10];
} Body;

typedef struct VTable {
    char pad0[0x20];
    short delta;
    short pad22;
    Body *(*getBody)(void *);
} VTable;

typedef struct Group {
    VTable *vt;
    unsigned int **kind;
    char pad8[8];
    Sound sounds[1];
} Group;

typedef struct Obj {
    char pad0[8];
    unsigned int state;
    char padC[0x18 - 0xC];
    int x18;
    char pad1C[0x40 - 0x1C];
    int indices[(0xDC - 0x40) / 4];
    float xDC;
    char padE0[0x130 - 0xE0];
    Group *group;
} Obj;

extern "C" void func_00222460(void *, void *);




extern "C" float func_00222B68(float, float);

#define GET_BODY(g) ((g)->vt->getBody((char *)(g) + (g)->vt->delta))

extern "C" void func_00126B20(Obj *obj, unsigned int state)
{
    Sound *entry;
    unsigned int kind;
    long sel;

    obj->x18 = 0;
    if (state < 2) {
        func_00222460(GET_BODY(obj->group)->x40, GET_BODY(obj->group)->xA0);
    }
    if (obj->state == state) return;
    obj->state = state;
    entry = &obj->group->sounds[obj->indices[state]];
    sel = (long)entry;
    if (entry->x0 == 0) return;
    if ((**obj->group->kind >> 18) == 0x1018) {
        { Sound copy = *entry; func_00203DC8((const AnimationHandle *)&copy, 0); }
    } else {
        { Sound copy = *entry; func_00203DC8((const AnimationHandle *)&copy, 1); }
    }
    kind = **obj->group->kind >> 18;
    if (kind == 0x1018) {
        SelectAnimationLegacyAddress(sel, 1, 1, 100.0f);
    } else if (kind == 0x101D || kind == 0x1022) {
        SelectAnimationLegacyAddress(sel, 1, 1, 250.0f);
    } else {
        float f = func_00222B68(0.0f, 1.0f);
        { Sound copy = *entry; func_00203B68((const AnimationHandle *)&copy, 1, f); }
        SelectAnimationLegacyAddress(sel, 0, 1, 250.0f);
        { Sound copy = *entry; func_00203E10((const AnimationHandle *)&copy, obj->xDC); }
    }
}
