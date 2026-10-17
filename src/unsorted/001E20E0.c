/* UNIT 0x001E20E0 0x001E2118 */
#include "menu.h"
typedef struct Obj { char pad[0x4F8]; void *x4F8; } Obj;

extern char D_002881B0[];
Obj *func_001E20E0(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_002881B0;
    return obj;
}
