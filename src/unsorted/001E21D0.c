/* UNIT 0x001E21D0 0x001E2710 */
#include "menu_items.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Global {
    char pad0[0x288];
    int x288;
    char pad28C[4];
    int x290;
} Global;

typedef struct Menu {
    char pad0[0x20];
    int x20;
    char pad24[0x64 - 0x24];
    char x64[1];
} Menu;

typedef struct Obj {
    int x0;
    char pad4[0x418 - 0x4];
    int x418;
    char pad41C[0x45C - 0x41C];
    int x45C;
    char pad460[0x500 - 0x460];
    int x500;
    char pad504[0x51C - 0x504];
    int x51C;
    char pad520[0x554 - 0x520];
    unsigned char x554[4];
} Obj;

extern Global D_004B25A0;
extern Menu D_003F52D0;
extern char D_004225A0[];
extern char D_004B2A90[];
extern char D_004C4210[];
extern char *D_00271800[];
extern int D_0027182C[];
extern char D_0029D708[];
extern char D_0029D710[];
extern char D_00293A60[];
extern void *D_0029DE68[];

extern void func_0018E408(Obj *obj, char *name, int a);
extern void func_00190358(Menu *menu);
extern void func_0020FB80(int a, int b, int c, int d, int e);
extern void func_0020F518(int a, int r, int g, int b, int alpha);
extern void func_0020F9A8(int a, int font, int x, int y, char *text);
extern void func_0020F9F0(int a, int font, int x, int y, int b);
extern int func_00194C80(void *p, int id);
extern int func_00194A10(void *p, int id);
extern void func_0023D650(int font, int *w, int *h);
extern int func_00190D60(void *p, int button, int mask);
extern void func_001E2958(Obj *obj, int a);
extern void func_00190660(Menu *menu, void *p);
extern void func_001C57A8(void *p, u128 name);
extern void func_001E2968(Obj *obj);
extern void func_001E2A40(Obj *obj);
extern void func_0018DC20(Obj *obj);

static inline void DrawCost(void)
{
    int w, h;
    int x;

    func_0023D650(D_003F52D0.x20, &w, &h);
    x = w / 2;
    func_0020F518(0, 0xA0, 0x64, 0x32, 0x80);
    func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
    func_0020F9F0(0, D_003F52D0.x20, x, 0xC2, D_0027182C[0]);
    func_0020FB80(0, 0x15D, 0xA7, -0x86, -0x7E);
    func_0020F518(0, 0x38, 0x38, 0x38, 0xFF);
}

void func_001E21D0(Obj *obj)
{
    char *pad;
    int id;
    int i;
    int any;

    any = 0;
    obj->x418 = 0;
    id = obj->x0 + 6;
    for (i = 0; i < 4; i++) {
        func_0018E408(obj, D_00271800[i], 1);
        if (obj->x554[i]) {
            any = 1;
        }
    }
    if (any && obj->x51C == 0) {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x23, D_0029DE68, 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x1A, D_0029DE68, 1);
    func_00190358(&D_003F52D0);
    pad = D_003F52D0.x64;
    func_0020FB80(0, 0x15D, 0xA7, -0x86, -0x7E);
    func_0020F518(0, 0x38, 0x38, 0x38, 0xFF);
    if (D_004B25A0.x288 == 1) {
        if (obj->x0 < 4 && obj->x554[obj->x0]) {
            if (func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, id) < obj->x554[obj->x0]) {
                if (obj->x51C) {
                    if (func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, id) > 0) {
                        func_0020F9A8(0, D_003F52D0.x20, 0x3C, 0xA0, D_0029D708);
                    }
                } else {
                    func_0020F9A8(0, D_003F52D0.x20, 0x3C, 0xA0, D_0029D708);
                    if (func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, id) > 0) {
                        DrawCost();
                    }
                }
            }
        }
    } else {
        if (obj->x0 < 4 && obj->x554[obj->x0]) {
            if (func_00194A10(D_004225A0, id) < obj->x554[obj->x0]) {
                if (obj->x51C) {
                    if (func_00194A10(D_004225A0, id) > 0) {
                        func_0020F9A8(0, D_003F52D0.x20, 0x3C, 0xA0, D_0029D708);
                    }
                } else {
                    func_0020F9A8(0, D_003F52D0.x20, 0x3C, 0xA0, D_0029D708);
                    if (func_00194A10(D_004225A0, id) > 0) {
                        DrawCost();
                    }
                }
            }
        }
    }
    func_0020F9A8(0, D_003F52D0.x20, 0x3C, 0xAC, D_0029D710);
    if (obj->x51C == 0) {
        func_0020F9A8(0, D_003F52D0.x20, 0x3C, 0xB8, D_00293A60);
    }
    if (obj->x0 < 4) {
        obj->x500 = obj->x0;
    } else {
        obj->x500 = 4;
    }
    if (obj->x51C == 0 && func_00190D60(pad, 6, obj->x45C) && obj->x0 < 4) {
        int n = func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, obj->x0 + 6);
        if (obj->x554[obj->x0] && n > 0) {
            func_001E2958(obj, 3);
        }
        func_00190660(&D_003F52D0, D_004C4210);
        func_001C57A8(D_004B2A90, (u128)0x5443454C45535F53LL);
    }
    func_001E2968(obj);
    func_001E2A40(obj);
    func_0018DC20(obj);
}
