/* UNIT 0x00133940 0x00133980 */
#include "object_registry.h"
typedef struct Owner { char pad0[0x792C]; int *x792C; } Owner;
typedef struct Obj { char pad0[4]; Owner *x4; int x8; char padC[0x1CC]; int x1D8; char pad1DC[4]; char x1E0; } Obj;

void func_00133940(Obj *obj) {
    obj->x1D8 = RegisterObjectRetail((void *)(obj), (unsigned int)(*obj->x4->x792C));
    obj->x8 = 0;
    obj->x1E0 = 0;
}

