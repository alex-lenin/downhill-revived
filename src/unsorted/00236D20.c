/* UNIT 0x00236D20 0x00236D80 */
#include "timer.h"
TimerNode *func_00236D20(TimerPool *obj) {
 TimerNode *node = obj->freeHead.next;
 if (!node) return 0;
 {
 if (node->prev) node->prev->next = node->next;
 if (node->next) node->next->prev = node->prev;
 node->next = 0;
 node->prev = (TimerNode *)obj;
 node->next = obj->active;
 obj->active = node;
 if (node->next) node->next->prev = node;
 return node;
 }
}
