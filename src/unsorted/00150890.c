/* UNIT 0x00150890 0x001508C8 */
#include "menu.h"
typedef struct Obj { char pad[0x4F8]; void *x4F8; char pad4FC[0x80]; int x57C; } Obj;
extern char D_002843C0[];

Obj *func_00150890(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_002843C0;
    obj->x57C = 0;
    return obj;
}
