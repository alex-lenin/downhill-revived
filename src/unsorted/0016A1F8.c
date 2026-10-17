/* UNIT 0x0016A1F8 0x0016A3E8 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00169F98_12_func_00169F98 func_00169F98
#else
#define CallScalarBridge00169F98_12_func_00169F98 ScalarBridge00169F98_12
#endif
typedef struct { float x, y, z, w; } Vec;
typedef struct Basis { Vec x0, x20; } Basis;
typedef struct Part {
    char pad0[0x36];
    unsigned short type : 5;
    unsigned short x36_5 : 11;
    char pad38[0x48];
} Part;
typedef struct State { char pad[0x7B9F]; unsigned char x7B9F; } State;
typedef struct Rider {
    char pad0[0x38];
    Basis *x38;
    char pad3C[0x158 - 0x3C];
    State *x158;
    char pad15C[0x2E0 - 0x15C];
    Part x2E0;
    Part x360;
    long x3E0;
} Rider;
typedef struct Obj {
    int x0;
    Rider *x4;
    float x8, xC;
    Vec x10, x20, x30, x40;
} Obj;
extern float D_0029C878, D_0029C87C;
extern char D_0026EC60[];
extern float func_00248B90(float), func_0016AD98(void *, float);

#if defined(__mips__)
extern void func_00169F98(Obj *, float, float);
#else

#endif

/* D_0029C874 is defined in func_00169F98's unit; hoist its size so the
   assembler uses one $gp-relative word for the store below (retail does). */
__asm__(".extern D_0029C874, 4");
extern float D_0029C874;
static inline float dot(Vec *a, Vec *b) {
    float out;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(out):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return out;
}
int func_0016A1F8(Obj *obj, float threshold) {
    Vec c;
    float angle, t, tilt, value, d3;
    int sa, sb;
    Rider *r;
    if (obj->x4->x158->x7B9F) return 0;
    angle = func_00248B90(dot(&obj->x10, &obj->x4->x38->x0)) * 0.31830988618379067153f;
    angle -= 0.5f;
    angle += angle;
    t = __builtin_fabsf(angle);
    __asm__ ("Lpin:" : : "f"(t) : "memory");
    angle = t;
    __asm__ ("Lpin2:" : "+f"(angle));
    c.x = 0.0f; c.y = 0.0f; c.z = 1.0f;
    tilt = (1.0f - dot(&c, &obj->x10)) * 90.0f;
    r = obj->x4;
    sa = r->x2E0.type; sb = r->x360.type;
    if (sa != 0x17 && sb != 0x17) {
        if (tilt < D_0029C878) return 0;
    }
    if (sa != 0x17 && sb != 0x17) {
        if (angle < threshold && tilt < D_0029C87C) return 0;
    } else {
        angle = 1.0f;
    }
    value = func_0016AD98(D_0026EC60, angle);
    if (obj->x0 == 0 || value <= obj->x8) {
        d3 = dot(&obj->x40, &obj->x20);
        CallScalarBridge00169F98_12_func_00169F98(obj, angle, d3);
        return 2;
    }
    if (obj->x0 == 2) {
        obj->x8 = value;
        D_0029C874 = angle;
    }
    return 0;
}
