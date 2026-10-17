/* UNIT 0x00120CE8 0x00120E70 */
#include "rider_geometry.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x4];
    unsigned char x4;
    char pad5[0x10 - 0x5];
    Vec4 x10;
} Body;

typedef Body Ctrl;

typedef struct Obj {
    char pad0[0x10];
    Body *x10;
    Ctrl *x14;
    char pad18[0x160 - 0x18];
    float x160[4];
    Vec4 x170;
    char x180[0x40];
    char x1C0[0x10];
    Vec4 x1D0;
    char pad1E0[0x24C - 0x1E0];
    float x24C;
} Obj;

extern void func_00121530(Obj *obj);
extern float func_00238F40(void);
extern void func_00120E70(Obj *obj, Vec4 *pos, int arg);

extern void func_00121200(Obj *obj, void *a, Vec4 *b);
extern void func_00120AA8(Obj *obj, void *a, void *b, int arg);
extern void func_00211280(Body *body);

/* quadword copy macro (sceVu0CopyVector style) */
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_00120CE8(Obj *obj)
{
    Vec4 *x1D0 = &obj->x1D0;
    float *x160 = obj->x160;
    Vec4 *vel = &obj->x170;
    Vec4 pos;
    float maxStep;
    float scale;
    float ground;
    float step;
    float clamped;

    func_00121530(obj);
    maxStep = func_00238F40() * 32.0f;
    scale = x1D0->z * func_00238F40();
    /* pos = body position + vel * scale */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(pos) : "r"(&obj->x10->x10), "r"(vel), "f"(scale) : "$2");
    pos.z = obj->x10->x10.z + obj->x24C * func_00238F40();
    func_00120E70(obj, &pos, 0);
    ground = func_00121348((GroundRider *)(obj), (void *)(&pos), 0);
    if (obj->x14) {
        obj->x14->x4 = 1;
        COPY_VECTOR(obj->x14->x10, &pos);
        obj->x14->x10.z -= ground;
        func_00121200(obj, obj->x180, x1D0);
    }
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(step) : "f"(ground), "f"(maxStep));
    pos.z -= step;
    obj->x24C -= step;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(clamped) : "f"(-117.333336f), "f"(obj->x24C));
    obj->x24C = clamped;
    COPY_VECTOR(obj->x10->x10, &pos);
    x160[0] = obj->x10->x10.x;
    x160[1] = obj->x10->x10.y;
    func_00120AA8(obj, obj->x180, obj->x1C0, 0);
    func_00211280(obj->x10);
}
