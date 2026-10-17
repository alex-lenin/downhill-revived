/* UNIT 0x001AC880 0x001AC918 */
#include "animation.h"
#include "timer.h"
typedef AnimationController Obj;
extern void func_001ACA08(Obj *);
extern char D_00262580[],D_00759780[];
void func_001AC880(Obj *obj,float time) {
 if(obj->x0 && obj->x0->x0) {
  if(time>0.0f) {
   obj->x5C = -obj->x44 * 16.0f / time;
   func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(D_00262580), obj, 1);
  } else {
   obj->x5C = -obj->x44;
   func_001ACA08(obj);
  }
 }
}
