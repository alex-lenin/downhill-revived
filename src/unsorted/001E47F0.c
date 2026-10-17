/* UNIT 0x001E47F0 0x001E4878 */
#include "menu.h"
struct Sub {
    void *vtbl;
};

struct Obj {
    char pad0[0x460];
    int x460;
    char pad464[0x478 - 0x464];
    int x478;
    char pad47C[0x4A0 - 0x47C];
    int x4A0;
    int x4A4;
    char pad4A8[0x4C8 - 0x4A8];
    struct Sub *x4C8;
    char pad4CC[0x4F8 - 0x4CC];
    void *vtbl;
    char pad4FC[0x504 - 0x4FC];
    struct Sub x504;
};

extern char D_00287308[];
extern char D_00288238[];
extern int D_0029DE68;

extern void func_0018E638(struct Obj *, int, void *);

struct Obj *func_001E47F0(struct Obj *obj)
{
    struct Sub *sub;

    func_0018D9D0((SharedMenu *)(obj));
    sub = &obj->x504;
    obj->vtbl = D_00288238;
    sub->vtbl = D_00287308;
    obj->x4A0 = 2;
    obj->x4A4 = 3;
    func_0018E638(obj, 0x97, &D_0029DE68);
    obj->x4C8 = sub;
    obj->x478 = 1;
    obj->x460 = 70;
    return obj;
}
