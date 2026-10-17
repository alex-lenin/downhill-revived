/* UNIT 0x00152FF8 0x00153118 */
#include "tween.h"
typedef struct Obj {
    char *world;
    char pad4[0xC - 0x4];
    float xC;
    char pad10[0x14 - 0x10];
    int x14;
} Obj;

extern void func_001525E8(Obj *obj);
extern void func_001ACFA0(void *p, int a);
extern void func_001AD580(void *p, int a, float b, float c);



extern void func_001D06C0(void *p, int a);

void func_00152FF8(Obj *obj)
{
    char *world;
    char *sub;
    char *p;
    float value;

    func_001525E8(obj);
    world = obj->world;
    sub = world + 0x1E40;
    if (obj->x14 == 0) {
        return;
    }
    if (*(unsigned char *)(sub + 0x2160) != 0) {
        p = world + 0x4580;
        func_001ACFA0(p, 1);
        value = obj->xC;
        if (obj->x14 == 0x17) {
            func_001AD580(p, 0, 0.25f, 0.5f);
            func_001B1698((RiderBlendController *)(world + 0x3E20), (int)value, 0.0f);
        } else {
            func_001AD580(p, 0, 0.75f, 0.5f);
            func_001B1698((RiderBlendController *)(world + 0x3D10), (int)value, 0.0f);
        }
        func_001B3170((RiderSecondaryBlend *)(sub + 0x16F0), (int)value, 0.0f);
        SetBodyBlendWeightFirst((RiderBodyBlend *)(world + 0x47F0), 0.0f, (int)value);
    }
    func_001D06C0(sub + 0x2168, 1);
}
