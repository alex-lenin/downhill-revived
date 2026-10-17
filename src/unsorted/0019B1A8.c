/* UNIT 0x0019B1A8 0x0019B350 .sbss=0x29DE70:0x29DE78 */
#include "ui_style.h"
typedef struct Menu {
    int x0;
    char pad4[4];
    void *x8;
    char padC[0x40C];
    int x418;
    char pad41C[0x44];
    int x460;
    char pad464[0x44];
    int x4A8;
    char pad4AC[0x5C];
    int x508;
} Menu;

typedef struct Setup {
    char pad0[0x500];
    int x500;
    char pad504[0x3C];
    signed char x540[1];
} Setup;

extern char D_0043C830[];
extern Setup D_0043CD50;
extern signed char D_0043D25C[][13];
extern char D_0029DE68[];
extern char D_0028E588[];
extern char D_0028E598[];
extern char *D_0026F7F8[];

int D_0029DE70 __attribute__((section(".sbss"))) = 0;

extern void func_0018E638(Menu *, int, void *);
extern void func_0018E408(Menu *, char *, int);
extern void func_0018E6B8(Menu *, int, void *);
extern void func_0018DB08(Menu *);

void func_0019B1A8(Menu *menu)
{
    int count;
    int i;
    int item;
    Setup *setup;

    menu->x460 = 40;
    menu->x508 = -1;
    if (D_0029DE70 == 0) {
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_0043C830), 0x10E, 0xA0, -0x5A, -0x21, 0x38, 0x38, 0x38, 0xFF);
        D_0029DE70 = 1;
    }
    menu->x8 = D_0043C830;
    setup = &D_0043CD50;
    menu->x0 = setup->x540[setup->x500];
    count = menu->x0 + 1;
    func_0018E638(menu, 0x7F, D_0029DE68);
    menu->x418 = 0;
    for (i = 0; i < count; i++) {
        if (count == 1) {
            func_0018E408(menu, D_0028E588, 1);
        } else {
            item = D_0043D25C[setup->x500][i];
            if (item == 40) {
                func_0018E408(menu, D_0028E598, 1);
            } else {
                func_0018E408(menu, D_0026F7F8[item], 1);
            }
        }
    }
    menu->x4A8 = 5;
    func_0018E6B8(menu, 0x7A, D_0029DE68);
    func_0018DB08(menu);
}
