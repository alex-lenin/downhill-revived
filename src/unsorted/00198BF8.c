/* UNIT 0x00198BF8 0x00198C30 */
#include "menu.h"
struct Obj {
    char pad0[0x4F8];
    void *x4F8;
};

extern char D_00287490[];
struct Obj *func_00198BF8(struct Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00287490;
    return obj;
}

