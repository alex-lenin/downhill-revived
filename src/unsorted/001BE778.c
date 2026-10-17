/* UNIT 0x001BE778 0x001BE8B0 */
#include "event_queue.h"
typedef struct Item { char pad0[0x84]; } Item;

typedef struct Manager {
    int x0;
    char x4[0x610];
    Item items[1];
} Manager;

typedef struct Global {
    char pad0[0x5350];
    Manager mgr;
} Global;

typedef struct Obj { int x0; int x4; int x8; } Obj;

extern Global D_00398C40;
extern int func_001C0768(Manager *, unsigned int);
extern int func_001BC6A0(Manager *, int, int);
extern void func_001BC868(Manager *, int, int, int, int, int, int, void *, int, int, int);
extern int func_001BC480(Manager *, int);
extern void func_001BBF30(void *, int, Item *);
extern void func_001C7A38(void *);

int func_001BE778(Obj *obj, unsigned int **data, char *arg2)
{
    Global *g;
    Manager *mgr;
    int slot;
    int id;
    int value;

    id = -1;
    g = &D_00398C40;
    mgr = &g->mgr;
    slot = func_001C0768(mgr, **data >> 18);
    if (slot == id)
        return -1;
    id = func_001BC6A0(mgr, obj->x4, obj->x8);
    if (id != 5) {
        func_001BC868(mgr, id, 80, 160, obj->x4, 0, 1, data + 4, 0, 0, 0);
        obj->x8 = func_001BC480(mgr, id);
        value = slot;
        func_00233BA8((EventQueue *)(g->mgr.x4), id, 32, (int *)(&value), 4, -1);
        func_001BBF30(g->mgr.x4, id, &mgr->items[id]);
    }
    func_001C7A38(arg2 + 0x7720);
    return id;
}
