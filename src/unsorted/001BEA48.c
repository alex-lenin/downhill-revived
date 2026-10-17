/* UNIT 0x001BEA48 0x001BEB50 */
#include "skat_queue.h"
typedef struct Obj {
    int x0;
    char pad4[0x20 - 0x4];
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
} Obj;

typedef struct Globals {
    char pad0[0x5350];
    char x5350[4];
    char x5354[4];
} Globals;

extern Globals D_00398C40;

extern int func_00238F48(void);
extern void func_001DB6B0(int arg, int a, int b, int *count, int *other, int flag);
extern int func_001BC618(void *p, int id);
extern int func_00222B08(int a, int b);
extern void func_001BEB50(Obj *obj, int arg);


void func_001BEA48(Obj *obj, int arg)
{
    int count = 0;
    int other = 0;
    int state;
    Globals *g;

    if (func_00238F48() - obj->x28 > obj->x2C * 60) {
        func_001DB6B0(arg, obj->x20, obj->x24, &count, &other, 0);
        g = &D_00398C40;
        state = func_001BC618(g->x5350, obj->x0);
        if (count > 0) {
            if (func_001BC618(g->x5350, obj->x0) == 5) {
                if (obj->x30 != -1) {
                    obj->x2C = func_00222B08(obj->x30, obj->x34);
                    obj->x28 = func_00238F48();
                }
                func_001BEB50(obj, arg);
            }
        } else if (state != 5) {
            func_00235798((SkatQueueOwner *)(g->x5354), state);
        }
    }
}
