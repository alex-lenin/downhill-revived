/* UNIT 0x00187378 0x001873C0 */
#include "menu.h"
typedef struct Obj { char pad0[0x46C]; int x46C; char pad470[0x88]; void *x4F8; } Obj;
extern char D_00286B38[];

extern int func_001873F8(Obj *);
Obj *func_00187378(Obj *obj) {
    func_0018D9D0((SharedMenu *)(obj));
    obj->x4F8 = D_00286B38;
    func_001873F8(obj);
    obj->x46C = 1;
    return obj;
}
