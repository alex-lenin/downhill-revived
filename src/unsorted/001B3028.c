/* UNIT 0x001B3028 0x001B3050 */
#include "camera.h"
typedef RiderCamera Obj;



void func_001B3028(Obj *obj, int index, float value)
{
    func_001B2E50(&obj->parameters[index], value);
}
