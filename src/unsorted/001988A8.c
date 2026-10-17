/* UNIT 0x001988A8 0x00198958 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0018D898_21_func_0018D898 func_0018D898
#else
#define CallScalarBridge0018D898_21_func_0018D898 ScalarBridge0018D898_21
#endif
typedef struct Item {
    char pad0[0xC];
    short xC;
    short xE;
} Item;

typedef struct Obj {
    char pad0[0x10];
    Item x10[1];
} Obj;

typedef struct Slot {
    char pad0[0x10];
} Slot;

typedef struct ObjSlots {
    char pad0[0x18];
    Slot x18[1];
} ObjSlots;

extern char D_003F52D0[], D_00436430[];
extern long D_0029DE68;
extern int D_004B2900[];
extern void func_00190660(void *, void *);

#if defined(__mips__)
extern void func_0018D898(void *, long, long *);
#else

#endif


static inline int IsType(Item *item, int type) {
    return item->xC == type;
}

void func_001988A8(Obj *obj, int index) {
    if (IsType(&obj->x10[index], 0x6A)) {
        func_00190660(D_003F52D0, D_00436430);
    } else if (IsType(&obj->x10[index], 0x68)) {
        CallScalarBridge0018D898_21_func_0018D898(&((ObjSlots *)obj)->x18[index], 0x69, &D_0029DE68);
        D_004B2900[0] = 1;
    } else if (IsType(&obj->x10[index], 0x69)) {
        CallScalarBridge0018D898_21_func_0018D898(&((ObjSlots *)obj)->x18[index], 0x68, &D_0029DE68);
        D_004B2900[0] = 0;
    }
}
