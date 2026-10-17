/* UNIT 0x0019ED18 0x0019EDE8 */
#include "timer.h"
typedef struct Timer { int x0,x4,x8,xC; } Timer;
typedef struct Obj { char pad[0x190]; Timer timer; char pad1A0[0x2E8]; int x488; } Obj;
extern char D_00759780[];
extern struct Global { char pad[0xF0]; int xF0; } D_004BD740;
extern int func_001C2A90(void *);
extern float func_00203C28(Timer *);
extern void func_0019EDE8(Obj *);
void func_0019ED18(Obj *obj) {
 Timer timer;
 if(obj->x488==5) {
  func_00236C60((TimerPool *)(D_00759780));
  if(func_001C2A90(&D_004BD740)) {
   if(D_004BD740.xF0) func_0019EDE8(obj);
  } else {
   if(!obj->timer.x0 || (timer=obj->timer, func_00203C28(&timer)>=1.0f)) func_0019EDE8(obj);
  }
 }
}
