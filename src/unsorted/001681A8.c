/* UNIT 0x001681A8 0x001682A0 */
#include "menu_items.h"
typedef struct Menu {
    char pad0[0x418];
    int count;
    char pad41C[0x460 - 0x41C];
    int x460;
} Menu;

typedef struct Profile {
    char pad0[0x553];
    unsigned char x553;
    char pad554;
    unsigned char x555;
} Profile;

extern int D_0029DE68;
extern Profile D_003985B0;
extern char D_003F52D0[];
extern int D_004B2840[];

extern void func_001B5AF0(void *, int, int);
extern void func_0018DB08(Menu *);

void func_001681A8(Menu *menu) {
    menu->count = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x47, &D_0029DE68, 1);
    func_001B5AF0(D_003F52D0, 1, 1);
    if (D_003985B0.x555) {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x9A, &D_0029DE68, 1);
    } else if (D_003985B0.x553) {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1B, &D_0029DE68, 1);
    } else {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x28, &D_0029DE68, 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x23, &D_0029DE68, 1);
    if (D_004B2840[0] != 0xB) {
        func_0018E3A8((struct SharedMenu *)(void *)(menu), 0x1A, &D_0029DE68, 1);
    }
    menu->x460 = 0xA5;
    func_0018DB08(menu);
}
