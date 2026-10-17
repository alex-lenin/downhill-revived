/* UNIT 0x0012A570 0x0012A6D0 */
#include "rider_geometry.h"
typedef struct Sound { int x0, x4, x8, xC; } Sound;
typedef struct Obj {
    int x0;
    unsigned int **x4;
    char pad8[0x2C - 0x8];
    Sound x2C;
    char pad3C[0x160 - 0x3C];
    float x160;
    char pad164[0x174 - 0x164];
    int x174;
    char pad178[0x190 - 0x178];
    int x190;
} Obj;

extern void func_00129FC8(Obj *);

extern float func_00238F40(void);
extern void func_00236558(void *, void *, float);
extern void func_0012B188(Obj *);
extern int func_00222BD8(int, int);
extern void func_00129860(Obj *, int);
extern void func_00203590(void *, int, int, int);
extern void func_00203DC8(Sound *, int);
extern void func_00203E10(Sound *, float);
extern void func_00202E38(void *, int, int, float);

void func_0012A570(Obj *obj)
{
    float vec[4];
    void *pos;
    Sound sound;

    func_00129FC8(obj);
    pos = (char *)obj + 0xD0;
    func_00260E08((char *)obj + 0x80, pos, (char *)obj + 0xE0, vec);
    func_00236558(pos, vec, func_00238F40());
    func_0012B188(obj);
    if ((**obj->x4 & 0xFFFC0000) == 0x12D40000 && obj->x174 >= 181) {
        if (func_00222BD8(0, 1) != 0) {
            func_00129860(obj, 3);
        } else {
            func_00203590(&obj->x2C, **obj->x4 >> 18, obj->x190, 1);
            sound = obj->x2C;
            func_00203DC8(&sound, 1);
            sound = obj->x2C;
            func_00203E10(&sound, obj->x160);
            func_00202E38(&obj->x2C, 1, 1, 600.0f);
            obj->x174 = 0;
        }
    }
}
