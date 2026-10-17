/* UNIT 0x00198508 0x001985C0 */
#include "menu_items.h"
#include "menu.h"
/* Constructor-like init: func_0018D9D0 is the base constructor (returns the object, which
 * is ignored), D_002876B0 is this class's vtable stored at 0x4F8. */
typedef struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x44];
    int x460;
    int x464;
    int x468;
    char pad46C[0x3C];
    int x4A8;
    char pad4AC[0x4C];
    void *vtable; /* 0x4F8 */
} Obj;

extern char D_002876B0[];
extern int D_0029DE68;

extern void func_0018E6B8(Obj *o, int id, void *arg);

Obj *func_00198508(Obj *o) {
    func_0018D9D0((SharedMenu *)(o));
    o->vtable = D_002876B0;
    o->x460 = 64;
    o->x468 = 12;
    o->x464 = 150;
    func_0018E6B8(o, 16, &D_0029DE68);
    o->x418 = 0;
    o->x4A8 = 5;
    func_0018E3A8((struct SharedMenu *)(void *)(o), 91, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(o), 92, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(o), 93, &D_0029DE68, 1);
    return o;
}
