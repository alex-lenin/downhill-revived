/* UNIT 0x0014DE90 0x0014E460 .rodata=0x289CC0:0x289DB0 */
#include "race_limit.h"
typedef struct {
    char pad0[0x10C];
    int x10C;
    int x110;
    char pad114[0x118 - 0x114];
    int x118;
    char pad11C[0x288 - 0x11C];
    int x288;
    int x28C;
    int x290;
    char pad294[0x2A0 - 0x294];
    int x2A0;
} Globals;

typedef struct {
    int x0;
    int x4;
} Entry;

typedef struct {
    char pad0[8];
    unsigned short x8;
    char padA[0x20 - 0xA];
    int x20[1];
} Bef;

extern Globals D_004B25A0;
extern char D_004225A0[];
extern char *D_0026EFE8[];
extern int D_00381DE0[];
extern char D_0029C5C0[];
extern char D_0029C5C8[];
extern char D_0029C5D0[];

extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern char *func_0014DD78(int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00194A10(void *, int);
extern int func_00194C80(void *, int);
extern int func_001822D8(int);

extern void func_00254D68(char *, char *, ...);
extern void func_00218498(void *, int);
extern void func_0020F2A8(int);

void func_0014DE90(Entry *e)
{
    char buf[64];
    int s3;
    char *s;
    int total;
    Bef *b;
    int *p;
    int i;
    char *fmt;
    int v;
    void *obj;

    if (D_004B25A0.x110 == 1) {
        if (D_004B25A0.x10C == 0x14) {
            return;
        }
        func_0020FB80(0, 0x180, 0x140, -0x8B, -0x20);
        func_0020F518(0, 0x6E, 0x6E, 0xC8, 0x96);
        s = func_0014DD78(e->x4);
        func_0020F9F0(0, 0, 0x114, 8, s);
        func_0020FB80(0, 0x130, 0x110, -0x80, -0x20);
        if (D_004B25A0.x288 == 2) {
            s3 = func_00194A10(D_004225A0, D_004B25A0.x10C);
        } else {
            s3 = func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, e->x4);
        }
        if (s3 == e->x0) {
            func_0020F9A8(0, 0, 10, 0x82, "Qualifying Heat");
            func_0020F9A8(0, 0, 10, 0x8E, "Place in top 3");
            func_0020F9A8(0, 0, 10, 0x9A, "to Advance");
        } else if (D_004B25A0.x10C < 3) {
            if (RaceLimitReachedLegacy(e) != 0) {
                func_0020F9A8(0, 0, 10, 0x82, "Final Race");
                func_0020F9A8(0, 0, 10, 0x8E, "Must Place 1st");
                func_0020F9A8(0, 0, 10, 0x9A, "to Win Career");
            }
        }
        func_0020FB80(0, 0x140, 0x120, -0x70, -0x20);
        if (D_004B25A0.x10C == 5) {
            func_00254D68(buf, "Course %d", s3 + 1);
            goto disp_done;
        }
        if (D_004B25A0.x10C < 6) {
            goto do_race;
        }
        if (D_004B25A0.x10C >= 10) {
            goto do_race;
        }
        fmt = "Race %d of %d";
        v = s3 + 1;
        total = func_001822D8(D_004B25A0.x10C);
        func_00254D68(buf, fmt, v, total);
        goto disp_done;
do_race:
        func_00254D68(buf, D_0029C5C0, s3 + 1);
disp_done:
        func_0020F9A8(0, 0, 10, 0xAA, buf);
        if (D_004B25A0.x2A0 == 0x16 || D_004B25A0.x2A0 == 0xE || D_004B25A0.x2A0 == 5) {
            func_0020FB80(0, 0x130, 0x110, -0x87, -0x20);
        } else {
            func_0020FB80(0, 0x130, 0x110, -0x80, -0x20);
        }
        func_0020F9A8(0, 0, 10, 0xB8, D_0026EFE8[D_004B25A0.x2A0]);
        b = (Bef *)D_00381DE0[0];
        if (b != 0 && b->x8 - 1 > 0) {
            i = 0;
            p = (int *)((char *)b + 0x20);
            do {
                obj = (void *)*p;
                if (s3 == i) {
                    func_00218498(obj, 1);
                } else if (s3 < i) {
                    func_00218498(obj, 2);
                } else {
                    func_00218498(obj, 0);
                }
                i++;
                p++;
            } while (i < b->x8 - 1);
        }
        func_0020F2A8(0);
        return;
    }
    if (D_004B25A0.x118 != 0) {
        return;
    }
    if (D_004B25A0.x10C != 12 && D_004B25A0.x10C != 14 && D_004B25A0.x10C != 18 && D_004B25A0.x10C != 17 && D_004B25A0.x10C != 11 && D_004B25A0.x10C != 10 && D_004B25A0.x10C != 16 && D_004B25A0.x10C != 15 && D_004B25A0.x10C != 19 && D_004B25A0.x10C != 13) {
        return;
    }
    func_0020FB80(0, 0x130, 0x110, -0x80, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0x96);
    if (D_004B25A0.x10C == 14) {
        func_0020F9A8(0, 0, 10, 0xAA, "Pickup Editor");
    } else if (D_004B25A0.x10C == 18) {
        func_0020F9A8(0, 0, 10, 0xAA, "Time Trial");
    } else if (D_004B25A0.x10C == 10) {
        func_0020F9A8(0, 0, 10, 0xAA, D_0029C5C8);
    } else if (D_004B25A0.x10C == 17) {
        func_0020F9A8(0, 0, 10, 0xAA, "Super Jump");
    } else if (D_004B25A0.x10C == 11) {
        func_0020F9A8(0, 0, 10, 0xAA, "Training");
    } else if (D_004B25A0.x10C == 19) {
        func_0020F9A8(0, 0, 10, 0xAA, D_0029C5D0);
    } else if (D_004B25A0.x10C == 16) {
        func_0020F9A8(0, 0, 10, 0xAA, "Dual Slalom");
    } else if (D_004B25A0.x10C == 15) {
        func_0020F9A8(0, 0, 10, 0xAA, "Mosh Bowl");
    } else {
        func_0020F9A8(0, 0, 10, 0xAA, "Single Event");
    }
    if (D_004B25A0.x2A0 == 0x16 || D_004B25A0.x2A0 == 0xE || D_004B25A0.x2A0 == 5) {
        func_0020FB80(0, 0x130, 0x110, -0x87, -0x20);
    }
    if (D_004B25A0.x10C != 17 && D_004B25A0.x10C != 11 && D_004B25A0.x10C != 16 && D_004B25A0.x10C != 15) {
        func_0020F9A8(0, 0, 10, 0xB8, D_0026EFE8[D_004B25A0.x2A0]);
    }
    func_0020F2A8(0);
}
