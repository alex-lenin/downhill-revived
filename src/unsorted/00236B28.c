/* UNIT 0x00236B28 0x00236B98 */
#include "timer.h"
extern int func_00238F48(void);
TimerNode *func_00236B28(TimerPool *arg, TimerCallback callback, void *context, int delta) {
    TimerNode *obj = func_00236D20(arg);
    if (obj) {
        int value = func_00238F48() + delta;
        obj->callback = callback;
        obj->due = value;
        obj->context = context;
    }
    return obj;
}
