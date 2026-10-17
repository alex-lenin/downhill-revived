/* UNIT 0x00183B10 0x001841A8 */
typedef struct Obj {
    char pad0[0x20];
    int x20;
    char pad24[0x2C - 0x24];
    int x2C;
    char pad30[0x34 - 0x30];
    int x34;
    char pad38[0x290 - 0x38];
    int x290[4][8];
    int x310[4][8];
} Obj;

typedef struct Global {
    char pad0[0x288];
    int count;
    char pad28C[0x2A0 - 0x28C];
    int x2A0;
} Global;

typedef struct Disp {
    char pad0[0x20];
    int x20;
} Disp;

typedef struct ArgObj {
    char pad0[0x460];
    int x460;
    char pad464[0x500 - 0x464];
    int x500;
    int x504;
} ArgObj;

typedef struct Entry {
    int x0;
    char pad4[0xD8 - 0x4];
} Entry;

typedef struct NameTable { void *names[4]; } NameTable;
extern Disp D_003F52D0;
extern Obj D_003C6930;
extern Global D_004B25A0;
extern char D_004225A0[];
extern void *D_002FF3F0[];
extern char D_0028C5C0[];
extern char D_0028C5D8[];
extern char D_0028C5E8[];
extern char D_0029CD18[];
extern char D_0029CD20[];
extern char D_0029CD28[];
extern char D_0029CD30[];
extern char D_0029CD38[];
extern char D_0029CD40[];

extern void *func_00254204(void *, int, int);
extern void func_0023D650(int, int *, int *);
extern void func_0020F648(int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, void *);
extern void func_0020FB80(int, int, int, int, int);
extern int func_00254D68(char *, char *, ...);
extern void func_0012F560(void *, void *);
extern void func_0018DC20(void *);

static inline int *GetScores(Obj *obj, int i) { return obj->x290[i]; }
static inline int *GetMultipliers(Obj *obj, int i) { return obj->x310[i]; }
static inline int GetScore(int *scores, int j) { return scores[j]; }
static inline int GetDisp(int *disp) { return *disp; }
static inline int GetNext(int *next) { return *next; }


void func_00183B10(ArgObj *obj)
{
    char buf[32];
    int pos[4];
    int xpos[4];
    int totals[4];
    int width;
    int height;
    int disp;
    int i;
    int j;
    int y;
    int next_i;
    int *base310;
    int *base290;
    NameTable *table;

    /* names read through a pointer so the index add keeps the table base first */
    table = (NameTable *)D_002FF3F0;
    func_00254204(totals, 0, 16);
    disp = D_003F52D0.x20;
    func_0023D650(GetDisp(&disp), &width, &height);
    width = width / 2;
    if (D_004B25A0.count == 1) {
        xpos[0] = width - 0x3C;
        pos[0] = width + 0x3C;
    } else if (D_004B25A0.count == 2) {
        xpos[0] = width - 0x82;
        pos[0] = width;
        pos[1] = width + 0x82;
    } else if (D_004B25A0.count == 3) {
        xpos[0] = width - 0xB4;
        pos[0] = width - 0x50;
        pos[1] = width + 0x1E;
        pos[2] = width + 0x96;
    } else if (D_004B25A0.count == 4) {
        xpos[0] = width - 0xDC;
        pos[0] = width - 0x82;
        pos[1] = width - 0x1A;
        pos[2] = width + 0x4C;
        pos[3] = width + 0xB2;
    }
    func_0020F648(0, 0x24, 0x18);
    func_0020F588(disp, 0, 0xC8, 0xC8, 0, 0xAA);
    i = 0;
    func_0020F9F0(0, D_003F52D0.x20, width, 5, D_0028C5C0);
    func_0020FB80(0, 0x160, 0x100, -0x8B, -0x20);
    func_00254D68(buf, D_0028C5D8, D_003C6930.x2C, 8);
    func_0020F9F0(0, D_003F52D0.x20, width, 0x19, buf);
    func_0020FB80(0, 0x120, 0xC0, -0x6B, -0x20);
    func_0020F588(disp, 0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9F0(0, D_003F52D0.x20, xpos[0], 0x32, D_0029CD18);
    for (; i < D_004B25A0.count;) {
        func_0020F588(disp, 0, 0x6E, 0x6E, 0xC8, 0xAA);
        y = 0x3A;
        j = 1;
        next_i = i + 1;
        func_0012F560(table->names[i], buf);
        func_0020F9F0(0, D_003F52D0.x20, pos[i], 0x32, buf);
        base290 = GetScores(&D_003C6930, i);
        base310 = GetMultipliers(&D_003C6930, i);
        func_0020F588(disp, 0, 0x58, 0x98, 0x49, 0xAA);
        if (D_003C6930.x2C > 0) {
            do {
                if (i == 0) {
                    func_00254D68(buf, D_0029CD20, j);
                    func_0020F9F0(0, D_003F52D0.x20, xpos[0], y, buf);
                }
                if (D_003C6930.x20 == 1) {
                    if (base310[j] == 0) {
                        func_00254D68(buf, D_0029CD20, 0);
                    } else {
                        int m = GetScore(base290, j);
                        func_00254D68(buf, D_0029CD28, (base310[j]) * m, m);
                    }
                    func_0020F9F0(0, D_003F52D0.x20, pos[i], y, buf);
                    totals[i] += (base290[j]) * (base310[j]);
                } else {
                    func_00254D68(buf, D_0029CD20, base290[j]);
                    func_0020F9F0(0, D_003F52D0.x20, pos[i], y, buf);
                    totals[i] += base290[j];
                }
                y += 8;
                j++;
            } while (j <= D_003C6930.x2C);
        }
        y += 5;
        if (i == 0) {
            func_0020F588(disp, 0, 0x6E, 0x6E, 0xC8, 0xAA);
            func_0020F9F0(0, D_003F52D0.x20, xpos[0], y, D_0029CD30);
        }
        func_0020F588(disp, 0, 0xC8, 0xC8, 0, 0xAA);
        func_00254D68(buf, D_0029CD20, totals[i]);
        func_0020F9F0(0, D_003F52D0.x20, pos[i], y, buf);
        i = GetNext(&next_i);
    }
    if (D_003C6930.x20 == 1) {
        char *player;
        func_0020F588(disp, 0, 0x6E, 0x6E, 0xC8, 0xAA);
        if (D_003C6930.x34 != 0) {
            func_0020F9F0(0, D_003F52D0.x20, (width >> 1) - 0x14, obj->x460, D_0028C5E8);
        } else {
            func_0020F9F0(0, D_003F52D0.x20, (width >> 1) - 0x14, obj->x460, D_0029CD38);
        }
        player = D_004225A0 + D_004B25A0.x2A0 * 0xD8;
        func_0020F588(disp, 0, 0xC8, 0xC8, 0, 0xAA);
        /* Player record at 0x7668 (see 001A1380.c); the volatile read keeps the
           +0x7668 as its own addiu after the call, as in retail. */
        player += 0x7668;
        func_00254D68(buf, D_0029CD40, *(volatile int *)player);
        func_0020F9F0(0, D_003F52D0.x20, (width >> 1) - 0x14, obj->x460 + 10, buf);
    }
    if (obj->x500 < 0xBE) {
        obj->x500 += 3;
    } else {
        obj->x500 = 0;
        obj->x504 += 1;
    }
    func_0018DC20(obj);
}
