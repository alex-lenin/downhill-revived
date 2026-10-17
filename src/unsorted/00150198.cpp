/* UNIT 0x00150198 0x00150308 */
#include "menu_items.h"
/* func_00150198 */
typedef struct ModeInfo {
    char pad0[8];
    int x8;
    int xC;
    char pad10[8];
    int HasX8() { return x8 != 0; }
    int HasXC() { return xC != 0; }
} ModeInfo;

typedef struct Menu {
    char pad0[0x418];
    int count;
} Menu;

typedef struct Options {
    char pad0[0x140];
    int x140;
} Options;

extern ModeInfo D_00381CF0[];
extern int D_004B26AC[];
extern Options D_0043F870;
extern int D_0029DE68;

extern void func_001A22D8(Options *p);
extern void func_0018DC20(Menu *menu);

void func_00150198(Menu *menu)
{
    func_001A22D8(&D_0043F870);
    menu->count = 0;
    if (D_00381CF0[D_004B26AC[0]].HasX8()) {
        if (!D_00381CF0[D_004B26AC[0]].HasXC()) {
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1F, &D_0029DE68, 1);
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x30, &D_0029DE68, 1);
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x23, &D_0029DE68, 1);
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1A, &D_0029DE68, 1);
        } else {
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x30, &D_0029DE68, 1);
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x23, &D_0029DE68, 1);
            func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1B, &D_0029DE68, 1);
        }
    } else {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x20, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x30, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x23, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1A, &D_0029DE68, 1);
    }
    func_0018DC20(menu);
}
