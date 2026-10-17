/* UNIT 0x00187438 0x0018A828 .sdata=0x29CD98:0x29CDD8 .sbss=0x29E4C0:0x29E4C4 */
#include "menu_refresh.h"
#include "menu_items.h"
#include "include_asm.h"

/* One translation unit: D_0029CDA0 is $gp-accessed (outside delay slots) by
   func_00187438 and func_001882C8, so it is defined before both. */

typedef struct Rider {
    char pad0[0x418];
    int x418;
    char pad41C[0x44C - 0x41C];
    short x44C;
    char pad44E[0x4B8 - 0x44E];
    unsigned char x4B8;
    unsigned char x4B9;
    char pad4BA[0x4F8 - 0x4BA];
    void *vtbl;
    char pad4FC[0x600 - 0x4FC];
    int x600;
    char pad604[0x610 - 0x604];
    unsigned char x610;
} Rider;

typedef struct State {
    int x0;
    int x4;
    char x8;
    char x9;
    unsigned char xA;char padB;
    int xC;
    int x10;
    char pad14[0x18 - 0x14];
    int x18;
    char pad1C[0x20 - 0x1C];
    unsigned char x20;
    char pad21[0x81 - 0x21];
    char x81[0x20];
    char xA1[0x20];
    char padC1[0xD4 - 0xC1];
    unsigned char xD4;
    char padD5[0xE8 - 0xD5];
    char xE8;
} State;

typedef struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x150 - 0x110];
    Rider *x150;
    struct Link *x154[12];
    int x184;
} Game;

typedef struct Strings {
    char pad0[0xD4];
    char *xD4;
    char *xD8;
    char *xDC;
    char padE0[0x108 - 0xE0];
    char *x108;
} Strings;

/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry {
    short delta;
    short pad2;
    void (*func)(void *);
} VEntry;

typedef struct Player {
    int x0;
    char pad4[0x460 - 0x4];
    int x460;
    int x464;
    char pad468[0x4F8 - 0x468];
    VEntry *vtbl;
} Player;

typedef struct PlayerHolder {
    char pad0[0x10];
    Player player;
} PlayerHolder;

typedef struct Menu {
    char pad0[0x610];
    unsigned char x610;
    char pad611[3];
    int x614;
    int x618;
} Menu;

typedef struct Pair {
    int x0;
    char *x4;
} Pair;

typedef struct Msg {
    int x0;
    char *x4;
    void *x8;
    char xC;
} Msg;

int D_0029CD98 = 0x30C00;
int D_0029CD9C = 0;
unsigned char D_0029CDA0 = 0;
unsigned long D_0029CDA8 = 0;
char D_0029CDB0[8] __attribute__((aligned(8))) = "yes";
char D_0029CDB8[8] __attribute__((aligned(8))) = "no";
char D_0029CDC0[8] __attribute__((aligned(8))) = " at ";
char D_0029CDC8[8] __attribute__((aligned(8))) = "EMPTY";
int D_0029CDD0 = 1;
unsigned char D_0029CDD4 = 0;
unsigned char D_0029CDD5 = 0;
short pad_CDD6 = 0;
extern char D_003E80B0[], D_003E3D70[];
extern void func_002065C0(void);

extern State D_003EB760;
extern PlayerHolder D_003EB140;
extern Game D_003F52D0;
extern Strings *D_0029DE30[2];
extern int D_0029E4C0;
extern Rider *D_003F5420[];
extern char D_003E3BD0[];
extern char D_00286070[];
extern char D_002861C0[];
extern unsigned char D_003F53E0[];
extern char D_004B25A0[];
extern char D_0026F330[];
extern char D_0026F340[];
extern char D_0026F350[];
extern char D_003E86D0[];
extern char D_003E55D0[];
extern char D_003E4390[];
extern char D_003E49B0[];
extern char D_003E6E50[];
extern char D_003E92F0[];
extern char D_0043E280[];
extern char D_0043E8B0[];
extern char D_003E6210[];
extern char D_003E4FC0[];
extern char D_003E9900[];
extern char D_003EA520[];
extern Menu D_003E6830;
extern Menu D_003E5BF0;

extern void func_001907B0(void *p);
extern void func_001907D0(void *p, int arg);
extern void func_00190358(void *p);
extern void func_00190378(void *p);
extern void func_001906A8(void *p, void *q);
extern void func_00187068(void *p);
extern void func_00184888(void *p, int arg);
extern void func_00187048(void *p, void *msg);
extern void func_001870E0(void *p, State *state);
extern int func_001875D0(Rider *obj);
extern void func_0018E098(Rider *obj);
extern void func_0018E5A8(Rider *obj);
extern void func_0018E1C0(Rider *obj);
extern void func_0018DB08(Rider *obj);
extern void func_001BB040(void *p, int arg);
extern void func_001B5AF0(void *p, int a, int b);
extern char *func_00254544(char *dst, const char *src);
extern void func_001879A8(void);
extern void func_001881C0(Rider *obj);

extern void func_0018DB20(Player *p);
extern void func_001873F8(Player *p);
extern int func_0021F818(void);
extern void func_00187780(char *obj);
extern void func_00187898(void);

void func_00187438(Rider *obj)
{
    Pair pair;
    int saved;

    if (D_003EB760.x0 == 0) {
        func_001907B0(&D_003F52D0);
        func_00187068(D_003E3BD0);
        if (!D_0029CDA0) {
            pair.x0 = 0;
            pair.x4 = D_00286070;
            saved = D_003EB760.x10;
            SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760), 0, 0);
            func_00187048(D_003E3BD0, &pair);
            func_001870E0(D_003E3BD0, &D_003EB760);
            SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760), saved, 0);
            func_001875D0(obj);
        }
    }
}

int func_00187500(Rider *obj)
{
    unsigned char *state = (unsigned char *)&D_003EB760;

    if (state[0xD4] || state[0xE8]) {
        return func_001875D0(obj);
    }
    return 1;
}

void func_00187548(Rider *obj) {
    Rider *current = D_003F52D0.x150;
    if (current == obj) {
        func_0018E098(current);
        func_0018E5A8(current);
        if (current->x44C) func_001BB040(D_004B25A0, 3);
        D_003F52D0.x10C = 15;
        func_00190358(&D_003F52D0);
        func_0018E1C0(current);
        D_003EB760.xD4 = 0;
        D_003EB760.xE8 = 0;
    }
}

typedef struct Link { int x0; Rider *x4; } Link;
extern Player D_003EAB30;
extern void func_00190660(Game *, Player *);
extern void func_00190740(Game *, int);
int func_001875D0(Rider *obj)
{
    int i, j, count;

    if (D_003EB760.x0) {
        D_003EAB30.x0 = -1;
        D_003EAB30.x464 = -1;
        func_0018DB20(&D_003EAB30);
        func_00190660(&D_003F52D0, &D_003EAB30);
        return 0;
    }
    count = D_003F52D0.x184;
    for (i = 0; i < count; i++) {
        Rider *r = D_003F52D0.x154[i]->x4;
        if (r == obj) return 1;
        if (r) {
            VEntry *v = r->vtbl;
            if (((int (*)(void *))v[13].func)((char *)r + v[13].delta)) {
                func_00190740(&D_003F52D0, i);
                for (j = i; j < count; j++) {
                    D_003F52D0.x154[j]->x4->x4B9 = 0;
                }
                return 0;
            }
        }
    }
    return 1;
}

void func_00187718(Rider *obj) {
    obj->x600 = obj->x418;
}

void func_00187728(Rider *obj)
{
    func_0018DB08(obj);
    RefreshGlobalMenuLegacy(obj);
}

void func_00187758(void)
{
    func_001907D0(&D_003F52D0, 0);
}

void func_00187780(char *obj) {
    func_00184888(obj, 0);
    func_00254544(obj + 0x21, D_0029DE30[0]->xDC);
    func_00254544(obj + 0x41, D_0029DE30[0]->xDC);
    func_00254544(obj + 0x61, D_0029DE30[0]->xDC);
    func_00254544(obj + 0x81, D_0029DE30[0]->xD4);
    func_00254544(obj + 0xA1, D_0026F330);
}

void func_00187808(void) {
 Pair pair; int a,b;
 pair.x0=0; pair.x4=D_00286070;
 b=D_003EB760.x10; a=D_003EB760.xC;
 SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760),0,0);
 func_00187048(D_003E3BD0,&pair);
 func_001870E0(D_003E3BD0,&D_003EB760);
 D_003EB760.x10=b; D_003EB760.xC=a;
}

void func_00187898(void)
{
    PlayerHolder *holder = &D_003EB140;
    Player *player = &holder->player;
    VEntry *entry;

    player->x0 = -1;
    func_0018DB20(player);
    func_001873F8(player);
    player->x464 = -1;
    entry = &player->vtbl[14];
    entry->func((char *)player + entry->delta);
    player->x460 = 90;
    func_001873C0((struct SharedMenu *)(void *)(player), 37, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 12, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), func_0021F818() + 16, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 14, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 12, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 15, 0);
    func_00187780((char *)&D_003EB760);
    func_00184888(&D_003EB760, 0);
    D_003EB760.x4 = 2;
    D_003EB760.x20 = 1;
}

void func_001879A8(void)
{
    PlayerHolder *holder = &D_003EB140;
    Player *player = &holder->player;
    VEntry *entry;

    player->x0 = -1;
    func_0018DB20(player);
    func_001873F8(player);
    player->x464 = -1;
    entry = &player->vtbl[14];
    entry->func((char *)player + entry->delta);
    player->x460 = 90;
    func_001873C0((struct SharedMenu *)(void *)(player), 11, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 12, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), func_0021F818() + 16, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 14, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 12, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 15, 0);
    func_00187780((char *)&D_003EB760);
    func_00184888(&D_003EB760, 0);
    D_003EB760.x4 = 1;
    D_003EB760.x20 = 0;
}

void func_00187AB0(void) {
    D_003EB760.x8 = 0;
    D_003EB760.x9 = 0;
    func_00187898();
    D_003EB760.x18 = 0x44C00;
    func_001B5AF0(&D_003F52D0, 1, 1);
    func_00190378(&D_003F52D0);
    func_001906A8(&D_003F52D0, D_003E55D0);
    func_001906A8(&D_003F52D0, D_003E4390);
    func_001906A8(&D_003F52D0, D_003E6210);
    func_001906A8(&D_003F52D0, &D_003E6830);
    func_001906A8(&D_003F52D0, &D_003E5BF0);
    func_001906A8(&D_003F52D0, D_003E4FC0);
    func_001907B0(&D_003F52D0);
    D_003E5BF0.x614 = 3;
    D_003E6830.x614 = 0x30;
    D_003E6830.x618 = 0x31;
}

void func_00187BA8(void) {
    D_003EB760.x8 = 1;
    D_003EB760.x9 = 0;
    func_001879A8();
    D_0029E4C0 = 1;
    func_001B5AF0(&D_003F52D0, 1, 1);
    func_00190378(&D_003F52D0);
    func_001906A8(&D_003F52D0, D_003E86D0);
    func_001906A8(&D_003F52D0, D_003E55D0);
    func_001906A8(&D_003F52D0, D_003E4390);
    func_001906A8(&D_003F52D0, D_003E49B0);
    func_001907B0(&D_003F52D0);
}

void func_00187C50(void) {
    D_003EB760.x8 = 0;
    D_003EB760.x9 = 0;
    func_00187898();
    func_00254544(D_003EB760.x81, D_0029DE30[0]->xD8);
    func_00254544(D_003EB760.xA1, D_0026F340);
    D_0029E4C0 = 0;
    func_001B5AF0(&D_003F52D0, 1, 1);
    func_00190378(&D_003F52D0);
    func_001906A8(&D_003F52D0, D_003E55D0);
    func_001906A8(&D_003F52D0, D_003E6E50);
    func_001906A8(&D_003F52D0, D_003E6210);
    func_001906A8(&D_003F52D0, &D_003E6830);
    func_001906A8(&D_003F52D0, &D_003E5BF0);
    func_001906A8(&D_003F52D0, D_003E9900);
    func_001907B0(&D_003F52D0);
    D_003E5BF0.x614 = 4;
    D_003E6830.x614 = 0x49;
    D_003E6830.x618 = 0x4A;
}

void func_00187D60(void) {
    D_003EB760.x8 = 1;
    D_003EB760.x9 = 0;
    func_001879A8();
    func_00254544(D_003EB760.x81, D_0029DE30[0]->xD8);
    func_00254544(D_003EB760.xA1, D_0026F340);
    D_0029E4C0 = 0;
    func_001B5AF0(&D_003F52D0, 1, 1);
    func_00190378(&D_003F52D0);
    func_001906A8(&D_003F52D0, D_003E55D0);
    func_001906A8(&D_003F52D0, D_003E6E50);
    func_001906A8(&D_003F52D0, D_003E92F0);
    func_001907B0(&D_003F52D0);
}

void func_00187E18(void)
{
    D_003EB760.x8 = 0;
    D_003EB760.x9 = 0;
    func_00187898();
    D_003EB760.x18 = D_0029CD98;
    func_00254544(D_003EB760.x81, D_0029DE30[0]->x108);
    func_00254544(D_003EB760.xA1, D_0026F350);
    D_0029E4C0 = 0;
    func_00190378(&D_003F52D0);
    func_001906A8(&D_003F52D0, D_003E55D0);
    func_001906A8(&D_003F52D0, D_0043E280);
    func_001B5AF0(&D_003F52D0, 1, 1);
    func_001906A8(&D_003F52D0, D_003E6210);
    func_001906A8(&D_003F52D0, &D_003E6830);
    func_001906A8(&D_003F52D0, &D_003E5BF0);
    func_001906A8(&D_003F52D0, D_003EA520);
    func_001907B0(&D_003F52D0);
    D_003E5BF0.x614 = 5;
    D_003E6830.x614 = 0x47;
    D_003E6830.x618 = 0x48;
    func_001B5AF0(&D_003F52D0, 5, 1);
}

void func_00187F48(void)
{
    D_003EB760.x8 = 1;
    D_003EB760.x9 = 0;
    func_001879A8();
    func_00254544(D_003EB760.x81, D_0029DE30[0]->x108);
    func_00254544(D_003EB760.xA1, D_0026F350);
    D_0029E4C0 = 0;
    func_00190378(&D_003F52D0);
    func_001906A8(&D_003F52D0, D_003E55D0);
    func_001906A8(&D_003F52D0, D_0043E280);
    func_001906A8(&D_003F52D0, D_0043E8B0);
    func_001907B0(&D_003F52D0);
    func_001B5AF0(&D_003F52D0, 5, 1);
}

/* defined after use: the assembler does not know their size at the points of use */
Strings *D_0029DE30[2];
int D_0029E4C0;

void func_00188000(void)
{
 PlayerHolder *holder;Player *player;VEntry *entry;
 if(D_0029CDD0) {
 D_0029CDD0=0;
 D_003EB760.x8=1;D_003EB760.x9=0;
 func_00187780((char*)&D_003EB760);
 func_00184888(&D_003EB760,0);
 D_003EB760.x18=0x44c00;D_003EB760.x4=1;D_003EB760.x20=0;
 D_0029E4C0=1;
 func_001B5AF0(&D_003F52D0,1,1);
 func_00190378(&D_003F52D0);
 func_001906A8(&D_003F52D0,D_003E80B0);
 func_001906A8(&D_003F52D0,D_003E3D70);
 func_001906A8(&D_003F52D0,D_003E4390);
 func_001906A8(&D_003F52D0,D_003E49B0);
 func_001907B0(&D_003F52D0);
 holder=&D_003EB140;player=&holder->player;
 player->x0=-1;
 func_0018DB20(player);func_001873F8(player);
 player->x464=-1;entry=&player->vtbl[14];
 entry->func((char*)player+entry->delta);
 player->x460=90;
 func_001873C0((struct SharedMenu *)(void *)(player),11,0);func_001873C0((struct SharedMenu *)(void *)(player),12,0);
 func_001873C0((struct SharedMenu *)(void *)(player),func_0021F818()+16,0);
 func_001873C0((struct SharedMenu *)(void *)(player),0,0);func_001873C0((struct SharedMenu *)(void *)(player),14,0);
 func_001873C0((struct SharedMenu *)(void *)(player),12,0);func_001873C0((struct SharedMenu *)(void *)(player),15,0);
 D_003EB760.xA=1;func_002065C0();func_00190188(&D_003F52D0);
 }
}

void func_001881C0(Rider *obj)
{
    Msg a;
    Msg b;
    Msg *msg;
    int saved;

    if (D_0029CDA0) return;
    if (D_003F53E0[0]) return;
    if (--D_0029CD9C > 0) return;
    a.x0 = 0;
    a.x4 = D_00286070;
    b.x0 = 0;
    b.x4 = D_002861C0;
    b.x8 = obj;
    D_0029CD9C = 30;
    saved = D_003EB760.x10;
    msg = &b;
    msg->xC = 1;
    SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760), 0, 0);
    func_00187048(D_003E3BD0, msg);
    func_00187048(D_003E3BD0, &a);
    D_0029CDA0 = 1;
    func_001870E0(D_003E3BD0, &D_003EB760);
    D_0029CDA0 = 0;
    SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760), saved, 0);
}

void func_001882C0(Rider *s) {
    s->x610 = 0;
}

void func_001882C8(Rider *obj) {
 obj->x4B8 = 1;
 if (!obj->x610) {
  func_001881C0(obj);
  obj->x610 = 1;
  if (D_003F5420[0] != obj) return;
 }
 if (!D_0029CDA0) func_00187438(obj);
}

/* ---- merged from src/unsorted/00188338.c (type renamed; func_00187728 from above) ---- */
typedef struct Obj_88338 {
    char pad0[0x610];
    char x610;
} Obj_88338;

void func_00188338(Obj_88338 *obj)
{
    obj->x610 = 1;
    func_00187728((Rider *)obj);
}

typedef struct {char pad[0x460]; int x460; char pad464[0x44]; int x4A8; char pad4AC[0xC]; unsigned char x4B8; char pad4B9[0x157]; unsigned char x610;} Obj;
typedef struct {unsigned char x0; char pad1[3]; int x4,x8; char padC[8];} Item;
typedef struct {char pad[0x18]; int x18; char pad1C[5]; char x21[0xA3]; Item xC4[1];} Global;

extern unsigned char D_003EB834[16];
extern void *func_00184F98(void *);
extern int func_001851F8(void *,char *);
extern void func_0018E6B8(void *,int,void *);
static __inline__ Item *get_item(Global *g,int i) {return (Item *)((char *)g+(i*20+0xC4));}
void func_00188358(Obj *obj) {
 char buffer[0xC90];
 int i;
 Item *item;

 if(!obj->x610) {
  func_001881C0((Rider *)obj);
  if(!func_00187500((Rider *)obj) || D_003F5420[0]!=obj)return;
  if(D_003EB834[0])obj->x610=1;
 }
 obj->x460=90;obj->x4B8=1;
 if(obj->x610 && !D_0029CDA0) {
  D_0029CDD4=0;D_0029CDD5=0;obj->x610=0;
  for(i=0;i<=0 && (item=get_item(&(*(Global *)&D_003EB760),i))->x0;i++) {
   if(item->x4==2) {
    D_0029CDD4=1;
    if((*(Global *)&D_003EB760).x18 < (item->x8<<10))D_0029CDD5=1;
    func_00184F98(buffer);
    SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&(*(Global *)&D_003EB760)),0,0);
    func_00187048(D_003E3BD0,buffer);
    func_001870E0(D_003E3BD0,&(*(Global *)&D_003EB760));
    if(func_001851F8(buffer,(*(Global *)&D_003EB760).x21)!=-1)D_0029CDD5=1;
   }
   if(D_0029CDD4 && D_0029CDD5) {func_00187438((Rider *)obj);return;}

  }
 }
 func_001873F8(obj);
 if(!D_0029CDD4) {func_001873C0((struct SharedMenu *)(void *)(obj),12,0);func_001873C0((struct SharedMenu *)(void *)(obj),1,0);}
 else {func_001873C0((struct SharedMenu *)(void *)(obj),9,0);func_001873C0((struct SharedMenu *)(void *)(obj),57,0);}
 func_001873C0((struct SharedMenu *)(void *)(obj),func_0021F818()+16,0);
 func_001873C0((struct SharedMenu *)(void *)(obj),0,0);func_001873C0((struct SharedMenu *)(void *)(obj),2,0);func_001873C0((struct SharedMenu *)(void *)(obj),3,0);func_001873C0((struct SharedMenu *)(void *)(obj),6,0);
 func_0018E6B8(obj,26,D_0029DE30);obj->x4A8=5;func_00187548((Rider *)obj);
}

/* ---- merged from src/unsorted/001885D8.c (verbatim) ---- */
int func_001885D8(void) {
    return 0;
}

/* ---- merged from src/unsorted/001885E0.c (type renamed; void callee needs no arg) ---- */
struct Obj_885E0 {
    char pad0[0x610];
    unsigned char x610;
};

void func_001885E0(struct Obj_885E0 *obj)
{
    obj->x610 = 1;
    func_00187808();
}

/* ---- merged from src/unsorted/00188600.c (D_003F52D0 via base Game decl) ---- */
typedef unsigned int U128 __attribute__((mode(TI)));
void func_00188600(void *o){func_001907D0(&D_003F52D0,0); *(U128 *)((char *)o+0x490)=(U128)0x5443454c45535f53ULL;}

/* ---- merged from src/unsorted/00188658.c (types renamed; shared globals via base decls) ---- */
typedef struct St_88658 { int x0; char pad4[0x1d]; char x21[0x5f]; } St_88658;
typedef struct Obj_88658 { char pad[0x460]; int x460; } Obj_88658;
typedef struct Set_88658 { char pad[0xf4]; int xf4; } Set_88658;
typedef struct Menu_88658 { char pad[0x70]; int x70; } Menu_88658;
extern char D_0029CDD8[],D_0032FC60[],D_004225A0[];
extern void *func_00184F98(void *);
extern int func_001851F8(void *,char *);
extern void *func_00184A78(void *);
extern void func_00184AA0(void *,char *,void *,int);
extern int func_00254D68(char *,char *,...);
extern void func_00194AB8(void *,void *);
extern void func_00194B50(void *,void *);
extern void func_00193B20(void *);
static __inline__ void *getBuffer(void *p) { return (char *)p+0x10; }
void func_00188658(Obj_88658 *obj) {
 char archive[0xc90]; char buffer[0x12240]; int reader[5]; char name[80]; St_88658 *state=(St_88658 *)&D_003EB760;
 if(!state->x0 || func_001875D0((Rider *)obj)) {
 obj->x460=90; func_00187548((Rider *)obj);
 func_00184F98(archive); SetMenuSelectionLegacy((MenuSelectionState *)(void *)((State *)state),0,0);
 func_00187048(D_003E3BD0,archive); func_001870E0(D_003E3BD0,(State *)state);
 if(func_001851F8(archive,state->x21)<0) { func_00187438((Rider *)obj); }
 else {
 func_00184A78(reader); func_00254D68(name,D_0029CDD8,state->x21,state->x21);
 func_00184AA0(reader,name,buffer,0x12240);
 func_00187048(D_003E3BD0,reader); func_00187048(D_003E3BD0,&D_003EB140); func_001870E0(D_003E3BD0,(State *)state);
 func_00254D68(name,D_0029CDD8,state->x21,((Set_88658 *)D_0029DE30[0])->xf4);
 func_00184AA0(reader,name,getBuffer(D_0032FC60),0x2bf10);
 func_00187048(D_003E3BD0,reader); func_00187048(D_003E3BD0,&D_003EB140);
 ((Menu_88658 *)D_003E3BD0)->x70=30; func_001870E0(D_003E3BD0,(State *)state);
 if(!state->x0) { func_00194AB8(buffer,D_004225A0); func_00194B50(D_004225A0,buffer); func_00193B20(D_004225A0); func_00187438((Rider *)obj); }
 }
 }
}

/* ---- merged from src/unsorted/00188888.c (verbatim) ---- */
int func_00188888(void) {
    return 0;
}

/* ---- merged from src/unsorted/00188890.c (types renamed; shared globals via decls above) ---- */
typedef struct St_88890 {
    int x0;
    char pad4[0x1d];
    char x21[0x60];
} St_88890;

typedef struct Obj_88890 {
    char pad0[0x460];
    int x460;
} Obj_88890;

typedef struct Set_88890 { char pad0[0xf4]; int xF4; } Set_88890;
typedef struct Object_88890 { long words[0x12240 / 8]; } Object_88890;

/* task objects queued to D_003E3BD0; vtbl at +4 */
typedef struct CheckTask {
    int x0;
    void *vtbl;
    char pad8[0xC88 - 8];
    int xC88;
    char *xC8C;
    int xC90;
    unsigned char xC94;
} CheckTask;

typedef struct BaseTask {
    int x0;
    void *vtbl;
    char pad8[0xC88 - 8];
    int xC88;
    char *xC8C;
} BaseTask;

typedef struct FileTask {
    int x0;
    void *vtbl;
    int x8;
    int xC;
    int x10;
} FileTask;

typedef struct MsgTask {
    int x0;
    void *vtbl;
    char *x8;
    unsigned char xC;
} MsgTask;

typedef struct LoadTask {
    int x0;
    void *vtbl;
    int x8;
    int xC;
    int x10;
} LoadTask;

union Work {
    CheckTask check;
    BaseTask base;
    FileTask file;
    struct {
        MsgTask msg;
        LoadTask load;
    } pair;
};

extern char D_00285FC8[];
extern char D_00286000[];
extern char D_003EB781[];

extern void func_001852A8(void *, char *);
extern void func_00184D20(void *);
extern void func_00184D48(void *, char *, void *, int);

void func_00188890(Obj_88890 *obj)
{
    char archive[0xC90];
    char name[0x50];
    Object_88890 buffer;
    union Work w;
    Object_88890 buf2;

    if (!D_003EB760.x0 || func_001875D0((Rider *)obj)) {
        CheckTask *chk;
        obj->x460 = 90;
        func_00187548((Rider *)obj);
        func_00184F98(archive);
        func_00187048(D_003E3BD0, archive);
        func_001870E0(D_003E3BD0, &D_003EB760);
        func_00254D68(name, D_0029CDD8, ((St_88890 *)&D_003EB760)->x21, ((St_88890 *)&D_003EB760)->x21);
        func_00194B50(&buffer, D_004225A0);
        if (func_001851F8(archive, ((St_88890 *)&D_003EB760)->x21) >= 0) {
            chk = &w.check;
            func_001852A8(chk, ((St_88890 *)&D_003EB760)->x21);
            func_00187048(D_003E3BD0, &D_003EB140);
            func_00187048(D_003E3BD0, chk);
            func_001870E0(D_003E3BD0, &D_003EB760);
            if (chk->xC94 != 0) goto load_file;
        }
        {
            BaseTask *base = &w.base;
            base->vtbl = D_00285FC8;
            base->xC8C = ((St_88890 *)&D_003EB760)->x21;
            base->x0 = 0;
            func_00187048(D_003E3BD0, &D_003EB140);
            func_00187048(D_003E3BD0, base);
            func_001870E0(D_003E3BD0, &D_003EB760);
        }
        goto load_done;
load_file:
        {
            FileTask *file = &w.file;
            func_00184A78(file);
            func_00184AA0(file, name, &buf2, 0x12240);
            func_00187048(D_003E3BD0, file);
            func_00187048(D_003E3BD0, &D_003EB140);
            func_001870E0(D_003E3BD0, &D_003EB760);
            func_00194AB8(&buffer, &buf2);
        }
load_done:
        {
            LoadTask *load;
            MsgTask *msg = &w.pair.msg;
            msg->x0 = 0;
            msg->x8 = ((St_88890 *)&D_003EB760)->x21;
            msg->xC = 0;
            msg->vtbl = D_00286000;
            func_00187048(D_003E3BD0, &D_003EB140);
            func_00187048(D_003E3BD0, msg);
            func_001870E0(D_003E3BD0, &D_003EB760);
            load = (LoadTask *)&archive[0x12F30];
            func_00184D20(load);
            func_00184D48(load, name, &buffer, 0x12240);
            func_00187048(D_003E3BD0, load);
            func_00187048(D_003E3BD0, &D_003EB140);
            func_001870E0(D_003E3BD0, &D_003EB760);
            func_00254D68(name, D_0029CDD8, ((St_88890 *)&D_003EB760)->x21, ((Set_88890 *)D_0029DE30[0])->xF4);
            func_00184D48(load, name, getBuffer(D_0032FC60), 0x2BF10);
            func_00187048(D_003E3BD0, load);
            func_00187048(D_003E3BD0, &D_003EB140);
            func_001870E0(D_003E3BD0, &D_003EB760);
            w.pair.msg.xC = 1;
            func_00187048(D_003E3BD0, &D_003EB140);
            func_00187048(D_003E3BD0, msg);
            func_001870E0(D_003E3BD0, &D_003EB760);
        }
        if (D_003EB760.x0 == 0) {
            func_001907B0(&D_003F52D0);
            func_00187068(D_003E3BD0);
        } else {
            func_00187438((Rider *)obj);
        }
    }
}

/* ---- merged from src/unsorted/00188C10.cpp, converted to C (methods take explicit this) ---- */
typedef struct Info_88C10 {
    int x0;
    char pad4[0x10];
    int x14;
    char pad18[9];
    char name[1];
} Info_88C10;

/* Stack-like scratch allocator: x4 = cursor, x8 = previous mark (linked through the heap). */
typedef struct Arena_88C10 {
    int x0;
    char *cursor;
    int *previous;
} Arena_88C10;

static __inline__ void Arena_push(Arena_88C10 *a) {
    int *old = a->previous;
    int *p = (int *)(((int)a->cursor + 3) & ~3);
    a->previous = p;
    a->cursor = (char *)(p + 1);
    *p = (int)old;
}
static __inline__ void *Arena_alloc(Arena_88C10 *a) {
    int p = ((int)a->cursor + 127) & ~127;
    a->cursor = (char *)(p + 50000);
    return (void *)p;
}
static __inline__ void Arena_pop(Arena_88C10 *a) {
    a->cursor = (char *)a->previous;
    a->previous = (int *)*a->previous;
}

typedef struct Obj_88C10 {
    char pad0[0x460];
    int x460;
} Obj_88C10;

extern Arena_88C10 D_00663B08;
extern char D_003E3C80[];
extern char D_0043F9F0[];
extern char D_0029CDE0[];

extern void func_001A8958(void *);
extern void func_001A9B70(void *, void *);

void func_00188C10(Obj_88C10 *this)
{
    char name[0x50];
    char handle[0x20];
    char *buffer;
    Arena_88C10 *arena;
    void *other;

    if (!D_003EB760.x0 || func_001875D0((Rider *)this)) {
        this->x460 = 90;
        func_001A8958(D_0043F9F0);
        func_00254D68(D_003E3C80, D_0029CDE0, ((Info_88C10 *)&D_003EB760)->name, ((Info_88C10 *)&D_003EB760)->x14);
        func_00254D68(name, D_0029CDD8, D_003E3C80, D_003E3C80);
        arena = &D_00663B08;
        Arena_push(arena);
        buffer = (char *)Arena_alloc(arena);
        func_00184A78(handle);
        func_00184AA0(handle, name, buffer, 2000);
        func_00187048(D_003E3BD0, handle);
        other = &D_003EB140;
        func_00187048(D_003E3BD0, other);
        func_001870E0(D_003E3BD0, &D_003EB760);
        func_00184AA0(handle, name, buffer, *(int *)(buffer + 12));
        func_00187048(D_003E3BD0, handle);
        func_00187048(D_003E3BD0, other);
        func_001870E0(D_003E3BD0, &D_003EB760);
        /* the do/while (0) is needed for the retail register allocation (arena in $s2) */
        do {
            func_001A9B70(D_0043F9F0, buffer);
            Arena_pop(arena);
            func_00187438((Rider *)this);
        } while (0);
    }
}

extern char D_003E3CA8[];
extern char D_0029CDE8[];
extern char D_0028CE58[];
extern int func_00254D08(void *, void *);
extern void func_001A8A98(void *);
extern int func_001A9790(void *, void *);

void func_00188DD0(Obj_88890 *obj)
{
    char name[0x50];
    char archive[0xC90];
    union Work w;
    CheckTask *chk;
    BaseTask *base;
    MsgTask *msg;
    LoadTask *load;
    Arena_88C10 *heap;
    char *data;
    int size;

    if (!D_003EB760.x0 || func_001875D0((Rider *)obj)) {
        obj->x460 = 90;
        func_00254D68(D_003E3CA8, D_0029CDE0, ((Info_88C10 *)&D_003EB760)->name, ((Info_88C10 *)&D_003EB760)->x14);
        func_00254D68(name, D_0029CDD8, D_003E3CA8, D_003E3CA8);
        func_00254D08(D_0028CE58, name);
        func_00254D68(D_003EB760.x81, D_0029CDE8, D_0029DE30[0]->xD8, ((Info_88C10 *)&D_003EB760)->x14 + 1);
        func_00184F98(archive);
        func_00187048(D_003E3BD0, archive);
        func_001870E0(D_003E3BD0, &D_003EB760);
        if (func_001851F8(archive, D_003E3CA8) >= 0) {
            chk = &w.check;
            func_001852A8(chk, D_003E3CA8);
            func_00187048(D_003E3BD0, &D_003EB140);
            func_00187048(D_003E3BD0, chk);
            func_001870E0(D_003E3BD0, &D_003EB760);
            if (w.check.xC94 != 0)
                goto msg_path;
        }
        base = &w.base;
        w.base.x0 = 0;
        w.base.vtbl = D_00285FC8;
        base->xC8C = D_003E3CA8;
        func_00187048(D_003E3BD0, &D_003EB140);
        func_00187048(D_003E3BD0, base);
        func_001870E0(D_003E3BD0, &D_003EB760);
    msg_path:
        msg = &w.pair.msg;
        w.pair.msg.x0 = 0;
        w.pair.msg.vtbl = D_00286000;
        msg->x8 = D_003E3CA8;
        w.pair.msg.xC = 0;
        func_00187048(D_003E3BD0, &D_003EB140);
        func_00187048(D_003E3BD0, msg);
        func_001870E0(D_003E3BD0, &D_003EB760);
        heap = &D_00663B08;
        Arena_push(heap);
        data = (char *)Arena_alloc(heap);
        func_001A8A98(D_0043F9F0);
        size = func_001A9790(D_0043F9F0, data);
        *(int *)(data + 12) = size;
        load = &w.pair.load;
        func_00184D20(load);
        func_00184D48(load, name, data, size);
        func_00187048(D_003E3BD0, load);
        func_00187048(D_003E3BD0, &D_003EB140);
        func_001870E0(D_003E3BD0, &D_003EB760);
        Arena_pop(heap);
        w.pair.msg.xC = 1;
        func_00187048(D_003E3BD0, &D_003EB140);
        func_00187048(D_003E3BD0, msg);
        func_001870E0(D_003E3BD0, &D_003EB760);
        if (D_003EB760.x0 == 0) {
            func_001907B0(&D_003F52D0);
            func_00187068(D_003E3BD0);
        } else {
            func_00187438((Rider *)obj);
        }
    }
}

/* ---- merged from src/unsorted/001890F0.c (type renamed; func_0018DB20 from above) ---- */
struct Obj_890F0 {
    char pad0[0x610];
    unsigned char x610;
};

void func_001890F0(struct Obj_890F0 *obj)
{
    obj->x610 = 1;
    func_0018DB20((Player *)obj);
}

/* ---- merged from src/unsorted/00189110.c (type renamed) ---- */
typedef struct Obj_89110 {
    char pad0[0x610];
    char x610;
} Obj_89110;

void func_00189110(Obj_89110 *obj) {
    obj->x610 = 1;
}

/* ---- func_00189120 (matching C; types renamed to merge with the units above) ---- */
typedef struct { short x0, x2; char pad4[3]; char x7; } LocalPair_89120;
typedef struct { char pad0[0x418]; int x418,x41C; char pad420[0x18]; LocalPair_89120 x438; char pad440[8]; LocalPair_89120 x448; char pad450[0x10]; int x460,x464; char pad468[0x14]; int x47C; char pad480[0x28]; int x4A8; char pad4AC[15]; char x4BB; char pad4BC[0x154]; unsigned char x610; } Obj_89120;
typedef struct { char pad0[4]; unsigned char x4; } Flag_89120;
typedef struct { char pad0[0x20]; int x20; char pad24[0x110]; Flag_89120 *x134; char pad138[0x18]; Obj_89120 *x150; } Global_89120;
#define D_003F52D0 (*(Global_89120 *)&D_003F52D0)

extern unsigned char D_003EB834[16],D_003EB780[16];

extern void func_0023D650(int,int *,int *);
extern void func_001895A0();
extern void func_0018E638(Obj_89120 *,int,void *); /* func_0018E6B8 declared above for func_00188358 */
static inline void set_pair(LocalPair_89120 *p,int x,int y,int active) { p->x0=x; p->x2=y; p->x7=active; }
static inline void set_flags(Obj_89120 *obj,int active) { obj->x4BB=active; obj->x47C=active; }
void func_00189120(Obj_89120 *obj) {
 int w,h,half; LocalPair_89120 *a,*b;
 func_0023D650(D_003F52D0.x20,&w,&h); half=w/2;
 if(!obj->x610) {
  func_001881C0(obj);
  if(!func_00187500((Rider *)obj)) return;
  if(D_003EB834[0]) obj->x610=1;
  if(D_003F52D0.x150!=obj) return;
  if(!obj->x610) goto done;
 }
 if(!D_0029CDA0) {
  func_001895A0(obj); obj->x610=0;
  if(!obj->x418 && !D_003EB780[0]) {
   func_0018E638(obj,0,&D_0029DE30);
   func_001873C0((struct SharedMenu *)(void *)(obj),0x38,0); func_001873C0((struct SharedMenu *)(void *)(obj),0,0); func_001873C0((struct SharedMenu *)(void *)(obj),0x39,0);
   func_001873C0((struct SharedMenu *)(void *)(obj),func_0021F818()+0x10,0);
   obj->x4BB=0; obj->x47C=0;
   func_0018E6B8(obj,0x1A,&D_0029DE30);
   obj->x464=half; obj->x460=0x5A;
   set_pair(&obj->x438,half,0x27,1); set_pair(&obj->x448,half,0xCC,1);
  } else {
   func_0018E6B8(obj,0x1B,&D_0029DE30); func_0018E638(obj,0x3A,&D_0029DE30);
   obj->x460=0x46; obj->x464=0x1E;
   set_flags(obj,1);
   set_pair(&obj->x438,half,0x27,1); set_pair(&obj->x448,half,0xCC,1);
  }
 }
done:
 obj->x4A8=5; obj->x41C=10;
 if(D_003F52D0.x134) D_003F52D0.x134->x4=0;
 func_00187548(obj);
}

#undef D_003F52D0

/* ---- merged from src/unsorted/00189340.c (types renamed; shared globals via base decls) ---- */
typedef struct St_89340 { int x0; char pad4[0x10]; int x14, x18; char pad1C[4]; unsigned char x20; char x21[1]; } St_89340;
typedef struct Heap_89340 { int x0,x4,x8; } Heap_89340;
typedef struct Context_89340 { char pad0[0x114]; char *x114; } Context_89340;
extern char D_003E3CD0[], D_003E8CE0[];
extern char D_003E6E40[];
extern int strcmp(const char *, const char *);
extern void func_001A8A98(void *);
extern int func_001A9790(void *,void *);
extern void func_00184938(void *,int);
extern void func_0018A5A8();
static inline void push_89340(Heap_89340 *heap) { int prev = heap->x8; int *mark = (int *)((heap->x4 + 3) & ~3); heap->x8 = (int)mark; heap->x4 = (int)mark + 4; *mark = prev; }
static inline void pop_89340(Heap_89340 *heap) { heap->x4 = heap->x8; heap->x8 = *(int *)heap->x8; }
static inline short get_type(char *obj) { return *(short *)(obj + 0x1C); }
void func_00189340(char *obj, int index) {
    char name[80];
    int buffer[500];
    char handle[32];
    Heap_89340 *heap;
    char *data;
    int value;
    if (!((St_89340 *)&D_003EB760)->x20) {
        if (!strcmp(((Context_89340 *)D_0029DE30[0])->x114,obj + (index * 48 + 0x611))) {
            func_00190660(&D_003F52D0,(void *)D_003E8CE0);
            return;
        }
    }
    if (((St_89340 *)&D_003EB760)->x20) {
        int offset;
        heap = (Heap_89340 *)&D_00663B08;
        push_89340(heap);
        data = (char *)((heap->x4 + 127) & ~127);
        heap->x4 = (int)data + 50000;
        func_001A8A98(D_0043F9F0);
        offset = func_001A9790(D_0043F9F0,data) + 0x7650;
        pop_89340(heap);
        ((St_89340 *)&D_003EB760)->x18 = offset;
    }
    if ((get_type(obj) ^ 0x38) == 0) { obj[0x610] = 1; return; }
    if ((get_type(obj + index * 16) ^ 0x34) == 0) {
        ((St_89340 *)&D_003EB760)->x14 = (unsigned char)obj[0x1251];
        D_003E6E40[0] = 0;
        func_0018A5A8(&D_003E5BF0,0);
        func_00184938(&D_003EB760,0);
    } else {
    value = (unsigned char)(obj + index)[0x1211];
    D_003E6E40[0] = 1;
    ((St_89340 *)&D_003EB760)->x14 = value;
    func_00254D68(D_003E3CD0,D_0029CDE0,((St_89340 *)&D_003EB760)->x21,value);
    func_00254D68(name,D_0029CDD8,D_003E3CD0,D_003E3CD0);
    func_00184A78(handle);
    func_00184AA0(handle,name,buffer,2000);
    func_00187048(D_003E3BD0,handle);
    func_001870E0(D_003E3BD0,&D_003EB760);
    if (((St_89340 *)&D_003EB760)->x0) {
        func_0018A5A8(&D_003E5BF0,0);
        func_00184938(&D_003EB760,0);
    } else {
        func_0018A5A8(&D_003E5BF0,(char *)buffer[3] + 0x7650);
    }
    }
    func_00187438((Rider *)obj);
}

/* ---- merged from src/unsorted/001895A0.c (types renamed; shared globals via base decls) ---- */
/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry_895A0 {
    short delta;
    short pad2;
    void (*func)();
} VEntry_895A0;

typedef struct Player_895A0 {
    char pad0[0x4F8];
    VEntry_895A0 *vtbl;
    char pad4FC[0x610 - 0x4FC];
} Player_895A0;

typedef struct PH_895A0 {
    int x0;
    void *x4;
    char pad8[0x8];
    Player_895A0 player;
} PH_895A0;

typedef struct ArchiveEntry {
    char name[0x20];
} ArchiveEntry;

typedef struct Archive {
    int x0;
    void *x4;
    ArchiveEntry entries[100];
    int count;
} Archive;

typedef struct St_895A0 {
    int x0;
    char pad4[0x1C];
    unsigned char x20;
    char x21[1];
} St_895A0;

typedef struct SaveHeader {
    unsigned char x0;
    char pad1;
    unsigned short x2;
    char pad4[8];
    int size;
} SaveHeader;

typedef union SaveBuffer {
    int words[500];
    SaveHeader header;
} SaveBuffer;

typedef struct Heap_895A0 {
    int x0, x4, x8;
} Heap_895A0;

typedef struct Context_895A0 {
    char pad0[0x8C];
    char *x8C;
    char pad90[0x114 - 0x90];
    char *x114;
} Context_895A0;

typedef struct Obj_895A0 {
    char pad0[0x418];
    int count;
    char pad41C[0x611 - 0x41C];
    char names[64][48];
    unsigned char ids[64];
    unsigned char nextId;
} Obj_895A0;

extern char D_003E3CF8[];
extern char D_0029CDF0[];
extern char D_0029CDF8[];
extern char D_0029CE00[];
extern char *D_0026EFE8[];

extern PH_895A0 *func_00184940(PH_895A0 *holder);
extern unsigned int strlen(const char *);
extern int func_00185278(Archive *, const char *, int, unsigned int);
extern char *func_00158650(int);
extern void func_0018E408(Obj_895A0 *, char *, int);

static inline int IsBadHeader(SaveHeader *h)
{
    unsigned int version = h->x2;
    return (version & 0x1F) != 0 || version < 0x20 || version > 0x220 || h->x0 > 0x21;
}

static inline void push(Heap_895A0 *heap) { int prev = heap->x8; int *mark = (int *)((heap->x4 + 3) & ~3); heap->x8 = (int)mark; heap->x4 = (int)mark + 4; *mark = prev; }
static inline char *alloc(Heap_895A0 *heap, int size) { int p = (heap->x4 + 127) & ~127; heap->x4 = p + size; return (char *)p; }
static inline void pop(Heap_895A0 *heap) { heap->x4 = heap->x8; heap->x8 = *(int *)heap->x8; }

void func_001895A0(Obj_895A0 *self)
{
    PH_895A0 holder;
    SaveBuffer buffer;
    Archive archive;
    char name[80];
    char handle[32];
    Player_895A0 *player;
    unsigned int len;
    char *entry;
    Heap_895A0 *heap;
    char *data;
    int bad;
    int n;
    int i;
    int changed;
    int total;
    char *digits;

    func_001873F8((Player *)self);
    func_00254D68(D_003E3CF8, D_0029CDF0, ((St_895A0 *)&D_003EB760)->x21);
    func_00184940(&holder);
    player = &holder.player;
    player->vtbl[14].func((char *)player + player->vtbl[14].delta);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), 0x3B, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), 0xC, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), func_0021F818() + 0x10, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), 0xE, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), 0xC, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)player), 0xF, 0);
    func_00184F98(&archive);
    SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760), 0, 0);
    func_00187048(D_003E3BD0, &archive);
    func_00187048(D_003E3BD0, &holder);
    func_001870E0(D_003E3BD0, &D_003EB760);
    len = strlen(D_003E3CF8);
    for (i = 0; i < archive.count; i++) {
        if (func_00185278(&archive, D_003E3CF8, i, len) != 0) {
            continue;
        }
        entry = archive.entries[i].name;
        func_00254D68(name, D_0029CDD8, entry, entry);
        func_00184A78(handle);
        func_00184AA0(handle, name, &buffer, 2000);
        func_00187048(D_003E3BD0, handle);
        func_00187048(D_003E3BD0, &holder);
        func_001870E0(D_003E3BD0, &D_003EB760);
        func_00184938(&D_003EB760, 0);
        bad = IsBadHeader(&buffer.header);
        if (!bad) {
            heap = (Heap_895A0 *)&D_00663B08;
            push(heap);
            data = alloc(heap, 50000);
            func_00184AA0(handle, name, data, buffer.header.size);
            func_00187048(D_003E3BD0, handle);
            func_00187048(D_003E3BD0, &holder);
            func_001870E0(D_003E3BD0, &D_003EB760);
            bad = ((St_895A0 *)&D_003EB760)->x0 != 0;
            func_00184938(&D_003EB760, 0);
            pop(heap);
        }
        n = self->count;
        if (!bad) {
            func_00254D68(self->names[n], D_0029CDF8, func_00158650(buffer.header.x2),
                          ((Context_895A0 *)D_0029DE30[0])->x8C, D_0026EFE8[buffer.header.x0]);
        } else {
            func_00254D68(self->names[n], D_0029CE00, ((Context_895A0 *)D_0029DE30[0])->x114);
        }
        func_0018E408(self, self->names[n], 1);
        digits = entry + len;
        self->ids[n] = (digits[0] - '0') * 10 + (digits[1] - '0');
    }
    self->nextId = 0;
    total = self->count;
    do {
        changed = 0;
        for (i = 0; i < total; i++) {
            if (self->ids[i] == self->nextId) {
                self->nextId++;
                changed = 1;
            }
        }
    } while (changed);
    if (((St_895A0 *)&D_003EB760)->x20 && total < 100) {
        func_0018E3A8((struct SharedMenu *)(void *)(self), 0x34, &D_0029DE30, 1);
    }
}

/* ---- merged from src/unsorted/00189A18.c (types renamed; shared globals via base decls) ---- */
typedef struct St_89A18 {
    int x0;
    char pad4[0x10];
    int x14;
    char pad18[0x9];
    char x21[0x5F];
} St_89A18;

/* bump allocator: x4 = top, x8 = last pushed mark */
typedef struct Heap_89A18 {
    int x0;
    int x4;
    int x8;
} Heap_89A18;

extern char D_003E3D20[];

static __inline__ void HeapPushMark(Heap_89A18 *heap)
{
    int prev;
    int *mark;

    prev = heap->x8;
    mark = (int *)((heap->x4 + 3) & ~3);
    heap->x8 = (int)mark;
    heap->x4 = (int)mark + 4;
    *mark = prev;
}

static __inline__ void HeapPopMark(Heap_89A18 *heap)
{
    heap->x4 = heap->x8;
    heap->x8 = *(int *)heap->x8;
}

/* Releases the load buffer and hands over to the player. The do/while(0)
   macro block is what gives the retail register allocation. */
#define FINISH_LOAD(obj, buf, heap, player) \
    do {                                    \
        func_001A9B70(obj, buf);            \
        HeapPopMark(heap);                  \
        func_00187438(player);              \
    } while (0)

void func_00189A18(void *player)
{
    char name[80];
    int reader[5];
    int *buffer;
    Heap_89A18 *heap;

    if (D_003EB760.x0 == 0 || ((int (*)())func_001875D0)() /* no-arg: ignored $a0, keeps retail regalloc */) {
        func_001A8958(D_0043F9F0);
        func_00254D68(D_003E3D20, D_0029CDE0, ((St_89A18 *)&D_003EB760)->x21, ((St_89A18 *)&D_003EB760)->x14);
        func_00254D68(name, D_0029CDD8, D_003E3D20, D_003E3D20);
        heap = (Heap_89A18 *)&D_00663B08;
        HeapPushMark(heap);
        buffer = (int *)((heap->x4 + 127) & ~127);
        heap->x4 = (int)buffer + 50000;
        func_00184A78(reader);
        func_00184AA0(reader, name, buffer, 2000);
        func_00187048(D_003E3BD0, reader);
        func_00187048(D_003E3BD0, &D_003EB140);
        func_001870E0(D_003E3BD0, &D_003EB760);
        func_00184AA0(reader, name, buffer, buffer[3]);
        func_00187048(D_003E3BD0, reader);
        func_00187048(D_003E3BD0, &D_003EB140);
        func_001870E0(D_003E3BD0, &D_003EB760);
        FINISH_LOAD(D_0043F9F0, buffer, heap, player);
    }
}

/* ---- merged from src/unsorted/00189BD0.c (types renamed; shared globals via base decls) ---- */
typedef struct { int x0; void *x4; char pad8[0xc84]; void *xC8C; char padC90[4]; unsigned char xC94; char tail[11]; } Request_89BD0;
typedef struct { int x0; char pad4[16]; void *x14; char pad18[9]; char x21[1]; } St_89BD0;
extern char D_003E3D48[], D_0029CE08[], D_0029CE10[], D_0028CE68[], D_0028CE58[];
extern struct { int x0; void *x4; char pad[8]; } D_0043DD38;
typedef struct { char pad[0x625]; char x625; } S_89BD0;
extern int func_0019C210(void *);
extern void *func_0019E518(void *);
extern int func_00254D08(void *,void *);
static inline void init_request(Request_89BD0 *r,void *name) { r->x0=0; r->x4=D_00285FC8;r->xC8C=name; }
static inline void *init_other(void *ptr,void *name) { struct Other {int x0;void *x4;void *x8;char xC;}; struct Other *r=ptr; r->x0=0;r->x4=D_00286000;r->x8=name;return r; }
void func_00189BD0(int arg0) {
 char text[0x50]; char first[0xc90];
 union { Request_89BD0 request; struct {int x0;void *x4;void *x8;char xC;char pad[3]; char data[0x20];} other; } u;
 char name[0x50]; char data[0x10];char last[0x20];void *p; void *payload; Request_89BD0 *r;
 if (((St_89BD0 *)&D_003EB760)->x0 != 0 && !((int (*)())func_001875D0)() /* no-arg: ignored $a0, keeps retail regalloc */) return;
 func_00254D68(D_003E3D48,D_0029CE08,((St_89BD0 *)&D_003EB760)->x21);
 p=func_0019E518(&D_0043E280);
 ((St_89BD0 *)&D_003EB760)->x14=p;func_00254D68(text,D_0028CE68,D_003E3D48,((St_89BD0 *)&D_003EB760)->x14);
 func_00254D08(D_0028CE58,text);
 func_00184F98(first);func_00187048(D_003E3BD0,first);func_001870E0(D_003E3BD0,&D_003EB760);
 if (func_001851F8(first,D_003E3D48)<0 || (func_001852A8(&u,D_003E3D48),func_00187048(D_003E3BD0,&D_003EB140),func_00187048(D_003E3BD0,&u),func_001870E0(D_003E3BD0,&D_003EB760),!u.request.xC94)) {
 r=&u.request;u.request.x0=0;u.request.x4=D_00285FC8;r->xC8C=D_003E3D48;
 func_00187048(D_003E3BD0,&D_003EB140);func_00187048(D_003E3BD0,&u);func_001870E0(D_003E3BD0,&D_003EB760);
 func_00254D68(name,D_0029CE10,D_003E3D48,D_003E3D48);
 func_00184D20(last);func_00184D48(last,name,data,16);
 func_00187048(D_003E3BD0,last);func_00187048(D_003E3BD0,&D_003EB140);func_001870E0(D_003E3BD0,&D_003EB760);
 }
 init_other(&u,D_003E3D48);u.other.xC=0;
 func_00187048(D_003E3BD0,&D_003EB140);func_00187048(D_003E3BD0,&u);func_001870E0(D_003E3BD0,&D_003EB760);
 ((void *(*)())func_00184D20)(u.other.data) /* void* result (dead): keeps $v0 liveness of original decl */;payload=(&D_0043DD38)->x4;
 func_00184D48(u.other.data,text,payload,func_0019C210(&D_0043DD38));
 func_00187048(D_003E3BD0,u.other.data);func_00187048(D_003E3BD0,&D_003EB140);func_001870E0(D_003E3BD0,&D_003EB760);
 u.other.xC=1;func_00187048(D_003E3BD0,&D_003EB140);func_00187048(D_003E3BD0,&u);func_001870E0(D_003E3BD0,&D_003EB760);
 if(!((St_89BD0 *)&D_003EB760)->x0){func_001907B0(&D_003F52D0);func_00187068(D_003E3BD0);}else func_00187438((void *)arg0);
 func_001B5AF0(&D_003F52D0,5,0);((S_89BD0 *)&D_0043E280)->x625 = 1;
}

/* ---- merged from src/unsorted/00189EF0.c (type renamed) ---- */
struct Obj_89EF0 {
    int x0;
    char pad4[0x610 - 0x4];
    char x610;
};

void func_00189EF0(struct Obj_89EF0 *obj)
{
    obj->x610 = 1;
    obj->x0 = -1;
    func_0018DB20((Player *)obj);
}

/* ---- func_00189F18: matching C (Obj/Item/Global/get_item shared with func_00188358) ---- */
extern int func_00261F60(State *);
static __inline__ void Menu_setFlag(Menu *m, int v) { m->x610 = v; }
void func_00189F18(Obj *obj) {
 char buffer[0xC90];
 int i;
 Item *item;
 obj->x460=90;obj->x4B8=1;
 if(!obj->x610) {
  func_001881C0((Rider *)obj);
  if(!func_00187500((Rider *)obj))return;
  if(D_003EB760.xD4) {RefreshGlobalMenuLegacy(obj);obj->x610=1;}
  if(D_003F5420[0]!=(Rider *)obj)return;
 }
 if(obj->x610 && !D_0029CDA0) {
  for(i=0;i<=0;i++) {
   SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760),i,0);
   item=get_item((Global *)&D_003EB760,func_00261F60(&D_003EB760));
   if(!item->x0)break;
   if(item->x4==2) {
    func_00184F98(buffer);
    func_00187048(D_003E3BD0,buffer);
    func_001870E0(D_003E3BD0,&D_003EB760);
    if(func_001851F8(buffer,((Global *)&D_003EB760)->x21)>=0) goto success;
   }
  }
  obj->x610=0;
 }
 SetMenuSelectionLegacy((MenuSelectionState *)(void *)(&D_003EB760),0,0);
 D_003E6830.x610 = 0;
 func_0018A5A8(&D_003E5BF0,0);
 if(D_003EB760.xA || D_003EB760.x20) {
  obj->x610=1;func_00187438((Rider *)obj);return;
 }
 goto show_options;
success:
 obj->x610=1;func_00187438((Rider *)obj);
 Menu_setFlag(&D_003E6830, 1);
 func_0018A5A8(&D_003E5BF0,0x44C00);
 return;
show_options:
 func_001873F8((Player *)obj);
 func_001873C0((struct SharedMenu *)(void *)((Player *)obj),44,0);func_001873C0((struct SharedMenu *)(void *)((Player *)obj),57,0);
 func_001873C0((struct SharedMenu *)(void *)((Player *)obj),func_0021F818()+16,0);
 func_001873C0((struct SharedMenu *)(void *)((Player *)obj),0,0);func_001873C0((struct SharedMenu *)(void *)((Player *)obj),24,0);func_001873C0((struct SharedMenu *)(void *)((Player *)obj),25,0);
 func_0018E6B8(obj,26,D_0029DE30);
 func_00187548((Rider *)obj);obj->x4A8=5;
}

/* ---- merged from src/unsorted/0018A198.c (type renamed; D_003EB760/func_00261F60 via decls above) ---- */
typedef struct { unsigned char x0; char pad[3]; int x4,x8,xC,x10; } Entry_8A198;
int func_0018A198(void) {
 int offset=func_00261F60(&D_003EB760)*20;
 Entry_8A198 *entry;
 int flag=0;
 offset += 0xC4;
 entry=(Entry_8A198 *)(offset+(char *)&D_003EB760);
 if(entry->x0) {
  if(entry->xC) flag=entry->x4==2;
 }
 return flag^1;
}

/* ---- merged from src/unsorted/0018A200.c (type renamed) ---- */
typedef struct Obj_8A200 {
    char pad0[0x610];
    unsigned char x610;
} Obj_8A200;

void func_0018A200(Obj_8A200 *obj)
{
    RefreshGlobalMenuLegacy(obj);
    obj->x610 = 1;
}

/* ---- merged from src/unsorted/0018A230.c (type renamed) ---- */
typedef struct Obj_8A230 {
    char pad0[0x610];
    unsigned char x610;
    unsigned char x611;
} Obj_8A230;

void func_0018A230(Obj_8A230 *obj)
{
    obj->x610 = 1;
    func_00187728((Rider *)obj);
    obj->x611 = 0;
}

typedef struct Obj_8A260 {
    char pad0[0x460];
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
    char pad4AC[0x4B8 - 0x4AC];
    unsigned char x4B8;
    char pad4B9[0x4F8 - 0x4B9];
    VEntry *vtbl;
    char pad4FC[0x610 - 0x4FC];
    unsigned char x610;
    unsigned char x611;
} Obj_8A260;

typedef struct Entry_8A260 {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Entry_8A260;

typedef struct State_8A260 {
    char pad[0xC8];
    Entry_8A260 entries[1];
} State_8A260;

extern unsigned char D_0043E8A5[];

void func_0018A260(Obj_8A260 *obj)
{
    int i;
    Entry_8A260 *p;
    VEntry *entry;
    unsigned char x611;
    State_8A260 *state;
    unsigned char one;

    obj->x460 = 90;
    if (!obj->x610) {
        func_001881C0((Rider *)obj);
        if (!func_00187500((Rider *)obj))
            return;
        if (D_003EB834[0])
            obj->x610 = 1;
        if (D_003F5420[0] != (Rider *)obj)
            return;
    }
    obj->x4B8 = 1;
    obj->x4A8 = 5;
    if (obj->x610 && !D_0029CDA0) {
        x611 = obj->x611;
        /* via State struct: retail 3-insn p setup (lui+addiu+addiu) + nop alignment */
        state = (State_8A260 *)&D_003EB760;
        p = state->entries;
        for (i = 0; i <= 0; i++, p++) {
            if (!x611 && p->x0 == 2) {
                func_00187438((Rider *)obj);
                return;
            }
        }
        obj->x610 = 0;
    }
    func_001873F8((Player *)obj);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 12, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 1, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), func_0021F818() + 16, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 23, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 12, 0);
    func_0018E6B8(obj, 26, D_0029DE30);
    D_0043E8A5[0] = 1;
    entry = &obj->vtbl[14];
    entry->func((char *)obj + entry->delta);
    /* fresh temp: retail \, separate li (direct =1 shares via \, 0x20 frame) */
    one = 1;
    obj->x611 = one;
    func_00187548((Rider *)obj);
}

/* ---- merged from src/unsorted/0018A3F0.c (types renamed) ---- */
typedef struct Entry_8A3F0 { int x0; int x4; int x8; int xC; int x10; } Entry_8A3F0;
typedef struct State_8A3F0 { char pad[0xC8]; Entry_8A3F0 entries[1]; } State_8A3F0;
int func_0018A3F0(void) {
    return ((State_8A3F0 *)&D_003EB760)->entries[func_00261F60(&D_003EB760)].x0 != 2;
}

/* ---- merged from src/unsorted/0018A438.c (type renamed) ---- */
typedef struct Obj_8A438 {
    char pad0[0x2C];
    short x2C;
    char pad2E[0x5E2];
    unsigned char x610;
    unsigned char x611;
} Obj_8A438;

void func_0018A438(Obj_8A438 *obj) {
    int state = obj->x2C;

    if ((state ^ 1) == 0) {
        RefreshGlobalMenuLegacy(obj);
        obj->x610 = 1;
        obj->x611 = 0;
    }
}

/* ---- merged from src/unsorted/0018A480.c (types renamed; shared globals via decls above) ---- */
typedef struct VEntry_8A480 {
    short delta;
    short pad2;
    int (*func)(void *);
} VEntry_8A480;

typedef struct Rider_8A480 {
    char pad0[0x460];
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
    char pad4AC[0x4B8 - 0x4AC];
    unsigned char x4B8;
    char pad4B9[0x4F8 - 0x4B9];
    VEntry_8A480 *vtbl;
    char pad4FC[0x614 - 0x4FC];
    int x614;
} Rider_8A480;

void func_0018A480(Rider_8A480 *obj)
{
    Rider_8A480 *cur;
    VEntry_8A480 *entry;

    obj->x460 = 90;
    func_001881C0((Rider *)obj);
    if (!func_00187500((Rider *)obj))
        return;
    cur = (Rider_8A480 *)D_003F5420[0];
    if (cur != obj)
        return;
    cur->x4B8 = 1;
    entry = &cur->vtbl[13];
    if (!entry->func((char *)cur + entry->delta)) {
        func_00187438((Rider *)cur);
        return;
    }
    func_001873F8((Player *)cur);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), 9, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), 57, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), func_0021F818() + 16, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), 2, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), cur->x614, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)cur), 6, 0);
    func_0018E6B8(cur, 26, &D_0029DE30);
    cur->x4A8 = 5;
    func_00187548((Rider *)cur);
}

/* ---- merged from src/unsorted/0018A5A8.c (type renamed; D_003F52D0 via base Game decl) ---- */
typedef struct Obj_8A5A8 {
    char pad0[0x610];
    int x610;
} Obj_8A5A8;

extern void func_00190608(void *list, Obj_8A5A8 *obj);

void func_0018A5A8(Obj_8A5A8 *obj, int value)
{
    func_00190608(&D_003F52D0, obj);
    obj->x610 = value;
}

/* ---- merged from src/unsorted/0018A5E8.c (types renamed) ---- */
struct Obj_8A5E8 { char pad[0x610]; int x610; };
struct Entry_8A5E8 { int x0; char pad[16]; };
struct Global_8A5E8 { char pad0[0x18]; int x18; char pad1[0xCC-0x1C]; struct Entry_8A5E8 xCC[1]; };
int func_0018A5E8(struct Obj_8A5E8 *obj) {
    struct Global_8A5E8 *global = (struct Global_8A5E8 *)&D_003EB760;
    int index = func_00261F60((State *)global);
    return (global->xCC[index].x0 << 10) < (global->x18 - obj->x610);
}

/* ---- merged from src/unsorted/0018A648.c (verbatim; func_00187808 from above) ---- */
void func_0018A648(void *obj)
{
    RefreshGlobalMenuLegacy(obj);
}

/* ---- merged from src/unsorted/0018A668.c (type renamed) ---- */
typedef struct {
    int x0;
    char pad4[0x610 - 0x4];
    char x610;
} Obj_8A668;

void func_0018A668(Obj_8A668 *obj)
{
    obj->x610 = 1;
    func_00187728((Rider *)obj);
    obj->x0 = 8;
}

/* ---- func_0018A6A0: matching C (type renamed to merge with the units above) ---- */
typedef struct Obj_8A6A0 {
    char pad0[0x460];
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
    char pad4AC[0x4B8 - 0x4AC];
    unsigned char x4B8;
    char pad4B9[0x4F8 - 0x4B9];
    VEntry *vtbl;
    char pad4FC[0x610 - 0x4FC];
    unsigned char x610;
} Obj_8A6A0;

void func_0018A6A0(Obj_8A6A0 *obj)
{
    obj->x460 = 90;
    obj->x4B8 = 1;
    if (obj->x610) goto active;
    {
        func_001881C0((Rider *)obj);
        if (!func_00187500((Rider *)obj)) return;
        if (D_003EB834[0]) obj->x610 = 1;
        if (D_003F5420[0] != (Rider *)obj) return;
        if (!obj->x610) goto setup;
    }
active:
    if (D_0029CDA0 == 0) {
        VEntry *entry;
        obj->x610 = 0;
        entry = &obj->vtbl[13];
        if (!((int (*)(void *))entry->func)((char *)obj + entry->delta)) {
            func_00187438((Rider *)obj);
            return;
        }
    }
setup:
    func_001873F8((Player *)obj);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 28, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 13, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), func_0021F818() + 19, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 29, 0);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 0, 0);
    func_00187718((Rider *)obj);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 30, 1);
    func_001873C0((struct SharedMenu *)(void *)((Player *)obj), 31, 1);
    func_0018E6B8(obj, 27, &D_0029DE30);
    obj->x4A8 = 5;
    func_00187548((Rider *)obj);
}
