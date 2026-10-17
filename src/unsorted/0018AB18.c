/* UNIT 0x0018AB18 0x0018AC50 */
#include "menu_items.h"
typedef struct VtblEntry { short delta; short pad; int (*fn)(void *); } VtblEntry;
typedef struct Vtbl { char pad0[0x68]; VtblEntry x68; } Vtbl;
typedef struct Obj {
    char pad0[0x460]; int x460;
    char pad464[0x44]; int x4A8;
    char pad4AC[0xC]; char x4B8;
    char pad4B9[0x3F]; Vtbl *x4F8;
    char pad4FC[0x118]; int x614; int x618;
} Obj;

extern void func_001881C0(Obj *);
extern int func_00187500(Obj *);
extern void func_00187438(Obj *);
extern void func_001873F8(Obj *);
extern void func_00187718(Obj *);
extern void func_0018E6B8(Obj *, int, void *);
extern void func_00187548(Obj *);
extern Obj *D_003F5420[];
extern unsigned char D_003EB769[];
extern int D_0029DE30;

void func_0018AB18(Obj *obj)
{
    obj->x460 = 90;
    func_001881C0(obj);
    if (func_00187500(obj) != 0 && D_003F5420[0] == obj) {
        obj->x4B8 = 1;
        if (obj->x4F8->x68.fn((char *)obj + obj->x4F8->x68.delta) == 0) {
            func_00187438(obj);
            return;
        }
        func_001873F8(obj);
        if (D_003EB769[0] != 0) {
            func_001873C0((struct SharedMenu *)(void *)(obj), 50, 0);
            func_001873C0((struct SharedMenu *)(void *)(obj), 51, 0);
        } else {
            func_001873C0((struct SharedMenu *)(void *)(obj), obj->x614, 0);
            func_001873C0((struct SharedMenu *)(void *)(obj), obj->x618, 0);
        }
        func_001873C0((struct SharedMenu *)(void *)(obj), 0, 0);
        func_00187718(obj);
        func_001873C0((struct SharedMenu *)(void *)(obj), 30, 1);
        func_001873C0((struct SharedMenu *)(void *)(obj), 31, 1);
        func_0018E6B8(obj, 27, &D_0029DE30);
        obj->x4A8 = 5;
        func_00187548(obj);
    }
}
