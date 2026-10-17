/* UNIT 0x00140248 0x001403C8 .sbss=0x29DD2C:0x29DD30 */
#include "menu_items.h"
#include "ui_style.h"
int D_0029DD2C __attribute__((section(".sbss"))) = 0;
typedef struct Obj { char pad0[0xC]; void *xC, *x10; char pad14[0x418-0x14]; int x418; } Obj;
typedef struct Global { char pad0[0x10C]; int x10C; char pad110[0x290-0x110]; int x290; } Global;
extern Global D_004B25A0;
extern char D_00362380[], D_0036B120[], D_00365800[], D_00365DE0[];
extern void *D_0029DD30;
extern void *func_0013AF60(int);
extern void func_00218498(void *, int);
extern void func_001F57D0(int, int);
extern void func_0013CE20(void *), func_001406C8(void *), func_0018DC20(Obj *);
static __inline__ int clamp(int value, int threshold) {
    if (value > threshold) return threshold + 1;
    return value;
}
void func_00140248(Obj *obj) {
    int id;
    obj->x418 = 0;
    if (D_0029DD2C == 0) {
        func_0018D7D0((UiStyle *)(void *)(D_00362380), 350, 200, -112, -32, 100, 100, 100, 255);
        D_0029DD2C = 1;
    }
    obj->xC = D_0036B120;
    obj->x10 = D_0036B120;
    id = clamp(D_004B25A0.x10C, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), id + 89, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 88, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 16, &D_0029DD30, 0);
    func_00218498(func_0013AF60(0x2591), D_004B25A0.x290 / 32 - 1);
    func_00218498(func_0013AF60(0x2598), 1);
    func_001F57D0(0x2351, D_004B25A0.x290 / 32 - 1);
    func_0013CE20(D_00365800);
    func_001406C8(D_00365DE0);
    func_0018DC20(obj);
}
