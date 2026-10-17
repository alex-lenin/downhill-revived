/* UNIT 0x001A4E08 0x001A4F70 */
#include "tween.h"
typedef struct Name {
    int words[4];
} Name;

typedef struct Body {
    char pad0[0x304];
    Name x304;
    char pad314[0x16D0 - 0x314];
    char *x16D0;
    char pad16D4[0x16F0 - 0x16D4];
    char x16F0[0x1CB0 - 0x16F0];
    char x1CB0[0x110];
    char x1DC0[0x110];
    char x1ED0[0x110];
    char x1FE0[0x110];
} Body;

typedef struct Obj {
    char pad0[0xDA4];
    Body *xDA4;
    char padDA8[0xDE8 - 0xDA8];
    int xDE8;
    unsigned char xDEC;
    char padDED[3];
    int xDF0;
} Obj;




extern void func_00202E38(Name *, int, int, float);
extern void func_00203E10(Name *, float);

void func_001A4E08(Obj *obj)
{
    Name name;
    Body *head;
    Body *body;
    char *first;

    if (!obj->xDEC) {
        if (++obj->xDF0 >= 21) {
            name = obj->xDA4->x304;
            func_00203E10(&name, 0.0f);
        }
    } else {
        head = obj->xDA4;
        obj->xDEC = 0;
        obj->xDF0 = 0;
        obj->xDE8 = 0;
        func_001B3170((RiderSecondaryBlend *)(head->x16F0), 0, 1.0f);
        SetBodyBlendWeightFirst((RiderBodyBlend *)(obj->xDA4->x16D0 + 0x47F0), 1.0f, 0);
        body = obj->xDA4;
        first = body->x1CB0;
        do {
            SetRiderBlendWeightFirst((RiderBlendController *)(body->x1DC0), 1.0f, 480);
            SetRiderBlendWeightFirst((RiderBlendController *)(first), 1.0f, 480);
            SetRiderBlendWeightFirst((RiderBlendController *)(body->x1ED0), 1.0f, 480);
            SetRiderBlendWeightFirst((RiderBlendController *)(body->x1FE0), 1.0f, 480);
            func_00202E38(&obj->xDA4->x304, 1, 1, 480.0f);
            name = obj->xDA4->x304;
            func_00203E10(&name, 1.0f);
        } while (0);
    }
}
