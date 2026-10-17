/* UNIT 0x001B2E80 0x001B3028 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001B3D58_26_func_001B3D58 func_001B3D58
#else
#define CallScalarBridge001B3D58_26_func_001B3D58 ScalarBridge001B3D58_26
#endif
struct Settings { float x0, x4, x8, xC, x10, x14, x18, x1C, x20, x24, x28, x2C, x30, x34, x38, x3C, x40, x44, x48; int x4C; };
struct Body { char pad[0x16D0]; char *x16D0; };
struct Obj { char pad[0xC0]; Body *xC0; };
extern "C" int D_0029D400;

#if defined(__mips__)
extern "C" void func_001B3D58(Obj *, int, int, float);
#else

#endif

extern "C" void func_001AE758(void *, int, float, float, float);
extern "C" void func_001B2E80(Settings *settings, Obj *obj, float amount) {
    float negative, positive, value;
    char *target;
    if (settings->x4C && (value = settings->x48 * amount) != 0.0f) {
        negative = value <? 0.0f;
        positive = value >? 0.0f;
        CallScalarBridge001B3D58_26_func_001B3D58(obj, 0, 0, negative * settings->x0);
        CallScalarBridge001B3D58_26_func_001B3D58(obj, 0, 0, positive * settings->x4);
        CallScalarBridge001B3D58_26_func_001B3D58(obj, 1, 0, negative * settings->x8);
        CallScalarBridge001B3D58_26_func_001B3D58(obj, 1, 0, positive * settings->xC);
        CallScalarBridge001B3D58_26_func_001B3D58(obj, 2, 0, negative * settings->x10);
        CallScalarBridge001B3D58_26_func_001B3D58(obj, 2, 0, positive * settings->x14);
        if (D_0029D400 == 0) {
            target = obj->xC0->x16D0 + 0x4780;
            func_001AE758(target, 0, negative * settings->x18, negative * settings->x1C, negative * settings->x20);
            func_001AE758(target, 1, negative * settings->x24, negative * settings->x28, negative * settings->x2C);
            func_001AE758(target, 0, positive * settings->x30, positive * settings->x34, positive * settings->x38);
            func_001AE758(target, 1, positive * settings->x3C, positive * settings->x40, positive * settings->x44);
        }
    }
}
