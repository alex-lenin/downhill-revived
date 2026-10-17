/* UNIT 0x0015F5D0 0x0015F608 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
    char pad4FC[4];
    int x500;
} Obj;

extern char D_00284F30[];

Obj *func_0015F5D0(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00284F30;
    obj->x500 = 0;
    return obj;
}
