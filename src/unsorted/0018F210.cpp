/* UNIT 0x0018F210 0x0018F278 */
#if defined(__mips__)
/* Constructor of a class derived from the one built by func_0018D9D0 (also a constructor:
 * both store a vtable pointer at 0x4F8). The classes are named after their constructors so
 * the mangled names (__13func_0018F210, __13func_0018D9D0) link to the splat names.
 * D_00286FB8 / D_00287308 are the vtables of this class and of its member at 0x504; they are
 * stored explicitly because gcc's own _vt$ symbols cannot be linked to the retail tables. */

extern char D_00286FB8[];
extern char D_00287308[];

struct Member {
    void *vtbl;
    char x4;
    char x5;
};

struct func_0018D9D0 {
    char pad0[0x4A0];
    int x4A0;
    int x4A4;
    char pad4A8[0x20];
    Member *x4C8;
    char pad4CC[0x2C];
    void *vtbl;             /* 0x4F8 */
    func_0018D9D0(void);
};

struct func_0018F210 : func_0018D9D0 {
    int x4FC;
    int x500;
    Member x504;
    func_0018F210();
};

func_0018F210::func_0018F210() {
    Member *member = &x504;

    vtbl = D_00286FB8;
    member->vtbl = D_00287308;
    x4A0 = 2;
    x4A4 = 3;
    x4C8 = member;
    x500 = 0;
    x504.x4 = 1;
    x504.x5 = 1;
}
#else
#include "derived_menu.h"
extern char D_00286FB8[], D_00287308[];
SharedDerivedMenu *func_0018F210(SharedDerivedMenu *menu) {
    func_0018D9D0(&menu->base);
    menu->base.vptr = D_00286FB8;
    menu->member.vptr = D_00287308;
    menu->base.x4A0 = 2;
    menu->base.x4A4 = 3;
    menu->base.x4C8 = (MenuLink *)&menu->member;
    menu->x500 = 0;
    menu->member.x4 = 1;
    menu->member.x5 = 1;
    return menu;
}
#endif
