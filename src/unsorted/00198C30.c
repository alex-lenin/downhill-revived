/* UNIT 0x00198C30 0x00198CC0 */
#include "menu_items.h"
struct Obj { int x0; char pad4[0x414]; int x418; char pad41c[0x44]; int x460; char pad464[0x44]; int x4A8; };
extern int D_0029DE68;
extern char D_003F52D0[];
extern void func_0018E638(struct Obj *, int, void *);
extern void func_001B5AF0(void *, int, int);
void func_00198C30(struct Obj *obj) {
    obj->x460=0x69;
    func_0018E638(obj,0x11,&D_0029DE68);
    func_001B5AF0(D_003F52D0,1,1);
    obj->x0=0;
    obj->x418=0;
    func_0018E3A8((struct SharedMenu *)(void *)(obj),0x13,&D_0029DE68,1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj),0xa4,&D_0029DE68,1);
    obj->x4A8=5;
}
