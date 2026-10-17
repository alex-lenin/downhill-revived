/* UNIT 0x00195828 0x00195960 */
#include "rtc.h"
typedef struct Name7 { int x0; short x4; char x6; } Name7;
typedef struct Name3 { short x0; char x2; } Name3;
typedef struct Obj {
    int x0; short x4, x6;
    unsigned char x8, x9, xA, xB;
    union { Name7 n7; Name3 n3; char s[1]; } xC;
} Obj;
typedef struct Global { char pad0[0x288]; int x288; char pad28C[0xB17C - 0x28C]; int xB17C; char xB180[1]; } Global;

extern Global D_004B25A0;
extern void *memcpy(void *, const void *, unsigned int);
/* the literals "player" and "AI" */
extern int D_0029D098[];
extern int D_0029D0A0[];
extern const char D_0028D948[];

extern void func_00254544(char *, char *);
extern void func_00254D68(char *, const char *, int);

void func_00195828(Obj *obj, int arg1, int arg2, int arg3, int player, int arg5)
{
    unsigned char tmp[8];
    Global *g;

    ReadRtcDiscard(tmp);
    obj->x8 = tmp[5];
    obj->x9 = tmp[6];
    obj->xA = tmp[7];
    obj->x0 = arg1;
    obj->x4 = arg2;
    obj->x6 = arg3;
    obj->xB = arg5;
    g = &D_004B25A0;
    if (g->xB17C) {
        if (player) {
            func_00254544(obj->xC.s, g->xB180);
            return;
        }
    } else if (player) {
        if (g->x288 >= 2) {
            func_00254D68(obj->xC.s, D_0028D948, player);
        } else {
            memcpy(obj->xC.s, D_0029D098, 7);
        }
        return;
    }
    memcpy(obj->xC.s, D_0029D0A0, 3);
}
