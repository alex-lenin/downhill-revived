/* UNIT 0x001507E8 0x00150890 */
typedef struct Obj {
    char pad0[0x500];
    int x500[5];
    char pad514[0x52C - 0x514];
    int x52C;
    int x530;
    int x534[14];
    int x56C;
    int x570;
    int x574;
    char pad578[0x584 - 0x578];
    int x584;
} Obj;

#if defined(__mips__)
extern void func_0020ECD0(float, float, float, float, int, int, int);
#else
#include "scene_effects.h"
#define func_0020ECD0 SceneEffectsLegacy
#endif

void func_001507E8(Obj *obj) {
    int i;

    obj->x574 = 0;
    obj->x570 = 0;
    obj->x530 = 0;
    obj->x56C = 0;
    obj->x52C = 0;
    obj->x584 = 0;
    for (i = 13; i >= 0; i--) {
        obj->x534[i] = 0;
    }
    for (i = 4; i >= 0; i--) {
        obj->x500[i] = 0;
    }
    func_0020ECD0(700.0f, 8000.0f, 255.0f, 8000.0f, 60, 60, 60);
}
