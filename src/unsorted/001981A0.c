/* UNIT 0x001981A0 0x00198368 */
#include "menu_items.h"
typedef struct Table {
    char pad0[0xBD0];
    int limits[6];
} Table;

typedef struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x464 - 0x41C];
    int x464;
    char pad468[0x500 - 0x468];
    int x500;
} Obj;

typedef struct State {
    char pad0[0x2A0];
    int x2A0;
} State;

extern Table D_004348F0;
extern State D_004B25A0;
extern char D_00435500[];
extern int D_0029DE68;
extern int func_00197518(void *, int, int, int *);
extern void func_0018E638(Obj *, int, void *);
extern void func_00196DF0(void *);
extern void func_0018DC20(Obj *);

void func_001981A0(Obj *obj)
{
    int out;

    obj->x418 = 0;
    if (func_00197518(D_00435500, D_004B25A0.x2A0, 5, &out) > D_004348F0.limits[5]) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x62, &D_0029DE68, 1);
    }
    if (func_00197518(D_00435500, D_004B25A0.x2A0, 2, &out) > D_004348F0.limits[2]) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x63, &D_0029DE68, 1);
    }
    if (func_00197518(D_00435500, D_004B25A0.x2A0, 0, &out) > D_004348F0.limits[0]) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x64, &D_0029DE68, 1);
    }
    if (func_00197518(D_00435500, D_004B25A0.x2A0, 3, &out) > D_004348F0.limits[3]) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x65, &D_0029DE68, 1);
    }
    if (func_00197518(D_00435500, D_004B25A0.x2A0, 1, &out) > D_004348F0.limits[1]) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x66, &D_0029DE68, 1);
    }
    obj->x464 = 70;
    if (obj->x500 == 0) {
        func_0018E638(obj, 0x5F, &D_0029DE68);
    } else {
        func_0018E638(obj, 0x60, &D_0029DE68);
    }
    func_00196DF0(D_00435500);
    func_0018DC20(obj);
}
