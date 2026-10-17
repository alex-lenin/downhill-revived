/* UNIT 0x001B00C8 0x001B01D8 */
#include "animation.h"
typedef AnimationHandle Desc;

typedef struct Sound {
    int *x0;
    char pad4[0x5C];
} Sound;

typedef struct Parent {
    char pad0[0x7AF8];
    int x7AF8;
    int x7AFC;
} Parent;

typedef struct Obj {
    Sound x0;
    Sound x60;
    Desc xC0;
    Desc xD0;
    Parent *xE0;
} Obj;


extern void func_001AC620(Sound *obj, Desc *src);
extern void func_001AC680(Sound *obj, float value);

void func_001B00C8(Obj *obj, Parent *parent)
{
    Desc *p;
    Desc *p2;
    int *pp;
    int result;
    long active;
    long active2;

    if (parent != 0) {
        obj->xE0 = parent;
    }
    p = &obj->xD0;
    func_002036C8(p, 0x26, 0, 0x25, obj->xE0->x7AFC);
    func_001AC620(&obj->x60, p);
    pp = obj->x60.x0;
    active = 0;
    if (pp != 0) {
        if (pp[0] != 0) {
            result = pp[1] != 0;
            active = result;
        }
    }
    if ((active & 0xFF) != 0) {
        func_001AC680(&obj->x60, 0.0f);
    }
    p2 = &obj->xC0;
    func_002036C8(p2, 0x27, 0, 0x25, obj->xE0->x7AF8);
    func_001AC620((Sound *)obj, p2);
    pp = obj->x0.x0;
    active2 = 0;
    if (pp != 0) {
        if (pp[0] != 0) {
            result = pp[1] != 0;
            active2 = result;
        }
    }
    if ((active2 & 0xFF) != 0) {
        func_001AC680((Sound *)obj, 0.0f);
    }
}
