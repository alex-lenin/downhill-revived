/* UNIT 0x0015FCB8 0x0015FD48 */
#include "menu_items.h"
#include "menu.h"
struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x44];
    int x460;
    char pad464[0x4];
    int x468;
    char pad46C[0x8C];
    void *x4F8;
};


extern void func_0018E638(struct Obj *, int, void *);
extern char D_00284E20[];
extern int D_0029DE68;

struct Obj *func_0015FCB8(struct Obj *p) {
    func_0018D9D0((SharedMenu *)(p));
    p->x4F8 = D_00284E20;
    func_0018E638(p, 0x3F, &D_0029DE68);
    p->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x19, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x46, &D_0029DE68, 1);
    p->x460 = 0x54;
    p->x468 = 10;
    return p;
}
