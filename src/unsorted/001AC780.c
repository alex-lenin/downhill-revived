/* UNIT 0x001AC780 0x001AC7B0 */
#include "animation.h"
typedef AnimationController Obj;
Obj *func_001AC780(Obj *obj, signed char arg1, short arg2) {
    obj->x9 = arg1;
    obj->xA = arg2;
    obj->x60 = 1.0f;
    obj->x58 = 0;
    obj->x54 = 0;
    obj->x8 = 0;
    obj->x48 = 0;
    obj->x0 = 0;
    return obj;
}
