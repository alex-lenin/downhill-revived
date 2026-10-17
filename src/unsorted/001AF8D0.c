/* UNIT 0x001AF8D0 0x001AF978 */
#include "animation.h"
typedef struct Parent {
    char pad0[0x3FA4];
    int x3FA4;
    char pad3FA8[0x796C - 0x3FA8];
    int x796C;
} Parent;

typedef AnimationHandle Desc;

typedef struct Obj {
    int *x0;
    char pad4[0x60 - 4];
    Desc x60;
    Parent *x70;
    float x74;
} Obj;


extern void func_001AC620(Obj *obj, Desc *src);
extern void func_001AC680(Obj *obj, float value);

void func_001AF8D0(Obj *obj, Parent *parent)
{
    Desc *p;
    int *pp;
    int result;
    long active;

    if (parent != 0) {
        obj->x70 = parent;
    }
    p = &obj->x60;
    obj->x74 = 3.0f;
    func_002036C8(p, obj->x70->x796C + 0x10, 0, 0x23, obj->x70->x3FA4);
    func_001AC620(obj, p);
    pp = obj->x0;
    active = 0;
    if (pp != 0) {
        if (pp[0] != 0) {
            result = pp[1] != 0;
            active = result;
        }
    }
    if ((active & 0xFF) != 0) {
        func_001AC680(obj, 0.0f);
    }
}
