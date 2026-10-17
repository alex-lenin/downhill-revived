/* UNIT 0x0015D278 0x0015D3D8 */
#include "menu_items.h"
struct Obj {
    int x0;
    char pad4[0x418 - 0x4];
    int x418;
    char pad41C[0x460 - 0x41C];
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
};

struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
};

struct Res;

extern struct Res *D_0029DE68;
extern char D_003F52D0[];
extern struct Game D_004B25A0;

extern void func_001B5AF0(char *, int, int);
extern void func_0018E6B8(struct Obj *, int, struct Res **);
extern void func_0018DB08(struct Obj *);

void func_0015D278(struct Obj *obj)
{
    obj->x460 = 100;
    func_001B5AF0(D_003F52D0, 1, 1);
    obj->x418 = 0;
    if (D_004B25A0.x10C != 0x11) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x9F, &D_0029DE68, 1);
    }
    if (D_004B25A0.x288 == 1) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x38, &D_0029DE68, 1);
    } else {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0xA0, &D_0029DE68, 1);
    }
    if (D_004B25A0.x288 >= 2) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0xA1, &D_0029DE68, 1);
    }
    if (D_004B25A0.x288 >= 3) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0xA2, &D_0029DE68, 1);
    }
    if (D_004B25A0.x288 >= 4) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0xA3, &D_0029DE68, 1);
    }
    func_0018E6B8(obj, 0x10, &D_0029DE68);
    obj->x0 = 0;
    obj->x4A8 = 5;
    func_0018DB08(obj);
}
