/* UNIT 0x001C1300 0x001C1420 */
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
struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x190];
    int x2A0;
};

extern struct State D_004B25A0;
extern struct Global D_00398C40;
extern int func_001BC6A0(struct Mgr *, int, int);
extern void func_001BC868(struct Mgr *, int, int, int, int, int, int, int, int, int, int);
extern int func_001BC480(struct Mgr *, int);
extern void func_001BBF30(void *, int, struct Entry *);

void func_001C1300(int *obj) {
    struct State *state = &D_004B25A0;
    struct Global *global;
    struct Mgr *mgr;
    int index;
    int buf[4];

    if (state->x2A0 == 0x1B) return;
    if (state->x2A0 == 0x1C) return;
    if (state->x10C == 0xD || state->x10C == 0x12 || state->x10C == 0xE) return;
    global = &D_00398C40;
    mgr = &global->mgr;
    index = func_001BC6A0(mgr, obj[1], obj[0]);
    if (index != 5) {
        func_001BC868(mgr, index, 0, 0, obj[1], 0, 0, 0, 0, 0, 8);
        obj[0] = func_001BC480(mgr, index);
        func_00233BA8((EventQueue *)(global->mgr.x4), index, 0x34, (int *)(buf), 1, -1);
        func_001BBF30(global->mgr.x4, index, &mgr->entries[index]);
    }
    obj[2] = index;
}
