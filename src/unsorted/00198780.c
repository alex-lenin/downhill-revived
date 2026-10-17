/* UNIT 0x00198780 0x001987B8 */
#include "menu.h"
struct Obj {
    char pad0[0x4F8];
    void *x4F8;
};

extern char D_002875A0[];
struct Obj *func_00198780(struct Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_002875A0;
    return obj;
}

