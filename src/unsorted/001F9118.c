/* UNIT 0x001F9118 0x001F94D0 .sbss=0x29E138:0x29E148 */
#include "derived_menu.h"
#if defined(__mips__)
#define InitializeDerivedMenuView(p) func_0018F210(p)
#else
#define InitializeDerivedMenuView(p) func_0018F210((SharedDerivedMenu *)(p))
#endif
#include "menu_items.h"
#include "ui_style.h"
#include "menu.h"
typedef struct Rect {
    short x0;
    short x2;
    char pad4[3];
    char x7;
} Rect;

typedef struct Menu {
    char pad0[0xC];
    void *xC;
    void *x10;
    void *x14;
    char pad18[0x438 - 0x18];
    Rect x438;
    char pad440[0x460 - 0x440];
    int x460;
    int x464;
    char pad468[0x478 - 0x468];
    int x478;
    char pad47C[0x4BB - 0x47C];
    char x4BB;
    char pad4BC[0x4F8 - 0x4BC];
    void *x4F8;
    char pad4FC[0x510 - 0x4FC];
} Menu;

typedef struct Screen {
    char pad0[0x20];
    int x20;
} Screen;

int D_0029E138 __attribute__((section(".sbss"))) = 0;
int D_0029E13C __attribute__((section(".sbss"))) = 0;
int D_0029E140 __attribute__((section(".sbss"))) = 0;
int D_0029E144 __attribute__((section(".sbss"))) = 0;

extern Menu D_004C6F90;
extern Screen D_003F52D0;
extern char D_00287190[], D_00288540[];
extern char D_004C74D0[], D_004C74F0[], D_004C7510[];
extern void *D_0029E148;
extern unsigned char D_004225A0[];

extern void func_0018E720(Menu *);
extern void func_0023D650(int, int *, int *);
extern void func_0018E638(Menu *, int, void **);
extern void func_001B5AF0(Screen *, int, int);
#if defined(__mips__)
extern void func_0018F210(Menu *);
#endif
extern void func_0018E398(Menu *);
extern int func_00158668(int);
extern int func_00158638(int);
extern int func_00193FC8(void *, int);
extern void func_0018E408(Menu *, int, int);
extern void func_0018EAB0(Menu *, int, Menu *);
extern void func_00190108(Screen *, Menu *);

static inline void SetRect(Rect *r, int x, int y, int visible)
{
    r->x0 = x;
    r->x2 = y;
    r->x7 = visible;
}

void func_001F9118(void)
{
    Menu list;
    int width;
    int height;
    Menu *menu;
    Menu *p;
    int i;

    if (!D_0029E138) {
        func_0018D9D0((SharedMenu *)(&D_004C6F90));
        D_004C6F90.x4F8 = D_00287190;
        func_0018E720(&D_004C6F90);
        D_0029E138 = 1;
        D_004C6F90.x4F8 = D_00288540;
    }
    func_0023D650(D_003F52D0.x20, &width, &height);
    menu = &D_004C6F90;
    func_0018E720(menu);
    if (!D_0029E13C) {
        func_0018D7D0((UiStyle *)(void *)(D_004C74D0), 0x181, 0xB6, -0x85, -0x22, 0x4C, 0x4C, 0x4C, 0xFF);
        D_0029E13C = 1;
    }
    if (!D_0029E140) {
        func_0018D7D0((UiStyle *)(void *)(D_004C74F0), 0x181, 0xB6, -0x85, -0x22, 0x78, 0x5F, 0x28, 0xFF);
        D_0029E140 = 1;
    }
    if (!D_0029E144) {
        func_0018D7D0((UiStyle *)(void *)(D_004C7510), 0x181, 0xB6, -0x85, -0x22, 0x26, 0x26, 0x26, 0xFF);
        D_0029E144 = 1;
    }
    menu->x14 = D_004C7510;
    menu->xC = D_004C74D0;
    menu->x10 = D_004C74F0;
    func_0018E638(menu, 1, &D_0029E148);
    SetRect(&menu->x438, width / 2, 0x58, 1);
    menu->x464 = 0xDC;
    menu->x460 = 0x6E;
    menu->x478 = 0;
    menu->x4BB = 1;
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 2, &D_0029E148, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 3, &D_0029E148, D_004225A0[0x12192] != 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 4, &D_0029E148, D_004225A0[0x12190] != 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 5, &D_0029E148, D_004225A0[0x12191] != 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 6, &D_0029E148, D_004225A0[0x12193] != 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 7, &D_0029E148, D_004225A0[0x12194] != 0);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 8, &D_0029E148, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(menu), 9, &D_0029E148, 1);
    func_001B5AF0(&D_003F52D0, 0, 1);
    InitializeDerivedMenuView(&list);
    func_0018E398(&list);
    p = &list;
    p->x464 = 0x1C2;
    p->x460 = 0xB4;
    p->x4BB = 1;
    list.xC = D_004C74D0;
    list.x10 = D_004C74F0;
    for (i = 0; i < 13; i++) {
        func_0018E408(&list, func_00158668(i), func_00193FC8(D_004225A0, i));
    }
    func_0018E408(&list, func_00158638(15), func_00193FC8(D_004225A0, 15));
    func_0018EAB0(&D_004C6F90, 7, &list);
    func_001B5AF0(&D_003F52D0, 1, 0);
    func_00190108(&D_003F52D0, &D_004C6F90);
}
