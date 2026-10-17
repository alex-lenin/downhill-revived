/* UNIT 0x00208660 0x002086C8 */
#include "timer.h"
#include "tween.h"
extern int func_00202DA8(void);
extern void func_00263AC8(void);
extern char D_00759780[];
int func_00208660(FloatTween *obj, float *arg1, int arg2, float start, float end) {
    int value;
    obj->target = arg1;
    obj->start = start;
    obj->delta = end - start;
    value = func_00202DA8();
    obj->duration = arg2;
    obj->startTick = value;
    return (int)func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00263AC8), obj, 1);
}
