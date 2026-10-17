/* UNIT 0x00236A30 0x00236B28 */
#include "timer.h"
struct Arena { int x0; int x4; };



extern struct Arena D_00663B08[];

/* placement operator new[](size, p): returns p */
extern TimerNode *func_001FFB68(int size, void *p);
extern void func_00263BA8(TimerNode *node);

static inline void *ArenaAlloc(struct Arena *arena, int size)
{
    int p = (arena->x4 + 15) & ~15;
    arena->x4 = p + size;
    return (void *)p;
}

void func_00236A30(TimerPool *pool, int count)
{
    TimerNode *nodes;
    TimerNode *node;
    TimerNode *link;
    TimerNode *head;
    int size;
    int i;

    pool->active = 0;
    pool->prev = 0;
    pool->freeHead.next = 0;
    pool->freeHead.prev = 0;
    size = count * sizeof(TimerNode);
    nodes = func_001FFB68(size, ArenaAlloc(D_00663B08, size));
    node = nodes;
    i = count;
    while (i-- != 0) {
        func_00263BA8(node);
        node++;
    }
    for (link = nodes; count != 0; count--) {
        head = &pool->freeHead;
        link->next = 0;
        link->prev = head;
        link->next = head->next;
        head->next = link;
        if (link->next != 0) {
            link->next->prev = link;
        }
        link++;
    }
}
