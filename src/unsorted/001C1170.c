/* UNIT 0x001C1170 0x001C1280 */
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
    int x8;
    int slot;
    int x10;
} Req;

extern Global D_00398C40;

extern int func_001BC6A0(void *, int, int);
extern void func_001BC868(void *, int, int, int, int, int, int, int, int, int, int);
extern int func_001BC480(void *, int);
extern int func_001C0958(void *);
extern void func_001BBF30(void *, int, void *);

int func_001C1170(Req *req)
{
    Global *g = &D_00398C40;
    Sys *sys = &g->sys;
    int slot;
    int params[3];

    slot = func_001BC6A0(sys, req->x4, req->slot);
    if (slot != 5) {
        func_001BC868(sys, slot, 0, 0, req->x4, 0, 0, 0, 0, 0, 2);
        req->slot = func_001BC480(sys, slot);
        if (req->x8 == 0x666) {
            params[0] = func_001C0958(sys);
            params[1] = 1;
            params[2] = 1;
            func_00233BA8((EventQueue *)(g->sys.x4), slot, 0x12, (int *)(params), 0xC, -1);
        }
        req->x10 = 0;
        func_001BBF30(g->sys.x4, slot, &sys->entries[slot]);
    }
    return slot;
}
