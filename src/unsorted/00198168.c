/* UNIT 0x00198168 0x001981A0 */
#include "menu.h"
struct Obj {
    char pad0[0x4F8];
    void *x4F8;
};

extern char D_00287738[];
struct Obj *func_00198168(struct Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00287738;
    return obj;
}

