/* UNIT 0x001D6090 0x001D6390 */
#include "team_roster.h"
/* func_001D6090: build per-slot flags from scores, fan out enable bits, update sub-objects.
 *
 * Retail notes:
 * - D_004B25A0.x288 is the slot count (splat D_004B2828) and x2A0 the mode
 *   (splat D_004B2840); both are reached via the D_004B25A0 base so the struct
 *   overlay below is required for matching lui/addiu codegen.
 * - D_004C26C0.xD0 holds the per-slot scores (splat D_004C2790 area).
 * - func_001D4E80 takes the object pointer in $a0 (still live at entry);
 *   declaring it void mis-schedules the prologue by one slot.
 * - Declaration order of the x* pointers below affects spill slots; keep as is.
 */
typedef struct Global {
    char pad0[0x288];
    int x288;
    char pad28C[0x14];
    int x2A0;
} Global;
typedef struct Floats {
    char pad0[0xD0];
    float xD0[4];
} Floats;
extern Global D_004B25A0;
extern Floats D_004C26C0;
extern int func_001D4E80(void *arg);

extern void func_001D2CB8(void *sub);
extern void func_001D1C58(void *sub);
extern void func_001DB510(void *sub);
extern void func_001C0B60(void *sub);
void func_001D6090(void *arg)
{
    char *obj = (char *)arg;
    int flags[4];
    int i, i2, i3, i4, j, *p, *d2, *p2, *d3, *p3, v, one, one2, one3, one4;
    char *q;
    char *x160, *x190, *x130, *x1C0, *x1F0, *x220, *xD0, *x10, *x280, *x100, *x2F0, *x360, *x250;
    if (func_001D4E80(arg)) {
        for (j = 0; j < D_004B25A0.x288; j++) {
            if (D_004C26C0.xD0[j] > 0.2f)
                flags[j] = 1;
        }
        i = 0;
        if (D_004B25A0.x288 > 0) {
            one = 1;
            q = obj + 0x240;
            p = flags;
            do {
                v = *p;
                if (v == one) {
                    /* slots 0x150,0x180,0x210,0x240; last-written 0x240 first */
                    *(int *)(q - 0xF0) = v;
                    *(int *)(q - 0xC0) = v;
                    *(int *)(q - 0x30) = v;
                    *(int *)q = v;
                }
                i++;
                q += 4;
                p++;
            } while (i < D_004B25A0.x288);
        }
    }
    func_001DB9E0(0x648, flags);
    i2 = 0;
    x130 = obj + 0x130; x160 = obj + 0x160; x190 = obj + 0x190; x1C0 = obj + 0x1C0;
    x1F0 = obj + 0x1F0; x220 = obj + 0x220; xD0 = obj + 0xD0; x100 = obj + 0x100;
    x250 = obj + 0x250; x280 = obj + 0x280; x2F0 = obj + 0x2F0; x360 = obj + 0x360;
    x10 = obj + 0x10;
    if (D_004B25A0.x288 > 0) {
        one2 = 1;
        d2 = (int *)(obj + 0x210);
        p2 = flags;
        do {
            v = *p2;
            if (v == one2)
                *d2 = v;
            i2++;
            d2++;
            p2++;
        } while (i2 < D_004B25A0.x288);
    }
    func_001DB9E0(0x64C, flags);
    i3 = 0;
    if (D_004B25A0.x288 > 0) {
        one3 = 1;
        d3 = (int *)(obj + 0x150);
        p3 = flags;
        do {
            v = *p3;
            if (v == one3)
                *d3 = v;
            i3++;
            d3++;
            p3++;
        } while (i3 < D_004B25A0.x288);
    }
    func_001DB9E0(0x64A, flags);
    i4 = 0;
    if (D_004B25A0.x288 > 0) {
        one4 = 1;
        q = obj + 0x120;
        p = flags;
        do {
            v = *p;
            if (v == one4) {
                /* 0x150,0x180,0x1E0,0x210,0x240,0xF0,0x120; last 0x120 first */
                *(int *)(q + 0x30) = v;
                *(int *)(q + 0x60) = v;
                *(int *)(q + 0xC0) = v;
                *(int *)(q + 0xF0) = v;
                *(int *)(q + 0x120) = v;
                *(int *)(q - 0x30) = v;
                *(int *)q = v;
            }
            i4++;
            q += 4;
            p++;
        } while (i4 < D_004B25A0.x288);
    }
    func_001D2CB8(x130);
    func_001D2CB8(x160);
    func_001D2CB8(x190);
    func_001D2CB8(x1C0);
    func_001D2CB8(x1F0);
    func_001D2CB8(x220);
    func_001D2CB8(xD0);
    func_001D2CB8(x100);
    func_001D2CB8(x250);
    func_001D1C58(x280);
    func_001D1C58(x2F0);
    func_001D1C58(x360);
    func_001DB510(x10);
    if (D_004B25A0.x2A0 != 6)
        func_001C0B60(obj + 0x3D0);
}
