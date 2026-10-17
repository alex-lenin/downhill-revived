/* UNIT 0x00128A28 0x00128B80 .sdata=0x29C230:0x29C248 */
#include "impulse.h"
#include "camera.h"
typedef float Vec4[4] __attribute__((aligned(16)));

typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Rider {
    char pad0[0x1E40];
    char x1E40[0xE94];
    Vec x2CD4;
    char pad2CE4[0x84C];
    char x3530[0xAA0];
    char x3FD0[0xF0];
    char x40C0[0xC0];
    Vec4 x4180;
    char pad4190[0x100];
    Vec4 x4290;
    char pad42A0[0x430];
    char x46D0[0x820];
    char x4EF0[0x10];
} Rider;

typedef struct Ctrl {
    Rider *rider;
    char pad4[0x10];
    float x14;
    float x18;
} Ctrl;

float D_0029C230 = 0.4f;
float D_0029C234 = 0.2f;
float D_0029C238 = 0.4f;
float D_0029C23C = 1500.0f;
float D_0029C240 = 1500.0f;
float D_0029C244 = -60.0f;

extern void func_001AC578(void *p);

extern float func_00203C28(Vec *v);

extern void func_00155E78(void *p, int n);
extern void func_001A7830(void *p, Vec4 v);
extern float func_001385A8(void *p);
extern void func_001A77E0(void *p, Vec4 v);

void func_00128A28(Ctrl *ctrl) {
    Vec4 pos;
    Vec dir;
    Vec4 push;
    Vec4 spin;
    Rider *rider = ctrl->rider;
    void *body;

    func_001AC578(rider->x1E40);
    ctrl->x14 -= D_0029C230;
    ctrl->x18 += D_0029C234;
    pos[0] = 0.0f;
    pos[1] = ctrl->x14;
    pos[2] = ctrl->x18;
    pos[3] = 0.0f;
    func_001AEB20((RiderCameraRig *)(rider->x46D0), (RiderCamera *)(rider->x3530), (Vector4 *)(pos));
    dir = rider->x2CD4;
    if (func_00203C28(&dir) > D_0029C238) {
        body = rider->x40C0;
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "mfc1 $2, %2\n\t"
                          "qmtc2.ni $2, $vf12\n\t"
                          "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(dir) : "r"(rider->x4290), "f"(D_0029C23C) : "$2", "memory");
        func_00231C88((ImpulseBody *)(body), (Vector4 *)(&dir));
        func_00155E78(rider->x3FD0, 6);
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "mfc1 $2, %2\n\t"
                          "qmtc2.ni $2, $vf12\n\t"
                          "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(push) : "r"(rider->x4290), "f"(-D_0029C240) : "$2", "memory");
        func_001A7830(rider->x4EF0, push);
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "mfc1 $2, %2\n\t"
                          "qmtc2.ni $2, $vf12\n\t"
                          "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(spin) : "r"(rider->x4180), "f"(D_0029C244 * func_001385A8(body)) : "$2", "memory");
        func_001A77E0(rider->x4EF0, spin);
    }
}
