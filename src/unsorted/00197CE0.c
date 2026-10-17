/* UNIT 0x00197CE0 0x00197DA0 */
#include "menu_items.h"
#include "menu.h"
typedef struct Obj { char pad0[0x418]; int x418; char pad41C[0x44]; int x460; int x464; int x468; char pad46C[0x8C]; void *x4F8; } Obj;

extern void func_0018E638(Obj *, int, void *);
extern char D_00287628[];
extern float D_0029DE68;
Obj *func_00197CE0(Obj *obj) {
    func_0018D9D0((SharedMenu *)(obj));
    obj->x4F8 = D_00287628;
    func_0018E638(obj, 94, &D_0029DE68);
    obj->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 34, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 89, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 41, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 26, &D_0029DE68, 1);
    obj->x460 = 64;
    obj->x468 = 10;
    return obj;
}
