/* UNIT 0x001B30B8 0x001B3108 */
#include "camera.h"
typedef RiderCamera Obj;
extern void func_001B3300(Obj *);
extern void func_001B31A0(Obj *);
void func_001B30B8(Obj *obj, struct RiderComponent *value) {
 obj->x10C=0; obj->x110=0; obj->xF0=1; obj->xC0=value; obj->xF4=1.0f;
 func_001B3300(obj); func_001B31A0(obj);
}
