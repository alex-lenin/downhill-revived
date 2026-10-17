/* UNIT 0x001BC968 0x001BCA88 */
#include "event_queue.h"
struct Entry { char pad[0x84]; };
struct Mgr {
    int x0;
    char x4[0x610];
    struct Entry entries[1];
};
struct Global {
    char pad[0x5350];
    struct Mgr mgr;
};

extern struct Global D_00398C40;
extern int func_001BC618(struct Mgr *, int);
extern int func_001BC6A0(struct Mgr *, int, int);
extern void func_001BC868(struct Mgr *, int, int, int, int, int, int, int, int, int, int);
extern int func_001BC480(struct Mgr *, int);
extern void func_001BBF30(void *, int, struct Entry *);

void func_001BC968(int *obj) {
    struct Global *global = &D_00398C40;
    struct Mgr *mgr = &global->mgr;
    int slot;
    int index;
    char buf[16];

    slot = func_001BC618(mgr, obj[0]);
    if (slot == 5) {
        index = func_001BC6A0(mgr, obj[1], obj[0]);
        if (index != slot) {
            func_001BC868(mgr, index, 0, 0, 15, 1, 0, 0, 0, 0, 2);
            obj[0] = func_001BC480(mgr, index);
            func_00233BA8((EventQueue *)(global->mgr.x4), index, 0x4E, (int *)(buf), 1, -1);
            func_001BBF30(global->mgr.x4, index, &mgr->entries[index]);
        }
    } else {
        func_001BBF30(global->mgr.x4, slot, &mgr->entries[slot]);
    }
}
