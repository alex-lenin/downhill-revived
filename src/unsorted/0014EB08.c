/* UNIT 0x0014EB08 0x0014EC78 */
#include "race_limit.h"
struct Obj { int x0, x4, x8; };
struct State { char pad0[0x10c]; int x10c; char pad110[0x178]; int x288; int x28c; int x290; };
struct Race { char pad[0x796C]; int x796C; char pad7970[0x1C]; int x798C; };
struct Global { char pad[0x50130]; struct Race *x50130; };
struct Entry { char pad0[0x44]; int x44; char pad48[0x8]; };
extern struct State D_004B25A0;
extern struct Global D_002AF2C0;
extern char D_004225A0[];
extern int func_00194C80(void *, int);
extern int func_00194940(void *, int);


void func_0014EB08(struct Obj *obj)
{
    int n;
    struct Entry *entries;
    struct Race *race;

    if (D_004B25A0.x288 == 1) {
        n = (D_002AF2C0.x50130->x796C >> 5) - 1;
        obj->x8 = 0;
        if (obj->x0 == func_00194C80(D_004225A0 + n * 0x694, obj->x4)) {
            race = D_002AF2C0.x50130;
            if (race->x798C < 4) {
                obj->x8 = 1;
            }
        } else if (D_004B25A0.x10c < 3 && RaceLimitReachedLegacy(obj)) {
            race = D_002AF2C0.x50130;
            if (race->x798C == 1) {
                obj->x8 = 1;
            }
        } else {
            entries = (struct Entry *)(D_004225A0 + n * 0x694);
            if (entries[D_004B25A0.x10c].x44 >= 12) {
                obj->x8 = 1;
            }
        }
    } else if (D_004B25A0.x288 == 2) {
        obj->x8 = 0;
        if (func_00194940(D_004225A0, D_004B25A0.x10c) >= 12) {
            obj->x8 = 1;
        }
    }
}
