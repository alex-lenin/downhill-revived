/* UNIT 0x00140810 0x00140BF8 .sbss=0x29DD30:0x29DD34 */
#include "derived_menu.h"
#if defined(__mips__)
#define InitializeDerivedMenuView(p) func_0018F210(p)
#else
#define InitializeDerivedMenuView(p) func_0018F210((SharedDerivedMenu *)(p))
#endif
#include "ui_style.h"
#include "menu.h"
#include "animation.h"
/* Static initializer for this file's global objects (called as (1, 0xFFFF)). */
typedef struct Obj { char pad0[0x4F8]; void *vtbl; } Obj;
typedef struct Pair { Obj a; char pad4FC[0x720 - 0x4FC]; Obj b; } Pair;
typedef struct Slot { char pad0[0x10]; void *vtbl; char pad14[0xC]; int x20; char pad24[0x8]; } Slot;

extern char D_003623A0[];
extern char D_00362468[];
extern Obj D_00362490;
extern Obj D_00362990;
extern Obj D_00362E90;
extern Obj D_00363390;
extern Obj D_003638A0;
extern char D_00363DA0[];
extern char D_003642D0[];
extern char D_00364800[];
extern char D_00364D30[];
extern char D_00365260[];
extern char D_00365790[];
extern char D_00365800[];
extern Obj D_00365DE0;
extern Obj D_003662F0;
extern Pair D_00366800;
extern Pair D_003674F0;
extern Pair D_003681E0;
extern Obj D_00368ED0;
extern Obj D_00369410;
extern Obj D_00369950;
extern Obj D_00369E90;
extern Obj D_0036A3D0;
extern Obj D_0036ABD0;
extern char D_0036B0E0[];
extern char D_0036B100[];
extern char D_0036B120[];
extern char D_0036B140[];
extern char D_0036B160[];
extern Slot D_0036B180[10];
extern char D_0036B338[];

extern char D_00283B98[];
extern char D_00283C20[];
extern char D_00283CA8[];
extern char D_00283D30[];
extern char D_00283DB8[];
extern char D_00283E40[];
extern char D_00283EC8[];
extern char D_00283F50[];
extern char D_00283FD8[];
extern char D_00284060[];
extern char D_00284170[];
extern char D_00287190[];
extern char D_0026E090[];
char *D_0029DD30 __attribute__((section(".sbss"))) = 0;

extern void func_00208378(void *);

#if defined(__mips__)
extern Obj *func_0018F210(Obj *);
#endif
extern void func_0018E720(Obj *);
extern void *func_0013B830(void *);

extern void func_0013CA68(void *);
extern void func_00140450(void *);

void func_00140810(int initialize, int priority)
{
    Slot *slot;
    int n;

    if (priority == 0xFFFF && initialize) {
        func_00208378(D_003623A0);
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_00362468), 0x120, 0xA0, -0x70, -0x20, 0x5A, 0x5A, 0x5A, 0xFF);
        func_0018D9D0((SharedMenu *)(&D_00362490));
        D_00362490.vtbl = D_00283CA8;
        func_0018D9D0((SharedMenu *)(&D_00362990));
        D_00362990.vtbl = D_00283DB8;
        func_0018D9D0((SharedMenu *)(&D_00362E90));
        D_00362E90.vtbl = D_00283D30;
        func_0018D9D0((SharedMenu *)(&D_00363390));
        D_00363390.vtbl = D_00283EC8;
        func_0018D9D0((SharedMenu *)(&D_003638A0));
        D_003638A0.vtbl = D_00283C20;
        D_0029DD30 = D_0026E090;
        func_0013B830(D_00363DA0);
        func_0013B830(D_003642D0);
        func_0013B830(D_00364800);
        func_0013B830(D_00364D30);
        func_0013B830(D_00365260);
        func_001AC780((AnimationController *)(D_00365790), 2, 1);
        func_0013CA68(D_00365800);
        InitializeDerivedMenuView(&D_00365DE0);
        D_00365DE0.vtbl = D_00283B98;
        InitializeDerivedMenuView(&D_003662F0);
        {
            Pair *pair = &D_00366800;
            Obj *second = &pair->b;
            InitializeDerivedMenuView(&pair->a);
            pair->a.vtbl = D_00283FD8;
            func_0018D9D0((SharedMenu *)(second));
            second->vtbl = D_00284060;
        }
        {
            Pair *pair = &D_003674F0;
            Obj *second = &pair->b;
            InitializeDerivedMenuView(&pair->a);
            pair->a.vtbl = D_00283FD8;
            func_0018D9D0((SharedMenu *)(second));
            second->vtbl = D_00284060;
        }
        {
            Pair *pair = &D_003681E0;
            Obj *second = &pair->b;
            InitializeDerivedMenuView(&pair->a);
            pair->a.vtbl = D_00283FD8;
            func_0018D9D0((SharedMenu *)(second));
            second->vtbl = D_00284060;
        }
        func_0018D9D0((SharedMenu *)(&D_00368ED0));
        D_00368ED0.vtbl = D_00287190;
        func_0018E720(&D_00368ED0);
        func_0018D9D0((SharedMenu *)(&D_00369410));
        D_00369410.vtbl = D_00287190;
        func_0018E720(&D_00369410);
        func_0018D9D0((SharedMenu *)(&D_00369950));
        D_00369950.vtbl = D_00287190;
        func_0018E720(&D_00369950);
        func_0018D9D0((SharedMenu *)(&D_00369E90));
        D_00369E90.vtbl = D_00287190;
        func_0018E720(&D_00369E90);
        func_0018D9D0((SharedMenu *)(&D_0036A3D0));
        D_0036A3D0.vtbl = D_00283F50;
        InitializeDerivedMenuView(&D_0036ABD0);
        D_0036ABD0.vtbl = D_00283E40;
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_0036B0E0), 0x15D, 0xC2, -0x70, -0x20, 0x50, 0x50, 0x50, 0xFF);
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_0036B100), 0x15D, 0xC2, -0x70, -0x20, 0x99, 0x77, 0x2E, 0xFF);
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_0036B120), 0x13D, 0xB0, -0x70, -0x20, 0x50, 0x50, 0x50, 0xFF);
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_0036B140), 0x13D, 0xB0, -0x70, -0x20, 0x64, 0x28, 0x28, 0xFF);
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_0036B160), 0x180, 0xD8, -0x70, -0x21, 0x50, 0x50, 0x50, 0xFF);
        slot = D_0036B180;
        for (n = 9; n != -1; n--) {
            slot->vtbl = D_00284170;
            slot->x20 = 0;
            slot++;
        }
        func_00140450(D_0036B338);
    }
}
