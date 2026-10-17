/* UNIT 0x001B2E50 0x001B2E80 */
#include "camera.h"
typedef CameraParameter Obj;
void func_001B2E50(Obj *obj, float value) {
    if (obj->enabled != 0 && value != 0.0f) obj->value = value;
}
