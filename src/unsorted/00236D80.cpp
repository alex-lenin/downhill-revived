/* UNIT 0x00236D80 0x00236DD0 */
#include "timer.h"
void TimerPool::func_00236D80(TimerNode *node) {
    TimerNode *head;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    head = &freeHead;
    node->next = 0;
    node->prev = head;
    node->next = head->next;
    head->next = node;
    if (node->next) node->next->prev = node;
}
