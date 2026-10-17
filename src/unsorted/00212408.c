/* UNIT 0x00212408 0x00212488 */
#include "spatial.h"
typedef SpatialIterator Obj;



static inline int IsDone(Obj *p)
{
    return p->x30 >= p->x28;
}

void func_00212408(Obj *p)
{
    while (!IsDone(p)) {
        if (func_002121E0(p)) {
            return;
        }
        p->x34++;
        if (p->x34 >= p->x2C) {
            p->x34 = p->x24;
            p->x30++;
        }
    }
}
