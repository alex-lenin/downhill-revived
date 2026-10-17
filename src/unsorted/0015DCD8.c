/* UNIT 0x0015DCD8 0x0015DD10 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
} Obj;

extern char D_00285370[];

Obj *func_0015DCD8(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00285370;
    return obj;
}

