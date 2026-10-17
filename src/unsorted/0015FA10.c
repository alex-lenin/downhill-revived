/* UNIT 0x0015FA10 0x0015FA48 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
} Obj;

extern char D_00284EA8[];

Obj *func_0015FA10(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00284EA8;
    return obj;
}

