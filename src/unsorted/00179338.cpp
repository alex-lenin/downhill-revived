/* UNIT 0x00179338 0x00179630 */
typedef struct Body { char pad0[0x18]; float x18; } Body;
typedef struct Rider {
    char pad0[0x792C];
    Body *x792C;
    char pad7930[0x7A40 - 0x7930];
    float x7A40;
    char pad7A44[0x7A5C - 0x7A44];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Rider;
typedef struct Players {
    Rider riders[10];
    char pad4D620[0x2B88];
    int count;
} Players;
typedef struct List { char pad0[8]; unsigned short count; char pada[0x16]; int *items[1]; } List;
#include "animation.h"
typedef AnimationHandle Sound;
typedef struct Obj {
    char pad0[0xF8C];
    List *xF8C;
    List *xF90;
    char padF94[0x30];
    Sound xFC4[9];
    Sound x1054[9];
    Sound x10E4, x10F4, x1104, x1114, x1124, x1134;
    char pad1144[0x1164 - 0x1144];
    Rider *x1164;
} Obj;
typedef struct State { char pad0[0x288]; int x288; } State;
typedef struct G32270 { char pad0[0x4F50]; float x4F50; float x4F54; } G32270;
extern Players D_002AF2C0;
extern State D_004B25A0;
extern G32270 D_003B2270;
extern int func_0023DED0(void);

void func_00179338(Obj *obj)
{
    Sound copy;
    Rider *r;
    float denom;
    float t;
    float start;
    int i;
    int k;

    if (obj->xF8C == 0) {
        return;
    }
    {
        G32270 *g = &D_003B2270;
        denom = g->x4F50 - g->x4F54;
    }
    i = 0;
    while (i < D_002AF2C0.count) {
        r = &D_002AF2C0.riders[i];
        if (r->x7A5C != 0) {
            start = D_003B2270.x4F50;
            t = (start - r->x792C->x18) / denom;
            if (t >= 1.0f) {
                t = 0.9999f;
            }
            r->x7A40 = t;
            if (func_0023DED0() == 2) {
                if (r == obj->x1164 && obj->x1124.x0 != 0) {
                    (copy = obj->x1124, func_00203B68(&copy, 1, t));
                } else if (r->x7A5C == 1 && obj->x1134.x0 != 0) {
                    (copy = obj->x1134, func_00203B68(&copy, 1, t));
                } else {
                    k = i - D_004B25A0.x288;
                    if (obj->x1054[k].x0 != 0) {
                        (copy = obj->x1054[k], func_00203B68(&copy, 1, t));
                    }
                }
            } else {
                if (r == obj->x1164 && obj->x10E4.x0 != 0) {
                    (copy = obj->x10E4, func_00203B68(&copy, 1, t));
                } else if (r->x7A5C == 1 && obj->x10F4.x0 != 0) {
                    (copy = obj->x10F4, func_00203B68(&copy, 1, t));
                } else {
                    k = i - D_004B25A0.x288;
                    if (obj->xFC4[k].x0 != 0) {
                        (copy = obj->xFC4[k], func_00203B68(&copy, 1, t));
                    }
                }
            }
        }
        i++;
    }
}
