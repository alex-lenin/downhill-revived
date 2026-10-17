/* UNIT 0x001969E8 0x00196A20 */
#include "menu.h"
struct Obj {
    char pad0[0x4F8];
    void *x4F8;
    char pad4FC[0xC];
    int x508;
};

extern char D_002878D0[];
struct Obj *func_001969E8(struct Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x508 = 0;
    obj->x4F8 = D_002878D0;
    return obj;
}
