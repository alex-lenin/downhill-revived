/* UNIT 0x0013DF28 0x0013E2E8 */
#include "ui_style.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Entry {
    int x0;
    int x4;
    int x8;
} Entry;

typedef struct IdTable {
    int id[3];
} IdTable;

typedef struct Item {
    char pad0[6];
    unsigned char x6;
    char pad7[9];
} Item;

typedef struct Obj {
    int x0;
    char pad4[0x10 - 0x4];
    void *x10;
    char pad14[0x18 - 0x14];
    Item items[(0x418 - 0x18) / 0x10];
    int x418;
    char pad41C[0x45C - 0x41C];
    int x45C;
    int x460;
    int x464;
    char pad468[0x510 - 0x468];
    int x510;
    int x514[64];
    int x614[64];
    char pad714[0xCE0 - 0x714];
    int xCE0;
} Obj;

typedef struct Res {
    char pad0[0x50];
    int x50;
    int x54;
    int x58;
    char pad5C[0x6C - 0x5C];
    int x6C;
    int x70;
} Res;

typedef struct Screen {
    char pad0[0x20];
    int x20;
    char pad24[0x64 - 0x24];
    char x64[1];
} Screen;

typedef struct Globals {
    char pad0[0x12208];
    int x12208;
} Globals;

extern Screen D_003F52D0;
extern Res *D_0029DD30;
extern Entry D_0036B6B0;
extern const IdTable D_00289A98;
extern Globals D_004225A0;
extern char D_0036B120[], D_0036B140[];
extern char D_004B2A90[];
extern char D_0029C4D8[];
extern void func_0013DCF8(Obj *);
extern int func_0013AF60(int);
extern void func_00218498(int, int);
extern void func_0020FA98(int, int, int, int, int);
extern int func_00190D60(void *, int, int);
extern void func_001C57A8(char *, u128);
extern int func_00194008(Globals *, int);
extern int func_00194050(Globals *, int);
extern int func_00194098(Globals *, int);
extern void func_0013B5E8(Entry *, int, int, int);
extern void func_00254D68(char *, char *, ...);
extern void func_0018DC20(Obj *);

/* an inline helper taking the item pointer keeps &items[i] as its own loop giv
   (base+0x18, field offset in the load) */
static inline int IsActive(Item *item)
{
    return item->x6;
}

void func_0013DF28(Obj *obj)
{
    char text[32];
    IdTable ids;
    Entry entry;
    int found;
    int i;
    int count;
    int half;
    int blink;

    found = 0;
    func_0013DCF8(obj);
    i = 0;
    if (i < obj->x418) {
        do {
            if (IsActive(&obj->items[i])) {
                found = 1;
            }
            i++;
        } while (i < obj->x418 && !found);
    }
    ids = D_00289A98;
    func_00218498(func_0013AF60(ids.id[obj->x510 - 1]), found);
    if (!found) {
        func_0018D800((const UiStyle *)(void *)(obj->x10));
        func_0020FA98(0, D_003F52D0.x20, obj->x464, obj->x460, D_0029DD30->x70);
        if (func_00190D60(D_003F52D0.x64, 4, obj->x45C)) {
            /* packed name "S_CANCEL" */
            func_001C57A8(D_004B2A90, (u128)0x4C45434E41435F53L);
        }
        return;
    }
    count = obj->x514[obj->x0];
    entry = D_0036B6B0;
    half = 0;
    if (obj->x510 == 1) {
        entry.x0 = obj->x614[obj->x0];
        half = func_00194008(&D_004225A0, entry.x0) != 0;
    } else if (obj->x510 == 2) {
        entry.x4 = obj->x614[obj->x0];
        half = func_00194050(&D_004225A0, entry.x4) != 0;
    } else if (obj->x510 == 3) {
        entry.x8 = obj->x614[obj->x0];
        half = func_00194098(&D_004225A0, entry.x8) != 0;
    }
    func_0013B5E8(&entry, 0, 0, 0);
    if (half) {
        count /= 2;
    }
    func_0018D800((const UiStyle *)(void *)(D_0036B120));
    obj->x10 = D_0036B120;
    if (D_004225A0.x12208 / 20 < count) {
        func_0018D800((const UiStyle *)(void *)(D_0036B140));
        obj->x10 = D_0036B140;
        blink = 1;
        if (obj->xCE0 > 0) {
            obj->xCE0--;
            if (obj->xCE0 % 10 >= 6) {
                blink = 0;
            }
        }
        if (blink) {
            func_0020FA98(0, D_003F52D0.x20, obj->x464, obj->x460 - 10, D_0029DD30->x6C);
        }
    } else if (half) {
        func_0020FA98(0, D_003F52D0.x20, obj->x464, obj->x460 - 10, D_0029DD30->x58);
    }
    if (half) {
        func_00254D68(text, D_0029C4D8, D_0029DD30->x54, obj->x514[obj->x0] / 2);
    } else {
        func_00254D68(text, D_0029C4D8, D_0029DD30->x50, obj->x514[obj->x0]);
    }
    func_0020FA98(0, D_003F52D0.x20, obj->x464, obj->x460 + 10, (int)text);
    func_0018DC20(obj);
}
