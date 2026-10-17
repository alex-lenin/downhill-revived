/* UNIT 0x001C0E00 0x001C0F88 */
#include "skat_queue.h"
typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Obj;

typedef struct Item {
    char pad0[0x20];
    int x20;
    char pad24[0xC];
} Item;
typedef struct GlobalA {
    char pad0[0x240];
    Item chunks[4][12];
    int counts[4];
    char padB50[0x5350 - 0xB50];
    char mgr[4];
    char payload[4];
} GlobalA;

typedef struct GlobalB {
    char pad0[0x288];
    int x288;
} GlobalB;

extern GlobalA D_00398C40;
extern GlobalB D_004B25A0;
static inline GlobalA *GetGlobal(void) { return &D_00398C40; }
extern int func_001BC618(void *a, int b);

extern int func_001C1170(Obj *o);
extern int func_001C0F88(Obj *o);

void func_001C0E00(Obj *obj)
{
    int i;
    int j;
    int found;
    int even = 0, odd = 0;
    int id1;
    int id2;
    int cnt;
    int target;
    int n;
    GlobalA *g;
    int *p;

    i = 0;
    found = 0;
    if (i < D_004B25A0.x288) {
        g = GetGlobal();
        for (; i < D_004B25A0.x288; i++) {
            GlobalA *t = g;
            j = 0;
            if (j < t->counts[i]) {
                
                cnt = g->counts[i];
                target = obj->x8;
                for (; j < t->counts[i]; j++) {
                    if (g->chunks[i][j].x20 == target) {
                        found = 1;
                        if (D_004B25A0.x288 >= 2) {
                            if (!(i & 1)) { found = 1; j = cnt; goto done; }
                            else { found = 1; j = cnt; goto done; }
                            goto done;
                        }
                    }
                }
            done:
                ;
            }
        }
    }
    id1 = func_001BC618(GetGlobal()->mgr, obj->xC);
    id2 = func_001BC618(GetGlobal()->mgr, obj->x0);
    if (found != 0) {
        if (id1 == 5) {
            if (obj->x10 != 0) {
                func_001C1170(obj);
            }
        }
        if (id2 == 5) {
            func_001C0F88(obj);
        }
    }
    if (found == 0) {
        if (id1 != 5) {
            func_00235798((SkatQueueOwner *)(GetGlobal()->payload), id1);
        }
    }
    if (found == 0) {
        if (id2 != 5) {
            func_00235798((SkatQueueOwner *)(GetGlobal()->payload), id2);
        }
    }
}
