/* UNIT 0x00161110 0x00161148 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
} Obj;

extern char D_00284B78[];

Obj *func_00161110(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00284B78;
    return obj;
}

