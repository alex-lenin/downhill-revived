/* UNIT 0x00153168 0x001531E8 */
#include "tween.h"
typedef struct Owner {
    char pad0[0x3D10];
    char x3D10[0x110];
    char x3E20[0x188];
    char x3FA8[1];
} Owner;

typedef struct Obj {
    Owner *owner;
    char pad4[0x8];
    float xC;
    char pad10[0x4];
    int x14;
} Obj;

extern void func_001525E8(Obj *);
extern void func_001D06C0(void *, int);


void func_00153168(Obj *obj)
{
    Owner *owner;
    float value;

    func_001525E8(obj);
    func_001D06C0(obj->owner->x3FA8, 2);
    owner = obj->owner;
    if (obj->x14 != 0) {
        value = obj->xC;
        if (obj->x14 == 25) {
            func_001B1698((RiderBlendController *)(owner->x3E20), value, 0.0f);
        } else {
            func_001B1698((RiderBlendController *)(owner->x3D10), value, 0.0f);
        }
    }
}
