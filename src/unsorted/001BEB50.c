/* UNIT 0x001BEB50 0x001BECD0 */
#include "event_queue.h"
typedef struct Entry { char pad0[0x84]; } Entry;

typedef struct Mgr {
    int x0;
    char x4[0x610];
    Entry entries[5];
} Mgr;

typedef struct Global {
    char pad0[0x5350];
    Mgr mgr;
} Global;

extern Global D_00398C40;
extern int func_001BC6A0(Mgr *, int, int);
extern void func_001BC868(Mgr *, int, int, int, int, int, int, void *, int, int, int);
extern int func_001BC480(Mgr *, int);
extern void func_001BBF30(void *, int, Entry *);

int func_001BEB50(int *obj, void *arg1)
{
    Global *global = &D_00398C40;
    Mgr *mgr = &global->mgr;
    Global *other;
    int index;
    unsigned char flags[4];

    index = func_001BC6A0(mgr, obj[1], obj[0]);
    if (index != 5) {
        func_001BC868(mgr, index, obj[8], obj[9], obj[1], 0, 1, arg1, 0, 0, 0);
        obj[0] = func_001BC480(mgr, index);
        switch (obj[2]) {
        case 30:
            func_00233BA8((EventQueue *)(global->mgr.x4), index, 30, (int *)(&flags[0]), 1, -1);
            break;
        case 42:
            func_00233BA8((EventQueue *)(global->mgr.x4), index, 42, (int *)(&flags[1]), 1, -1);
            break;
        case 76:
            func_00233BA8((EventQueue *)(global->mgr.x4), index, 76, (int *)(&flags[2]), 1, -1);
            break;
        case 41:
            func_00233BA8((EventQueue *)(global->mgr.x4), index, 41, (int *)(&flags[3]), 1, -1);
            break;
        }
        /* A second pointer to D_00398C40 and a byte offset (== &other->mgr.entries[index])
           are needed for the retail address arithmetic. */
        other = &D_00398C40;
        func_001BBF30(other->mgr.x4, index, (Entry *)((char *)other + index * 0x84 + 0x5964));
    }
    return index;
}
