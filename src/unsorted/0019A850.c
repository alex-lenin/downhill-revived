/* UNIT 0x0019A850 0x0019AA88 */
#include "menu_items.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    int x0;
    char pad4[0x414];
    int x418;
    char pad41C[0x40];
    int x45C;
    char pad460[0xA0];
    int x500;
    char pad504[0x3C];
    signed char x540[4];
} Obj;

typedef struct Game {
    char pad0[0x20];
    int x20;
} Game;

typedef struct Config {
    char pad0[0x288];
    int x288;
} Config;

extern char *D_0026F988[];
extern int D_0029DE68[];
extern char D_0029D168[], D_0029D170[], D_0028E558[];
extern char D_003F52D0[];
extern char D_0043D2A0[];
extern Config D_004B25A0;
extern char D_004B2A90[];

extern void func_0018DC20(void *);
extern void func_0018E408(void *, char *, int);
extern void func_00190358(void *);
extern void func_00190660(void *, void *);
extern int func_00190D60(void *, int, int);
extern void func_0019AB60(Obj *);
extern void func_0019AC38(Obj *, int);
extern void func_001C57A8(char *, u128);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F9A8(int, int, int, int, char *);
extern void func_0020FB80(int, int, int, int, int);

void func_0019A850(Obj *obj)
{
    int i;
    int flag;
    char *menu;
    char *q;
    int v1;

    obj->x418 = 0;
    flag = 0;
    for (i = 0; i < 4; i++) {
        func_0018E408(obj, D_0026F988[i], 1);
        if (obj->x540[i] > 0 && D_004B25A0.x288 <= 1) {
            flag = 1;
        }
    }
    if (flag) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x23, D_0029DE68, 1);
    }
    menu = D_003F52D0;
    func_00190358(menu);
    v1 = obj->x0;
    q = menu + 0x64;
    if (v1 < 4) {
        obj->x500 = v1;
    } else {
        obj->x500 = 4;
    }
    if (func_00190D60(q, 6, obj->x45C) && obj->x0 < 4) {
        func_00190660(D_003F52D0, D_0043D2A0);
        func_001C57A8(D_004B2A90, (u128)0x5443454C45535F53UL);
    }
    func_0019AB60(obj);
    func_0020FB80(0, 0x15D, 0xA7, -0x86, -0x7E);
    func_0020F518(0, 0x38, 0x38, 0x38, 0xFF);
    v1 = obj->x0;
    if (v1 < 4 && obj->x540[v1] > 0) {
        func_0020F9A8(0, ((Game *)D_003F52D0)->x20, 0x1E, 0xA0, D_0029D168);
    }
    func_0020F9A8(0, ((Game *)D_003F52D0)->x20, 0x1E, 0xAA, D_0029D170);
    if (obj->x0 < 4) {
        func_0020F9A8(0, ((Game *)D_003F52D0)->x20, 0x1E, 0xB4, D_0028E558);
        func_0019AC38(obj, obj->x0);
    }
    func_0018DC20(obj);
}
