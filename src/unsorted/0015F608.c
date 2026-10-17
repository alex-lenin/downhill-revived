/* UNIT 0x0015F608 0x0015F730 */
#include "menu_items.h"
typedef struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x45C - 0x41C];
    int x45C;
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
    char pad4AC[0x504 - 0x4AC];
    int x504;
    int x508;
} Obj;

extern long D_0029DE68;
extern int D_004B26AC[];
extern void func_0018E638(Obj *, int, long *);
extern void func_0018E6B8(Obj *, int, long *);

void func_0015F608(Obj *obj) {
    func_0018E638(obj, 0x21, &D_0029DE68);
    obj->x45C = 0x10;
    if (obj->x504 == 0) {
        func_0018E6B8(obj, 0xA5, &D_0029DE68);
        obj->x4A8 = 5;
    } else {
        func_0018E6B8(obj, 0xAC, &D_0029DE68);
    }
    obj->x418 = 0;
    if (obj->x504 != 0) {
        if (obj->x508) {
            func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x22, &D_0029DE68, 1);
        } else {
            func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x1B, &D_0029DE68, 1);
        }
    } else {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x19, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x1A, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x22, &D_0029DE68, 1);
        if (D_004B26AC[0] != 0x13) {
            func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x23, &D_0029DE68, 1);
        }
    }
    obj->x460 = 0x50;
}
