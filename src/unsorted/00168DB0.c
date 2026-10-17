/* UNIT 0x00168DB0 0x00168F70 */
#include "timer.h"
typedef struct Obj {
    char pad0[0x500];
    int x500[8];
    int x520;
    int x524;
    int x528;
    int x52C;
    int x530;
    int x534;
    int x538;
    int x53C;
    short x540;
    unsigned short x542;
    unsigned short x544;
    short x546;
    short x548;
    short x54A;
    unsigned char x54C;
    unsigned char x54D;
    unsigned char x54E;
    unsigned char x54F;
    unsigned char x550;
    unsigned char x551;
    unsigned char x552;
    unsigned char x553;
    unsigned char x554;
    unsigned char x555;
    unsigned char x556;
    unsigned char x557;
    unsigned char x558;
} Obj;

typedef struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x290 - 0x110];
    int x290;
    char pad294[0x2A0 - 0x294];
    int x2A0;
} Game;

typedef struct Slot {
    char pad0[0x32];
    signed char x32;
    char pad33[0x44 - 0x33];
    int x44;
    int x48;
    int x4C;
} Slot;

typedef struct LevelData {
    char pad0[0x7724];
    int x7724;
} LevelData;

extern char D_00261C90[];
extern char D_00759780[];
extern char D_004225A0[];
extern Game D_004B25A0;

extern int func_00194C80(void *, int);
extern void func_00169BD0(Obj *, Obj *, int, int, int, int);

void func_00168DB0(Obj *p)
{
    int i;
    int *entry;
    int value;

    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(D_00261C90), p, 1);
    p->x524 = 0;
    p->x530 = func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, D_004B25A0.x10C);
    {
        char *t;
        t = D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694;
        p->x53C = ((Slot *)(t + D_004B25A0.x10C * 0x50))->x48;
    }
    p->x542 = 0;
    p->x544 = 0;
    p->x54C = 0;
    p->x54D = 0;
    p->x54E = 0;
    p->x534 = 0;
    p->x54F = 0;
    p->x550 = 0;
    p->x551 = 0;
    p->x552 = 0;
    p->x553 = 0;
    p->x554 = 0;
    p->x546 = 0;
    p->x548 = 0;
    p->x54A = 0;
    p->x538 = 0;
    p->x556 = 0;
    p->x557 = 0;
    p->x558 = 0;
    p->x540 = 0;
    p->x520 = 0;
    value = -1;
    i = 7;
    entry = &p->x500[7];
    do {
        *entry = value;
        i--;
        entry--;
    } while (i >= 0);
    p->x528 = ((LevelData *)(D_004225A0 + D_004B25A0.x2A0 * 0xD8))->x7724;
    p->x52C = 0;
    if (p->x53C != 0) {
        char *t2;
        t2 = D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694;
        p->x52C = ((Slot *)(t2 + D_004B25A0.x10C * 0x50))->x4C;
    }
    func_00169BD0(p, p, 0x19, 0x4F, 0x1AE, 0x12);
}
