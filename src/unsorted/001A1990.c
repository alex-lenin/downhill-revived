/* UNIT 0x001A1990 0x001A1E88 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: retail computes profile*0x694 twice at entry and keeps "%d" $gp-relative (owned
   .sdata); here one record pointer, extern strings via lui/%lo, so entry scheduling and registers
   shift (built 0x4EC vs 0x4F4); 88 aligned rows differ, all register/order/address-form */
typedef unsigned short u16;

/* Per-profile trick counters, one 0x694-byte record per profile in D_004225A0.
   Index 0 of each array is unused (trick ids start at 1). */
typedef struct TrickRecord {
    char pad0[0x46A];
    u16 basic[43];       /* 0x46A: landed plain */
    u16 superStyled[43]; /* 0x4C0: landed super-styled */
    u16 special[8];      /* 0x516: special tricks 1..7 */
    char pad526[0x694 - 0x526];
} TrickRecord;

extern TrickRecord D_004225A0[];
extern int D_003F52F0;      /* font */
extern char *D_0026ED40[];  /* trick names, index 1..42 */
extern char *D_00272428[];  /* special trick names, index 1..7 */
extern char D_0029D268[];   /* "%d" */
extern char D_0029D2A0[];   /* "--" */
extern char D_0029D2B8[];   /* "Basic" */
extern char D_0029D2C0[];   /* "SS" */
extern char D_0028E6B8[];   /* "TRICKS LANDED" */
extern char D_0028E6C8[];   /* "Trick Name" */
extern char D_0028E6D8[];   /* "TRICKS LANDED:" */
extern char D_0028E6E8[];   /* "%d of %d" */
extern char D_0028E6F8[];   /* "TOTAL TRICKS:" */

extern void func_0023D650(int font, int *width, int *height);
extern int func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00254D68(char *, char *, ...);
extern int func_001E51B0(void);

/* Draws the "TRICKS LANDED" table for a profile: two columns of tricks with
   their basic / super-styled counts, then the special tricks, then totals. */
void func_001A1990(void *unused, int profile)
{
    char text[48];
    int stats[4]; /* width, height, total landed, distinct tricks landed */
    TrickRecord *record;
    u16 *specialStart;
    int font;
    int trick;
    int y;
    int remaining;
    int line;
    int odd;
    int column;
    int centerX;
    u16 *basic;
    u16 *superStyled;
    u16 *special;
    char **trickNames;
    char **specialNames;
    u16 count;

    record = &D_004225A0[profile];
    font = D_003F52F0;
    specialStart = record->special;
    superStyled = &record->superStyled[1];
    line = 0;
    basic = &record->basic[1];
    y = 0x2B;
    trick = 1;
    func_0023D650(font, &stats[0], &stats[1]);
    centerX = stats[0] / 2;
    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    stats[2] = 0;
    func_0020FB80(0, 0x160, 0x100, -0x7B, -0x20);
    stats[3] = 0;
    func_0020F9F0(0, font, centerX, 0x14, D_0028E6B8);
    func_0020FB80(0, 0xF0, 0xC0, -0x62, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0x23, 0x23, D_0028E6C8);
    func_0020F9A8(0, font, 0xC6, 0x23, D_0029D2B8);
    func_0020F9A8(0, font, 0x10E, 0x23, D_0029D2C0);
    func_0020F9A8(0, font, 0x154, 0x23, D_0028E6C8);
    func_0020F9A8(0, font, 0x1F7, 0x23, D_0029D2B8);
    func_0020F9A8(0, font, 0x23F, 0x23, D_0029D2C0);
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);

    /* tricks 1..42; even lines in the left column, odd lines on the right */
    trickNames = &D_0026ED40[1];
    do {
        if (*basic != 0) {
            column = (line & 1) ? 0x131 : 0;
            func_0020F9A8(0, font, column + 0x23, y, *trickNames);
            func_00254D68(text, D_0029D268, *basic);
            func_0020F9A8(0, font, column + 0xD4, y, text);
            func_00254D68(text, D_0029D268, *superStyled);
            if (trick >= 0x1D) {
                func_0020F9A8(0, font, column + 0x111, y, D_0029D2A0);
            } else {
                func_0020F9A8(0, font, column + 0x111, y, text);
            }
            if (*basic != 0) {
                count = *basic;
                stats[3] += 1;
                stats[2] += count;
            }
            if (*superStyled != 0) {
                count = *superStyled;
                stats[3] += 1;
                stats[2] += count;
            }
            y = (line & 1) ? (y + 8) : y;
            line += 1;
        }
        trick += 1;
        superStyled += 1;
        basic += 1;
        trickNames += 1;
    } while (trick < 0x2B);

    /* special tricks 1..7 (no super-styled column) */
    remaining = 6;
    special = specialStart + 1;
    specialNames = &D_00272428[1];
    do {
        if (*special != 0) {
            odd = line & 1;
            column = odd ? 0x131 : 0;
            func_0020F9A8(0, font, column + 0x23, y, *specialNames);
            line += 1;
            stats[3] += 1;
            func_00254D68(text, D_0029D268, *special);
            func_0020F9A8(0, font, column + 0xD4, y, text);
            func_0020F9A8(0, font, column + 0x111, y, D_0029D2A0);
            y = odd ? (y + 8) : y;
            stats[2] += *special;
        }
        specialNames += 1;
        remaining -= 1;
        special += 1;
    } while (remaining >= 0);

    y = (line & 1) ? (y + 8) : y;
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xCC, y, D_0028E6D8);
    func_00254D68(text, D_0028E6E8, stats[3], func_001E51B0());
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    func_0020F9A8(0, font, 0x165, y, text);
    y += 8;
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xD7, y, D_0028E6F8);
    func_00254D68(text, D_0029D268, stats[2]);
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    func_0020F9A8(0, font, 0x165, y, text);
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_001A1990);
#endif
