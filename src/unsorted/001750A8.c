/* UNIT 0x001750A8 0x00175100 */
#include "camera.h"
typedef struct { char *x0; char pad[12]; float x10,x14; } Obj;
typedef struct { char pad[12]; float xC; } Value;

void func_001750A8(Obj *obj) {
 Value *v;
 func_001B3028((RiderCamera *)(obj->x0+0x3530), 0, obj->x10);
 v=(Value *)(obj->x0+0x49AC);
 v->xC *= 1.0f-obj->x14;
}
