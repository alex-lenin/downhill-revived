/* UNIT 0x001288B8 0x00128A20 */
#include "tween.h"
typedef struct Sound { int x0,x4,x8,xC; } Sound;
typedef struct World { char pad[0x2cd4]; Sound x2cd4; char pad2ce4[0x48b0-0x2ce4]; int x48b0; char pad48b4[0x7988-0x48b4]; int x7988; } World;
typedef struct Obj { World *x0; char pad4[12]; float x10,x14,x18; } Obj;
extern char D_0037EA60[];
extern void func_001ACFA0(void *,int);
extern void func_001AD700(void *,float);
extern void func_00136030(void *,int);
extern void func_00202E38(void *,int,int,float);
extern void func_00203E10(Sound *,float);



extern void func_00148560(void *,int);
void func_001288B8(Obj *obj) {
    World *world = obj->x0;
    void *x4580 = (char *)world + 0x4580;
    void *x3af0 = (char *)world + 0x3af0;
    Sound sound;
    func_001ACFA0(x4580,1);
    func_001AD700(x4580,0.0f);
    world->x48b0 = 1;
    func_00136030((char *)world + 0x40c0,1);
    func_00202E38(&world->x2cd4,1,1,120.0f);
    sound = world->x2cd4;
    func_00203E10(&sound,0.5f);
    SetSecondaryBlendWeightFirst((RiderSecondaryBlend *)((char *)world + 0x3530), 0.5f, 60);
    func_001AE630((RiderBodyBlend *)((char *)world + 0x47f0), 60, 0.5f);
    SetRiderBlendWeightFirst((RiderBlendController *)((char *)world + 0x3c00), 0.0f, 120);
    SetRiderBlendWeightFirst((RiderBlendController *)(x3af0), 0.0f, 120);
    SetRiderBlendWeightFirst((RiderBlendController *)((char *)world + 0x3d10), 0.0f, 120);
    SetRiderBlendWeightFirst((RiderBlendController *)((char *)world + 0x3e20), 0.0f, 120);
    obj->x10=0.0f;
    obj = (Obj *)((char *)obj + 0x10);
    ((float *)obj)[1] = 0.0f;
    ((float *)obj)[2] = 0.0f;
    if (world->x7988 >= 0) func_00148560(world->x7988 * 0x380 + D_0037EA60,2);
}
