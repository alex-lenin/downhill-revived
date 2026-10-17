/* UNIT 0x00212048 0x00212098 */
#include "spatial.h"
typedef SpatialIterator Obj;




Obj *func_00212048(Obj *obj)
{
    obj->x28 = 0;
    obj->x2C = 0;
    obj->x30 = 0;
    obj->x34 = 0;
    func_00211D68(&obj->cell);
    func_00212738(&obj->reference);
    obj->reference.actor = 0;
    obj->reference.node = 0;
    return obj;
}
