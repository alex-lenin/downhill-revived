/* UNIT 0x0016A3E8 0x0016A488 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00169FF0_13_func_00169FF0 func_00169FF0
#else
#define CallScalarBridge00169FF0_13_func_00169FF0 ScalarBridge00169FF0_13
#endif
typedef struct { char pad[0x158]; int x158; } Link;
typedef struct { int type; Link *x4; char pad[0x18]; float x20[4]; char pad2[0x10]; float x40[4]; } Obj;
extern float func_0012E070(int);

#if defined(__mips__)
extern void func_00169FF0(Obj *, float, float);
#else

#endif

int func_0016A3E8(Obj *obj, float value) {
    float *a, *b;
    float dot, x, y, z, u, v, w;
    if (obj->type == 2 || func_0012E070(obj->x4->x158) == 0.0f) return 0;
    a = obj->x40;
    b = obj->x20;
    y = a[1]; v = b[1]; z = a[2]; w = b[2]; x = a[0]; u = b[0];
    __asm__ volatile("mula.s %0,%1\n\tmadda.s %4,%5\n\tmadd.s %0,%2,%3" : "+f"(x) : "f"(u), "f"(z), "f"(w), "f"(y), "f"(v));
    dot = x;
    CallScalarBridge00169FF0_13_func_00169FF0(obj, value, dot);
    return 3;
}
