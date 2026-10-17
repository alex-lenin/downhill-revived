/* UNIT 0x001AC810 0x001AC880 */
#include "animation.h"
typedef AnimationController Obj;

void func_001AC810(Obj *obj, float value) {
    AnimationHandle handle;
    short pri;
    int type;
    float f;
    if (obj->x0 == 0)
        return;
    if (obj->x0->x0 == 0)
        return;
    pri = obj->xA;
    type = obj->x9;
    handle = *obj->x0;
    f = obj->x60;
    func_002031E0(&handle, (char *)obj + 4, type, pri, 1, f, value);
}
