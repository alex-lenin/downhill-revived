/* UNIT 0x0019A2B8 0x0019A3C0 */
#include "menu_items.h"
typedef struct Sub {
    char pad0[0x4];
    char x4;
} Sub;

typedef struct Global {
    char pad0[0x134];
    Sub *x134;
} Global;

typedef struct Obj {
    int x0;
    char pad4[0x414];
    int x418;
    char pad41C[0x44];
    int x460;
    int x464;
    char pad468[0x40];
    int x4A8;
} Obj;

extern char D_0043DCC0[];
extern int D_0026F978[];
extern long D_0029DE68[];
extern Global D_003F52D0;

extern int func_0019BDD8(char *);
extern void func_0018E408(Obj *, int, int);
extern void func_0018E6B8(Obj *, int, long *);

void func_0019A2B8(Obj *obj) {
    int i;
    Sub *sub;

    obj->x464 = 60;
    obj->x460 = 70;
    obj->x0 = func_0019BDD8(D_0043DCC0);
    obj->x418 = 0;
    for (i = 0; i < 4; i++) {
        func_0018E408(obj, D_0026F978[i], 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x81, D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x82, D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x80, D_0029DE68, 1);
    obj->x464 = 120;
    func_0018E6B8(obj, 0xA5, D_0029DE68);
    obj->x4A8 = 5;
    obj->x464 = 60;
    sub = D_003F52D0.x134;
    if (sub != 0) {
        sub->x4 = 0;
    }
}
