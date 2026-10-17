/* UNIT 0x001956C0 0x00195828 */
#include "rtc.h"
typedef struct Name7 { int x0; short x4; char x6; } Name7;
typedef struct Name3 { short x0; char x2; } Name3;
typedef struct Entry {
    int x0; short x4, x6;
    unsigned char x8, x9, xA, xB;
    union { Name7 n7; Name3 n3; char s[12]; } xC;
} Entry;
typedef struct List {
    char pad0[0x5C];
    Entry entries[1];
} List;
typedef struct Global { char pad0[0x288]; int x288; char pad28C[0xB17C - 0x28C]; int xB17C; char xB180[1]; } Global;

extern Global D_004B25A0;
extern void *memcpy(void *, const void *, unsigned int);
/* the literals "player" and "AI" */
extern int D_0029D098[];
extern int D_0029D0A0[];
extern const char D_0028D948[];

extern void func_00254544(char *, char *);
extern void func_00254D68(char *, const char *, int);

void func_001956C0(List *list, int index, int arg2, int arg3, int arg4, int player, int arg6)
{
    unsigned char tmp[8];
    Global *g;

    ReadRtcDiscard(tmp);
    list->entries[index].x8 = tmp[5];
    list->entries[index].x9 = tmp[6];
    list->entries[index].xA = tmp[7];
    list->entries[index].x0 = arg2;
    list->entries[index].x4 = arg3;
    list->entries[index].x6 = arg4;
    list->entries[index].xB = arg6;
    g = &D_004B25A0;
    if (g->xB17C) {
        if (player) {
            func_00254544(list->entries[index].xC.s, g->xB180);
            return;
        }
    } else if (player) {
        if (g->x288 >= 2) {
            func_00254D68(list->entries[index].xC.s, D_0028D948, player);
        } else {
            memcpy(list->entries[index].xC.s, D_0029D098, 7);
        }
        return;
    }
    memcpy(list->entries[index].xC.s, D_0029D0A0, 3);
}
