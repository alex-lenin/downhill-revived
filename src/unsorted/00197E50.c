/* UNIT 0x00197E50 0x00197E88 */
#include "menu.h"
struct Obj {
    char pad0[0x4F8];
    void *x4F8;
};

extern char D_00287848[];
struct Obj *func_00197E50(struct Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00287848;
    return obj;
}
