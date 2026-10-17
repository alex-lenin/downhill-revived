/* UNIT 0x0018F310 0x0018F438 .sbss=0x29DE68:0x29DE70 */
#include "ui_style.h"
/* Static initializer of a C++ translation unit (called as func_0018F310(1, 0xFFFF)
 * from func_0018F438): constructs the global objects below. */
typedef UiStyle Widget;

/* 20 objects whose (inlined) constructor only sets the vtable pointer at 0x10 */
typedef struct Item {
    char pad0[0x10];
    void *vtbl;
} Item;

extern Widget D_003F4B20;
extern Widget D_003F4B40;
extern Widget D_003F4B60;
extern Widget D_003F4B80[1];
extern Item D_003F4BA0[20];
extern char D_0026F540[];
extern char D_002872A0[];

void *D_0029DE68 __attribute__((section(".sbss"))) = 0;

void func_0018F310(int initialize, int priority)
{
    Item *item;
    int i;
    Widget *w;
    int alpha;

    if (priority == 0xFFFF && initialize) {
        func_0018D7D0((UiStyle *)(void *)(&D_003F4B20), 0x120, 0xA0, -0x64, -0x20, 0x4A, 0x4E, 0x9A, 0x80);
        func_0018D7D0((UiStyle *)(void *)(&D_003F4B40), 0x120, 0xA0, -0x64, -0x20, 0x8C, 0x8C, 0xB4, 0x80);
        func_0018D7D0((UiStyle *)(void *)(&D_003F4B60), 0x1D0, 0xC0, -0xC5, 0x18, 0x6E, 0x6E, 0xC8, 0x80);
        w = D_003F4B80;
        alpha = 255;
        func_0018D7D0((UiStyle *)(void *)(w), 0x15D, 0xA7, -0x86, -0x1A, 0x38, 0x38, 0x38, alpha);
        D_0029DE68 = D_0026F540;
        item = D_003F4BA0;
        i = 19;
        do {
            item->vtbl = D_002872A0;
            i--;
            item++;
        } while (i != -1);
    }
}
