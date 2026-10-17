/* UNIT 0x001A0628 0x001A06F8 */
#include "menu_finalize.h"
typedef struct Item {
    char pad0[4];
    char x4;
} Item;

typedef struct Obj {
    char pad0[0x510];
    Item *x510;
    Item *x514;
    unsigned char x518;
    char pad519;
    unsigned char x51A;
    unsigned char x51B;
    int x51C;
} Obj;

typedef struct Game {
    char pad0[0x5C];
    int x5C;
} Game;

extern char D_004225A0[];
extern int D_004B2830[];
extern Game D_003F52D0;

extern int func_00193F38(char *);
extern Item *func_00208FB8(void);


void func_001A0628(Obj *obj)
{
    int last;
    int i;
    int bit;
    int mask;

    mask = func_00193F38(D_004225A0);
    last = (D_004B2830[0] >> 5) - 1;
    obj->x518 = 1;
    obj->x51C = last;
    obj->x51A = 0;
    for (i = 0; i < 26; i++) {
        bit = 1 << i;
        if (bit & mask) {
            obj->x51A++;
        }
    }
    D_003F52D0.x5C = 100;
    obj->x510 = func_00208FB8();
    obj->x514 = func_00208FB8();
    if (obj->x510 != 0) {
        obj->x510->x4 = 0;
    }
    if (obj->x514 != 0) {
        obj->x514->x4 = 0;
    }
    obj->x51B = 1;
    FinalizeMenuLegacy(obj);
}
