/* UNIT 0x00128720 0x00128818 */
#include "animation.h"
typedef struct Item {
    int x0;
    int x4;
    char pad8[0xF0 - 0x8];
} Item;

typedef struct Obj {
    char pad0[0x68];
    char x68[0x220 - 0x68];
    void *vtbl;
    char pad224[0x230 - 0x224];
} Obj;

typedef struct Pool {
    Obj objs[200];
    char order[200];
    char count;
} Pool;

typedef struct Manager {
    int x0;
    char pad4[0xC];
    Item items[100];
    Pool pool;
} Manager;

extern Manager D_00308D40;
extern char D_002839D8[];


/* static initializer: constructs the global manager (inlined C++ constructors) */
void func_00128720(int initialize, int priority)
{
    int n;
    int k;
    int i;
    Manager *m;
    Pool *pool;
    Item *item;
    Obj *obj;
    char *order;

    if (priority == 0xFFFF && initialize != 0) {
        m = &D_00308D40;
        m->x0 = 0;
        item = m->items;
        n = 99;
        do {
            item->x0 = 0;
            item->x4 = 0;
            item++;
        } while (--n != -1);
        pool = &m->pool;
        obj = pool->objs;
        k = 199;
        do {
            obj->vtbl = D_002839D8;
            func_001AC780((AnimationController *)(obj->x68), 2, 1);
            obj++;
        } while (--k != -1);
        order = &pool->order[199];
        for (i = 199; i >= 0; i--) {
            *order-- = i;
        }
        pool->count = 0;
    }
}
