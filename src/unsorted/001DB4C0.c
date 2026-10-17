/* UNIT 0x001DB4C0 0x001DB510 */
#include "sound.h"
void func_001DB4C0(SoundEmitter *obj, SoundName name, int a, int b, int x, int y, int z, int c) {
    obj->x38 = c;
    obj->name = name;
    obj->nearDistance = a;
    obj->position.x = (float)x;
    obj->position.y = (float)y;
    obj->position.z = (float)z;
    obj->farDistance = b;
    obj->secondaryHandle = 0;
    obj->handle = 0;
}
