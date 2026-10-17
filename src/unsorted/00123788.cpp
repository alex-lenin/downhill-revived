/* UNIT 0x00123788 0x001238B8 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00126D90_0_func_00126D90 func_00126D90
#else
#define CallScalarBridge00126D90_0_func_00126D90 ScalarBridge00126D90_0
#endif
struct Vec4 { float x, y, z, w; } __attribute__((aligned(16)));
struct Body {
    char pad0[0x10];
    float pos[12];
    float mtx[4][4];
    char pad80[0x28];
    float heading;
};
class Actor {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual Body *v4();
};
struct Obj {
    char pad0[0xCC];
    float xCC;
    char padD0[0x60];
    Actor *x130;
    char pad134[0xCC];
    Vec4 x200;
};

extern "C" float func_00126D30(Obj *obj, void *a, void *b);

#if defined(__mips__)
extern "C" float func_00126D90(Obj *obj, float a, float b);
#else

#endif

extern "C" void func_002087C0(float *angle);
extern "C" float func_00235F38(float cur, float target, float rate, float min);
extern "C" void func_00221F50(void *m, void *rot);

/* copies xyz, keeping dst's w (128-bit lq/sq copy, src address in a register) */
static inline void CopyXYZ(Vec4 *dst, const float *src) {
    float w = dst->w;
    __asm__ volatile ("lq $2, 0(%1)\n\tsq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
    dst->w = w;
}

extern "C" void func_00123788(Obj *obj, void *target) {
    float angle;
    float diff;
    float rate;
    Body *body;

    body = obj->x130->v4();
    angle = func_00126D30(obj, body->pos, target) + obj->xCC;
    func_002087C0(&angle);
    diff = __builtin_fabsf(CallScalarBridge00126D90_0_func_00126D90(obj, body->heading, angle));
    rate = 0.08f;
    if (diff < 0.34906584f) {
        rate = 0.02f;
    } else if (diff < 0.7853982f) {
        rate = 0.05f;
    }
    body->heading = func_00235F38(body->heading, angle, rate, 0.001f);
    func_00221F50(body->mtx, (char *)body + 0xA0);
    CopyXYZ(&obj->x200, body->mtx[1]);
}
