/* UNIT 0x001A22D8 0x001A2660 */
typedef struct State {
    char pad0[0x138];
    int x138;
    int x13C;
    volatile int x140;
    int x144;
    int x148;
    int x14C;
    int x150;
    int x154;
    int x158;
    int x15C;
    int x160;
} State;

typedef struct Screen {
    char pad0[0x20];
    int x20;
} Screen;

extern State D_0043F870;
extern Screen D_003F52D0;
extern int D_003F52F0[];
extern int D_004B2828[];
extern char D_003B2270[];
extern char D_00398C40[];
extern char D_0028E750[];
extern char D_0028E760[];
extern char D_0028E770[];
extern char D_0028E780[];

extern void func_001A2C58(void *, int);
extern int func_001A2E28(void *, int, int);
extern int func_001A2660(void *);
extern void func_001A30B8(void *, int);
extern void func_0017A678(void *, int);
extern void func_001D4890(void *);
extern void func_001D4A30(void *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_00254D68(char *, char *, ...);

static inline void *Child150(char *p) { return p + 0x150; }
static inline void *Off240(char *p) { return p + 0x240; }

void func_001A22D8(State *p)
{
    char text[64];
    int max = 0xAA;
    int i;

    if (p->x144 == 0) {
        func_001A2C58(&D_0043F870, p->x150);
    }
    p->x150 += p->x154;
    if (p->x150 > max) {
        p->x150 = max;
    }
    func_0017A678(Child150(D_003B2270), -2);
    if (p->x144 == 0) {
        for (i = 1; i < 3; i++) {
            if (i < p->x148) {
                func_001A2E28(&D_0043F870, i, 0xAA);
            }
        }
        if (func_001A2E28(&D_0043F870, p->x148, p->x14C) == 0) {
            p->x148++;
        }
        if (p->x148 == 1 && p->x14C == 0) {
            func_001D4890(Off240(D_00398C40));
        }
        p->x14C += p->x154;
        if (p->x14C > max) {
            if (p->x148 < 3) {
                func_001D4890(Off240(D_00398C40));
            }
            p->x148++;
            if (p->x148 >= 4) {
                p->x144 = 1;
                p->x148 = 1;
            }
            p->x14C = 0;
        }
    }
    if (p->x144 == 1) {
        p->x15C = func_001A2660(&D_0043F870);
        p->x14C += p->x154;
        if (p->x14C > max) {
            p->x14C = 0;
            p->x148++;
            if (p->x148 < 5) {
                func_001D4A30(Off240(D_00398C40));
            }
        }
        func_0020FB80(0, 0x118, 0x100, -0x62, -0x20);
        func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
        func_00254D68(text, D_0028E750, p->x158 + p->x15C);
        if (D_004B2828[0] >= 2) {
            func_0020F9F0(0, D_003F52F0[0], 0x100, 0x99, text);
        } else {
            func_0020F9F0(0, D_003F52F0[0], 0x100, 0x91, text);
        }
    }
    if (p->x144 != 0) {
        func_001A30B8(&D_0043F870, 0xAA);
    }
    if (D_0043F870.x140 != 0) {
        if (D_0043F870.x138 != 0) {
            func_0020F518(0, 0x6E, 0x6E, 0xC8, p->x150);
            func_0020FB80(0, 0x100, 0xC0, -0x5B, -0x20);
            func_0020F9F0(0, D_003F52D0.x20, 0x50, 0xA0, D_0028E760);
            func_0020F9F0(0, D_003F52D0.x20, 0x50, 0xAA, D_0028E770);
        }
    }
    if (D_0043F870.x140 != 0 && D_0043F870.x13C != 0) {
        func_0020F518(0, 0x6E, 0x6E, 0xC8, p->x150);
        func_0020FB80(0, 0x100, 0xC0, -0x5B, -0x20);
        func_0020F9F0(0, D_003F52D0.x20, 0x1AE, 0xA0, D_0028E780);
        func_0020F9F0(0, D_003F52D0.x20, 0x1AE, 0xAA, D_0028E770);
    }
}
