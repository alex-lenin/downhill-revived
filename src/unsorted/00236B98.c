/* UNIT 0x00236B98 0x00236BF8 */
#include "timer.h"
extern int func_00238F48(void);
TimerNode *func_00236B98(TimerPool *obj, TimerCallbackNoArg callback, int offset) {
 TimerNode *node = func_00236D20(obj);
 if (node) {
 int time = func_00238F48() + offset;
 node->callback = (TimerCallback)callback;
 node->due = time;
 node->context = 0;
 }
 return node;
}
