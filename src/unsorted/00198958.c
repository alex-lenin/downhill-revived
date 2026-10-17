/* UNIT 0x00198958 0x001989C0 */
#include "menu.h"
struct Obj {
    char pad0[0x460];
    int x460;
    char pad464[0x478 - 0x464];
    int x478;
    char pad47C[0x4A0 - 0x47C];
    int x4A0;
    int x4A4;
    char pad4A8[0x4C8 - 0x4A8];
    void **x4C8;
    char pad4CC[0x4F8 - 0x4CC];
    void *x4F8;
    char pad4FC[0x504 - 0x4FC];
    void *x504;
};

extern char D_00287308[];
extern char D_00287518[];


struct Obj *func_00198958(struct Obj *obj)
{
    void **sub;

    InitializeMenuDiscard((SharedMenu *)(obj));
    sub = &obj->x504;
    *sub = D_00287308;
    obj->x4F8 = D_00287518;
    obj->x460 = 100;
    obj->x4A0 = 2;
    obj->x4A4 = 3;
    obj->x4C8 = sub;
    obj->x478 = 1;
    return obj;
}
