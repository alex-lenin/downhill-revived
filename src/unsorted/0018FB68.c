/* UNIT 0x0018FB68 0x0018FC98 */
#include "ui_style.h"
typedef struct Entry {
    char data[0x10];
} Entry;

typedef struct Item {
    int index;
    char pad4[0x10 - 0x4];
    int x10;
    char pad14[0x18 - 0x14];
    Entry entries[1];
    char pad28[0x418 - 0x28];
    int visible;
    char pad41C[0x460 - 0x41C];
    int y;
    int x;
    char pad468[0x478 - 0x468];
    int x478;
    char pad47C[0x4BB - 0x47C];
    unsigned char x4BB;
    unsigned char x4BC;
} Item;

extern char D_0029D058[];
extern char D_0029D068[];
extern int D_003F52F0[];
extern int func_0018D860(Entry *entry);
extern int func_00254D68(char *dst, const char *fmt, ...);
extern void func_0020F9A8(int a, int font, int x, int y, char *text);
extern void func_0020FA98(int a, int font, int x, int y, char *text);
extern void func_0020F9F0(int a, int font, int x, int y, char *text);

void func_0018FB68(void *unused, Item *item)
{
    char text[0x70];
    int x;
    int y;

    if (item->visible == 0) {
        return;
    }
    x = item->x;
    y = item->y;
    func_0018D800((const UiStyle *)(void *)(item->x10));
    if (item->x478 != 0) {
        func_00254D68(text, D_0029D058, func_0018D860(&item->entries[item->index]));
    } else {
        func_00254D68(text, D_0029D068, func_0018D860(&item->entries[item->index]));
    }
    if (item->x4BB) {
        func_0020F9A8(0, D_003F52F0[0], x, y, text);
    } else if (item->x4BC) {
        func_0020FA98(0, D_003F52F0[0], x, y, text);
    } else {
        func_0020F9F0(0, D_003F52F0[0], x, y, text);
    }
}
