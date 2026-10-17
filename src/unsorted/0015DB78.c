/* UNIT 0x0015DB78 0x0015DBB8 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x45C];
    int x45C;
    char pad460[0x98];
    void *x4F8;
} Obj;

extern char D_002851D8[];

Obj *func_0015DB78(Obj *obj) {
    func_0018D9D0((SharedMenu *)(obj));
    obj->x45C = 0x10;
    obj->x4F8 = D_002851D8;
    return obj;
}
