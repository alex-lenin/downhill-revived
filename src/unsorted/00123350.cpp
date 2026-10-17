/* UNIT 0x00123350 0x00123760 */
#include "rider_ai.h"
#include "rider_event.h"
#include "model.h"
struct Vec4 { float x, y, z, w; } __attribute__((aligned(16)));
struct Body { char pad0[0x40]; float mtx[4][4]; };
class ActorBase {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual Body *v4();
};
class Actor : public ActorBase {
public:

    char pad4[0x248];
    char x24C;
};
struct Obj {
    char pad0[0x8];
    int x8;
    int xC;
    char pad10[0x8];
    int x18;
    int x1C;
    int x20;
    char pad24[0x20];
    int x44;
    char pad48[0x20];
    char x68[0x68];
    int xD0;
    char padD4[0x4];
    float xD8;
    float xDC;
    Vec4 xE0;
    char padF0[0x40];
    Actor *x130;
    char pad134[0xC];
    int x140;
    int x144;
};

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern "C" int func_00122D10(Obj *obj, float dist);
extern "C" void func_00123760(Obj *obj, int state);
extern "C" void func_00123788(Obj *obj, Vec4 *target);
extern "C" void func_001238B8(Obj *obj);
extern "C" void func_00123AD0(Obj *obj, Vec4 *target, int);
extern "C" void func_00123DB0(Obj *obj, float (*mtx)[4]);
extern "C" void func_00123F60(Obj *obj, int);

extern "C" void func_00126B20(Obj *obj, int state);

extern "C" void func_001ACAE0(void *, float);
extern "C" float func_001ACBA0(void *);

extern "C" float func_00222B68(float, float);
extern "C" float func_00235EA8(float, float, float, float);

extern "C" void func_00123350(Obj *obj) {
    Vec4 target;
    float dist;
    int state;

    obj->xC = obj->x8;
    if (!func_00122D10(obj, 200.0f)) {
        obj->x144 = 0;
        if (obj->x8) {
            func_00126B20(obj, 0);
            obj->x130->x24C = 1;
        }
        return;
    }
    obj->x130->x24C = 0;
    state = obj->x140;
    dist = obj->xD8;
    COPY_VECTOR(target, &obj->xE0);
    if (obj->x1C > 60 || obj->x20 > 60) {
        if (obj->x8 != 1) {
            func_00126B20(obj, 1);
            obj->xDC = 1.0f - func_00222B68(-0.35f, 0.35f);
        }
    } else {
        func_00123760(obj, state);
        if (obj->x144 && dist < 9.0f) {
            if (obj->x8 != 3) {
                func_00126B20(obj, 3);
                SubmitRiderEvent((char *)obj->x130 + 0x234, obj->x130);
            }
        } else if (obj->x144 && dist < 144.0f) {
            if (obj->x8 != 2) {
                func_00126B20(obj, 2);
                SubmitRiderEvent((char *)obj->x130 + 0x234, obj->x130);
                obj->xD0 = 0;
                if (obj->x130 && func_00192BA8((int)(func_002623C0(obj->x130))) == 16)
                    obj->x44 = 1;
            }
        } else if (obj->x8 != 1) {
            if (obj->x144) {
                if (dist > 576.0f)
                    func_00126B20(obj, 1);
            } else {
                func_00126B20(obj, 1);
            }
        }
    }
    if (obj->x8 == 0)
        func_00126B20(obj, 1);
    switch (obj->x8) {
    case 0:
        if (dist < 40000.0f)
            func_00123788(obj, &target);
        break;
    case 1:
        if (dist < 40000.0f)
            func_00123788(obj, &target);
        if (obj->xC != obj->x8)
            obj->xDC = 1.0f - func_00222B68(-0.35f, 0.35f);
        func_001ACAE0(obj->x68, func_00235EA8(func_001ACBA0(obj->x68), 0.5f, 0.03f, 0.0001f));
        func_00123F60(obj, 0);
        func_00123DB0(obj, obj->x130->v4()->mtx);
        break;
    case 2:
        func_00123AD0(obj, &target, 0);
        func_001238B8(obj);
        if (obj->x18 > 300)
            func_00126B20(obj, 1);
        break;
    case 3:
        UpdateRiderAiLegacy(obj, &target);
        func_001ACAE0(obj->x68, func_00235EA8(func_001ACBA0(obj->x68), 0.5f, 0.1f, 0.0001f));
        break;
    }
}
