/* UNIT 0x001C1548 0x001C1648 */
#include "event_queue.h"
typedef struct Entry {
    char pad0[0x84];
} Entry;

typedef struct Sys {
    int x0;
    char x4[0x610];
    Entry entries[4];
} Sys;

typedef struct Global {
    char pad0[0x5350];
    Sys sys;
} Global;

typedef struct Req {
    int x0;
    int x4;
    int slot;
} Req;

typedef struct Rider {
    char pad0[0x796C];
    int x796C;
} Rider;

extern Global D_00398C40;

extern int func_001BC6A0(void *, int, int);
extern void func_001BC868(void *, int, int, int, int, int, int, int, int, int, int);
extern int func_001BC480(void *, int);
extern int func_001BFFF0(void *, int);
extern void func_001BBF30(void *, int, void *);

void func_001C1548(Req *req, Rider *rider)
{
    Global *g = &D_00398C40;
    Sys *sys = &g->sys;
    int slot;
    int value;

    slot = func_001BC6A0(sys, req->x4, req->x0);
    if (slot != 5) {
        func_001BC868(sys, slot, 0, 0, req->x4, 0, 0, 0, 0, 0, 8);
        req->x0 = func_001BC480(sys, slot);
        value = func_001BFFF0(sys, rider->x796C);
        func_00233BA8((EventQueue *)(g->sys.x4), slot, 0x37, (int *)(&value), 4, -1);
        func_001BBF30(g->sys.x4, slot, &sys->entries[slot]);
    }
    req->slot = slot;
}
