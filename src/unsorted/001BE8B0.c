/* UNIT 0x001BE8B0 0x001BE9D0 */
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
struct Src {
    unsigned int *x0;
    char pad4[0xC];
    char x10[1];
};

extern struct Global D_00398C40;
extern int func_001C0768(struct Mgr *, unsigned int);
extern int func_001BC6A0(struct Mgr *, int, int);
extern void func_001BC868(struct Mgr *, int, int, int, int, int, int, void *, int, int, int);
extern int func_001BC480(struct Mgr *, int);
extern void func_001BBF30(void *, int, struct Entry *);

int func_001BE8B0(int *obj, struct Src *src) {
    struct Global *global = &D_00398C40;
    struct Mgr *mgr = &global->mgr;
    int id;
    int index;
    int buf[4];

    index = -1;
    id = func_001C0768(mgr, *src->x0 >> 18);
    if (id == index) return -1;
    index = func_001BC6A0(mgr, obj[1], obj[0]);
    if (index != 5) {
        func_001BC868(mgr, index, 0x50, 0xA0, obj[1], 0, 1, src->x10, 0, 0, 0);
        obj[0] = func_001BC480(mgr, index);
        buf[0] = id;
        func_00233BA8((EventQueue *)(global->mgr.x4), index, 0x16, (int *)(buf), 8, -1);
        func_001BBF30(global->mgr.x4, index, &mgr->entries[index]);
    }
    return index;
}
