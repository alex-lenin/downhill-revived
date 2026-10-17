/* UNIT 0x001A4CE0 0x001A4E08 */
#include "tween.h"
typedef struct Vec4 { float x, y, z, w; } Vec4;

typedef struct Part { char pad0[0x110]; } Part;

typedef struct Inner { char pad0[0x47F0]; char x47F0[1]; } Inner;

typedef struct Sub {
    char pad0[0xE74]; Vec4 xE74;
    char padE84[0x16D0 - 0xE84]; Inner *x16D0;
    char pad16D4[0x1C]; char x16F0[0x5C0];
    Part x1CB0[4];
} Sub;

typedef struct Obj {
    char pad0[0xDA4]; Sub *xDA4;
    char padDA8[0x40]; int xDE8;
    unsigned char xDEC;
} Obj;




extern void func_00202E38(Vec4 *, int, int, float);
extern void func_00204080(Vec4 *, int);

void func_001A4CE0(Obj *obj)
{
    Sub *sub;
    Part *part;
    Vec4 v;

    if (obj->xDEC == 0) {
        obj->xDEC = 1;
        obj->xDE8 = 1;
        SetSecondaryBlendWeightFirst((RiderSecondaryBlend *)(obj->xDA4->x16F0), 0.5f, 1000);
        func_001AE630((RiderBodyBlend *)(obj->xDA4->x16D0->x47F0), 1000, 0.5f);
        sub = obj->xDA4;
        part = &sub->x1CB0[0];
        SetRiderBlendWeightFirst((RiderBlendController *)(&sub->x1CB0[1]), 0.0f, 1000);
        SetRiderBlendWeightFirst((RiderBlendController *)(part), 0.0f, 1000);
        SetRiderBlendWeightFirst((RiderBlendController *)(&sub->x1CB0[2]), 0.0f, 1000);
        func_001B1698((RiderBlendController *)(&sub->x1CB0[3]), 1000, 0.0f);
        func_00202E38(&obj->xDA4->xE74, 1, 1, 1000.0f);
        v = obj->xDA4->xE74;
        func_00204080(&v, 1);
    }
}
