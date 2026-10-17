/* UNIT 0x001E4150 0x001E41A0 */
#include "timer.h"
struct Obj { char pad0[0x518]; char x518, x519; char pad51A[0x12]; int x52C, x530; };
extern char D_00759780[];
extern void func_002626B8(void);
void func_001E4150(struct Obj *obj) {
    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_002626B8), obj, 1);
    obj->x518 = 0;
    obj->x519 = 0;
    obj->x530 = 0;
    obj->x52C = 0;
}
