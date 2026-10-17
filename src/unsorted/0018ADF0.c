/* UNIT 0x0018ADF0 0x0018AE48 */
#include "menu_items.h"
struct Obj { char pad[0x418]; int x418; };
extern void func_00187548(struct Obj *);
void func_0018ADF0(struct Obj *obj) {
    obj->x418 = 0;
    func_001873C0((struct SharedMenu *)(void *)(obj), 0x46, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), 7, 0);
    func_00187548(obj);
}
