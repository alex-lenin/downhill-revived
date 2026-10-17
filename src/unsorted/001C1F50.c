/* UNIT 0x001C1F50 0x001C2138 */
#include "event_queue.h"
struct Manager { int x0; char x4[0x610]; char x614[0x4000]; };
struct Global { char p0[0x5350]; struct Manager mgr; };
extern struct Global D_00398C40;
extern char *D_0037ED10[];
static inline struct Global *global(void) { return &D_00398C40; }
extern int func_001BC6A0(void *, int, int);
extern int func_001BC480(void *, int);
extern int func_001BFFF0(void *, int);
extern void func_001BC868(void *, int, int, int, int, int, int, void *, int, int, int);
extern void func_001BBF30(void *, int, void *);
int func_001C1F50(int *obj, int id, int value, int tmp)
{
    struct Manager *mgr = &global()->mgr;
    int slot;
    int prop;
    int arg26;
    int arg27;
    slot = func_001BC6A0(mgr, obj[1], obj[0]);
    if (slot != 5) {
        obj[0] = func_001BC480(mgr, slot);
        prop = func_001BFFF0(mgr, tmp);
        if (id == 26) {
            arg26 = prop;
            func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 26, (int *)(&arg26), 4, -1);
            func_001BC868(mgr, slot, 13, 25, obj[1], 0, 1, (void *)value, 0, 0, 2);
        } else if (id == 27) {
            arg27 = prop;
            func_00233BA8((EventQueue *)(global()->mgr.x4), slot, 27, (int *)(&arg27), 4, -1);
            prop = 0x200;
            if (tmp == prop) {
                func_001BC868(mgr, slot, 0x46, 0x64, obj[1], 1, 0, (void *)value, 0, 0, 2);
            } else {
                func_001BC868(mgr, slot, 0x46, 0x64, obj[1], 1, 0, D_0037ED10[0] + 0x10, 0, 0, 2);
            }
        }
        func_001BBF30(global()->mgr.x4, slot,
                      (char *)global() + slot * 0x84 + 0x5964);
    }
    return slot;
}
