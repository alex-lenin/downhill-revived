/* UNIT 0x00236C00 0x00236C60 */
#include "timer.h"
extern unsigned int func_00238F48(void);
void func_00236C00(TimerPool *obj) {
    unsigned int elapsed = func_00238F48();
    TimerNode *node = obj->active;
    while (node) {
        if (elapsed < node->due) node->due -= elapsed;
        else node->due = 1;
        node = node->next;
    }
}
