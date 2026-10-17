/* UNIT 0x0012A3E8 0x0012A4D0 */
#include "rider_geometry.h"
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Game { char pad0[0x31B8]; float x31B8; } Game;
typedef struct Obj {
    char pad0[0x20];
    Game *x20;
    char pad24[0x4C - 0x24];
    Vec x4C;
    char pad5C[0x80 - 0x5C];
    char x80[0x50];
    char xD0[0x10];
    char xE0[0x6C];
    float x14C;
} Obj;

extern void func_0012B540(Obj *obj, float t);

extern float func_00238F40(void);
extern void func_00236558(void *a, void *b, float t);
extern void func_00203E10(Vec *v, float t);
extern void func_0012B188(Obj *obj);
extern void func_00129FC8(Obj *obj);
extern void func_00129860(Obj *obj, int state);

void func_0012A3E8(Obj *obj)
{
    float tmp[4] __attribute__((aligned(16)));
    Vec value;
    float t;

    if (obj->x14C > 0.0f) {
        __asm__ volatile ("max.s %0, %1, %2" : "=&f"(t) : "f"(obj->x14C - 0.0073333336f), "f"(0.0f));
        func_0012B540(obj, t);
        func_00260E08(obj->x80, obj->xD0, obj->xE0, tmp);
        func_00236558(obj->xD0, tmp, func_00238F40());
        value = obj->x4C;
        func_00203E10(&value, obj->x14C / obj->x20->x31B8);
        func_0012B188(obj);
        func_00129FC8(obj);
    } else {
        func_00129860(obj, 4);
    }
}
