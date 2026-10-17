/* UNIT 0x001C1420 0x001C1548 */
#include "event_queue.h"
typedef struct Entry {
    char pad0[0x84];
} Entry;

typedef struct Mgr {
    int x0;
    int x4;
    char pad8[0x614 - 0x8];
    Entry entries[1];
} Mgr;

typedef struct Global {
    char pad0[0x5350];
    Mgr mgr;
} Global;

typedef struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x2A0 - 0x110];
    int x2A0;
} State;

typedef struct Obj {
    int x0;
    int x4;
    int x8;
} Obj;

extern Global D_00398C40;
extern State D_004B25A0;

extern int func_001BC6A0(Mgr *mgr, int a, int b);
extern void func_001BC868(Mgr *mgr, int id, int a, int b, int c, int d, int e, int f, int g, int h, int i);
extern int func_001BC480(Mgr *mgr, int id);
extern void func_001BBF30(int *p, int id, Entry *entry);

void func_001C1420(Obj *obj)
{
    Global *g;
    Mgr *mgr;
    int id;
    char buf[16];

    if (D_004B25A0.x2A0 == 0x1B)
        return;
    if (D_004B25A0.x2A0 == 0x1C)
        return;
    if (D_004B25A0.x2A0 == 0x17)
        return;
    if (D_004B25A0.x10C == 0xD)
        return;
    if (D_004B25A0.x10C == 0x12)
        return;
    if (D_004B25A0.x10C == 0xE)
        return;
    g = &D_00398C40;
    mgr = &g->mgr;
    id = func_001BC6A0(mgr, obj->x4, obj->x0);
    if (id != 5) {
        func_001BC868(mgr, id, 0, 0, obj->x4, 0, 0, 0, 0, 0, 8);
        obj->x0 = func_001BC480(mgr, id);
        func_00233BA8((EventQueue *)(&g->mgr.x4), id, 0x35, (int *)(buf), 1, -1);
        func_001BBF30(&g->mgr.x4, id, &mgr->entries[id]);
    }
    obj->x8 = id;
}
