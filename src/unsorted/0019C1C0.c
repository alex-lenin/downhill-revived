/* UNIT 0x0019C1C0 0x0019C1F0 */
#include "camera.h"
typedef struct Obj { char *x0; int x4; float x8; } Obj;

void func_0019C1C0(Obj *obj) {
    func_001B3028((RiderCamera *)(obj->x0 + 0x3530), 1, __builtin_fabsf(obj->x8));
}
