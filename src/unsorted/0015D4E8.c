/* UNIT 0x0015D4E8 0x0015D520 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
} Obj;

extern char D_00285480[];

Obj *func_0015D4E8(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00285480;
    return obj;
}

