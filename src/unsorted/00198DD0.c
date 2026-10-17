/* UNIT 0x00198DD0 0x00198E08 */
#include "menu.h"
typedef struct Obj { char pad[0x4F8]; void *x4F8; } Obj;

extern char D_00287408[];
Obj *func_00198DD0(Obj *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00287408;
    return obj;
}
