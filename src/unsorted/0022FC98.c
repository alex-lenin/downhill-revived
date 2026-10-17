/* UNIT 0x0022FC98 0x0022FD20 */
#include "soft_double.h"
typedef struct Obj { char pad[0x40]; float x40, x44, x48; } Obj;



void func_0022FC98(Obj *obj, float x, float y, float z) {
    float length;
    if (obj) {
        length = func_001FD010(func_00248A80(func_001FD300(x*x + y*y + z*z)));
        obj->x40 = x / length;
        obj->x44 = y / length;
        obj->x48 = z / length;
    }
}
