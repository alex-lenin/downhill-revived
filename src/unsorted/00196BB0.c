/* UNIT 0x00196BB0 0x00197640 .rodata=0x28D980:0x28DA80 */
#include "menu_items.h"
#include "include_asm.h"
typedef unsigned int u128 __attribute__((mode(TI)));

struct Menu {
    char pad0[0x500];
    int x500;
};

typedef struct Body973B0 { int x0; char pad4[0xC]; float x10; float x14; float x18; } Body973B0;
typedef struct Entry973B0 { int x0; unsigned char x4; char pad5[3]; Body973B0 *x8; int xC; int x10; } Entry973B0;
struct Stats { char pad0[0x18]; Entry973B0 entries[150]; int xBD0,xBD4,xBD8,xBDC,xBE0,xBE4; char padBE8[0x18]; int xC00; };
extern struct Stats D_004348F0;
typedef struct { int x0; short x4[150]; short x130[150]; short x25c[150]; unsigned char x388[150]; } Obj973B0;
extern char D_004225A0[];
extern void func_00195B68(Obj973B0 *, int, float *, int);
extern void func_00190890(void *);
extern void func_00187AB0(void);
static inline char *get_player973B0(int p) { return D_004225A0 + p * 0x420; }
static inline int get_offset973B0(int p) { return p * 0x420 + 0x9240; }
extern int D_004B2840[];
extern struct Menu D_00435F20;
struct Display { char pad0[0x20]; int x20; char pad24[0x12D-0x24]; char x12D; };
extern struct Display D_003F52D0;
extern char D_004B2914[];
extern int func_00196198(void *);
extern void func_00190660(void *, void *);
extern void func_001C6E88(void *);
extern void func_001C6EA8(void *);
extern int func_00190D60(void *, int, int);
extern void func_001C57A8(void *, u128);
extern char D_00386680[];
extern char D_00437850[];
extern char D_00437D50[];
extern char D_00438250[];
extern char D_003F53FD[];
extern char D_004B28FC[];
typedef struct { char pad[0x500]; int x500, x504, x508; } Menu437340;
extern Menu437340 D_00437340;
typedef struct { char pad0[0x45C]; int x45C; char pad460[0x504-0x460]; int x504; } Arg97190;
typedef struct Entry97190 { char pad0[0xC]; short xC; char padE[0x2]; } Entry97190;
typedef struct Obj97190 { char pad0[0x10]; Entry97190 entries[1]; } Obj97190;
static inline int IsType97190(Entry97190 *entry, int type) { return entry->xC == type; }
void func_00197338(void *arg0);
void func_001973B0(void *arg0, void *arg1);

const char D_0028D980[] = "Unable to Find/Add when Crashing";
const char D_0028D9A8[] = "%2d / %2d";

int func_00197518(void *, int, int, int *);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,const char *);
extern void func_0020F9A8(int,int,int,int,const char *);
extern int func_00254D68(char *,const char *,...);
struct State { char pad[0x2A0]; int x2A0; char pad2A4[0x35C-0x2A4]; char x35C; char pad35D[0x4F0-0x35D]; char x4F0; };
extern struct State D_004B25A0;
extern char D_0029D0A8[];
extern char D_0029D0B0[];
extern char D_0029D0B8[];
extern char D_0029D0C0[];
extern char D_0029D0C8[];
extern char D_0029D0D0[];
extern char D_0029D0D8[];
extern char D_0029D0E0[];
extern char D_0029D0E8[];
typedef struct Obj196BB0 { char pad0[0x418]; int x418; char pad41C[0x40]; int x45C; char pad460[4]; int x464; char pad468[0x504-0x468]; int x504; } Obj196BB0;
typedef struct Rider196BB0 { char pad0[0x4098]; unsigned long x4098; } Rider196BB0;
typedef struct Global196BB0 { char pad0[0x50130]; Rider196BB0 *objs[1]; } Global196BB0;
extern Global196BB0 D_002AF2C0;
extern int D_0029DE68;
extern int D_0043F348[];
extern void func_0018DC20(void *);
void func_00196DF0(void *arg0);
void func_00196BB0(Obj196BB0 *p)
{
    int a;
    int b;
    int flag;
    struct Display *disp;
    p->x418 = 0;
    p->x464 = 0x46;
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x19, &D_0029DE68, 1);
    b = D_004348F0.xC00;
    a = func_00196198(&D_004348F0);
    if ((unsigned)(D_0043F348[0] - 2) < 3) {
        flag = 0;
        if ((((unsigned char)(D_002AF2C0.objs[0]->x4098 >> 2)) & 1) != 0) {
            func_0020F518(0, 0xA0, 0x64, 0x32, 0xAA);
            func_0020FB80(0, 0x100, 0xC0, -0x5B, -0x20);
            func_0020F9F0(0, D_003F52D0.x20, 0x100, 0xC8, D_0028D980);
            flag = 1;
        }
        if (flag == 0) {
            if (b > 0 && a > 0) {
                func_0018E3A8((struct SharedMenu *)(void *)(p), 0x58, &D_0029DE68, 1);
            }
            func_0018E3A8((struct SharedMenu *)(void *)(p), 0x53, &D_0029DE68, 1);
        }
        func_0018E3A8((struct SharedMenu *)(void *)(p), 0x59, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(p), 0x5A, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(p), 0x29, &D_0029DE68, 1);
        func_0018E3A8((struct SharedMenu *)(void *)(p), 0x22, &D_0029DE68, 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(p), 0x1A, &D_0029DE68, 1);
    disp = &D_003F52D0;
    func_00196DF0(p);
    if (func_00190D60((char *)disp + 0x64, 8, p->x45C) != 0) {
        D_00437340.x500 = -1;
        D_00437340.x508 = -1;
        D_00437340.x504 = -1;
        p->x504 = -1;
        disp->x12D = 1;
        D_004B25A0.x35C = 1;
        func_001C57A8(&D_004B25A0.x4F0, (u128)0x5443454c45535f53UL);
    }
    func_0018DC20(p);
}
void func_00196DF0(void *arg0)
{
    char text[32];
    int count;
    int value;
    int y = 0x3C;
    int selection;
    func_0020FB80(0,0x100,0x90,-0x60,-0x20);
    func_0020F518(0,0xE6,0xE6,0xC8,0x80);
    func_0020F9F0(0,D_003F52D0.x20,0x19A,0x2A,D_0029D0A8);
    func_0020F9F0(0,D_003F52D0.x20,0x19A,0x32,D_0029D0B0);
    func_0020F518(0,0xC8,0xC8,0x64,0x80);
    func_0020F9A8(0,0,0x154,0x3C,D_0029D0B8);
    func_0020F9A8(0,0,0x154,0x44,D_0029D0C0);
    func_0020F9A8(0,0,0x154,0x4C,D_0029D0C8);
    func_0020F9A8(0,0,0x154,0x54,D_0029D0D0);
    func_0020F9A8(0,0,0x154,0x5C,D_0029D0D8);
    func_0020F9A8(0,0,0x154,0x6C,D_0029D0E0);
    value = func_00197518(arg0,D_004B25A0.x2A0,5,&count);
    func_00254D68(text,D_0028D9A8,D_004348F0.xBE4,value);
    func_0020F9A8(0,0,0x19A,0x3C,text);
    value = func_00197518(arg0,D_004B25A0.x2A0,2,&count);
    func_00254D68(text,D_0028D9A8,D_004348F0.xBD8,value);
    func_0020F9A8(0,0,0x19A,0x44,text);
    value = func_00197518(arg0,D_004B25A0.x2A0,0,&count);
    func_00254D68(text,D_0028D9A8,D_004348F0.xBD0,value);
    func_0020F9A8(0,0,0x19A,0x4C,text);
    value = func_00197518(arg0,D_004B25A0.x2A0,3,&count);
    func_00254D68(text,D_0028D9A8,D_004348F0.xBDC,value);
    func_0020F9A8(0,0,0x19A,0x54,text);
    value = func_00197518(arg0,D_004B25A0.x2A0,1,&count);
    func_00254D68(text,D_0028D9A8,D_004348F0.xBD4,value);
    func_0020F9A8(0,0,0x19A,0x5C,text);
    func_00254D68(text,D_0028D9A8,func_00196198(&D_004348F0),count);
    func_0020F9A8(0,0,0x19A,0x6C,text);
    selection = *(int *)((char *)arg0+0x504);
    if(selection >= 0) {
        if(selection == 0) y = 0x4C;
        else if(selection == 1) y = 0x5C;
        else if(selection == 3) y = 0x54;
        else if(selection == 2) y = 0x44;
        func_0020F518(0,0xE6,0xE6,0xE6,0x80);
        func_0020FB80(0,0x100,0x90,-0xC0,-0x20);
        func_0020F9A8(0,0,0x148,y,D_0029D0E8);
    }
}


void func_00197190(Arg97190 *arg0, int arg1)
{
    Obj97190 *obj = (Obj97190 *)arg0;
    struct Display *disp;

    if (arg1 == 0 || (disp = &D_003F52D0, func_00190D60((char *)disp + 0x64, 8, arg0->x45C)) != 0) {
        D_00437340.x500 = -1;
        D_00437340.x508 = -1;
        D_00437340.x504 = -1;
        arg0->x504 = -1;
        D_003F52D0.x12D = 1;
        D_004B25A0.x35C = 1;
        func_001C57A8(&D_004B25A0.x4F0, (u128)0x5443454c45535f53UL);
    } else if (IsType97190(&obj->entries[arg1], 0x53)) {
        func_00197338(arg0);
    } else if (IsType97190(&obj->entries[arg1], 0x59)) {
        func_001973B0(arg0, (char *)arg0 + arg1 * 16);
    } else if (IsType97190(&obj->entries[arg1], 0x29)) {
        func_00190660(disp, D_00386680);
    } else if (IsType97190(&obj->entries[arg1], 0x5A)) {
        func_00190660(disp, D_00437850);
    } else if (IsType97190(&obj->entries[arg1], 0x58)) {
        func_00190660(disp, &D_00437340);
    } else if (IsType97190(&obj->entries[arg1], 0x22)) {
        func_00190660(disp, D_00437D50);
    } else if (IsType97190(&obj->entries[arg1], 0x1A)) {
        func_00190660(disp, D_00438250);
    }
}

int func_00197518(void *arg0, int type, int index, int *out);

void func_00197338(void *arg0)
{
    int limit;
    int count;
    int value;

    limit = func_00196198(&D_004348F0);
    func_00197518(arg0, D_004B2840[0], 0, &count);
    if (limit < count) {
        D_00435F20.x500 = 0;
        func_00190660(&D_003F52D0, &D_00435F20);
    }
}

void func_001973B0(void *arg0, void *arg1)
{
    int count;
    int limit;
    int idx;
    int offset;
    int i;
    Entry973B0 *e;
    Body973B0 *body;
    float tmp[3];

    count = D_004348F0.xC00;
    limit = func_00196198(&D_004348F0);
    offset = 0x9240;
    *(int *)(get_player973B0(D_004B25A0.x2A0) + offset) = limit;
    if (count > 0 && limit > 0) {
        idx = 0;
        for (i = 0; i < count; i++) {
            e = &D_004348F0.entries[i];
            body = e->x8;
            if (body->x0 != 0) {
                tmp[0] = body->x10;
                tmp[1] = body->x14;
                tmp[2] = body->x18;
                func_00195B68((Obj973B0 *)(D_004225A0 + get_offset973B0(D_004B25A0.x2A0)), idx, tmp, e->x4);
                idx++;
            }
        }
    }
    func_00190890(&D_003F52D0);
    func_00187AB0();
}

void func_001974D8(void)
{
    func_001C6E88(D_004B2914);
}

void func_001974F8(void)
{
    func_001C6EA8(D_004B2914);
}

int func_00197518(void *arg0, int type, int index, int *out)
{
    int a[6] = { 46, 15, 30, 18, 0, 35 };
    int b[6] = { 20, 10, 10, 10, 0, 10 };
    int c[6] = { 25, 10, 10, 10, 0, 10 };
    int result = 0;

    switch (type) {
    case 0: case 1: case 2: case 3: case 5: case 11: case 12: case 19: case 20:
        *out = 0x90;
        result = a[index];
        break;
    case 6: case 7: case 8: case 9: case 14: case 15: case 17: case 25:
        *out = 0x3C;
        result = b[index];
        break;
    case 4: case 10: case 13: case 16: case 18: case 22: case 24:
        *out = 0x41;
        result = c[index];
        break;
    }
    return result;
}

/* used by func_00198680; defined here so this unit's .rodata ends on the 16-byte boundary retail has */
const char D_0028DA68[] = "Too many, cant load\n";
