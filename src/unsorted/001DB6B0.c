/* UNIT 0x001DB6B0 0x001DB920 */
typedef int s32; typedef unsigned int u32;
extern s32 func_00208D68(void *, void *, void *, s32);
extern s32 func_00208E40(s32, s32, s32, s32);
struct Rider { char pad[0x2B0]; int x2B0; int x2B4; char tail[0xC8]; };
extern struct Rider D_0037EA60[];
extern char *D_0037ED10[];
extern s32 D_0043F348[];
struct State { char pad[0x288]; int x288; };
extern struct State D_004B25A0;
extern s32 D_004B29BC[];

void func_001DB6B0(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 *arg5) {
    s32 values[4];
    struct Rider *rider;
    s32 alternate;
    s32 anyHit;
    s32 *clear;
    s32 hit;
    s32 index;
    s32 maxFirst;
    s32 maxSecond;
    s32 count;

    anyHit = 0;
    maxFirst = 0;
    maxSecond = 0;
    alternate = 1;
    if ((u32) (D_0043F348[0] - 1) >= 3U || D_004B29BC[0] == 0) {
        *arg3 = 0;
        *arg4 = 0;
        return;
    }
    count = 3;
    clear = &values[3];
    do {
        *clear = 0;
        count -= 1;
        clear -= 1;
    } while (count >= 0);
    for (index = 0; index < D_004B25A0.x288; index++) {
        rider = &D_0037EA60[index];
        if (arg5 != 0 && arg5[index] == 1) {
            values[index] = 0;
        } else {
            if (rider->x2B4 != 0) {
                values[index] = func_00208E40(rider->x2B0 + 0x10, arg0, arg1, arg2);
            }
            if (values[index] >= 1) anyHit = 1;
        }
        hit = values[index];
        if (alternate && maxFirst < hit) {
            maxFirst = hit;
        } else if (maxSecond < hit) {
            maxSecond = hit;
        }
        alternate ^= 1;
    }
    if (anyHit == 0) {
        *arg3 = 0;
        *arg4 = 0;
        return;
    }
    if (D_004B25A0.x288 == 1) {
        *arg3 = values[0];
        *arg4 = func_00208D68(D_0037ED10[0] + 0x40, D_0037ED10[0] + 0x50, D_0037ED10[0] + 0x10, arg0);
        return;
    }
    if (maxFirst == 0) {
        *arg3 = maxSecond;
        *arg4 = 0x5A;
        return;
    }
    if (maxSecond == 0) {
        *arg3 = maxFirst;
        *arg4 = 0x10E;
        return;
    }
    if (maxSecond < maxFirst) {
        *arg3 = maxFirst;
        *arg4 = maxSecond * 0x5A / maxFirst + 0x10E;
        return;
    }
    *arg3 = maxSecond;
    *arg4 = 0x5A - maxFirst * 0x5A / maxSecond;
}
