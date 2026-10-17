/* UNIT 0x0015FB58 0x0015FC70 */
#include "menu_items.h"
#include "menu.h"
typedef struct Menu {
    char pad0[0x418];
    int x418;
    char pad41C[0x460 - 0x41C];
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
    char pad4AC[0x4F8 - 0x4AC];
    void *vtbl;
} Menu;

extern int D_0029DE68;
extern char D_00284D98[];

extern void func_0018E638(Menu *menu, int id, void *ctx);
extern void func_0018E6B8(Menu *menu, int id, void *ctx);

Menu *func_0015FB58(Menu *menu)
{
    func_0018D9D0((SharedMenu *)(menu));
    menu->vtbl = D_00284D98;
    func_0018E638(menu, 0x40, &D_0029DE68);
    menu->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x41, &D_0029DE68, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x42, &D_0029DE68, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0, &D_0029DE68, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x43, &D_0029DE68, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x44, &D_0029DE68, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0, &D_0029DE68, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0xAB, &D_0029DE68, 0);
    func_0018E6B8(menu, 0x45, &D_0029DE68);
    menu->x460 = 0x54;
    menu->x4A8 = 5;
    return menu;
}
