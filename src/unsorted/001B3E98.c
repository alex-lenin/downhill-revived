/* UNIT 0x001B3E98 0x001B3F48 */
#include "camera.h"
#include "rider.h"
typedef float Vec4[4] __attribute__((aligned(16)));


typedef RiderComponent Parent;

typedef RiderCamera Obj;

void func_001B3E98(Obj *obj, Vector4 *out)
{
    Vec4 tmp;
    SceneNode *rider = obj->xC0->rider->bodyNode;
    float t = obj->xF4;

    if (obj->xF1) {
        t = 0.0f;
    }
    __asm__ volatile ("lqc2 $vf11, 0x0(%2)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "lqc2 $vf13, 0x10(%1)\n\t"
                      "lqc2 $vf14, 0x20(%1)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\t"
                      "vmadday.xyz ACC, $vf13, $vf11y\n\t"
                      "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(tmp) : "r"(&rider->matrix), "r"(&obj->x10) : "memory");
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(tmp) : "r"(tmp), "r"(&rider->position) : "memory");
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(tmp) : "r"(tmp), "f"(1.0f - t) : "$2", "memory");
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(out), "r"(&obj->x0), "f"(t) : "$2", "memory");
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t"
                      "lqc2 $vf12, 0x0(%1)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, 0x0(%0)"
                      : : "r"(out), "r"(tmp) : "memory");
}
