/* UNIT 0x0018FE08 0x0018FEC0 */
#include "ui_style.h"
typedef struct Item {
    int x0;
    int x4;
    unsigned char x8;
    char pad9[3];
    int xC;
} Item;

typedef struct List {
    char pad0[0x4];
    Item items[5];
    int count;
    int x58;
} List;

typedef struct Game {
    char pad0[0x20];
    int x20;
} Game;

extern Game D_003F52D0;
extern void func_0020F9F0(int, int, int, int, int);
extern void func_0020F9A8(int, int, int, int, int);

void func_0018FE08(List *list) {
    int i;
    Item *item;

    if (list->x58 != 0) {
        func_0018D800((const UiStyle *)(void *)(list->x58));
    }
    for (i = 0; i < list->count; i++) {
        item = &list->items[i];
        if (item->x8) {
            func_0020F9F0(0, D_003F52D0.x20, item->x0, item->x4, item->xC);
        } else {
            func_0020F9A8(0, D_003F52D0.x20, item->x0, item->x4, item->xC);
        }
    }
}
