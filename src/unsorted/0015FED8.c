/* UNIT 0x0015FED8 0x0015FF38 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x45C];
    int x45C;
    int x460;
    char pad464[0x44];
    int x4A8;
    char pad4AC[0x4C];
    void *x4F8;
} Obj;


extern void func_0018E6B8(Obj *obj, int id, void *arg);
extern char D_00284D10[];
extern int D_0029DE68;

Obj *func_0015FED8(Obj *obj) {
    func_0018D9D0((SharedMenu *)(obj));
    obj->x4F8 = D_00284D10;
    obj->x460 = 0x28;
    func_0018E6B8(obj, 0x7C, &D_0029DE68);
    obj->x4A8 = 5;
    obj->x45C = 0x10;
    return obj;
}
