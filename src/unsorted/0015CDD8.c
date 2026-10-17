/* UNIT 0x0015CDD8 0x0015CE10 */
#include "menu.h"
struct Obj {
    char pad0[0x4F8];
    void *x4F8;
};

extern char D_00285590[];


struct Obj *func_0015CDD8(struct Obj *o) {
    InitializeMenuDiscard((SharedMenu *)(o));
    o->x4F8 = D_00285590;
    return o;
}
