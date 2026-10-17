/* UNIT 0x0018ACB8 0x0018AD78 */
#include "menu_items.h"
typedef struct Obj { char pad0[0x460]; int x460; char pad464[0x44]; int x4A8; } Obj;
extern void func_001873F8(Obj *);
extern int func_0021F818(void);
extern void func_0018E6B8(Obj *, int, void *);
extern void func_00187548(Obj *);
extern int D_003EB760[];
extern float D_0029DE30;
void func_0018ACB8(Obj *obj) {
    obj->x460 = 90;
    func_001873F8(obj);
    func_001873C0((struct SharedMenu *)(void *)(obj), D_003EB760[0] + 40, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), 12, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), func_0021F818() + 16, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), 24, 0);
    func_001873C0((struct SharedMenu *)(void *)(obj), 25, 0);
    func_0018E6B8(obj, 26, &D_0029DE30);
    obj->x4A8 = 5;
    func_00187548(obj);
}
