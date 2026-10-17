/* UNIT 0x00180910 0x00180A10 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0017F3F0_18_func_0017F3F0 func_0017F3F0
#else
#define CallScalarBridge0017F3F0_18_func_0017F3F0 ScalarBridge0017F3F0_18
#endif
typedef struct ModelInfo {
    unsigned int pad0 : 18;
    unsigned int id : 14;
} ModelInfo;

typedef struct Model {
    ModelInfo *x0;
    char pad4[0x14];
    float x18;
} Model;

typedef struct Target {
    void *x0;
    void *x4;
    short x8;
    short xA;
} Target;

typedef struct Obj {
    char pad0[4];
    int x4;
    Model *x8;
    void *xC;
    char pad10[0xD0 - 0x10];
    void *xD0;
    char padD4[4];
    Target xD8;
} Obj;

typedef struct World {
    char pad0[0x50198];
    float x50198;
} World;

extern World D_002AF2C0;
extern char D_003BD580[];

extern void *func_001817E0(void *, unsigned int);

#if defined(__mips__)
extern void *func_0017F3F0(void *, void *, float);
#else

#endif

extern void func_002182A8(void *, int);
extern void func_0017FFB8(Obj *, int);

static inline int IsInRange(unsigned int id)
{
    return id >= 0x55A && id <= 0x56E;
}

void func_00180910(Obj *obj)
{
    float margin;
    unsigned int id;
    void *found;
    Target *target;

    id = obj->x8->x0->id;
    margin = 150.0f;
    if (IsInRange(id)) {
        margin = 100.0f;
    }
    if (D_002AF2C0.x50198 + margin < obj->x8->x18) {
        found = CallScalarBridge0017F3F0_18_func_0017F3F0(obj->xD0, func_001817E0(D_003BD580, id), 100.0f);
        if (found != 0) {
            target = &obj->xD8;
            target->x0 = D_003BD580;
            target->x4 = found;
            target->xA = -1;
            target->x8 = -1;
            obj->x4 = 0;
        } else if (obj->x4 == 3) {
            func_002182A8(obj->x8, 0);
            func_002182A8(obj->xC, 0);
            func_0017FFB8(obj, 4);
        }
    }
}
