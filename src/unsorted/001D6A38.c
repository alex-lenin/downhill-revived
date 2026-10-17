/* UNIT 0x001D6A38 0x001D6D68 */
#include "team_roster.h"
typedef struct Global {
    char pad0[0x288];
    int x288;
    char pad28C[0x14];
    int x2A0;
} Global;

extern Global D_004B25A0;

extern void func_001DB510(void *);
extern void func_001C0E00(void *);
extern void func_001D2CB8(void *);
extern void func_001D1C58(void *);
extern void func_001C0B60(void *);

void func_001D6A38(void *arg)
{
    char *obj = (char *)arg;
    int flags[4];
    int i, i2, i3, i4, one, one2, one3, one4, v, n;
    int *p, *p2, *p3, *p4, *q, *q2, *q3;

    func_001DB9E0(0x645, flags);
    i = 0;
    if (D_004B25A0.x288 > 0) {
        one = 1;
        q = (int *)(obj + 0x120);
        p = flags;
        do {
            v = *p;
            if (v == one) {
                *(int *)((char *)q - 0xF0) = v;
                *(int *)((char *)q - 0xC0) = v;
                *q = v;
            }
            i++;
            q++;
            p++;
        } while (i < D_004B25A0.x288);
    }

    if (D_004B25A0.x2A0 == 0x16)
        func_001C0E00(obj + 0x5E0);

    if (*(int *)(obj + 0x560) > 0) {
        func_001DB510(obj + 0x3E0 + (*(int *)(obj + 0x564) << 6));
        n = *(int *)(obj + 0x564) + 1;
        *(int *)(obj + 0x564) = n;
        if (n >= *(int *)(obj + 0x560))
            *(int *)(obj + 0x564) = 0;
    }

    {
    int mode, five;
    register int initial_count;
    register char *xD0;
    char *x10, *x40, *x70, *x130, *x160;
    char *x220, *x1C0, *x290, *x300, *x100, *x1F0, *x190, *x5B0;
    mode = D_004B25A0.x2A0;
    five = 5;
    xD0 = obj + 0xD0;
    x190 = obj + 0x190;
    x10 = obj + 0x10;
    x100 = obj + 0x100;
    x40 = obj + 0x40;
    x1C0 = obj + 0x1C0;
    x70 = obj + 0x70;
    x130 = obj + 0x130;
    x1F0 = obj + 0x1F0;
    x160 = obj + 0x160;
    x220 = obj + 0x220;
    x290 = obj + 0x290;
    x300 = obj + 0x300;
    x5B0 = obj + 0x5B0;

    if (mode == five && *(int *)(obj + 0x568) == 0) {
            func_001DB9E0(0x672, flags);
            i2 = 0;
            initial_count = D_004B25A0.x288;
            if (initial_count > 0) {
                one2 = 1;
                p2 = flags;
                do {
                    v = *p2;
                    if (v == one2)
                        *(int *)(obj + 0x568) = v;
                    i2++;
                    p2++;
                } while (i2 < D_004B25A0.x288);
            }
    }

    if (*(int *)(obj + 0x568) != 0)
        func_001DB510(obj + 0x570);

    func_001DB9E0(0x64C, flags);
    i3 = 0;
    if (D_004B25A0.x288 > 0) {
        one3 = 1;
        q2 = (int *)(obj + 0x30);
        p3 = flags;
        do {
            v = *p3;
            if (v == one3)
                *q2 = v;
            i3++;
            q2++;
            p3++;
        } while (i3 < D_004B25A0.x288);
    }

    func_001DB9E0(0x645, flags);
    i4 = 0;
    if (D_004B25A0.x288 > 0) {
        one4 = 1;
        q3 = (int *)(obj + 0x1E0);
        p4 = flags;
        do {
            v = *p4;
            if (v == one4) {
                *(int *)((char *)q3 - 0xF0) = v;
                *(int *)((char *)q3 - 0x1B0) = v;
                *(int *)((char *)q3 - 0x180) = v;
                *(int *)((char *)q3 - 0x90) = v;
                *(int *)((char *)q3 - 0x60) = v;
                *(int *)((char *)q3 - 0x30) = v;
                *(int *)((char *)q3 - 0xC0) = v;
                *q3 = v;
            }
            i4++;
            q3++;
            p4++;
        } while (i4 < D_004B25A0.x288);
    }

    func_001D2CB8(xD0);
    func_001D2CB8(x10);
    func_001D2CB8(x40);
    func_001D2CB8(x70);
    func_001D2CB8(x130);
    func_001D2CB8(x160);
    func_001D2CB8(x190);
    func_001D2CB8(x100);
    func_001D2CB8(x1C0);
    func_001D2CB8(x1F0);
    func_001D1C58(x220);
    func_001D1C58(x290);
    func_001D1C58(x300);
    func_001C0B60(x5B0);
    if (D_004B25A0.x2A0 != 0xE)
        func_001C0B60(obj + 0x600);
    }
}
