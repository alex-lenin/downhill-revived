/* UNIT 0x001C1650 0x001C17B8 */
#include "event_queue.h"
struct Entry { int x0; char data[0x80]; };
struct Mgr {
    int x0;
    char x4[0x60C];
    struct Entry entries[1];
};
struct Global {
    char pad[0x5350];
    struct Mgr mgr;
};

extern struct Global D_00398C40;

static inline struct Global *global(void) { return &D_00398C40; }
extern int D_003817BC[];
extern int func_001BC6A0(struct Mgr *, int, int);
extern void func_001BC868(struct Mgr *, int, int, int, int, int, int, int, int, int, int);
extern int func_001BC480(struct Mgr *, int);
extern int func_001C0AD0(struct Mgr *);
extern void func_001BBF30(void *, int, void *);

void func_001C1650(int *obj, int id) {
    struct Mgr *mgr = &global()->mgr;
    int index;
    int mode;
    int args[2];

    index = func_001BC6A0(mgr, obj[1], obj[0]);
    if (index != 5) {
        func_001BC868(mgr, index, 0, 0, obj[1], 0, 0, 0, 0, 0, 8);
        obj[0] = func_001BC480(mgr, index);
        switch (D_003817BC[0]) {
        case 2:
            mode = 1;
            break;
        case 0:
            mode = 2;
            break;
        case 1:
            mode = 3;
            break;
        default:
            mode = 0;
            break;
        }
        if (id == 0x36) {
            args[0] = func_001C0AD0(&global()->mgr);
            args[1] = mode;
            func_00233BA8((EventQueue *)(global()->mgr.x4), index, 0x36, (int *)(args), 8, -1);
        }
        func_001BBF30(global()->mgr.x4, index, global()->mgr.entries[index].data);
    }
    obj[2] = index;
}
