/* UNIT 0x0017F160 0x0017F3F0 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0017F3F0_17_func_0017F3F0 func_0017F3F0
#else
#define CallScalarBridge0017F3F0_17_func_0017F3F0 ScalarBridge0017F3F0_17
#endif
typedef struct Entry {
    char pad0[0x8];
    float x8;
} Entry;

typedef struct Item {
    char pad0[0x1B8];
    unsigned char x1B8;
    char pad1B9[0x220 - 0x1B9];
} Item;

/* Layout shared with sibling func_0017F070 (init) / func_0017F8B8 (update). */
typedef struct Obj {
    char pad0[0x30];
    Item items[40];
    unsigned int **x5530[40];
    int x55D0[40];
    float x5670;
    float x5674;
    int x5678;
    float x567C[8];
    int x569C[40];
    int x573C; /* item count */
    int x5740;
    int x5744;
    int x5748;
    int x574C[8];
    int x576C;
} Obj;

typedef struct Config {
    char pad0[0x2A0];
    int x2A0;
} Config;

/* Scalar FLT_MAX in .sdata; declared as array so the address is
   materialized explicitly (lui $v0) as in retail. */
extern float D_0029CAAC[];
extern char D_003BD580[];
extern Config D_004B25A0;
extern int func_001817E0(void *table, unsigned int id);

#if defined(__mips__)
extern Entry *func_0017F3F0(Obj *obj, int id, float range);
#else

#endif

extern void func_0017FB18(Item *item, Obj *obj, unsigned int **entity, int arg3, int arg4, Entry *result);
extern void func_00203590(void *ref, int id, int arg2, int arg3);

void func_0017F160(Obj *obj)
{
    int i;
    int type;
    int slot;
    long one;
    float init;
    float *fbase;
    float *fslot;
    Entry *result;

    fbase = obj->x567C;
    init = D_0029CAAC[0];
    for (i = 0; i < 8; i++) {
        obj->x567C[i] = init;
        obj->x574C[i] = -1;
    }

    for (i = 0; i < obj->x573C; i++) {
        /* Loop-invariant active marker; hoisted above the loop into $fp.
           (long, so the hoist is scheduled after the table-base lui.) */
        one = 1;
        if (obj->x5530[i] != 0) {
            result = CallScalarBridge0017F3F0_17_func_0017F3F0(obj, func_001817E0(D_003BD580, **obj->x5530[i] >> 18), 250.0f);
        } else {
            result = CallScalarBridge0017F3F0_17_func_0017F3F0(obj, 0, 250.0f);
        }
        if (result != 0) {
            type = func_001817E0(D_003BD580, **obj->x5530[i] >> 18);
            switch (type) {
            case 4:
                slot = 1;
                break;
            case 8:
                slot = 2;
                break;
            case 16:
                slot = 3;
                break;
            case 32:
                slot = 4;
                break;
            case 64:
                slot = 5;
                break;
            case 128:
                slot = 6;
                break;
            case 256:
                slot = 7;
                break;
            default:
                slot = 0;
                break;
            }
            fslot = fbase + slot;
            *fslot = result->x8;
            func_0017FB18(&obj->items[i], obj, obj->x5530[i], obj->x55D0[i], obj->x569C[i], result);
            /* i[items] addresses t+obj to match retail's addu operand order. */
            i[obj->items].x1B8 = one;
        }
    }
    obj->x576C = 0;
    if (D_004B25A0.x2A0 == 12) {
        func_00203590(obj, 0x34BC, 0, 0);
        func_00203590((char *)obj + 0x10, 0x34BD, 0, 0);
        func_00203590((char *)obj + 0x20, 0x34BE, 0, 0);
    }
}
