/* UNIT 0x001AC680 0x001AC6F0 */
#include "animation.h"
typedef AnimationController Obj;

void func_001AC680(Obj *obj, float value) {
    AnimationHandle handle = *obj->x0;
    func_002031E0(&handle, (char *)obj + 4, 2, 1, 1, 1.0f, value);
    obj->x5C = 0;
}
