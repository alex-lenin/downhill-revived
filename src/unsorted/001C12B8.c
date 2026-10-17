/* UNIT 0x001C12B8 0x001C1300 */
#include "skat_queue.h"
extern int func_001BC618(void *a, int b);


struct Game {
    char pad0[0x5350];
    int x5350;
    int x5354;
};

extern struct Game D_00398C40;

void func_001C12B8(int *arg)
{
    struct Game *g;
    int r;

    g = &D_00398C40;
    r = func_001BC618(&g->x5350, *arg);
    if (r != 5) {
        func_00235798((SkatQueueOwner *)(&g->x5354), r);
    }
}
