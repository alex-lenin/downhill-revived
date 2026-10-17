/* UNIT 0x0015CE50 0x0015CF48 */
#include "menu_items.h"
typedef struct Menu {
    char pad0[0x418];
    int count;
    char pad41C[0x460 - 0x41C];
    int x460;
    char pad464[4];
    int x468;
} Menu;

typedef struct Options {
    char pad0[0x140];
    int x140;
} Options;

extern int D_0029DE68;
extern unsigned char D_0043DD41[];
extern Options D_0043F870;

extern void func_001A22D8(Options *);
extern void func_0018DC20(Menu *);

void func_0015CE50(Menu *menu) {
    menu->count = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x28, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x29, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x23, &D_0029DE68, 1);
    if (D_0043DD41[0] == 0) {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1D, &D_0029DE68, 1);
    }
    if (D_0043F870.x140 != 0) {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x30, &D_0029DE68, 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1A, &D_0029DE68, 1);
    menu->x460 = 0xA5;
    menu->x468 = 8;
    func_001A22D8(&D_0043F870);
    func_0018DC20(menu);
}
