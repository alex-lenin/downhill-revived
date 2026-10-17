/* UNIT 0x001B3D58 0x001B3D78 */
#include "scalar_api.h"
struct Elem001B3D58 {
    float x0;
    char pad4[0x1C];
};

struct Obj001B3D58 {
    char pad0[0x534];
    struct Elem001B3D58 x534[1];
};

void func_001B3D58(struct Obj001B3D58 *obj, int index, float delta)
{
    obj->x534[index].x0 += delta;
}
