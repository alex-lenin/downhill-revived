/* UNIT 0x0013FFF8 0x00140130 */
#include "menu_items.h"
struct Global { char pad0[0x10C]; int x10C; char pad110[0x180]; int x290; };
struct Obj { char pad0[0x418]; int x418; char pad41C[0x8C]; int x4A8; };

extern struct Global D_004B25A0;
extern char D_004225A0[];
extern char D_003F53FD[];
extern void *D_0029DD30;
extern void *D_0029DE68;
extern int func_00194C80(void *, int);
extern void func_0018E6B8(struct Obj *, int, void **);
extern void func_0018DC20(struct Obj *);

void func_0013FFF8(struct Obj *obj) {
    if (func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, D_004B25A0.x10C) == 0) {
        D_003F53FD[0] = 1;
    }
    obj->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x73, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x74, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x75, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x19, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x1A, &D_0029DD30, 1);
    func_0018E6B8(obj, 0x45, &D_0029DE68);
    obj->x4A8 = 5;
    func_0018DC20(obj);
}
