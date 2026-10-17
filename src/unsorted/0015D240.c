/* UNIT 0x0015D240 0x0015D278 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
} Obj;

extern char D_002853F8[];

Obj *func_0015D240(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_002853F8;
    return obj;
}
