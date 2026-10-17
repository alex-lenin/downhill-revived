/* UNIT 0x0015FF38 0x00161050 .rodata=0x28A498:0x28A5B0 .sdata=0x29C770:0x29C7C0 */
#include "menu_items.h"
#include "format.h"
#include "menu.h"
typedef unsigned int u128 __attribute__((mode(TI)));

struct Rider {
    char pad0[0x6610];
    char x6610[1];
};

typedef struct Racer {
    char pad0[0x796C];
    int x796C;
    char pad7970[0x79AC - 0x7970];
    int x79AC;
    char pad79B0[0x7A5C - 0x79B0];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Racer;

struct World {
    Racer racers[10];
    char pad4D620[0x50130 - 10 * 0x7BD0];
    struct Rider *riders[(0x501A8 - 0x50130) / 4];
    int x501A8;
};

struct Label {
    char pad0[4];
    char visible;
};

struct Fonts {
    char pad0[0x20];
    int font;
    char pad24[0x134 - 0x24];
    struct Label *x134;
};

struct Config {
    char pad0[0x10C];
    int mode;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x364 - 0x28C];
    int x364;
};

struct Best {
    char pad0[0x12210];
    int x12210;
    int x12214;
};

typedef struct Info {
    char pad0[0xC8];
    char *xC8;
    char padCC[0x1D0 - 0xCC];
    char *x1D0;
} Info;

struct Menu {
    char pad0[0x45C];
    int x45C;
    char pad460[0xA0];
    int rider;
};

struct TrickMenu {
    char pad0[0x500];
    int rider;
};

struct CombatMenu {
    char pad0[0x500];
    int time;
};

struct Trick {
    int type;
    int superStyled;
    int slot;
};

typedef struct MenuObj {
    char pad0[0x418];
    int x418;
    char pad41C[0x40];
    int x45C;
    int x460;
    char pad464[0x44];
    int x4A8;
    char pad4AC[0x4C];
    void *x4F8;
} MenuObj;

struct Obj490 {
    char pad[0x490];
    u128 x490;
};

extern struct World D_002AF2C0;
extern struct Fonts D_003F52D0;
extern struct Menu D_0038A7D0;
extern struct Config D_004B25A0;
extern struct Best D_004225A0;
extern Info *D_0029DE68;
extern int D_004B26AC[];
extern char *D_0026ED40[];
extern char D_003B2270[];
extern char D_00284C88[];
extern char D_00284C00[];

char D_0029C770[] = "%d";
/* Same "%d" via lui: retail uses gp here and lui below for one address. */
extern char D_0029C770_vialui[16] __attribute__((alias("D_0029C770")));

extern void func_001E5300(char *, int **, int **, int **, char ***);
extern void func_001E5228(void *, struct Trick **, int *, int *, char ***);
extern void func_0023D650(int, int *, int *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00254D68(char *, char *, ...);
extern int func_001E51B0(void);
extern void func_00190358(struct Fonts *);
extern int func_00190D60(void *, int, int);
extern void func_00190660(struct Fonts *, struct Menu *);
extern void func_0018DC20(void *);

extern void func_0018E6B8(void *obj, int id, void *arg);
extern void func_001B5AF0(void *, int, int);
extern void func_0018DB08(void *);
extern void func_00175730(void *hud, int arg);
extern float func_001301B8(Racer *r);
extern char *func_00158650(int x);


void func_0015FF38(struct Menu *menu)
{
    char buf[0x30];
    int *basic;
    int *ss;
    int *special;
    char **specialNames;
    int width;
    int height;
    int font;
    int x;
    int y;
    int i;
    char **name;
    int line;
    int total;
    int unique;
    char **trickNames;
    int column;

    line = 0;
    total = 0;
    unique = 0;
    font = D_003F52D0.font;
    func_001E5300(D_002AF2C0.riders[menu->rider]->x6610,
                  &basic, &ss, &special, &specialNames);
    func_0023D650(font, &width, &height);
    x = width / 2;
    if (D_003F52D0.x134 != 0) {
        D_003F52D0.x134->visible = 0;
    }

    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    func_0020FB80(0, 0x160, 0x100, -0x8B, -0x20);
    if (D_004B26AC[0] == 0x11) {
        func_0020F9F0(0, font, x, 8, "TRICKS LANDED THIS JUMP");
    } else {
        func_0020F9F0(0, font, x, 8, "TRICKS LANDED THIS RACE");
    }

    func_0020FB80(0, 0xF0, 0xC0, -0x5B, -0x20);
    y = 0x1F;
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xF, 0x17, "Trick Name");
    func_0020F9A8(0, font, 0xA8, 0x17, "Basic");
    func_0020F9A8(0, font, 0xDA, 0x17, "SS");
    func_0020F9A8(0, font, 0x104, 0x17, "Trick Name");
    func_0020F9A8(0, font, 0x19D, 0x17, "Basic");
    func_0020F9A8(0, font, 0x1CF, 0x17, "SS");
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);

    trickNames = D_0026ED40;
    name = trickNames + 1;
    i = 1;
    do {
        if (basic[i] > 0) {
            column = (line & 1) ? 0xF5 : 0;
            func_0020F9A8(0, font, column + 0xF, y, *name);
            func_00254D68(buf, D_0029C770, basic[i]);
            func_0020F9A8(0, font, column + 0xB6, y, buf);
            func_00254D68(buf, D_0029C770, ss[i]);
            if ((int)name >= (int)(trickNames + 29)) {
                func_0020F9A8(0, font, column + 0xDD, y, "--");
            } else {
                func_0020F9A8(0, font, column + 0xDD, y, buf);
            }
            if (basic[i] > 0) {
                total += basic[i];
                unique++;
            }
            if (ss[i] > 0) {
                total += ss[i];
                unique++;
            }
            if (line & 1) {
                y += 8;
            }
            line++;
        }
        name++;
        i++;
    } while ((int)name < (int)(trickNames + 43));

    i = 1;
    do {
        if (special[i] > 0) {
            int parity = line & 1;
            column = parity ? 0xF5 : 0;
            func_0020F9A8(0, font, column + 0xF, y, specialNames[i]);
            unique++;
            line++;
            func_00254D68(buf, D_0029C770_vialui, special[i]);
            func_0020F9A8(0, font, column + 0xB6, y, buf);
            func_0020F9A8(0, font, column + 0xDD, y, "--");
            if (parity) {
                y += 8;
            }
            total += special[i];
        }
        i++;
    } while (i < 8);

    if (line & 1) {
        y += 8;
    }
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    y += 2;
    func_0020F9A8(0, font, 0x72, y, "UNIQUE TRICKS LANDED:");
    func_00254D68(buf, "%d of %d", unique, func_001E51B0());
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    func_0020F9A8(0, font, 0x139, y, buf);
    y += 9;
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, font, 0xBC, y, "TOTAL TRICKS:");
    func_00254D68(buf, D_0029C770_vialui, total);
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    func_0020F9A8(0, font, 0x139, y, buf);

    func_00190358(&D_003F52D0);
    if (func_00190D60((char *)&D_003F52D0 + 0x64, 4, menu->x45C) != 0) {
        D_0038A7D0.rider = menu->rider;
        func_00190660(&D_003F52D0, &D_0038A7D0);
    }
    func_0018DC20(menu);
}

MenuObj *func_001604D8(MenuObj *obj) {
    func_0018D9D0((SharedMenu *)(obj));
    obj->x4F8 = D_00284C88;
    obj->x45C = 0x10;
    obj->x460 = 0x28;
    func_0018E6B8(obj, 0x10, &D_0029DE68);
    obj->x4A8 = 5;
    return obj;
}

void func_00160538(struct Obj490 *obj) { obj->x490 = 0; }

void func_00160548(struct TrickMenu *menu)
{
    char buf[0x50];
    int counts[8];
    struct Trick *tricks;
    int numTricks;
    int total;
    char **names;
    int width;
    int height;
    int font;
    int x;
    int i;
    int line;
    int lines;
    int y;
    struct Rider *rider;

    line = 1;
    lines = 0;
    y = 0x1C;
    rider = D_002AF2C0.riders[menu->rider];
    font = D_003F52D0.font;
    for (i = 0; i < 8; i++) {
        counts[i] = 0;
    }
    func_001E5228(rider->x6610, &tricks, &numTricks, &total, &names);
    func_0023D650(font, &width, &height);
    x = width / 2;
    if (D_003F52D0.x134 != 0) {
        D_003F52D0.x134->visible = 0;
    }
    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    func_0020FB80(0, 0x180, 0x120, -0x8B, -0x20);
    if (D_004B25A0.mode == 0x11) {
        func_0020F9F0(0, font, x, 0xC, "BEST TRICK THIS JUMP");
    } else {
        func_0020F9F0(0, font, x, 0xC, "BEST TRICK THIS RACE");
    }
    func_0020FB80(0, 0x130, 0xE0, -0x73, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    for (i = 0; i < numTricks; i++) {
        if (tricks[i].type == 0x2E) {
            counts[tricks[i].slot]++;
        } else {
            lines++;
        }
    }
    for (i = 0; i < 8; i++) {
        if (counts[i] > 0) {
            lines++;
        }
    }
    for (i = 0; i < 8; i++) {
        if (counts[i] > 0) {
            if (line < lines) {
                func_00254D68(buf, "%s %d +", names[i], counts[i] * 360);
            } else {
                func_00254D68(buf, "%s %d", names[i], counts[i] * 360);
            }
            line++;
            func_0020F9A8(0, font, 0x28, y, buf);
            y += 0xA;
        }
    }
    for (i = 0; i < numTricks; i++) {
        if (tricks[i].type != 0x2E) {
            if (line < lines) {
                if (tricks[i].superStyled) {
                    func_00254D68(buf, "%s Super-Styled +", D_0026ED40[tricks[i].type]);
                } else {
                    func_00254D68(buf, "%s +", D_0026ED40[tricks[i].type]);
                }
            } else {
                if (tricks[i].superStyled) {
                    func_00254D68(buf, "%s Super-Styled", D_0026ED40[tricks[i].type]);
                } else {
                    func_00254D68(buf, "%s", D_0026ED40[tricks[i].type]);
                }
            }
            line++;
            func_0020F9A8(0, font, 0x28, y, buf);
            y += 0xA;
        }
    }
    y += 4;
    func_0020FB80(0, 0x160, 0x100, -0x7B, -0x20);
    func_00254D68(buf, "TOTAL PTS: %d", total);
    func_0020F9F0(0, font, x, y, buf);
    func_0018DC20(menu);
}

void func_00160980(MenuObj *p) {
    func_001B5AF0(&D_003F52D0, 1, 1);
    p->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x28, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x23, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x1A, &D_0029DE68, 1);
    p->x460 = 0xA5;
    func_0018DB08(p);
}

MenuObj *func_00160A10(MenuObj *obj) {
    func_0018D9D0((SharedMenu *)(obj));
    obj->x4F8 = D_00284C00;
    return obj;
}

static __inline__ void resetHud(void *hud)
{
    func_00175730((char *)hud + 0x150, -2);
}

static __inline__ Racer *getRider(struct World *world, int idx)
{
    return &world->racers[idx];
}

static __inline__ int isTimed(struct Config *cfg)
{
    return cfg->mode == 15 && cfg->x364 == 1;
}

void func_00160A48(struct CombatMenu *menu)
{
    char buf[0x20];
    int done[12];
    int width;
    int height;
    struct CombatMenu *saved;
    int x;
    int count;
    int font;
    int y;
    int i;
    int clear;
    int best;
    int bestIdx;
    int idx;
    int ok;
    int v;
    Racer *r;
    char *name;
    int val;

    count = D_002AF2C0.x501A8;
    font = D_003F52D0.font;
    saved = menu;
    for (clear = 0; clear < count; clear++) {
        done[clear] = 0;
    }
    if (D_004B25A0.x288 == 1) {
        resetHud(D_003B2270);
    }
    y = 0x28;
    func_0023D650(font, &width, &height);
    x = width / 2;
    func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
    func_0020FB80(0, 0x160, 0x100, -0x8B, -0x20);
    func_0020F9F0(0, font, x, 0xA, "COMBAT RESULTS");
    func_0020FB80(0, 0x100, 0xC0, -0x6B, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, D_003F52D0.font, 0x82, 0x1E, D_0029DE68->xC8);
    func_0020F9A8(0, D_003F52D0.font, 0xC8, 0x1E, D_0029DE68->x1D0);
    func_0020F9A8(0, D_003F52D0.font, 0x14A, 0x1E, "HITS");
    for (i = 0; i < count; i++) {
            bestIdx = -1;
            best = -1;
            for (idx = 0; idx < count; idx++) {
                Racer *rp = getRider(&D_002AF2C0, idx);
                if (done[idx] == 0) {
                    if (func_001301B8(rp) >= (float)best) {
                        bestIdx = idx;
                        best = (int)func_001301B8(rp);
                    }
                }
            }
            if (bestIdx != -1) {
                done[bestIdx] = 1;
                r = getRider(&D_002AF2C0, bestIdx);
                if (r->x7A5C == 1) {
                    func_0020F518(0, 0x58, 0x98, 0x49, 0xAA);
                } else {
                    func_0020F518(0, 0x50, 0x64, 0x50, 0xAA);
                }
                name = func_00158650(r->x796C);
                func_0020F9A8(0, font, 0x82, y, name);
                val = (int)func_001301B8(r);
                func_00254D68(buf, D_0029C770, val);
                func_0020F9A8(0, font, 0xE6, y, buf);
                func_00254D68(buf, D_0029C770, r->x79AC);
                func_0020F9A8(0, font, 0x158, y, buf);
                y += 0xA;
                ok = isTimed(&D_004B25A0);
                if (ok == 0) {
                    val = (int)func_001301B8(r);
                    if (D_004225A0.x12214 < val) {
                        D_004225A0.x12214 = val;
                    }
                }
            }
    }
    y += 4;
    v = 0;
    if (D_004B25A0.mode == 0xF) {
        v = (D_004B25A0.x364 == 1);
    }
    if (v == 0) {
        func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
        func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
        func_0020F9A8(0, D_003F52D0.font, 0x64, y, "COMBAT RECORD:");
        func_0020F518(0, 0x58, 0x98, 0x49, 0xAA);
        func_00254D68(buf, D_0029C770_vialui, D_004225A0.x12214);
        func_0020F9A8(0, font, 0x122, y, buf);
        y += 0xE;
    }
    if (D_004B25A0.x288 == 1) {
        v = 0;
        if (D_004B25A0.mode == 0xF) {
            v = (D_004B25A0.x364 == 1) ? D_004B25A0.x288 : 0;
        }
        if (v != 0) {
            func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
            func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
            func_0020F9A8(0, D_003F52D0.font, 0xCC, y, "TIME:");
            FormatHmsDiscard(saved->time, (char *)(buf));
            func_0020F518(0, 0x58, 0x98, 0x49, 0xAA);
            func_0020F9A8(0, font, 0x122, y, buf);
            y += 0xC;
            func_0020F518(0, 0xDD, 0xDB, 0xA, 0xAA);
            func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
            func_0020F9A8(0, D_003F52D0.font, 0x90, y, "BEST TIME:");
            func_0020F518(0, 0x58, 0x98, 0x49, 0xAA);
            FormatHmsDiscard(D_004225A0.x12210, (char *)(buf));
            func_0020F9A8(0, font, 0x122, y, buf);
        }
    }
    func_0018DC20(saved);
}
