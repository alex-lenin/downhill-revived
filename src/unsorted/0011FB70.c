/* UNIT 0x0011FB70 0x0011FC70 */
#include "rider_geometry.h"
typedef struct Obj {
    char pad0[4];
    int state;
    char pad8[0x110 - 8];
    char x110[0x50];
    char x160[0x10];
    char x170[0xEC];
    short x25C;
    char pad25E[0x268 - 0x25E];
    unsigned long x268_lo : 50;
    unsigned long x268_moving : 1;
    unsigned long x268_hi : 13;
} Obj;


extern float func_00238F40(void);
extern void func_00236558(void *, float *, float);
extern void func_00120CE8(Obj *);
extern void func_0011EDA8(Obj *);
extern int func_00222BD8(int, int);
extern void func_0011E568(Obj *, int);

void func_0011FB70(Obj *obj)
{
    float buf[4];
    float dist;

    dist = func_00260E08(obj->x110, obj->x160, obj->x170, buf);
    func_00236558(obj->x160, buf, func_00238F40());
    obj->x268_moving = dist > 5.0f;
    func_00120CE8(obj);
    func_0011EDA8(obj);
    if (obj->x25C >= 0xB5 && obj->state != 9) {
        if (func_00222BD8(0, 100) == 0x2A) {
            func_0011E568(obj, 4);
        }
    }
}
