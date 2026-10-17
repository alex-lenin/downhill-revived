/* UNIT 0x00211D68 0x00211DF0 */
#include "spatial.h"
typedef SpatialFilter Sub;

typedef SpatialCellIterator Obj;

typedef struct {
    char pad0[0x24];
    char *x24;
} Global;

extern Global *D_0029DA50;
extern int D_0029DA54;

Obj *func_00211D68(Obj *obj) {
    Sub *sub;

    obj->id = ++D_0029DA54;
    obj->base = D_0029DA50->x24;
    obj->cur = 0;
    obj->end = 0;
    obj->entry = 0;
    obj->node = 0;
    sub = &obj->filter;
    sub->x0 = 0xFFFFFFFF;
    sub->x4.bits.id = 0xFFFF;
    sub->x4.bits.flag1 = 0;
    sub->x4.bits.flag2 = 0;
    sub->x4.bits.flag3 = 0;
    return obj;
}
