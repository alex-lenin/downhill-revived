/* UNIT 0x00236C60 0x00236D20 */
#include "timer.h"


extern unsigned int func_00238F48(void);

int func_00236C60(TimerPool *list)
{
    unsigned int now;
    TimerNode *timer;
    TimerNode *next;

    now = func_00238F48();
    timer = list->active;
    while (timer != 0) {
        if (now >= timer->due) {
            if (timer->context != 0) {
                timer->due = timer->callback(timer->context);
            } else {
                timer->due = ((TimerCallbackNoArg)timer->callback)();
            }
            if (timer->due != 0) {
                timer->due += now;
                timer = timer->next;
            } else {
                next = timer->next;
                func_00236D80(list, timer);
                timer = next;
            }
        } else {
            timer = timer->next;
        }
    }
    return 1;
}
