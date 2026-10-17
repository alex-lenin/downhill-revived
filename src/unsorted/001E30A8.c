/* UNIT 0x001E30A8 0x001E30E0 */
#include "menu.h"
typedef struct Obj { char pad[0x4F8]; void *x4F8; } Obj;

extern char D_00288128[];
Obj *func_001E30A8(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00288128;
    return obj;
}

