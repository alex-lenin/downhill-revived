/* UNIT 0x00139BF8 0x0013B5E8 .sdata=0x29C468:0x29C46C .sbss=0x29DD00:0x29DD10 */
#include "load_transition.h"
#include "menu_items.h"
#include "menu.h"
#include "timer.h"
#include "include_asm.h"
__asm__(".extern D_0029C46C, 4");
__asm__(".extern D_0029C470, 1");
__asm__(".extern D_0029C471, 1");
__asm__(".extern D_0029C488, 8");

/* one-time init flags of func_00139BF8, func_0013A578 (two) and func_0013B8D0 */
int D_0029DD00 __attribute__((section(".sbss"))) = 0;
int D_0029DD04 __attribute__((section(".sbss"))) = 0;
int D_0029DD08 __attribute__((section(".sbss"))) = 0;
int D_0029DD0C __attribute__((section(".sbss"))) = 0;

int D_0029C468 = 0;

struct Entry {
    int x0;
    int x4;
    int x8;
};

typedef struct Vec3i {
    int x;
    int y;
    int z;
} Vec3i;

struct Obj {
    char pad0[0x10C];
    int x10C;
    char pad110[0x118 - 0x110];
    int x118;
    char pad11C[0x1B8 - 0x11C];
    struct Entry x1B8;
    char pad1C4[0x254 - 0x1C4];
    int x254;
    char pad258[0x38];
    int count;
    char pad294[0x374 - 0x294];
    char x374[0xA8];
    char pad41C[0x4F0 - 0x41C];
    char x4F0[0x524];
};

typedef struct State {
    int index;
    char pad4[0x505];
    unsigned char x509;
    char pad50A[0x62];
    unsigned char x56C;
    char pad56D[7];
    struct Entry *x574;
    unsigned char x578[1];
} State;

typedef struct Dest {
    char pad0[0xC];
    Vec3i xC;
} Dest;

struct Sub {
    char pad0[0x40];
};

struct Game {
    char pad0[0x78D8];
    struct Sub x78D8;
};

typedef struct { int x0; short x4; char x6, x7; int x8, xC; } Sub830;
typedef struct { char pad[0x4F8]; void *x4F8; int x4FC, x500; Sub830 sub; int x514, x518, x51C, x520; } Obj830;

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
} Obj361680;

typedef struct Font {
    char pad0[0xA0];
} Font;

struct Player {
    char pad0[0x524];
    int x524;
};

extern struct Obj D_004B25A0;
extern int D_00365DE0[];
extern struct Entry *D_00365D74[];
extern struct Entry D_0036B6A0;
extern struct Entry D_0036B6B0;
extern Dest D_0036B730;
extern struct Game D_002AF2C0;
extern int D_0036ABD0[];
extern State D_00365800;
extern int D_0036B380[];
extern int D_0029C46C;
extern char D_0036B338[];
extern char D_00284190[];
extern unsigned int *D_0029E4A4;
extern unsigned char D_003F52D0[];
extern Obj361680 D_00361680;
extern Font D_004C8508[];
extern int func_0013AF60(int);
extern int D_0026E2B0[];
typedef struct { unsigned char x0, x1, x2, x3; } Rec2F8;
extern Rec2F8 D_0026E2F8[];
typedef struct { char pad0[0x2188]; int x2188; } GState325A0;
extern GState325A0 D_004325A0;
extern unsigned char D_0029C470;
extern unsigned char D_0029C471;
typedef struct { char pad0[4]; unsigned char x4; } Flagged;
extern Flagged *D_0029DD34;
extern int D_0036B3E0[];
typedef struct Sound { int x0, x4, x8, xC; } Sound;
extern Sound D_003613F0;
extern char D_003642D0[];
extern char D_00364800[];
extern void func_0021E428(int);
extern void func_001C50E0(void *);
extern void func_001C7300(void *);
extern void func_0013B330(void);
extern void func_0013A578(void);
extern void func_0013A270(void);
extern void func_0013CDB0(void *);
extern int func_001FA7C0(int);
extern void func_0013FE50(int *);
extern void func_002193E8(void);
extern void func_001414A0(void *, void *, void *);
extern void func_001F7A60(int);
extern void func_001F79D8(int, int);
extern int func_00238F48(void);
extern void func_00238BC8(int);
extern void func_00203A80(void *, int, float);
extern void func_002039B0(void *, int);
extern void func_002038E0(void *, int);
extern void func_00228BE0(int);
extern void func_00190108(void *, void *);
extern void func_00141130(void *);
extern void func_001C5180(void *);
extern void func_001C72A0(void *);
extern void func_00219438(void);

extern int func_0013A258(void);
extern unsigned char func_00141128(Dest *);
extern void func_001B9AE8(void *, int, int, int, int);
extern void func_00140C18(void *, void *, void *);
extern void func_001B97E0(void *, int, int, int, int);
extern void func_001FABE8(void *, void *);
extern void func_001B9890(void *, int, int);
extern void func_001B9860(struct Obj *s, int a, int *x, int *y, int *z);
extern void func_0013D888(void *p, Vec3i *v);
extern void func_00140C68(void *);
extern void func_00140450(void *);

extern void func_001B5AF0(void *, int, int);
extern void func_00203590(void *, int, int, int);
extern void func_00204080(void *, int);
extern void func_00202E38(void *, int, int, float);
extern void func_00218498(unsigned int *, int);
extern int func_001F9B98(Font *);
extern int func_001F9BF0(Font *);
extern void func_0013D750(void *, int, int);
extern char D_00663B08[];
extern char D_00759780[];
extern char D_0029C478[];
extern char D_0029C480[];

extern void func_001B4AF8(void *);
extern void func_001B5A38(void *);
extern void func_001B4DD0(void *);
extern void func_001B5130(void *);
extern void func_00223B00(void *);
extern void func_00218038(void);
extern void func_0020E860(void);
extern void func_00228BE8(int);
extern void func_002079B0(void *);
extern void func_002079C8(void *);
extern void func_001FACD8(struct Sub *, int);

static inline void SetFont(void *obj, Font *fonts, int index) {
    Font *font = &fonts[index];
    func_0013D750(obj, func_001F9B98(font), func_001F9BF0(font));
}

void func_00139BF8(int arg0) {
    int saved118;
    Sound snd;
    int t4, t8;
    struct Entry *srcEntry;
    struct Entry *p1B8;
    struct Entry *sel;

    D_0029C470 = 1;
    D_0029C471 = arg0;
    func_0021E428(0);
    saved118 = D_004B25A0.x118;
    D_004B25A0.x118 = 2;
    func_001C50E0(&D_004B25A0.x4F0);
    func_001C7300(&D_004B25A0.x374);
    if (arg0 != 0) {
        func_0013B330();
    }
    D_0036B6A0.x0 = D_004B25A0.count / 32 - 1;
    D_0036B6A0.x4 = 0;
    D_0036B6A0.x8 = 0;
    func_0013A578();
    if (arg0 != 0) {
        p1B8 = &D_004B25A0.x1B8;
        t4 = p1B8->x4;
        t8 = p1B8->x8;
        D_0036B6A0 = D_004B25A0.x1B8;
        D_00365DE0[0] = t4 + t8 + (t4 != 0);
        func_0013A270();
    } else {
        func_0013CDB0(&D_00365800);
    }
    sel = &D_0036B6B0;
    srcEntry = &D_00365800.x574[func_0013A258()];
    *sel = *srcEntry;
    D_0036ABD0[0] = func_001FA7C0(sel->x0);
    func_0013FE50(D_0036ABD0);
    func_002193E8();
    D_0036B3E0[0] = 0;
    func_001414A0(&D_0036B730, &D_0036B6A0, sel);
    func_001B5AF0(D_003F52D0, 2, 1);
    if (arg0 != 0) {
        func_001B5AF0(D_003F52D0, 8, 1);
    }
    func_001F7A60(0x80);
    func_001F79D8(0, 0x3C);
    func_00238BC8(func_00238F48() + 1);
    snd.x8 = 0;
    func_00203590(&snd, 0x2582, 0, D_0029C471 + 3);
    func_00203A80(&snd, 0, 1.0f);
    func_002039B0(&snd, 0);
    if (!D_0029DD00) {
        D_003613F0.x8 = 0;
        D_0029DD00 = 1;
    }
    func_00203590(&D_003613F0, 0x2588, 0, 1);
    func_00204080(&D_003613F0, 1);
    func_002038E0(&D_003613F0, 1);
    func_00228BE0(0);
    if (D_0029C471 != 0) {
        func_00190108(D_003F52D0, D_00364800);
    } else {
        func_00190108(D_003F52D0, D_003642D0);
    }
    D_0029C470 = 0;
    if (D_003F52D0[0x12C] != 0) {
        func_00228BE0(1);
        if ((Flagged *)D_0029C468 != 0) {
            ((Flagged *)D_0029C468)->x4 = 0;
        }
        if ((Flagged *)D_0029C46C != 0) {
            ((Flagged *)D_0029C46C)->x4 = 0;
        }
        if (D_0029DD34 != 0) {
            D_0029DD34->x4 = 0;
        }
        func_00218498((unsigned int *)func_0013AF60(0x2584), 0);
        func_00141130(&D_0036B730);
    }
    func_001C5180(&D_004B25A0.x4F0);
    if (D_003F52D0[0x12C] != 0) {
        func_001C72A0(&D_004B25A0.x374);
    }
    func_00219438();
    D_004B25A0.x118 = saved118;
}

int func_00139F50(void) {
    int n = D_00365DE0[0];
    struct Entry *entry = D_00365D74[0] + func_0013A258();
    struct Entry *range;
    struct Entry *sel;

    if (func_00141128(&D_0036B730)) {
        if (D_0036B6A0.x4 != (n - 1 > -1 ? n - 1 : 0)) goto differs;
        if (D_0036B6A0.x8 != (n == 1)) goto differs;
        if (entry->x4 != D_0036B6B0.x4) goto differs;
        if (entry->x0 != D_0036B6B0.x0) goto differs;
        if (entry->x8 != D_0036B6B0.x8) goto differs;
    } else {
        range = (struct Entry *)&D_0036B730;
        if (range->x4 != (n - 1 > -1 ? n - 1 : 0)) goto differs;
        if (range->x8 != (n == 1)) goto differs;
        sel = (struct Entry *)&D_0036B730.xC;
        if (entry->x4 != sel->x4) goto differs;
        if (entry->x0 != sel->x0) goto differs;
        if (entry->x8 != sel->x8) goto differs;
    }
    return 0;
differs:
    return 1;
}

int func_0013A088(void) {
    int n = D_00365DE0[0] - 1;
    struct Entry *entry;
    struct Entry *range = &D_0036B6A0;
    struct Entry *sel = &D_0036B6B0;

    D_0036B6A0.x0 = D_004B25A0.count / 32 - 1;
    D_0036B6A0.x8 = D_00365DE0[0] == 1;
    D_0036B6A0.x4 = n > -1 ? n : 0;
    func_001B9AE8(&D_004B25A0, D_004B25A0.count, 0, D_0036B6A0.x4, D_0036B6A0.x8);
    entry = &D_00365D74[0][func_0013A258()];
    D_0036B6B0 = *entry;
    func_00140C18(&D_0036B730, range, sel);
    return 0;
}

void func_0013A170(void) {
    struct Obj *obj = &D_004B25A0;
    struct Entry *range = &D_0036B6A0;
    struct Game *game;
    struct Entry *entry;
    int n = D_00365DE0[0] - 1;

    range->x0 = obj->count / 32 - 1;
    range->x4 = n > -1 ? n : 0;
    range->x8 = D_00365DE0[0] == 1;
    func_001B9AE8(obj, obj->count, 0, range->x4, range->x8);
    entry = &D_00365D74[0][func_0013A258()];
    func_001B97E0(obj, 0, entry->x0, entry->x4, entry->x8);
    game = &D_002AF2C0;
    func_001FABE8(&game->x78D8, entry);
    func_001B9890(obj, 0, D_0036ABD0[0]);
}

int func_0013A258(void)
{
    return D_00365800.x578[D_00365800.index];
}

void func_0013A270(void)
{
    Vec3i v;

    func_001B9860(&D_004B25A0, 0, &v.x, &v.y, &v.z);
    func_0013D888(&D_00365800, &v);
    D_0036ABD0[0] = D_004B25A0.x254;
    D_0036B730.xC = v;
}

/* Preserve the already-decompiled function when promoting this whole unit. */
void func_0013A2E8(void)
{
    int *shifts;
    int i;
    struct Entry *items;
    int count;
    int id;
    int k;

    shifts = D_0026E2B0;
    i = 0;
    do {
        unsigned int *obj;
        int bit;

        obj = (unsigned int *)func_0013AF60(i + 0x25F9);
        i++;
        bit = 1 << *shifts;
        shifts++;
        func_00218498(obj, (D_004325A0.x2188 & bit) != 0);
    } while (i < 0x11);

    i = 0;
    do {
        int found = 0;
        int next = i + 1;
        items = (struct Entry *)func_001F9B98(&D_004C8508[D_0026E2F8[i].x0]);
        count = func_001F9BF0(&D_004C8508[D_0026E2F8[i].x0]);
        id = i + 0x2617;
        k = 0;
        if (count > 0) {
            do {
                if (items[k].x0 == D_0026E2F8[i].x1 && items[k].x4 == D_0026E2F8[i].x2 && items[k].x8 == D_0026E2F8[i].x3) {
                    found = 1;
                    break;
                }
                k++;
            } while (k < count);
        }
        i = next;
        do {
            func_00218498((unsigned int *)func_0013AF60(id), found);
        } while (0);
    } while (i < 0xE);
}

void func_0013A480(void) {
    int width = D_004B25A0.count;
    int mode = D_004B25A0.x10C;
    int wide = 0;

    if (width == 0xE0 || width == 0x100 || width == 0x140 || width == 0x180 || width == 0x1A0) {
        wide = 1;
    }
    if (mode == 4 && !wide) {
        SetFont(&D_00365800, D_004C8508, 4);
    } else {
        SetFont(&D_00365800, D_004C8508, 3);
    }
}

/* --- func_0013A578 types --- */
typedef struct { void *x0; int x4; void *x8; } A578_E270;
typedef struct { void *x0; int x4; } A578_E330;
typedef struct { char x[0x2C]; } A578_Item58;
typedef struct { char pad[0x418]; int x418; } A578_ObjA;
typedef struct {
    char pad0[0xC]; void *xC; void *x10;
    char pad14[0x460 - 0x14]; int x460; int x464;
    char pad468[0x530 - 0x468]; unsigned char x530;
} A578_ObjB;
typedef struct { short delta; short pad; void (*fn)(void *); } A578_VEntry;
typedef struct { char pad[0x70]; A578_VEntry x70; } A578_Vtbl;
typedef struct {
    char pad0[0x10]; void *x10;
    char pad14[0x460 - 0x14]; int x460; int x464;
    char pad468[0x4BC - 0x468]; unsigned char x4BC;
    char pad4BD[0x4F8 - 0x4BD]; A578_Vtbl *x4F8;
} A578_ObjC;
typedef struct {
    void *x0; int x4; void *x8; unsigned char xC; char padD[3];
    char x10[0x10]; void *x20; char pad24[0xC]; int x30; char pad34[8];
} A578_E61400;
typedef struct { void *x0; unsigned char x4; char pad5[3]; int x8; void *xC; } A578_Sub615F0;
typedef struct {
    void *x0; int x4; int x8; unsigned char xC; char padD[3];
    A578_Sub615F0 sub;
} A578_E615E0;
typedef struct { char pad[0x88]; void *x88; void *x8C; char pad90[0x1C8 - 0x90]; void *x1C8; } A578_ResDD30;
typedef struct { char pad[0x514]; int x514; int x518; int x51C; int x520; } A578_Obj514;
typedef struct { char pad[0x460]; int x460; int x464; } A578_Obj6A3D0;
typedef struct { int x0; char pad4[0x40]; int x44; } A578_Obj65790;

typedef union { struct {void *x0; int x4; int x8; unsigned char xC;} h; A578_Sub615F0 sub; } A578_Block;
typedef struct { char pad[0x509]; unsigned char x509; char pad50A[0x26]; unsigned char x530; } A578_Flags;
extern char D_003623A0[];
extern A578_E270 D_0026E270[];
extern A578_Item58 D_0036B180[10];
extern A578_ResDD30 *D_0029DD30;
extern char D_0036B100[];
extern char D_0036B0E0[];
extern char D_0036B120[];
extern char D_0036B160[];
extern A578_E61400 D_00361400[];
extern char A578_614Bytes[] __asm__("D_00361400");
extern char D_00284170[];
extern char D_00366800[];
extern char D_003674F0[];
extern char D_003681E0[];
extern char D_00368ED0[];
extern char D_00369410[];
extern char D_00369950[];
extern char D_00369E90[];
extern char D_0043F350[];
extern char D_003662F0[];
extern A578_E615E0 D_003615E0[];
extern char D_003615F0[];
extern char D_002872F0[];
extern A578_Obj514 A578_D_003642D0 __asm__("D_003642D0");
extern A578_Obj514 A578_D_00364800 __asm__("D_00364800");
extern A578_Obj514 D_00364D30;
extern A578_Obj514 D_00365260;
typedef struct { char x[0x10]; } A578_E10;
extern A578_E10 D_00277EC0[];
extern A578_E330 D_0026E330[];
extern A578_Obj6A3D0 D_0036A3D0;
extern int D_00365790[];

extern void func_002083E8(void *);
extern void func_0013C9D0(void *);
extern void func_0013CA48(void *, int, void *);
extern void func_0018E4F0(void *);
extern void func_0018E720(void *);
extern void func_0013C828(void *, void *);
extern void func_0013DA80(void *, void *, int, int, int, int, int, int);
extern void func_0018E4F8(void *, void *);
extern void *func_00254204(void *, int, unsigned int);
extern void func_0018E398(void *);
extern void func_0018FD70(void *, void *);
extern void func_0018FDB0(void *, int);
extern void func_0018DFA8(void *, void *);
extern void func_0013D5A8(void *);
extern void func_0013CAF0(void *, int, int, int);
extern void func_0018E408(void *, void *, int);
extern int func_001F9D58(int);
extern void func_0013DC78(void *, int, int);
extern void func_0018EA60(void *, void *);
extern void func_0013EC38(void *);
extern void func_0013FC98(void *);
extern int func_001FA7C0(int);


static inline void A578_SubInit(A578_Sub615F0 *p) {
    p->x0 = D_002872F0;
    p->x4 = 0;
    p->x8 = 0;
    p->xC = 0;
}

void func_0013A578(void)
{
    int j;
    int k;
    int wide;
    int width;
    int *list;
    int id;
    int cnt;
    int next, prev, following;
    A578_Item58 *item, *back;
    int i;
    A578_E270 *e;
    A578_ObjB *obj2;
    A578_E61400 *p614;
    A578_E61400 *p614end;
    void *q10;
    A578_E615E0 *p615;
    A578_Sub615F0 *psub;
    char *sp;

    func_002083E8(D_003623A0);
    i = 0;
    do {
        e = &D_0026E270[i];
        func_0013C9D0(e->x0);
        func_0013CA48(e->x0, D_0026E270[i].x4, &D_0029DD30);
        ((A578_ObjA *)e->x0)->x418 = 0;
        func_0018E4F0(e->x0);
        obj2 = (A578_ObjB *)D_0026E270[i].x8;
        if (obj2 != 0) {
            obj2->x530 = 0;
            func_0018E720(obj2);
            { int size = 0x6E; int page = 0x19;
              obj2->x464 = size;
              obj2->x460 = page;
            }
            func_0013C828(e->x0, obj2);
            obj2->xC = D_0036B0E0;
            obj2->x10 = D_0036B100;
        }
        following = i + 1;
        next = following % 5;
        prev = (i + 4) % 5;
        item = &D_0036B180[i * 2];
        back = &D_0036B180[i * 2 + 1];
        func_0013DA80(item, D_0026E270[next].x0, 0x2582, 0, i + 2, 0, 0, 3);
        func_0018E4F8(e->x0, item);
        func_0013DA80(back, D_0026E270[prev].x0, 0x2582, 0, ((i + 4) % 5) + 2, 1, 0, 2);
        func_0018E4F8(e->x0, back);
        i = following;
    } while (i < 5);

    if (D_0029DD04 == 0) {
        A578_E61400 *p614;
        p614 = D_00361400;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00368ED0;
        p614->x4 = 0xD;
        p614->x8 = &D_00365800;
        p614->xC = 1;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00368ED0;
        p614->x4 = 0xB;
        p614->x8 = (void *)D_00365DE0;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00369410;
        p614->x8 = D_00366800;
        p614->x4 = 7;
        p614->xC = 1;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00369410;
        p614->x4 = 8;
        p614->x8 = D_003674F0;
        p614->xC = 1;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00369410;
        p614->x4 = 9;
        p614->x8 = D_003681E0;
        p614->xC = 1;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00369E90;
        p614->x4 = 0x24;
        p614->x8 = D_0043F350;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00368ED0;
        p614->x4 = 0x3E;
        p614->x8 = (void *)D_0036ABD0;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        p614++;
        func_00254204(p614, 0, 0x3C);
        p614->x0 = D_00369950;
        p614->x4 = 0x48;
        p614->x8 = D_003662F0;
        p614->x20 = D_00284170;
        p614->x30 = 0;
        D_0029DD04 = 1;
    }

    {
        void **pp;
        int n;
        A578_E61400 *base;

        base = (A578_E61400 *)A578_614Bytes;
        pp = &base[0].x8;
        for (n = 7; n >= 0; n--) {
            void (*fn)(void *);
            int delta;
            A578_ObjC *o;
            A578_Vtbl *v;

            o = (A578_ObjC *)*pp;
            v = o->x4F8;
            delta = v->x70.delta;
            fn = v->x70.fn;
            fn((char *)o + delta);
            func_0018E398((A578_ObjC *)*pp);
            ((A578_ObjC *)*pp)->x10 = D_0036B120;
            pp = (void **)((char *)pp + 0x3C);
        }
    }

    if (D_0029DD08 == 0) {
        A578_E615E0 *p615;
        A578_Sub615F0 *psub;
        p615 = D_003615E0;
        func_00254204(p615, 0, 0x20);
        p615->x0 = D_00366800;
        p615->x4 = 0x2595;
        A578_SubInit(&p615->sub);

        p615++;
        func_00254204(p615, 0, 0x20);
        p615->x4 = 0x2596;
        p615->x0 = D_003674F0;
        A578_SubInit(&p615->sub);

        p615++;
        func_00254204(p615, 0, 0x20);
        p615->x4 = 0x2597;
        p615->x0 = D_003681E0;
        A578_SubInit(&p615->sub);

        p615++;
        func_00254204(p615, 0, 0x20);
        p615->x0 = D_003662F0;
        p615->x4 = 0x25F8;
        A578_SubInit(&p615->sub);

        p615++;
        func_00254204(p615, 0, 0x20);
        p615->x0 = D_003662F0;
        p615->x4 = 0x2616;
        A578_SubInit(&p615->sub);

        D_0029DD08 = 1;
    }

    {
    int address = (int)D_003615F0;
    for (k = 4; k >= 0; k--) {
        func_0018FD70((void *)address, (void *)func_0013AF60(*(int *)(address - 12)));
        ((A578_Sub615F0 *)address)->x8 = *(int *)(address - 8);
        func_0018FDB0((void *)address, *(unsigned char *)(address - 4));
        func_0018DFA8(*(void **)(address - 16), (void *)address);
        address += 32;
    }
    }

    {
        int thirdSecond = 4;
        A578_D_003642D0.x514 = 1;
        j = 5;
        A578_D_003642D0.x518 = j;
        A578_D_003642D0.x51C = 6;
        A578_D_00364800.x514 = 2;
        A578_D_00364800.x518 = 3;
        A578_D_00364800.x51C = thirdSecond;
    }
    D_00364D30.x520 = 0x1E;
    D_00365260.x520 = 0x1F;
    func_0013A480();
    func_0013D5A8(&D_00365800);
    D_00365800.x56C = 0;
    { int middle = 0x25D1;
    func_0013CAF0(&D_00365800, 0x25D0, middle, 0x25D2); }

    wide = 0;
    width = D_004B25A0.count;
    if (width == 0xE0 || width == 0x100 || width == 0x140 || width == 0x180 || width == 0x1A0) {
        wide = 1;
    }
    D_00365DE0[0x140] = (int)func_0013A088;
    if (wide != 0) {
        func_0018E408(D_00365DE0, D_0029DD30->x1C8, 1);
    } else {
        func_0018E408(D_00365DE0, D_0029DD30->x88, 1);
    }
    list = D_00365DE0;
    list[4] = (int)D_0036B160;
    if (wide == 0) {
        func_0018E408(list, D_0029DD30->x8C, width != 0x200);
        for (j = 1; j < 0x12; j++) {
            int *dst = D_00365DE0;
            int bit = 1 << j;
            int en = 0;
            int ok;
            if (D_004325A0.x2188 & bit) {
                ok = !wide;
                width = ok + 1;
                en = ok;
            }
            func_0018E408(dst, &D_00277EC0[j], en);
        }
    }

    {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        cnt = func_001F9D58(D_0026E330[i].x4);
        ((int *)D_0026E330[i].x0)[0x510 / 4] = D_0026E330[i].x4;
        for (j = 0; j < cnt; j++) {
            func_0013DC78(D_0026E330[i].x0, j, 1);
        }
    }
    }

    for (i = 0; i < 8; i++) {
        p614 = &D_00361400[i];
        func_0013DA80(p614->x10, p614->x8, 0, 0, 0, 0, 1, 0xFF);
        func_0018EA60(p614->x0, p614->x8);
        func_0018E3A8((struct SharedMenu *)(void *)(p614->x0), p614->x4, &D_0029DD30, 1);
        if (p614->xC != 0) {
            ((A578_ObjC *)p614->x8)->x4BC = 1;
            ((A578_ObjC *)p614->x8)->x464 = 0x262;
        } else {
            ((A578_ObjC *)p614->x8)->x464 = 0x1CC;
        }
        ((A578_ObjC *)p614->x8)->x460 = 0xAA;
    }

    func_0018E3A8((struct SharedMenu *)(void *)(D_003662F0), 0x49, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(D_003662F0), 0x4A, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(D_003662F0), 0x4B, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(D_003662F0), 0x4C, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(D_003662F0), 0x4D, &D_0029DD30, 1);
    func_0013A2E8();
    func_0018E398(&D_0036A3D0);
    func_0013EC38(&D_0036A3D0);
    func_0013C828(&D_00364D30, &D_0036A3D0);
    D_0036A3D0.x464 = 0x23;
    D_0036A3D0.x460 = 0x46;
    D_00365790[0] = 0;
    ((A578_Obj65790 *)D_00365790)->x44 = 0;
    func_0013FC98(D_0036ABD0);
    D_0036ABD0[4] = (int)D_0036B160;
    D_0036ABD0[0] = func_001FA7C0(D_0036B6B0.x0);
    func_0013FE50(D_0036ABD0);
    D_00365800.x509 = 0;
    ((A578_Flags *)D_00365DE0)->x509 = 0;
    ((A578_Flags *)D_003662F0)->x509 = 0;
    ((A578_Flags *)D_00369E90)->x530 = 0;
    ((A578_Flags *)D_0036ABD0)->x509 = 0;
    ((A578_Flags *)D_0043F350)->x509 = 0;
    func_00218498((unsigned int *)func_0013AF60(0x259D), D_004B25A0.count / 32 - 1);
}

int func_0013AF60(int id)
{
    return D_0036B380[id - 0x2580];
}

void func_0013AF80(void) {
    D_0029C468 = 0;
    D_0029C46C = 0;
    func_00140C68(&D_0036B730);
    func_00140450(D_0036B338);
}

void func_0013AFB8(void) {
    char buf[0x70];
    struct Game *game;
    int i;

    D_0029C468 = 0;
    D_0029C46C = 0;
    func_00140C68(&D_0036B730);
    InitializeLoadTransitionDiscard(buf);
    func_001B4AF8(buf);
    func_001B5A38(buf);
    func_001B4DD0(buf);
    func_001B5130(buf);
    for (i = 198; i >= 0; i--) {
        D_0036B380[i] = 0;
    }
    func_00223B00(D_00663B08);
    func_00218038();
    func_0020E860();
    func_00236A30((TimerPool *)(D_00759780), 100);
    func_00228BE8(0);
    func_002079B0(D_00663B08);
    func_002079C8(D_0029C478);
    func_002079C8(D_0029C480);
    game = &D_002AF2C0;
    func_001FACD8(&game->x78D8, 0);
}

extern void func_0013B8B0(unsigned int *);
typedef struct Vector { float x, y, z, w; } Vector __attribute__((aligned(16)));
extern Vector D_0026E350[], D_0026E360[];
extern int D_004B26B8[], D_0036B378[];
extern float D_0029C488;
extern int func_00209108(void);
extern int func_00208FB8(void);
extern void func_00218278(void *, unsigned int *);
extern void func_00222040(void *, Vector *);
extern void func_00140678(void *, unsigned int *);
static inline void ApplyVector(Vector *out, void *mat, Vector *v) {
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)
"
        "lqc2 $vf12, 0x0(%2)
"
        "lqc2 $vf13, 0x10(%2)
"
        "lqc2 $vf14, 0x20(%2)
"
        "vmulax.xyz ACC, $vf12, $vf11x
"
        "vmadday.xyz ACC, $vf13, $vf11y
"
        "vmaddz.xyz $vf11, $vf14, $vf11z
"
        "sqc2 $vf11, %0" : "=m"(*out) : "r"(v), "r"(mat) );
}
static inline void AddVector(Vector *out, Vector *a, Vector *b) {
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)
"
        "lqc2 $vf12, 0x0(%2)
"
        "vadd.xyz $vf11, $vf11, $vf12
"
        "sqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b) );
}
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_0013B0A8(unsigned int *p) {
    Vector v;
    void *body;
    int id = *p >> 18;
    if ((unsigned int)(id-0x20)<0x3C9 && D_004B26B8[0]==2) {
        if (((id/32)*32 & 0x7FE0) == id-16) {
            if (!D_0029C468) D_0029C468=func_00209108();
            func_00218278((void *)D_0029C468,p);
            v.z=D_0029C488*0.017453292f; v.x=0; v.y=0;
            func_00222040((void *)(D_0029C468+0x40),&v);
            ApplyVector(&v,(void *)(D_0029C468+0x40),D_0026E350);
            AddVector((Vector *)(D_0029C468+0x10),D_0026E360,&v);
        } else if ((*p & 0xFFFC0000)==0x0E100000) {
            if (!D_0029C46C) D_0029C46C=func_00208FB8();
            func_00140678(D_0036B338,p);
            body=(void *)D_0029C46C;
            func_00218278((void *)body,p);
            COPY_VECTOR(*(Vector *)(D_0029C46C+0x10),D_0026E360);
            v.z=D_0029C488*0.017453292f; v.x=0; v.y=0;
            func_00222040((void *)(D_0029C46C+0x40),&v);
        }
    } else if ((unsigned int)(id-0x2580)<200) {
        D_0036B380[id-0x2580]=(int)p;
        if (id==0x25EE) {
            D_0029DD34=(void *)func_00209108();
            func_00218278(D_0029DD34,p);
            v.z=D_0029C488*0.017453292f; v.x=0; v.y=0;
            func_00222040((char *)D_0029DD34+0x40,&v);
            ApplyVector(&v,(char *)D_0029DD34+0x40,D_0026E350);
            AddVector((Vector *)((char *)D_0029DD34+0x10),D_0026E360,&v);
        }
        if ((unsigned int)(id-0x2634)<11) D_0036B378[0]=id-0x2634;
    }
    func_0013B8B0(p);
}




void func_0013B328(void) {
}

typedef struct {int x0;int x4;int x8;} InitArena;
typedef struct {char pad[0x1684];InitArena *x1684;} InitObj;
extern InitArena D_004D0368;
extern InitObj D_004D0380,D_004D1A18,D_004D30B0,D_004D4748;
extern char D_003C6370[],D_00289A58[],D_00289A68[],D_0029C490[],D_0029C498[],D_0029C4A0[],D_0029C4A8[],D_0029C4B0[];
extern int D_0029D9D4,D_0029DD68;
extern int func_00139BE0(void);
extern void func_00182838(void *,void *,int,int,float);
extern void func_0021E428(int);
extern void func_0020D0A0(void *);
extern void func_0020F058(int,int);
extern void func_0020F240(void);
extern int func_0020D290(void *,void *,void *,void *,int,int);
extern int func_0020D170(void *,int);
extern int func_00218E88(int,int);
extern void func_00218D98(int);
extern void func_0020B228(int,int,int);
extern int func_00158680(void *,int,int);
extern void func_00182920(void *);
extern void func_0023D6C8(void);
extern void func_0023D610(int);
extern void func_0023C8D8(int,int);
extern void func_0023CB00(int);
extern void func_0023D898(int,float *,float *);
extern void func_0023D8D8(int,float,float);
static inline void InitPush(InitArena *arena) {
 int start=(arena->x4+3)&~3;int prev=arena->x8;
 arena->x8=start;arena->x4=start+4;*(int *)start=prev;
}
static inline int *InitAlloc(InitArena *arena,int size) {int start=(arena->x4+3)&~3;arena->x4=start+size;return (int *)start;}
static inline void InitPushFirst(InitArena *arena) {int prev=arena->x8;int start=(arena->x4+3)&~3;arena->x8=start;arena->x4=start+4;*(int *)start=prev;}
static inline void InitAttach(int handle,int id) {func_00218E88(id,handle);}
static inline void InitSize(int id,float height) {func_0023D8D8(id,688.0f,height);}
static inline void InitPop(InitArena *arena) {arena->x4=arena->x8;arena->x8=*(int *)arena->x8;}
void func_0013B330(void) {
 float coords[2];int id=func_00139BE0();
 func_00140450(D_0036B338);func_0013AFB8();
 func_00182838(D_003C6370,D_00289A58,12,99,288.0f);func_0021E428(0);
 func_00223B00(&D_004D0368);
 func_0020D0A0(&D_004D0380);func_0020D0A0(&D_004D1A18);func_0020D0A0(&D_004D30B0);func_0020D0A0(&D_004D4748);
 func_0020F058(id,1);func_0020F240();
 D_004D0380.x1684=&D_004D0368;
 func_0020D290(&D_004D0380,D_0029C490,D_00289A68,D_0029C498,1,0);
 {int prev=D_004D0368.x8; int *start=InitAlloc(&D_004D0368,4); D_004D0368.x8=(int)start; *start=prev;}
 D_004D30B0.x1684=&D_004D0368;D_004D1A18.x1684=&D_004D0368;
 func_0020D290(&D_004D30B0,D_0029C490,D_00289A68,D_0029C4A0,1,0);
 InitAttach(func_0020D170(&D_004D30B0,0),id);InitPop(&D_004D0368);
 func_0020D290(&D_004D1A18,D_0029C490,D_00289A68,D_0029C4A8,1,0);func_00218D98(id);
 InitPush((InitArena *)D_00663B08);D_004D4748.x1684=(InitArena *)D_00663B08;
 func_0020D290(&D_004D4748,D_0029C490,D_00289A68,D_0029C4B0,1,0);
 func_0020B228(func_0020D170(&D_004D4748,0),id,0);InitPop((InitArena *)D_00663B08);
 func_00158680(&D_0029DD68,D_0029D9D4,0);func_00182920(D_003C6370);
 func_0023D6C8();func_0023D610(1);func_0023C8D8(5,0);func_0023CB00(0);
 func_0023D898(0,&coords[0],&coords[1]);InitSize(0,coords[1]);
}
