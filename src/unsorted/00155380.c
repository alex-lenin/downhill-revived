/* UNIT 0x00155380 0x001554B0 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001554B8_6_func_001554B8 func_001554B8
#else
#define CallScalarBridge001554B8_6_func_001554B8 ScalarBridge001554B8_6
#endif

#if defined(__mips__)
#define CallScalarBridge001554B0_7_func_001554B0 func_001554B0
#else
#define CallScalarBridge001554B0_7_func_001554B0 ScalarBridge001554B0_7
#endif
typedef struct Obj {
    int x0;
    char pad4[0xB8 - 0x4];
    int timer;
} Obj;

extern char D_0037EA60[];
extern int func_00148BA8(void *table, int id);

#if defined(__mips__)
extern void func_001554B0(Obj *obj, float a, float b);
#else

#endif


#if defined(__mips__)
extern void func_001554B8(Obj *obj, float a);
#else

#endif


void func_00155380(Obj *obj)
{
    float t;
    float scale;

    if (obj->timer == 0) {
        return;
    }
    if (func_00148BA8(D_0037EA60, obj->x0) != 0) {
        obj->timer = 0;
        CallScalarBridge001554B8_6_func_001554B8(obj, 1.0f);
        return;
    }
    obj->timer--;
    if (obj->timer <= 0) {
        CallScalarBridge001554B8_6_func_001554B8(obj, 1.0f);
        return;
    }
    t = obj->timer;
    if (t > 192.0f) {
        scale = 1.0f;
    } else if (t > 144.0f) {
        scale = 10.0f;
    } else if (t > 96.0f) {
        scale = 1.0f;
    } else if (t > 48.0f) {
        scale = 10.0f;
    } else {
        scale = 1.0f;
    }
    CallScalarBridge001554B0_7_func_001554B0(obj, scale, 0.1f);
}
