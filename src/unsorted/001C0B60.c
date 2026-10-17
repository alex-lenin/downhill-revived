/* UNIT 0x001C0B60 0x001C0BF8 */
#include "skat_queue.h"
struct Obj {
    int x0;
    char pad4[0x10 - 0x4];
    char x10[0x10];
    int x20;
    int x24;
};

struct Global {
    char pad0[0x5350];
    char x5350[4];
    char x5354[4];
};

extern struct Global D_00398C40;

extern void func_001DB6B0(void *a, int b, int c, int *d, int *e, int f);
extern int func_001BC618(void *a, int b);
extern void func_001C0BF8(struct Obj *obj);


void func_001C0B60(struct Obj *obj)
{
    int out[2];
    struct Global *g;
    int state;

    out[0] = 0;
    out[1] = 0;
    func_001DB6B0(obj->x10, obj->x20, obj->x24, &out[0], &out[1], 0);
    g = &D_00398C40;
    state = func_001BC618(g->x5350, obj->x0);
    if (out[0] > 0) {
        if (state == 5) {
            func_001C0BF8(obj);
        }
    } else {
        if (state != 5) {
            func_00235798((SkatQueueOwner *)(g->x5354), state);
        }
    }
}
