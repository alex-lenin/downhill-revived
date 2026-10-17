/* UNIT 0x0015DE40 0x0015DED0 */
#include "menu_items.h"
struct Obj {
    int x0;
    char pad4[0x414];
    int x418;
    char pad41C[0x44];
    int x460;
    char pad464[0x44];
    int x4A8;
};

extern void func_0018E638(struct Obj *, int, void *);
extern void func_0018E6B8(struct Obj *, int, void *);
extern int D_0029DE68;

void func_0015DE40(struct Obj *p) {
    p->x460 = 0x69;
    func_0018E638(p, 0x11, &D_0029DE68);
    p->x0 = 0;
    p->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x13, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x12, &D_0029DE68, 1);
    func_0018E6B8(p, 0xA5, &D_0029DE68);
    p->x4A8 = 5;
}
