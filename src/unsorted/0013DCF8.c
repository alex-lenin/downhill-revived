/* UNIT 0x0013DCF8 0x0013DF28 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001F97A8_5_func_001F97A8 func_001F97A8
#else
#define CallScalarBridge001F97A8_5_func_001F97A8 ScalarBridge001F97A8_5
#endif
typedef struct Item {
    char pad0[6];
    unsigned char x6;
    char pad7[9];
} Item;
typedef struct Obj {
    int x0;
    char pad4[0x10 - 0x4];
    void *x10;
    char pad14[0x18 - 0x14];
    Item items[(0x418 - 0x18) / 0x10];
    int x418;
    char pad41C[0x45C - 0x41C];
    int x45C;
    int x460;
    int x464;
    char pad468[0x510 - 0x468];
    int x510;
    int x514[64];
    int x614[64];
    char pad714[0xCE0 - 0x714];
    int xCE0;
} Obj;
typedef struct Tbl { int x0, x4, x8; } Tbl;
typedef struct Font { char pad0[0xA0]; } Font;
typedef struct Glob { char pad0[0x12208]; int x12208; } Glob;
extern int func_0013A258(void);
extern int func_00194008(Glob *, int), func_00194050(Glob *, int), func_00194098(Glob *, int);

#if defined(__mips__)
extern int func_001F97A8(Font *, int, int), func_001F9DB0(int, Tbl *);
#else
extern int func_001F9DB0(int, Tbl *);
#endif

extern Tbl *D_00365D74[];
extern unsigned int D_004B26AC[];
extern Font D_004C8508[];
extern Glob D_004225A0;
void func_0013DCF8(Obj *obj)
{
    int present;
    int i;
    Tbl *item = 0;
    Font *context;
    Item *entry;
    int selected, group, special, category, hidden;
    context = &D_004C8508[D_004B26AC[0]];
    if (D_004B26AC[0] < 4)
        item = &D_00365D74[0][func_0013A258()];
    for (i = 0; i < obj->x418; ++i) {
        entry = &obj->items[i];
        /* previous flag; recomputed from scratch below */
        hidden = entry->x6;
        selected = 0;
        if (!item || func_001F9DB0(obj->x510, item) == i)
            selected = 1;
        group = 0;
        if (!item || (unsigned int)(item->x0 - 0x23) < 3)
            group = 1;
        special = 0;
        if (!item || item->x0 == 0x21)
            special = 1;
        category = 0;
        if (!item || item->x0 == 0x1A || item->x0 == 0x1D || item->x0 == 0x1C || item->x0 == 0x1E || item->x0 == 0x1F || item->x0 == 0x22)
            category = 1;
        present = 0;
        switch (obj->x510) {
        case 1:
            present = func_00194008(&D_004225A0, i) > 0;
            break;
        case 2:
            present = func_00194050(&D_004225A0, i) > 0;
            break;
        case 3:
            present = func_00194098(&D_004225A0, i) > 0;
            break;
        }
        hidden = 0;
        if (!selected && !group && !special && !category)
            hidden = present | (CallScalarBridge001F97A8_5_func_001F97A8(context, obj->x510, i) != 0);
        entry->x6 = hidden;
    }
}
