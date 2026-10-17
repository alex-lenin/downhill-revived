/* UNIT 0x001B3068 0x001B30B8 */
#include "camera.h"
typedef RiderCamera Obj;

void func_001B3068(Obj *o) {
 CameraParameter *p; int i;
 func_001B3108((RiderCamera *)(o));
 p=o->parameters;
 for(i=12;i>=0;i--,p++) p->enabled=1;
}
