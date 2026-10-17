/* UNIT 0x001B0E00 0x001B0EA8 */
#include "camera.h"
#include "rider.h"

typedef RiderComponent Parent;
typedef RiderCamera Obj;
extern void func_002185C0(void *,int);

extern void func_0012C840(Rider *);

extern void func_001B1098(void *);

void func_001B0E00(Obj *obj) {
 Parent *parent = obj->xA10;
 if(parent && parent->rider) {
  func_002185C0(parent->node,0);
  func_002185C0(obj->xA10->rider->bodyNode,0);
  func_001B3128(obj);
  func_0012C840(obj->xA10->rider);
  func_001AEB88((RiderCameraRig *)((char *)obj->xA10->rider+0x46D0), (RiderCamera *)(obj));
  func_001B1098((char *)obj+0x5C0);
  if(obj->xA10->rider->x7A5C == 1) func_001B0D38((RiderCamera *)(obj));
 }
}
