/* UNIT 0x0018D9D0 0x0018DB00 */
#include "menu.h"
typedef MenuItem Item;
typedef MenuLink Link;
typedef SharedMenu Menu;

extern char D_00287218[];
extern char D_00287320[];
extern char D_003F4B20[];
extern char D_003F4B40[];
extern char D_003F4B60[];
extern char D_003F4B80[];

static inline void Item_Init(Item *item)
{
    item->x4 = 0;
    item->x7 = 0;
    item->x8 = 0;
    item->xC = 0;
}

static inline void Link_Init(Link *link)
{
    link->vptr = D_00287320;
    link->x4 = 0;
}

Menu *func_0018D9D0(Menu *menu)
{
    Item *item;
    int i;

    menu->vptr = D_00287218;
    item = menu->items;
    for (i = 63; i != -1; i--) {
        Item_Init(item);
        item++;
    }
    menu->x418 = 0;
    menu->x41C = 50;
    menu->x420 = 0;
    menu->x434 = 0;
    Item_Init(&menu->x438);
    Item_Init(&menu->x448);
    menu->x45C = 1;
    menu->x460 = 70;
    menu->x464 = -1;
    menu->x468 = 10;
    menu->x46C = 1;
    menu->x470 = 0;
    menu->x474 = 0;
    menu->x478 = 0;
    menu->x47C = 0;
    menu->x480 = -1;
    menu->x4A0 = 0;
    menu->x4A4 = 1;
    menu->x4A8 = -1;
    menu->x4AC = 0;
    menu->x4B0 = 0;
    menu->x4B8 = 0;
    menu->x4BA = 0;
    menu->x4BB = 0;
    menu->x4BC = 0;
    Link_Init(&menu->x4C0);
    menu->x4C8 = &menu->x4C0;
    menu->x4 = D_003F4B60;
    menu->x8 = D_003F4B80;
    menu->x10 = D_003F4B40;
    menu->x4F4 = 0;
    menu->xC = D_003F4B20;
    menu->x14 = D_003F4B20;
    return menu;
}
