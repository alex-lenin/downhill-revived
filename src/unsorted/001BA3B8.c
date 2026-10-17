/* UNIT 0x001BA3B8 0x001BB040 .rodata=0x28EEE0:0x28F090 .sdata=0x29D4A0:0x29D544 */
#include "include_asm.h"

/* The short strings of this unit (.sdata 0x29D4A0-0x29D544), each written as two word
   scalars: gcc aligns any array or string literal to 8, and the assembler then pads .sdata
   to a multiple of 8, overrunning 0x29D544 (retail ends on a 4-byte boundary). */
#define WORD4(a, b, c, d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)
#define STR8(name, a, b, c, d, e, f, g, h) \
    int name = WORD4(a, b, c, d); int name##_hi = WORD4(e, f, g, h)
#define S(name) ((char *)&name)

extern char D_004BD4DC[];
extern void func_00254D68(char *, char *, ...);

typedef struct {
    char x0[0x288];
    int x288;
    char x28C[0x14];
    int x2A0;
} TrackObj;

void func_001BA3B8(TrackObj *o, char *name) {
    /* incomplete type: addressed with lui/%lo like the original literals (defined below) */
    extern char str_ALP2[], str_ALPX2[], str_ALPT2[], str_AUBTDH2[], str_BC2[], str_BCTDX2[],
        str_CITY2[], str_FUJI2[], str_GLAC2P[], str_GLACX2P[], str_GLACT2P[], str_JUNGLE2[],
        str_MOAB2[], str_MOABMX2[], str_PERU2[], str_PERUMX2[], str_MOSH2[], str_fmt_s[];

    if (o->x288 >= 2) {
        switch (o->x2A0) {
        case 0:  func_00254D68(D_004BD4DC, str_ALP2, name); break;
        case 6:  func_00254D68(D_004BD4DC, str_ALPX2, name); break;
        case 24: func_00254D68(D_004BD4DC, str_ALPT2, name); break;
        case 11: func_00254D68(D_004BD4DC, "AUBERST2", name); break;
        case 15: func_00254D68(D_004BD4DC, "AUBERMX2", name); break;
        case 16: func_00254D68(D_004BD4DC, str_AUBTDH2, name); break;
        case 19: func_00254D68(D_004BD4DC, str_BC2, name); break;
        case 25: func_00254D68(D_004BD4DC, str_BCTDX2, name); break;
        case 20: func_00254D68(D_004BD4DC, str_CITY2, name); break;
        case 12: func_00254D68(D_004BD4DC, str_FUJI2, name); break;
        case 5:  func_00254D68(D_004BD4DC, str_GLAC2P, name); break;
        case 14: func_00254D68(D_004BD4DC, str_GLACX2P, name); break;
        case 22: func_00254D68(D_004BD4DC, str_GLACT2P, name); break;
        case 2:  func_00254D68(D_004BD4DC, str_JUNGLE2, name); break;
        case 10: func_00254D68(D_004BD4DC, "JUNGLET2", name); break;
        case 8:  func_00254D68(D_004BD4DC, "JUNGLEX2", name); break;
        case 1:  func_00254D68(D_004BD4DC, str_MOAB2, name); break;
        case 7:  func_00254D68(D_004BD4DC, str_MOABMX2, name); break;
        case 13: func_00254D68(D_004BD4DC, "MOABTDH2", name); break;
        case 3:  func_00254D68(D_004BD4DC, str_PERU2, name); break;
        case 9:  func_00254D68(D_004BD4DC, str_PERUMX2, name); break;
        case 4:  func_00254D68(D_004BD4DC, "PERUTDH2", name); break;
        case 23: func_00254D68(D_004BD4DC, str_MOSH2, name); break;
        default: func_00254D68(D_004BD4DC, str_fmt_s, name); break;
        }
    } else {
        func_00254D68(D_004BD4DC, str_fmt_s, name);
    }
}

STR8(str_ALP2, 'A', 'L', 'P', '2', 0, 0, 0, 0);
STR8(str_ALPX2, 'A', 'L', 'P', 'X', '2', 0, 0, 0);
STR8(str_ALPT2, 'A', 'L', 'P', 'T', '2', 0, 0, 0);
STR8(str_AUBTDH2, 'A', 'U', 'B', 'T', 'D', 'H', '2', 0);
STR8(str_BC2, 'B', 'C', '2', 0, 0, 0, 0, 0);
STR8(str_BCTDX2, 'B', 'C', 'T', 'D', 'X', '2', 0, 0);
STR8(str_CITY2, 'C', 'I', 'T', 'Y', '2', 0, 0, 0);
STR8(str_FUJI2, 'F', 'U', 'J', 'I', '2', 0, 0, 0);
STR8(str_GLAC2P, 'G', 'L', 'A', 'C', '2', 'P', 0, 0);
STR8(str_GLACX2P, 'G', 'L', 'A', 'C', 'X', '2', 'P', 0);
STR8(str_GLACT2P, 'G', 'L', 'A', 'C', 'T', '2', 'P', 0);
STR8(str_JUNGLE2, 'J', 'U', 'N', 'G', 'L', 'E', '2', 0);
STR8(str_MOAB2, 'M', 'O', 'A', 'B', '2', 0, 0, 0);
STR8(str_MOABMX2, 'M', 'O', 'A', 'B', 'M', 'X', '2', 0);
STR8(str_PERU2, 'P', 'E', 'R', 'U', '2', 0, 0, 0);
STR8(str_PERUMX2, 'P', 'E', 'R', 'U', 'M', 'X', '2', 0);
STR8(str_MOSH2, 'M', 'O', 'S', 'H', '2', 0, 0, 0);
STR8(str_fmt_s, '%', 's', 0, 0, 0, 0, 0, 0);
STR8(D_0029D530, 'B', 'I', 'K', 'E', 0, 0, 0, 0);
STR8(D_0029D538, 'm', 'c', 's', 't', 'a', 'r', 't', 0);
float unk_29D540 = 1.8f; /* unreferenced */

/* Menu hint strings emitted after the jump table in
   this translation unit (func_001BAF10 reads them). */
const char D_0028EFA8[0x30] __attribute__((aligned(8))) = "\220 PREV \235 NEXT           \255 SELECT  \247 BACK";
const char D_0028EFD8[0x30] __attribute__((aligned(8))) = "\215 PREV \217 NEXT   \240 INFO  \255 SELECT  \247 BACK";
const char D_0028F008[0x30] __attribute__((aligned(8))) = "\215 PREV \217 NEXT           \255 SELECT  \247 BACK";
const char D_0028F038[0x20] __attribute__((aligned(8))) = "\220 PREV \235 NEXT \255 SELECT \247 BACK";
const char D_0028F058[0x20] __attribute__((aligned(8))) = "\215 PREV \217 NEXT      \247 BACK";

typedef struct Obj6B8 { char pad0[0x118]; int x118; } Obj6B8;
int func_001BA6B8(Obj6B8 *obj) {
    int value = obj->x118;
    if (value != 1) {
        return value != 2 ? 0x54840 : 0x69840;
    }
    return 0xC8840;
}

extern char D_0029D480[];
extern char bike_type_name[] __asm__("D_0029D530");
struct Loader;
extern struct Loader D_004D0380;
extern char *func_001FA4B0(void *, int);
extern int strcmp(const char *, const char *);
extern void func_0020D078(void *, int);
extern void func_00230F80(void *, void *, void *, void *, int, unsigned char);

void func_001BA6F0(void *unused, int mode, void *selection, void *stream, char *type, int index)
{
    char *value = func_001FA4B0(selection, mode);

    if (mode == 3 && strcmp(type, D_0029D480) == 0) {
        func_0020D078(&D_004D0380, 1);
    }
    func_00230F80(stream, bike_type_name, value, type, 1, (unsigned char)index);
    if (mode == 3 && strcmp(type, D_0029D480) == 0) {
        func_0020D078(&D_004D0380, 2);
    }
}

typedef struct Obj {
    char pad0[0x10C];
    int x10C;
    char pad110[0x178];
    int x288;
    char pad28C[0xC8];
    int x354;
    char pad358[0x4];
    unsigned char x35C;
    char pad35D[0x13];
    int x370;
} Obj;

typedef struct Screen {
    char pad0[0x504];
    int x504;
    int x508;
} Screen;

typedef struct Menu3F52D0 { char pad0[0x130]; int x130; } Menu3F52D0;
typedef struct Unk43EEC0 { char pad0[0x488]; int x488; } Unk43EEC0;
typedef struct Unk43F9F0 { char pad0[0x44]; unsigned char x44; } Unk43F9F0;
typedef struct Track { char pad0[0x14]; unsigned char x14; } Track;
typedef struct World { char pad0[0x11A60]; Track x11A60; } World;
typedef struct Entry { char pad0[0x14]; int x14; } Entry;

extern Menu3F52D0 D_003F52D0;
extern Screen D_00435500;
extern Screen D_0038C100;
extern Unk43EEC0 D_0043EEC0;
extern Unk43F9F0 D_0043F9F0;
extern World D_004225A0;
extern Entry D_00381CF0[];
extern int D_0039E828[];
extern unsigned char D_0043FA34[];
extern int D_004B26B0[];
extern char D_00398C40[], D_0038ACE0[], D_00389890[], D_0038BC00[], D_00436E40[];
extern char D_00397690[], D_0043F870[], D_00397BA0[], D_003C6420[], D_004C3230[];
extern char D_004C4720[], D_0038C610[], D_003817D0[], D_0037F870[], D_00386B90[];
extern char D_0038D650[], D_0037FD70[], D_00387090[];

extern void func_00220550(int);
extern void func_0016C298(void *, int);
extern void func_00190108(void *, void *);
extern void func_0019EDE8(void *);
extern int func_00238F50(void);
extern int func_00238F48(void);
extern void func_00238BD0(int);
extern void func_00238BC8(int);
extern void func_001946E0(void *);
extern void func_00168198(void *);
extern void func_001A39D8(void *);
extern void func_001687F0(void *);
extern void func_001839A8(void *);
extern void func_001E1E98(void *);
extern void func_001A8A48(void *);
extern void func_001A3220(void *);
extern void func_001A2280(void *);
extern void func_0014F1F8(void *, int);
extern void func_001A40B8(void *);
extern void func_001615F0(void *);

static inline Track *get_track(World *w) { return &w->x11A60; }

void func_001BA7D0(Obj *obj, int mode)
{
    obj->x354 = 0;
    func_00220550(0);
    if (obj->x10C == 10) {
        func_0016C298(D_00398C40, get_track(&D_004225A0)->x14);
        obj->x35C = 0;
        obj->x354 = 1;
    } else if (obj->x10C == 15 && mode == 1) {
        func_00190108(&D_003F52D0, D_0038ACE0);
    } else if (mode == 2) {
        if (D_0043EEC0.x488 == 5) {
            func_0019EDE8(&D_0043EEC0);
        } else if (D_0039E828[0] != 0) {
            func_00190108(&D_003F52D0, D_00389890);
        } else if (obj->x10C == 14) {
            D_00435500.x508 = 0;
            func_00190108(&D_003F52D0, &D_00435500);
        } else {
            if (D_0043FA34[0] != 0) {
                int time = func_00238F50();

                D_0038C100.x504 = 0;
                D_0038C100.x508 = 0;
                func_00190108(&D_003F52D0, &D_0038C100);
                if (time < func_00238F48()) {
                    func_00238BD0(time);
                    func_00238BC8(time);
                }
            } else {
                int time = func_00238F50();

                obj->x370 = 1;
                func_00190108(&D_003F52D0, D_0038BC00);
                obj->x370 = 0;
                if (time < func_00238F48()) {
                    func_00238BD0(time);
                    func_00238BC8(time);
                }
            }
        }
    } else if (mode == 4) {
        if (obj->x10C == 14) {
            D_00435500.x508 = 0;
            func_00190108(&D_003F52D0, &D_00435500);
        } else {
            if (D_0043FA34[0] != 0) {
                int time = func_00238F50();

                D_0038C100.x504 = 1;
                D_0038C100.x508 = 1;
                func_00190108(&D_003F52D0, &D_0038C100);
                if (time < func_00238F48()) {
                    func_00238BD0(time);
                    func_00238BC8(time);
                }
            } else {
                int time = func_00238F50();

                obj->x370 = 1;
                func_00190108(&D_003F52D0, D_0038BC00);
                obj->x370 = 0;
                if (time < func_00238F48()) {
                    func_00238BD0(time);
                    func_00238BC8(time);
                }
            }
        }
    } else if (mode == 1) {
        if (obj->x10C != 14 && obj->x10C != 18 && obj->x10C != 17 && obj->x10C != 11
            && obj->x10C != 19 && obj->x10C != 15 && obj->x10C != 20) {
            func_001946E0(&D_004225A0);
        }
        if (obj->x10C == 14) {
            func_00190108(&D_003F52D0, D_00436E40);
        } else if (obj->x10C == 5) {
            func_00168198(D_00397690);
            func_00190108(&D_003F52D0, D_00397690);
        } else if (obj->x10C == 13) {
            func_001A39D8(D_0043F870);
            func_001687F0(D_00397BA0);
            func_00190108(&D_003F52D0, D_00397BA0);
        } else if (obj->x10C == 17) {
            func_001839A8(D_003C6420);
            func_00190108(&D_003F52D0, D_003C6420);
        } else if (obj->x10C == 18) {
            func_001E1E98(D_004C3230);
            func_00190108(&D_003F52D0, D_004C3230);
        } else if (obj->x10C == 11) {
            func_00190108(&D_003F52D0, D_004C4720);
        } else if (obj->x10C == 19) {
            func_00190108(&D_003F52D0, D_0038C610);
        } else if (obj->x288 == 1) {
            if (D_0043F9F0.x44 != 0) {
                func_001A8A48(&D_0043F9F0);
            } else {
                func_001A3220(D_0043F870);
                func_001A2280(D_0043F870);
            }
            if (D_004B26B0[0] == 1) {
                func_0014F1F8(D_003817D0, (D_00381CF0 + obj->x10C)->x14);
                func_00190108(&D_003F52D0, D_003817D0);
                func_00190108(&D_003F52D0, D_0037F870);
            } else {
                func_00190108(&D_003F52D0, D_00386B90);
            }
        } else if (obj->x288 == 2) {
            D_003F52D0.x130 = 0;
            if (obj->x10C == 16) {
                func_001A40B8(D_0043F870);
                func_001615F0(D_0038D650);
                func_00190108(&D_003F52D0, D_0038D650);
            } else {
                func_001A3220(D_0043F870);
                func_001A2280(D_0043F870);
                if (D_004B26B0[0] == 1) {
                    func_00190108(&D_003F52D0, D_0037FD70);
                } else {
                    func_00190108(&D_003F52D0, D_00387090);
                }
            }
        } else {
            func_001A3220(D_0043F870);
            func_001A2280(D_0043F870);
            func_00190108(&D_003F52D0, D_00387090);
        }
    } else if (mode == 5) {
        D_00435500.x508 = 1;
        func_00190108(&D_003F52D0, &D_00435500);
    } else if (mode == 6) {
        D_0038C100.x504 = 1;
        D_0038C100.x508 = 0;
        func_00190108(&D_003F52D0, &D_0038C100);
    }
    func_00220550(1);
}

typedef struct Loader {
    char pad0[0x1684];
    void *x1684;
} Loader;


extern Loader D_004D0380, D_004D1A18, D_004D30B0, D_004D4748;
extern char D_004D0368[], D_00663B08[], D_003C6370[];
extern char D_0029CCF8[], D_0029D480[], D_0029D468[], D_0029D478[];
extern int D_0029DD68;

extern void func_001B9B70(void *);
extern void func_001B9E88(void *);
extern void func_0020F058(int, int);
extern void func_00223B00(void *);
extern void func_002079B0(void *);
extern void func_0020D0A0(Loader *);
extern void func_0020D290(Loader *, char *, char *, char *, int, int);
extern void func_0020B088(int, char *, int);
extern int func_0020D170(Loader *, int);
extern void func_00218E88(int, int);
extern void func_00218D98(int);
extern void func_00158680(void *, int, int);
extern void func_00238BD8(void);
extern void func_00182CF8(void *, int);
extern void func_001B5AF0(void *, int, int);
extern void func_00188000(void);

void func_001BAD48(void *arg)
{
    int value;

    func_001B9B70(arg);
    func_001B9E88(arg);
    func_0020F058(0x69840, 1);
    func_00223B00(D_004D0368);
    func_002079B0(D_00663B08);
    func_0020D0A0(&D_004D0380);
    func_0020D0A0(&D_004D1A18);
    func_0020D0A0(&D_004D30B0);
    func_0020D0A0(&D_004D4748);
    D_004D0380.x1684 = D_004D0368;
    D_004D1A18.x1684 = D_004D0368;
    D_004D30B0.x1684 = D_004D0368;
    D_004D4748.x1684 = D_004D0368;
    func_0020D290(&D_004D0380, D_0029CCF8, S(D_0029D538), D_0029D480, 0, 0);
    func_0020B088(0x69840, D_0029CCF8, 0);
    func_0020D290(&D_004D30B0, D_0029CCF8, S(D_0029D538), D_0029D468, 0, 0);
    func_00218E88(0x69840, func_0020D170(&D_004D30B0, 0));
    func_0020D290(&D_004D1A18, D_0029CCF8, S(D_0029D538), D_0029D478, 0, 0);
    func_00218D98(0x69840);
    value = func_0020D170(&D_004D0380, 0);
    func_00158680(&D_0029DD68, value, 1);
    func_00238BD8();
    func_00182CF8(D_003C6370, 0);
    func_001B5AF0(&D_003F52D0, 8, 1);
    func_00188000();
}

typedef struct Hud { char pad[0x150]; int x150, x154; float x158, x15c; } Hud;
extern Hud D_004C6560;
extern char D_004B25A0[];
extern void func_0020F518(int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,void *);
extern void func_001BB040(void *,int);
void func_001BAF10(void *obj, int index) {
 char *names[5] = { D_0028EFA8,D_0028EFD8,D_0028F008,D_0028F038,D_0028F058 };
 func_0020F518(0,56,56,56,255);
 func_0020FB80(0,349,166,-128,-16);
 if(index < 3) {
  func_0020F9A8(0,0,25,199,index[names]);
  func_001BB040(D_004B25A0,1);
 } else {
  D_004C6560.x150 = 256;
  D_004C6560.x154 = 177;
  D_004C6560.x158 = -88.0f;
  D_004C6560.x15c = -44.12f;
  func_0020FB80(0,256,160,-80,-32);
  func_0020F9A8(0,0,25,204,index[names]);
 }
}
