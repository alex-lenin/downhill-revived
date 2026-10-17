/* UNIT 0x001516D8 0x00151730 */
#include "menu_items.h"
struct Obj { char pad0[0x418]; int x418; char pad41c[0x44]; int x460; };
extern int D_0029DE68;
void func_001516D8(struct Obj *obj) {
    obj->x418 = 0;
    obj->x460 = 0xaa;
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x23, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x1b, &D_0029DE68, 1);
}
