/* UNIT 0x001E9F10 0x001EA070 */
#include "tween.h"
typedef struct Entry {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    float x14;
    float x18;
    char pad1C[0xC];
} Entry;

typedef struct Sub {
    char pad0[0x3530];
    char x3530[0x5C0];
    char x3AF0[0x110];
    char x3C00[0x110];
    char x3D10[0x110];
    char x3E20[0x9D0];
    char x47F0[0x10];
} Sub;

typedef struct Obj {
    Sub *sub;
    char pad4[0x4];
    char x8;
    char pad9[0x23];
    int x2C;
    int x30;
} Obj;

extern Entry D_002736A8[];
extern Entry D_00274BD0[];





void func_001E9F10(Obj *obj, int alt, float time)
{
    Sub *sub;
    Entry *entry;
    char *x3530;
    char *x3AF0;

    obj->x8 = 1;
    sub = obj->sub;
    entry = &D_002736A8[obj->x2C];
    if (alt != 0) {
        entry = &D_00274BD0[obj->x30];
    }
    x3530 = sub->x3530;
    x3AF0 = sub->x3AF0;
    if (entry->x4 != 0) {
        func_001B1698((RiderBlendController *)(sub->x3E20), (int)time, 0.0f);
    }
    if (entry->x8 != 0) {
        func_001B1698((RiderBlendController *)(sub->x3D10), (int)time, 0.0f);
    }
    if (entry->xC != 0) {
        func_001B1698((RiderBlendController *)(sub->x3C00), (int)time, 0.0f);
    }
    if (entry->x10 != 0) {
        func_001B1698((RiderBlendController *)(x3AF0), (int)time, 0.0f);
    }
    if (entry->x14 < 1.0f) {
        func_001B3170((RiderSecondaryBlend *)(x3530), (int)time, entry->x14);
    }
    if (entry->x18 < 1.0f) {
        func_001AE630((RiderBodyBlend *)(sub->x47F0), (int)time, entry->x18);
    }
}
