/* UNIT 0x001E1EA8 0x001E1F50 */
#include "menu_items.h"
struct Obj { char pad[0x418]; int x418; char pad41c[0x44]; int x460; };
extern char D_0029DE68[], D_003F52D0[];
extern void func_001B5AF0(void *, int, int);
extern void func_0018DB08(struct Obj *);
void func_001E1EA8(struct Obj *p) {
    p->x418 = 0;
    func_001B5AF0(D_003F52D0, 1, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x28, D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x29, D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x23, D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x1a, D_0029DE68, 1);
    p->x460 = 0x9b;
    func_0018DB08(p);
}
