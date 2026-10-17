/* UNIT 0x001E3618 0x001E3650 */
#include "derived_menu.h"
#if defined(__mips__)
#define InitializeDerivedMenuView(p) func_0018F210(p)
#else
#define InitializeDerivedMenuView(p) func_0018F210((SharedDerivedMenu *)(p))
#endif
typedef struct Obj { char pad[0x4F8]; void *x4F8; } Obj;
#if defined(__mips__)
extern void func_0018F210(Obj *);
#endif
extern char D_002880A0[];
Obj *func_001E3618(Obj *obj) {
    InitializeDerivedMenuView(obj);
    obj->x4F8 = D_002880A0;
    return obj;
}

