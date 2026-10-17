/* UNIT 0x0017F3F0 0x0017F560 */
#include "animation_track_search.h"
#if defined(__mips__)
#define CallShared00163C58_0 func_00163C58
#else
#define CallShared00163C58_0 Forward00163C58_0
#endif
#include "scalar_api.h"
typedef struct Scalar0017F3F0Entry {
    char pad0[0x8];
    float x8;
} Entry;

typedef struct Scalar0017F3F0Obj {
    char pad0[0x567C];
    float x567C[8];
    char pad569C[0xB0];
    int x574C[8];
} Obj;

typedef struct Globals {
    char pad0[0x50198];
    float x50198;
} Globals;

extern Globals D_002AF2C0;
extern char D_003BD580[];
extern int func_00163C58(void *table, float position, int index, int id);
extern Entry *func_00163D40(void *table, int index);

Entry *func_0017F3F0(Obj *obj, int id)
{
    Entry *result = 0;
    int slot;
    int index;
    float *values;
    float *position;
    int *cur;
    float limit;
    float minimum;

    switch (id) {
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

    limit = D_002AF2C0.x50198 - 300.0f;
    values = obj->x567C;
    position = &values[slot];
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(minimum) : "f"(*position), "f"(limit));
    *position = minimum;
    cur = obj->x574C;
    cur += slot;
    index = CallShared00163C58_0(D_003BD580, minimum, *cur, id);
    *cur = index;
    if (index >= 0) {
        result = func_00163D40(D_003BD580, index);
        *position = result->x8;
    }
    return result;
}
