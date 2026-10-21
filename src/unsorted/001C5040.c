/* UNIT 0x001C5040 0x001D53C0 .rodata=0x28FF90:0x2935A0 .sdata=0x29D5A0:0x29D628 */
#include "sound_fades.h"
#include "scalar_api.h"

#if !defined(__mips__)
#define func_001CFA00__c1 func_001CFA00
#endif
#include "skat_queue.h"
#include "sound.h"
#include "include_asm.h"

typedef unsigned int u128 __attribute__((mode(TI)));

/* ---- merged from src/unsorted/001C5040.c ---- */
typedef unsigned int u128__c1 __attribute__((mode(TI)));

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct Mode8380 { char pad0[0x10C]; int x10C; char pad110[0x178]; int x288; char pad28C[0x14]; int x2A0; } Mode8380;
extern Mode8380 D_004B25A0__c1 asm("D_004B25A0");

float D_0029D5A0 = 1.44f;
float D_0029D5A4 = 1.8f;
int D_0029D5A8 = 75;
int D_0029D5AC = 75;
int D_0029D5B0 = 6;
int D_0029D5B4 = 20;
int D_0029D5B8 = 20;
int D_0029D5BC = 40;

extern char D_00398C40[];
extern char D_004225A0[];
extern char D_004B2914[];
struct Obj_C5C40;
void func_001C54A0();
void func_001C6220(void *, int);
void func_001C6258(void *, int, int);
void func_001C65F0(void *, int);
void func_001C5C40(struct Obj_C5C40 *, int, float);
void func_001C6EC8();
struct Obj_C7A60;
void func_001C7A60__c1(struct Obj_C7A60 *obj, int id, float duration);

/* ---- 001C5040 ---- */
extern int func_001C5040__c1(int size, int id) asm("func_001C5040");
int func_001C5040__c1(int size, int id)
{
    static const void *table[14] __attribute__((section(".rodata"), aligned(8))) = {
        &&case1, &&case2, &&case3, &&case4, &&case5, &&case6, &&case7,
        &&case8, &&case9, &&case10, &&case11, &&case12, &&case13, &&case14
    };
    int index;

    index = id - 1;
    size = 0;
    if ((unsigned int)index >= 14)
        goto end;
    goto *table[index];

case1:
    size = 0x20;
    goto end;
case2:
    size = 0x40;
    goto end;
case3:
    size = 0x60;
    goto end;
case4:
    size = 0x80;
    goto end;
case5:
    size = 0xA0;
    goto end;
case6:
    size = 0xC0;
    goto end;
case7:
    size = 0x160;
    goto end;
case8:
    size = 0x120;
    goto end;
case9:
    size = 0xE0;
    goto end;
case10:
    size = 0x100;
    goto end;
case11:
    size = 0x1A0;
    goto end;
case12:
    size = 0x140;
    goto end;
case13:
    size = 0x180;
    goto end;
case14:
    size = 0x200;
end:
    return size;
}

/* ---- 001C50E0 ---- */
typedef struct Obj_C50E0 {char pad[0xC];int xC,x10;unsigned int x14,x18;} Obj_C50E0;
extern char D_004BD840[]; extern int func_002013F8(void); extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern void func_001C50E0(Obj_C50E0 *obj);
void func_001C50E0(Obj_C50E0 *obj){if((obj->x18!=0xFFFFFFFF && obj->x18!=0)||(obj->x14!=0xFFFFFFFF && obj->x14!=0)){func_00208BF8__c1(D_004BD840,&obj->x18);func_00208BF8__c1(D_004BD840,&obj->x14);obj->xC=0;while(func_002013F8());}}
INCLUDE_ASM("asm/nonmatchings/text", func_001C5178);

/* ---- 001C5180 ---- */
typedef struct { char pad0[0x1C]; unsigned int x1C; } Obj_C5180;
extern char D_004BD840[];
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern void func_001C5180(Obj_C5180 *obj);
void func_001C5180(Obj_C5180 *obj) {
    unsigned int value = obj->x1C;
    if (value != 0xFFFFFFFFU && value != 0) func_00208BF8__c1(D_004BD840, &obj->x1C);
}

/* ---- 001C51C0 ---- */
typedef struct Obj_C51C0 {
    char pad0[0x14];
    unsigned int x14;
    unsigned int x18;
    unsigned int x1C;
} Obj_C51C0;

extern char D_004BD840[];
extern int D_004BD8B8[];

extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern int func_002013F8(void);

/* "S_MUSIC_L" packed little-endian into 128 bits */
#define NAME_S_MUSIC_L (((u128__c1)0x4C << 64) | 0x5F434953554D5F53)

extern void func_001C51C0__c1(Obj_C51C0 *obj) asm("func_001C51C0");
void func_001C51C0__c1(Obj_C51C0 *obj)
{
    if (obj->x14 != 0xFFFFFFFF && obj->x14 != 0) {
        func_00208BF8__c1(D_004BD840, &obj->x14);
    }
    if (obj->x18 != 0xFFFFFFFF && obj->x18 != 0) {
        func_00208BF8__c1(D_004BD840, &obj->x18);
    }
    if (obj->x1C == 0) {
        obj->x1C = 0xFFFFFFFF;
        func_00201BD0(D_004BD8B8[0], 0, NAME_S_MUSIC_L, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x1C));
    } else if (obj->x1C != 0xFFFFFFFF && obj->x1C != 0) {
        func_00201D80(obj->x1C, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x1C));
    }
    func_002013F8();
}

/* ---- 001C52D0 ---- */
struct Entry_C52D0 {
    int x0;
    char pad4[0x80];
};
struct Global_C52D0 {
    char pad0[0x5350];
    int x5350;
    char x5354[0x628];
    struct Entry_C52D0 entries[4];
};

extern void func_00235878(void *, int);
extern int func_001BC480(void *, int);
extern void func_001C52D0(void);
void func_001C52D0(void)
{
    struct Global_C52D0 *g = &(*(struct Global_C52D0 *)&D_00398C40);
    int i;
    for (i = 0; i < 4; i++) {
        func_00235798((SkatQueueOwner *)(g->x5354), i);
        func_00235878(g->x5354, i);
        g->entries[i].x0 = 0;
        func_001BC480(&g->x5350, i);
    }
}

/* ---- 001C5358 ---- */
typedef struct Entry_C5358 {
    int x0;
    char pad4[0x40];
    int x44;
    char x48[8];
} Entry_C5358;

typedef struct Obj_C5358 {
    int x0;
    int x4;
    int x8;
    char padC[0x4C];
    Entry_C5358 entries[15];
} Obj_C5358;

typedef struct Unit_C5358 {
    int x0;
    char pad4[0x80];
} Unit_C5358;

typedef struct Global_C5358 {
    char pad0[0x597C];
    Unit_C5358 units[1];
} Global_C5358;

extern char D_004C6370[];
extern char D_004B2A90[];
extern unsigned char D_004B25DC[];
extern int D_004C6780[];

extern int func_00204230(void *);
extern int func_00238F48();
extern int func_001C1F50(void *, int, int, int);
extern void func_001C3740(Entry_C5358 *, int);
extern int func_002013F8(void);

extern void func_001C5358__c1(Obj_C5358 *obj, int arg1) asm("func_001C5358");
void func_001C5358__c1(Obj_C5358 *obj, int arg1)
{
    int i;
    int index;
    int value;
    int handle;
    int tmp;
    char *data;

    if (func_00204230(D_004C6370)) {
        obj->x8 = func_00238F48();
    }
    if (obj->x0 != -1) {
        index = obj->x0;
        data = obj->entries[index].x48;
        /* same as obj->entries[index].x0 (entries at 0x58, stride 80),
           written out so gcc shares one base computation with data */
        value = *(int *)((char *)obj + index * 80 + 0x58);
        tmp = func_001C5040__c1(D_004B2A90, index);
        handle = func_001C1F50(data, 27, value, tmp);
        obj->entries[obj->x0].x44 = handle;
        (*(Global_C5358 *)&D_00398C40).units[handle].x0 = 1;
        obj->x0 = -1;
    }
    for (i = 1; i < 15; i++) {
        if (D_004B25DC[i] && D_004C6780[i] == 0) {
            func_001C3740(&obj->entries[i], arg1);
        }
    }
    func_001C54A0(obj);
    func_002013F8();
}

/* ---- 001C54A0 ---- */
typedef struct { int a,b; } Entry_C54A0;
typedef struct { int x0,x4,x8,xC; unsigned int x10; unsigned int x14,x18,x1C; Entry_C54A0 entries[4]; } Obj_C54A0;
extern int D_0029D9F8;
extern int D_0029DAFC;
extern void func_00202998(int,int,int);
extern int func_002013F8(void);

#if defined(__mips__)
extern void func_00202688(int,int,int,int,int,int,int,int,int,int,void (*)(void),unsigned long);
#else

#endif

extern void func_00201CB8(unsigned int);
extern void func_001C54A0(Obj_C54A0 *obj);
void func_001C54A0(Obj_C54A0 *obj) {
 unsigned int handle;
 if(obj->x4) {
 if(!D_0029D9F8) {
 if(!D_0029DAFC) goto normal;
 } else { obj->x14=0; obj->x18=0; obj->xC=0; }
 func_00202998(2,0,0);
 goto end;
 normal:;
 func_00202998(2,0x51E,0x51E);
 handle=obj->x18;
 if(handle!=0xFFFFFFFF && handle!=0) { obj->x18=0xFFFFFFFF; func_00201D80(handle, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x18)); }
 handle=obj->x14;
 if(handle!=0xFFFFFFFF && handle!=0) { obj->x14=0xFFFFFFFF; func_00201D80(handle, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x14)); }
 while(func_002013F8()) {}
 if(obj->x18==0) {
 if(obj->x10==0) obj->x10=1; else if(obj->x10==1 || obj->x10==2) obj->x10=3; else if(obj->x10==3) obj->x10=0;
 if(obj->x10==1) obj->xC=obj->x10; else obj->xC=0;
 if(obj->xC) {
 obj->x18=0xFFFFFFFF; obj->x14=0xFFFFFFFF;
 func_00202688(obj->entries[1].a,obj->entries[1].b,0,0,0x28B,0,1,0,0,0,func_00208C50,(unsigned long)(unsigned int)&obj->x14);
 func_00202688(obj->entries[2].a,obj->entries[2].b,0,0,0x28B,0,13,0,0,5,func_00208C50,(unsigned long)(unsigned int)&obj->x18);
 } else {
 obj->x18=0xFFFFFFFF;
 func_00202688(obj->entries[obj->x10].a,obj->entries[obj->x10].b,0,0,0x28B,0,1,0,0,4,func_00208C50,(unsigned long)(unsigned int)&obj->x18);
 }
 }
 if(obj->x14==0 && obj->xC) { obj->xC=0; obj->x10=2; func_00201CB8(obj->x18); }
 end:;
 }
}

/* ---- 001C5728 ---- */
typedef struct Obj_C5728 { char pad0[12]; int xC; int x10, x14, x18, x1C; } Obj_C5728;
extern char D_004BD840[], D_004B2914[];
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern int func_002013F8(void);
extern void func_001C5728(Obj_C5728 *obj);
void func_001C5728(Obj_C5728 *obj) {
    obj->xC = 0;
    func_00208BF8__c1(D_004BD840, &obj->x18);
    func_00208BF8__c1(D_004BD840, &obj->x1C);
    func_00208BF8__c1(D_004BD840, &obj->x14);
    func_001C6EC8(D_004B2914);
    while (func_002013F8()) {}
}

/* ---- 001C57A8 ---- */
extern int D_004BD8B0[], D_004BD8A8[];
extern void func_001C57A8(int unused, u128__c1 value);
void func_001C57A8(int unused, u128__c1 value) {
 if(D_004BD8B0[0]) { func_00201B68(D_004BD8B0[0], 0, value, 0x400, 0, 0, 0); return; }
 if(D_004BD8A8[0]) func_00201B68(D_004BD8A8[0], 0, value, 0x400, 0, 0, 0);
}

/* ---- 001C5818 ---- */
typedef struct Obj_C5818 {
    char pad0[0x40];
    unsigned int x40;
} Obj_C5818;

extern int D_004BD8B4[];

/* "S_ENV_FAR_L" packed little-endian into a 128-bit value */
#define NAME_S_ENV_FAR_L (((u128__c1)0x4C5F52LL << 64) | (u128__c1)0x41465F564E455F53LL)

extern void func_001C5818__c1(Obj_C5818 *obj) asm("func_001C5818");
void func_001C5818__c1(Obj_C5818 *obj)
{
    if (obj->x40 == 0) {
        func_00201BD0(D_004BD8B4[0], 0, NAME_S_ENV_FAR_L, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x40));
        obj->x40 = 0xFFFFFFFF;
    }
}

/* ---- 001C58A8 ---- */
typedef struct Obj_C58A8 {
    char pad0[0x44];
    unsigned int x44;
} Obj_C58A8;

extern int D_004BD8B4[];

/* "S_ENV_CLOSE_L" packed little-endian into a 128-bit value */
#define NAME_S_ENV_CLOSE_L (((u128__c1)0x4C5F45534FLL << 64) | (u128__c1)0x4C435F564E455F53LL)

extern void func_001C58A8__c1(Obj_C58A8 *obj) asm("func_001C58A8");
void func_001C58A8__c1(Obj_C58A8 *obj)
{
    if (obj->x44 == 0) {
        obj->x44 = 0xFFFFFFFF;
        func_00201BD0(D_004BD8B4[0], 0, NAME_S_ENV_CLOSE_L, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x44));
    }
}
INCLUDE_ASM("asm/nonmatchings/text", func_001C5938);

/* ---- 001C5940 ---- */
extern void func_001C5940(void);
void func_001C5940(void) {
}

/* ---- 001C5948 ---- */
extern void func_001C5948(void);
void func_001C5948(void) {
}

/* ---- 001C5950 ---- */
extern void func_001C5950(void);
void func_001C5950(void) {
}

/* ---- 001C5958 ---- */
extern void func_001C5958(void);
void func_001C5958(void) {
}

/* ---- 001C5960 ---- */
typedef struct Obj_C5960 {
    char pad0[0x40];
    int x40;
} Obj_C5960;

extern char D_004BD840[];
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");

extern void func_001C5960(Obj_C5960 *obj);
void func_001C5960(Obj_C5960 *obj)
{
    func_00208BF8__c1(D_004BD840, &obj->x40);
}

/* ---- 001C5988 ---- */
typedef struct Obj_C5988 {
    char pad0[0x44];
    int x44;
} Obj_C5988;

extern char D_004BD840[];
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");

extern void func_001C5988(Obj_C5988 *obj);
void func_001C5988(Obj_C5988 *obj)
{
    func_00208BF8__c1(D_004BD840, &obj->x44);
}

/* ---- 001C59B0 ---- */
typedef struct Obj_C59B0 {
    char pad0[0x48];
    int x48;
} Obj_C59B0;

extern char D_004BD840[];
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");

extern void func_001C59B0(Obj_C59B0 *obj);
void func_001C59B0(Obj_C59B0 *obj)
{
    func_00208BF8__c1(D_004BD840, &obj->x48);
}

/* ---- 001C59D8 ---- */
typedef struct Obj_C59D8 {
    char pad0[0x4C];
    int x4C;
} Obj_C59D8;

extern char D_004BD840[];
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");

extern void func_001C59D8(Obj_C59D8 *obj);
void func_001C59D8(Obj_C59D8 *obj)
{
    func_00208BF8__c1(D_004BD840, &obj->x4C);
}

/* ---- 001C5A00 ---- */
struct Obj001C5A00_C5A00 {
    char pad0[0xBC];
    int xBC;
};

extern void func_002088D0(struct Obj001C5A00_C5A00 *obj);

extern struct Obj001C5A00_C5A00 *func_001C5A00(struct Obj001C5A00_C5A00 *obj);
struct Obj001C5A00_C5A00 *func_001C5A00(struct Obj001C5A00_C5A00 *obj)
{
    func_002088D0(obj);
    obj->xBC = 0;
    return obj;
}

/* ---- 001C5A30 ---- */
extern void func_00201948(int a, int b);
extern void func_00202960(int a, int b);
extern void func_00201AB8(int a);
extern void func_00201A78(int a, int b, int c);

extern void func_001C5A30(void *obj);
void func_001C5A30(void *obj)
{
    func_00201948(0x10, 0x400);
    func_001C6220(obj, 0x200);
    func_001C6258(obj, 0x3C3, 0);
    func_001C65F0(obj, 0x399);
    func_00202960(2, 3);
    func_00201AB8(1);
    func_00201A78(1, 0x18, 0x2F);
    func_00201A78(6, 0x18, 0x2F);
    func_00201A78(8, 0x18, 0x2F);
}

/* ---- 001C5AC8 ---- */
struct Game_C5AC8 {
    char pad0[0x5350];
    int x5350;
    int x5354;
};

typedef struct Obj_C5AC8 {
    char pad0[0xAC];
    int xAC;
} Obj_C5AC8;

extern void func_00201D50(unsigned int mask);
extern int func_002013F8(void);

extern void func_00235878(void *a, int b);
extern int func_001BC480(void *a, int b);

extern void func_001C5AC8(Obj_C5AC8 *obj);
void func_001C5AC8(Obj_C5AC8 *obj)
{
    struct Game_C5AC8 *g;
    int i;

    i = 0;
    obj->xAC = 0;
    func_00201D50(~2);
    g = &(*(struct Game_C5AC8 *)&D_00398C40);
    for (; i < 4; i++) {
        func_00235798((SkatQueueOwner *)(&g->x5354), i);
        func_00235878(&g->x5354, i);
        func_001BC480(&g->x5350, i);
    }
    while (func_002013F8() != 0) {
    }
}

/* ---- 001C5B60 ---- */
extern void func_001C5B60(void *obj);
void func_001C5B60(void *obj)
{
    func_001C5C40(obj, 2, 0.0f);
}

/* ---- 001C5B80 ---- */
struct Global_C5B80 {
    char pad0[0x5350];
    int x5350;
    int x5354;
    char pad5358[0x5B84 - 0x5358];
    int x5B84;
};

typedef struct Obj_C5B80 {
    char pad0[0xAC];
    int xAC;
    int xB0;
} Obj_C5B80;

extern void func_00201D50(unsigned int mask);
extern int func_002013F8(void);

extern void func_00235878(void *a, int b);
extern int func_001BC480(void *a, int b);
extern int func_00238F48();

static inline struct Global_C5B80 *global(void) { return &(*(struct Global_C5B80 *)&D_00398C40); }

/* The pointer is fetched inside a do/while(0) (macro-style): the loop-end note
   stops cse there, so the calls don't fold &g->x5354 into a constant, and the
   post-loop &(*(struct Global_C5B80 *)&D_00398C40) keeps its own %lo instead of reusing the loop's register. */
#define GET_GLOBAL(g) do { (g) = global(); } while (0)

extern void func_001C5B80(Obj_C5B80 *obj) asm("func_001C5B80");
void func_001C5B80(Obj_C5B80 *obj)
{
    struct Global_C5B80 *g;
    int i;

    func_00201D50(~0x200);
    for (i = 0; i < 4; i++) {
        GET_GLOBAL(g);
        func_00235798((SkatQueueOwner *)(&g->x5354), i);
        func_00235878(&g->x5354, i);
        func_001BC480(&g->x5350, i);
    }
    global()->x5B84 = 1;
    obj->xAC = 1;
    obj->xB0 = func_00238F48();
    while (func_002013F8() != 0) {
    }
}

/* ---- 001C5C40 ---- */
struct Obj_C5C40 { char pad[0x7c]; float x7c; int x80,x84,x88,x8c,x90,x94,x98,x9c,xa0,xa4,xa8,xac,xb0; };
struct Values_C5C40 { int x0,x4,x8; };
struct Settings_C5C40 { char pad[0x11a60]; struct Values_C5C40 values; };
struct State_C5C40 { char pad[0x118]; int x118; char pad2[0x3e8]; int x504; };
extern int func_00238F48() asm("func_00238F48"),func_002013F8(void) asm("func_002013F8");
extern void func_00201D20(unsigned int);
static inline struct Values_C5C40 *get_values(struct Settings_C5C40 *g) {return &g->values;}
extern void func_001C5C40(struct Obj_C5C40 *o,int mode,float value);
void func_001C5C40(struct Obj_C5C40 *o,int mode,float value) {
 struct Values_C5C40 *v;
 o->x7c=value; o->x80=func_00238F48(); o->x84=1; o->x88=mode; o->xac=0;
 if(mode==1) {o->x90=0;o->x94=0;o->x98=0;} else {struct Values_C5C40 *initial;initial=get_values(&(*(struct Settings_C5C40 *)&D_004225A0));o->x90=initial->x8;o->x94=initial->x4;o->x98=initial->x0;}
 func_001C6220(o,o->x90);func_001C6258(o,o->x94,1);func_001C65F0(o,o->x98);
 do {} while(func_002013F8());
 v=get_values(&(*(struct Settings_C5C40 *)&D_004225A0));
 o->x9c=(int)(v->x8*0.041666668f);o->xa0=(int)(v->x4*0.041666668f);o->xa4=(int)(v->x0*0.041666668f);
 if(o->x88==1) {if((*(struct State_C5C40 *)&D_004B25A0__c1).x118==2) {if((*(struct State_C5C40 *)&D_004B25A0__c1).x504) func_00201D20(0xffffdfffU);else func_00201D20(0xffffffffU);}else func_00201D20(0xffffffffU);}
 do {} while(func_002013F8());
}
struct Obj_C6628;
extern void func_001C6628(struct Obj_C6628 *);
/* Unprototyped here so ~0x200 is passed as unsigned (lui/ori, as in retail). */
extern void func_00201CF0() asm("func_00201CF0");
extern void func_001C5DF0(void *object);
void func_001C5DF0(void *object) {
    struct Obj_C5C40 *o = object;
    int n, x, y, z;
    if (o->x84 != 0) {
        int elapsed = func_00238F48() - o->x80;
        if (o->x88 == 1) {
            union { char *ptr; unsigned int address; } u;
            struct Values_C5C40 *v;
            int target8, target4;
            u.ptr = D_004225A0;
            v = (struct Values_C5C40 *)(u.address + 0x11a60);
            o->x90 += o->x9c;
            o->x94 += o->xa0;
            o->x98 += o->xa4;
            if (v->x8 < o->x90) o->x90 = v->x8;
            if (v->x4 < o->x94) o->x94 = v->x4;
            if (v->x0 < o->x98) o->x98 = v->x0;
            /* The target is read first (comma) but compared second, as in retail. */
            if ((target8 = v->x8, o->x90 == target8) && (target4 = v->x4, o->x94 == target4)
                && o->x98 == v->x0)
                o->x8c = 1;
        } else {
            if (!(o->x7c * 60.0f < (float)elapsed)) goto write_volume;
            x = o->x90 - o->x9c;
            y = o->x94 - o->xa0;
            z = o->x98 - o->xa4;
            o->x90 = x;
            o->x94 = y;
            o->x98 = z;
            if (x < 0) o->x90 = 0;
            if (o->x94 < 0) o->x94 = 0;
            if (o->x98 < 0) o->x98 = 0;
            if (o->x90 == 0 && o->x94 == 0 && o->x98 == 0)
                o->x8c = 1;
        }
write_volume:
        func_001C6220(o, o->x90);
        func_001C6258(o, o->x94, 1);
        func_001C65F0(o, o->x98);
    }
    if (o->x8c != 0) {
        n = o->x88;
        o->x8c = 0;
        o->x84 = 0;
        if (n == 0) {
            func_00201CF0(~0x200U);
            do {} while (func_002013F8());
            o->xac = 1;
            o->xb0 = func_00238F48();
        } else if (n == 1) {
            union { char *ptr; unsigned int address; } u;
            struct Values_C5C40 *v;
            u.ptr = D_004225A0;
            v = (struct Values_C5C40 *)(u.address + 0x11a60);
            func_001C6220(o, v->x8);
            func_001C6258(o, v->x4, 1);
            func_001C65F0(o, v->x0);
            do {} while (func_002013F8());
        } else {
            struct Global_C5B80 *g;
            int i;
            func_00201D50(~0x200);
            for (i = 0; i < 4; i++) {
                GET_GLOBAL(g);
                func_00235798((SkatQueueOwner *)(&g->x5354), i);
                func_00235878(&g->x5354, i);
                func_001BC480(&g->x5350, i);
            }
            global()->x5B84 = 1;
            o->xac = 1;
            o->xb0 = func_00238F48();
        }
    }
    if (o->xac != 0) {
        int elapsed = func_00238F48() - o->xb0;
        if (elapsed < 0) func_001C6628((struct Obj_C6628 *)o);
        if ((float)elapsed > 48.0f) {
            union { char *ptr; unsigned int address; } u;
            struct Values_C5C40 *v;
            int target8, target4;
            u.ptr = D_004225A0;
            v = (struct Values_C5C40 *)(u.address + 0x11a60);
            o->x90 += o->x9c;
            o->x94 += o->xa0;
            o->x98 += o->xa4;
            if (v->x8 < o->x90) o->x90 = v->x8;
            if (v->x4 < o->x94) o->x94 = v->x4;
            if (v->x0 < o->x98) o->x98 = v->x0;
            /* The target is read first (comma) but compared second, as in retail. */
            if ((target8 = v->x8, o->x90 == target8) && (target4 = v->x4, o->x94 == target4)
                && o->x98 == v->x0)
                o->xac = 0;
            func_001C6220(o, o->x90);
            func_001C6258(o, o->x94, 1);
            func_001C65F0(o, o->x98);
        }
    }
}



/* ---- 001C6220 ---- */
extern void func_00201948(int, int);
extern void func_001C6220(void *unused, int value);
void func_001C6220(void *unused, int value) {
    func_00201948(1, value);
    func_00201948(13, value);
}

/* ---- 001C6258 ---- */
extern void func_00201948(int,int);
extern void func_001C6258(void *unused,int value,int flag);
void func_001C6258(void *unused,int value,int flag) {
 func_00201948(0,value);
 func_00201948(3,value);
 func_00201948(4,value);
 func_00201948(5,value);
 func_00201948(6,(int)(value*0.7f));
 func_00201948(7,value);
 func_00201948(14,value);
 if(!flag) func_00201948(9,value);
}
/* ---- 001C6310 ---- */
typedef struct { char pad0[0x14C]; unsigned int handle; char x150[4]; } Obj_C6310;
typedef struct { int x0, x4; char pad8[0xC]; int x14; } Sub_C6310;
typedef struct { char pad0[0x118]; int x118; char pad11C[0x3D4]; Sub_C6310 x4F0; } State_C6310;
struct Root_C6310 { char pad0[8]; char x8[1]; };
static __inline__ struct Root_C6310 *GetRoot_C6310(void) { return (struct Root_C6310 *)D_00398C40; }
extern char D_004BD840[];
extern int D_004BD8A8[], D_004BD898__c1[] asm("D_004BD898");
extern void func_00201D20(unsigned int) asm("func_00201D20"), func_00201CF0(int) asm("func_00201CF0"), func_001DEE20(void *) asm("func_001DEE20");
extern void func_001BC968(void *) asm("func_001BC968"), func_001BCA88(void *) asm("func_001BCA88");
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
#define STATE_C6310 (*(State_C6310 *)&D_004B25A0__c1)
#define NAME_S_HELI_SLIDER (((u128__c1)0x524544494cUL << 64) | (u128__c1)0x535f494c45485f53UL)
/* Start the stream once: the handle is set to -1 while it loads. */
#define START_C6310(id) { if (obj->handle == 0) { obj->handle = 0xFFFFFFFF; \
    func_00201BD0(id, 0, NAME_S_HELI_SLIDER, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->handle)); } }
extern void func_001C6310__c1(Obj_C6310 *obj, int code) asm("func_001C6310");
void func_001C6310__c1(Obj_C6310 *obj, int code) {
    Sub_C6310 *p;
    if (STATE_C6310.x118 != 2) {
        if (code == 0x4C) { func_00201D20(2); func_001DEE20(GetRoot_C6310()->x8); }
        else func_00201CF0(2);
        if (code == 0x4E) { START_C6310(D_004BD8A8[0]); }
        else func_00208BF8__c1(D_004BD840, &obj->handle);
        if (code != 0x4D) { func_001BCA88(obj->x150); return; }
        goto active;
    }
    if (code == 0x4C) {
        p = &STATE_C6310.x4F0;
        if (p->x14 == 0) func_00201D20(0x2000);
        p->x4 = 1;
        func_00201D20(2);
        func_001C54A0((Obj_C54A0 *)p);
    } else STATE_C6310.x4F0.x4 = 0;
    if (code == 0x4E) { func_00201CF0(2); START_C6310(D_004BD898__c1[0]); }
    else func_00208BF8__c1(D_004BD840, &obj->handle);
    if (code == 0x4D) {
        func_00201CF0(2);
active:
        func_001BC968(obj->x150);
        return;
    }
    func_001BCA88(obj->x150);
}

/* ---- 001C6558 ---- */
extern int D_004B26B8[];extern char D_004BD840[];extern void func_001BCA88(void*);extern int func_002013F8(void);extern void func_00201CF0(int);extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern void func_001C6558(char *obj);
void func_001C6558(char *obj){if(D_004B26B8[0]!=2){func_00201CF0(2);func_00208BF8__c1(D_004BD840,obj+0x14C);func_001BCA88(obj+0x150);}else{func_00201CF0(2);func_00208BF8__c1(D_004BD840,obj+0x14C);func_001BCA88(obj+0x150);}while(func_002013F8());}

/* ---- 001C65F0 ---- */
extern void func_00201948(int, int);
extern void func_001C65F0(void *unused, int value);
void func_001C65F0(void *unused, int value) {
    func_00201948(2, value);
    func_00201948(8, value);
}

/* ---- 001C6628 ---- */
struct Params_C6628 { int x0,x4,x8; };
struct Global_C6628 { char pad[0x11a60]; struct Params_C6628 params; };
struct Obj_C6628 { char pad[0x7c]; int x7c,x80,x84,x88,x8c; char p90[12]; int x9c,xa0,xa4,xa8,xac; };
extern int func_00238F48();
extern void func_001C6628(struct Obj_C6628 *o);
void func_001C6628(struct Obj_C6628 *o) {
 union { struct Global_C6628 *base; unsigned int address; } u_C6628;
 struct Params_C6628 *p;
 u_C6628.base=&(*(struct Global_C6628 *)&D_004225A0);
 p=(struct Params_C6628 *)(u_C6628.address+0x11a60);
 func_001C6220(o,p->x8);
 func_001C6258(o,p->x4,0);
 func_001C65F0(o,p->x0);
 o->xac=0; o->x7c=0;
 { int result=func_00238F48();
 o->x88=2; o->x8c=0; o->x84=0; o->x9c=0; o->xa0=0; o->xa4=0; o->x80=result; }
}

/* ---- 001C66B8 ---- */
typedef struct Obj_C66B8 { char pad[0x7C]; int x7C,x80,x84,x88,x8C; char pad90[12]; int x9C,xA0,xA4,xA8,xAC; int xB0,xB4,xB8,xBC; float xC0,xC4; char padC8[0x84]; int x14C; char pad150[12]; int x15C,x160,x164,x168,x16C[4]; } Obj_C66B8;
typedef struct Settings_C66B8 { int x0,x4,x8; char pad[0x1B]; unsigned char x27; } Settings_C66B8;
typedef struct Sound_C66B8 { int x0,x4,x8,xC,x10,x14; } Sound_C66B8;
extern float D_0029E4E4,D_0029E4E8,D_0029E4EC,D_0029E4F0,D_0029E4F4,D_0029E4F8;
extern int D_004B2840__c1[] asm("D_004B2840");
static inline int get_mode(int *p) { return *p; }
static inline void clear4(int *p) { int i=3; do { p[i]=0; } while(--i>=0); }
typedef struct Global_C66B8 { char pad[0x11A60]; Settings_C66B8 x11A60; } Global_C66B8;
static inline Settings_C66B8 *get_settings(Global_C66B8 *g) { return &g->x11A60; }
extern void func_00201A30(unsigned char);
extern void func_00202998(int,int,int);
extern void func_002019A8(int,Sound_C66B8 *);
extern long func_001FB6E0(long,int);
extern int func_00238F48();
extern void func_001C66B8(Obj_C66B8 *obj);
void func_001C66B8(Obj_C66B8 *obj) {
    Sound_C66B8 sound; Settings_C66B8 *settings; int volume,i,*p;
    D_0029E4E4=1.8f; D_0029E4E8=1.6f; D_0029E4EC=1.4f; D_0029E4F0=1.2f; D_0029E4F4=1.0f;
    obj->xA8=1;
    if(get_mode(D_004B2840__c1)==1) D_0029E4E8=2.0f;
    settings=get_settings(&(*(Global_C66B8 *)&D_004225A0));
    D_0029E4F8=0.06f;
    func_00201A30(settings->x27);
    func_001C6220(obj,settings->x8); func_001C6258(obj,settings->x4,0); func_001C65F0(obj,settings->x0);
    volume=(int)(D_0029E4F8*32767.0f);
    func_00202998(2,volume,volume);
    i=3; do { obj->xC4=(float)volume; obj->xC0=(float)volume; } while(--i>=0);
    clear4(obj->x16C);
    obj->x164=0; obj->x14C=0; obj->x15C=0; obj->x168=0;
    sound.x0=12; sound.x4=3; sound.x8=0x8CCC; sound.xC=0x8000; sound.x10=0x18000; sound.x14=0;
    func_002019A8(0,&sound);
    sound.x0=11; sound.x4=128; sound.x8=(int)func_001FB6E0((long)D_0029D5A8<<16,100); sound.xC=0x30000; sound.x10=0x10000; sound.x14=0;
    func_002019A8(1,&sound);
    sound.x0=10; sound.x4=2; sound.x8=(int)func_001FB6E0((long)D_0029D5AC<<16,100); sound.xC=0x10000; sound.x10=0x8000; sound.x14=0;
    func_002019A8(2,&sound);
    sound.x0=4; sound.x4=2; sound.x8=(int)func_001FB6E0((long)D_0029D5AC<<16,100); sound.xC=0x10000; sound.x10=0x8000; sound.x14=0;
    func_002019A8(3,&sound);
    obj->x7C=0; obj->x80=func_00238F48(); obj->x88=2; obj->x84=0; obj->x9C=0; obj->xA0=0; obj->xA4=0; obj->x8C=0; obj->xAC=0; obj->xB8=0; obj->xB4=0;
}

/* ---- 001C69A0 ---- */
typedef struct { char pad0[0xC8]; int xC8; } Obj_C69A0;
extern int func_002559D8(void);
extern void func_001C69A0(Obj_C69A0 *obj);
void func_001C69A0(Obj_C69A0 *obj) {
    obj->xC8 = func_002559D8() % 3;
}

/* ---- 001C69E0 ---- */
typedef struct { char pad[0xB4]; int xB4,xB8; char padBC[0x9C]; int x158; } Obj_C69E0;
extern int D_004BD8A0[];
extern void func_001C69E0__c1(Obj_C69E0 *obj,int value) asm("func_001C69E0");
void func_001C69E0__c1(Obj_C69E0 *obj,int value) {
 if (!obj->x158) {
 func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x30315245 << 64) | (u128__c1)0x53555F4D55445F50ULL, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x158));
 obj->xB4=value; obj->xB8=1;
 }
}
INCLUDE_ASM("asm/nonmatchings/text", func_001C6A78);

/* ---- 001C6A80 ---- */
typedef struct Obj_C6A80 {
    char pad0[0xB4];
    int xB4, xB8;
    char padBC[4];
    float xC0;
    char padC4[0x88];
    unsigned int x14C;
    char pad150[8];
    int x158, x15C, x160;
    unsigned int x164;
    int x168, x16C[4];
} Obj_C6A80;
typedef struct Game_C6A80 { char pad0[0x488]; int x488; } Game_C6A80;
extern char D_004BD840[];
extern int D_004BD8A0[];
extern int D_004B2828[];
extern Game_C6A80 D_0043EEC0;
extern float D_0029E4E4, D_0029E4E8, D_0029E4EC, D_0029E4F0, D_0029E4F4, D_0029E4F8;
extern int func_001BC618(void *, int);
extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern void func_001C5DF0(void *);
extern void func_00208B40(void *);
extern void func_00202998(int, int, int);
extern int func_001DEDE0(void *);
extern int func_002013F8(void);

static inline void *member(char *g, int offset) { return g + offset; }
static inline int lookup(char *g, int key) { return func_001BC618(g + 0x5350, key); }

extern void func_001C6A80__c1(Obj_C6A80 *obj) asm("func_001C6A80");
void func_001C6A80__c1(Obj_C6A80 *obj)
{
    int volume;
    int mode;
    int i;

    if (obj->xB8) {
        if (lookup(D_00398C40, obj->xB4) == 5) {
            obj->xB8 = 0;
            func_00208BF8__c1(D_004BD840, &obj->x158);
        }
    }
    func_001C5DF0(((int *)D_004B2914));
    func_00208B40(D_004BD840);

    mode = obj->x16C[0];
    if (mode == 0x644) volume = (int)(D_0029E4E4 * (D_0029E4F8 * 32767.0f));
    else if (mode == 0x643) volume = (int)(D_0029E4E8 * (D_0029E4F8 * 32767.0f));
    else if (mode == 0x642) volume = (int)(D_0029E4EC * (D_0029E4F8 * 32767.0f));
    else if (mode == 0x641) volume = (int)(D_0029E4F0 * (D_0029E4F8 * 32767.0f));
    else if (mode == 0x640) volume = (int)(D_0029E4F4 * (D_0029E4F8 * 32767.0f));
    else if (mode == 0x6A4) volume = 0;
    else volume = (int)(D_0029E4F8 * 32767.0f);

    if (D_004B2828[0] == 1) {
        float f = (float)volume;
        if (obj->xC0 != f) {
            func_00202998(2, volume, volume);
            obj->xC0 = f;
        }
    }
    for (i = 0; i < 4; i++) obj->x16C[i] = 0;

    if ((D_0043EEC0.x488 < 1 || D_0043EEC0.x488 > 3) && func_001DEDE0(member(D_00398C40, 8))) {
        if (obj->x14C == 0) {
            obj->x14C = 0xFFFFFFFF;
            func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x5245564F594CULL << 64) | (u128__c1)0x465F494C45485F45ULL, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x14C));
        }
    } else {
        func_00208BF8__c1(D_004BD840, &obj->x14C);
    }

    if (D_0043EEC0.x488 < 1 || D_0043EEC0.x488 > 3) {
        if (obj->x15C == 0) {
            func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x30315245 << 64) | (u128__c1)0x53555F4D55445F50ULL, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x158));
            obj->x15C = 1;
        }
    } else if (obj->x15C) {
        func_00208BF8__c1(D_004BD840, &obj->x158);
        obj->x15C = 0;
        obj->x168 = 0;
    }

    if (D_0043EEC0.x488 >= 1 && D_0043EEC0.x488 <= 3) {
        if (obj->x160 == 0 && obj->x164 == 0) {
            obj->x164 = 0xFFFFFFFF;
            func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x39305245 << 64) | (u128__c1)0x53555F4D55445F50ULL, 0x400, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x164));
        } else if (obj->x160 == 1) {
            func_00208BF8__c1(D_004BD840, &obj->x164);
        }
    }
    func_002013F8();
}

/* ---- 001C6E88 ---- */
extern void func_001C6E88(void *obj);
void func_001C6E88(void *obj) {
    func_001C5C40(obj, 0, 0.0f);
}

/* ---- 001C6EA8 ---- */
extern void func_001C6EA8(void *obj);
void func_001C6EA8(void *obj) {
    func_001C5C40(obj, 1, 0.0f);
}

/* ---- 001C6EC8 ---- */
struct Obj_C6EC8 { char pad0[0xBC]; int xBC; };
extern void func_002025D8(struct Obj_C6EC8 *);
extern void func_001C6EC8(struct Obj_C6EC8 *obj);
void func_001C6EC8(struct Obj_C6EC8 *obj) {
    if (obj->xBC != 0) {
        func_002025D8(obj);
        obj->xBC = 0;
    }
}

/* ---- 001C6F00 ---- */
typedef struct { char pad[0xBC]; int xBC; } Obj_C6F00;
extern int D_004BD898__c1[] asm("D_004BD898");
extern void func_00202998(int, int, int);
extern void func_00208A20(void *);
extern int func_002013F8(void);
extern void func_002024D0(int, int, int, int);
extern void func_00208900(void *, void *, void *);
extern void func_002018E8(void);

const char sRiderBank[] = "Rider_S.bnk";
const char sPckpBank[] = "Pckp_S.bnk";
const char sPauseBank[] = "Pause_S.bnk";
const char sMoabBank[] = "Moab_S.bnk";
const char sGlacBank[] = "Glac_S.bnk";
const char sGlacTDBank[] = "GlacTD_S.bnk";
const char sJungleBank[] = "Jungle_S.bnk";
const char sFujiBank[] = "Fuji_S.bnk";
const char sAlpsBank[] = "Alps_S.bnk";
const char sAubeBank[] = "Aube_S.bnk";
const char sPeruBank[] = "Peru_S.bnk";
const char sCityBank[] = "City_S.bnk";
const char sCityMXBank[] = "CityMX_S.bnk";
const char sCityTDBank[] = "CityTD_S.bnk";
const char sBCBank[] = "BC_S.bnk";
const char sMoshBank[] = "Mosh_S.bnk";
const char sPodiumBank[24] = "Podium_S.bnk";

extern void func_001C6F00(Obj_C6F00 *obj);
void func_001C6F00(Obj_C6F00 *obj) {
    static const void *bankForTrack[33] __attribute__((section(".rodata"))) = {
        &&l_alps, &&l_moab, &&l_jungle, &&l_peru, &&l_peru,
        &&l_glac, &&l_alps, &&l_moab, &&l_jungle, &&l_peru,
        &&l_jungle, &&l_aube, &&l_fuji, &&l_moab, &&l_glac,
        &&l_aube, &&l_aube, &&l_citymx, &&l_citytd, &&l_bc,
        &&l_city, &&l_fuji, &&l_glactd, &&l_mosh, &&l_alps,
        &&l_bc, &&l_fuji, &&l_alps, &&l_alps, &&l_podium,
        &&l_podium, &&l_podium, &&l_podium,
    };
    func_00208A20(obj);
    while (func_002013F8() != 0) {
    }
    func_00202998(2, 0, 0);
    if (obj->xBC == 0) {
        obj->xBC = 1;
        func_002024D0(6, 0xA000, 0, 1);
    }
    func_00208900(obj, D_004BD898__c1 + 3, (char *)sRiderBank);
    func_00208900(obj, D_004BD898__c1, (char *)sPckpBank);
    func_00208900(obj, D_004BD898__c1 + 4, (char *)sPauseBank);
    if ((unsigned int)D_004B25A0__c1.x2A0 < 0x21) {
        goto *bankForTrack[D_004B25A0__c1.x2A0];
    }
    goto l_end;
l_moab:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sMoabBank);
    goto l_end;
l_glac:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sGlacBank);
    goto l_end;
l_glactd:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sGlacTDBank);
    goto l_end;
l_jungle:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sJungleBank);
    goto l_end;
l_fuji:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sFujiBank);
    goto l_end;
l_aube:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sAubeBank);
    goto l_end;
l_peru:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sPeruBank);
    goto l_end;
l_city:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sCityBank);
    goto l_end;
l_citymx:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sCityMXBank);
    goto l_end;
l_citytd:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sCityTDBank);
    goto l_end;
l_bc:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sBCBank);
    goto l_end;
l_mosh:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sMoshBank);
    goto l_end;
l_alps:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sAlpsBank);
    goto l_end;
l_podium:
    func_00208900(obj, D_004BD898__c1 + 2, (char *)sPodiumBank);
    goto l_end;
l_end:
    func_002018E8();
    while (func_002013F8() != 0) {
    }
}

/* ---- 001C71D0 ---- */
extern int D_004BD8B0[];
extern void func_00208900(void *, void *, void *);
extern void func_002018E8(void);
extern void func_001C71D0(void *obj);
void func_001C71D0(void *obj) {
    func_00208900(obj, D_004BD8B0, "Shell_s.bnk");
    func_002018E8();
}

/* ---- 001C7200 ---- */
typedef struct { char pad[0xBC]; int xBC; } Obj_C7200;
extern int D_004BD8B0[];
extern void func_00202998(int, int, int);
extern void func_00208A20(void *);
extern void func_002024D0(int, int, int, int);
extern void func_00208900(void *, void *, void *);
extern void func_002018E8(void);
extern void func_001C7200(Obj_C7200 *obj);
void func_001C7200(Obj_C7200 *obj) {
    func_00202998(2, 0x51E, 0x51E);
    func_00208A20(obj);
    if (obj->xBC == 0) {
        obj->xBC = 1;
        func_002024D0(6, 0xA000, 0, 1);
    }
    func_00208900(obj, D_004BD8B0, "Shell_s.bnk");
    func_00208900(obj, D_004BD8B0 + 1, "Shell_ch.bnk");
    func_002018E8();
}

/* ---- 001C72A0 ---- */
extern int D_004BD8B0[];
extern void func_00208A20(void *);
extern void func_00208900(void *, void *, void *);
extern void func_001C72A0(void *obj);
void func_001C72A0(void *obj) {
 func_00208A20(obj);
 func_00208900(obj,D_004BD8B0,"Shell_s.bnk");
 func_00208900(obj,D_004BD8B0+1,"Shell_ch.bnk");
}

/* ---- 001C7300 ---- */
typedef struct { char pad[0xC8]; int xC8; } Obj_C7300;
extern int D_004BD8B0[];
extern void func_00208A20(void *);
extern void func_00208900(void *, void *, void *);
extern void func_001C7300(Obj_C7300 *obj);
void func_001C7300(Obj_C7300 *obj) {
 func_00208A20(obj);
 func_00208900(obj,D_004BD8B0,"Shell_s.bnk");
 if(obj->xC8==0) func_00208900(obj,D_004BD8B0+2,"smusic01.bnk");
 else if(obj->xC8==1) func_00208900(obj,D_004BD8B0+2,"smusic02.bnk");
 else if(obj->xC8==2) func_00208900(obj,D_004BD8B0+2,"smusic03.bnk");
 else if(obj->xC8==3) func_00208900(obj,D_004BD8B0+2,"smusic04.bnk");
 obj->xC8++; if(obj->xC8>=4)obj->xC8=0;
}

/* ---- 001C73D8 ---- */
typedef struct Player_C73D8 {
    char pad0[0x7978];
    int x7978;
    char pad797C[0xE0];
    int x7A5C;
} Player_C73D8;

typedef struct Values_C73D8 {
    char pad0[0x16C];
    int arr[4];
} Values_C73D8;

typedef struct Vec_C73D8 {
    float x, y, z, w;
} Vec_C73D8;

typedef struct Record_C73D8 {
    Vec_C73D8 pos0;
    Vec_C73D8 pos1;
    int value;
    char pad24[0xC];
} Record_C73D8;

typedef struct Global_C73D8 {
    char pad0[0x240];
    Record_C73D8 frames[4][12];
    int counts[4];
} Global_C73D8;



extern void func_001C73D8(Values_C73D8 *values, Player_C73D8 *player, int value, int unused, Vec_C73D8 *vec0, Vec_C73D8 *vec1);
void func_001C73D8(Values_C73D8 *values, Player_C73D8 *player, int value, int unused, Vec_C73D8 *vec0, Vec_C73D8 *vec1) {
    if (player->x7A5C != 1) return;
    if ((unsigned int)(value - 0x640) < 5U) {
        int *p = (int *)((char *)values + 0x16C);
        int *q = &p[player->x7978];
        if (*q < value) {
            *q = value;
        }
    } else {
        Global_C73D8 *g = (Global_C73D8 *)D_00398C40;
        int *counts = g->counts;
        Record_C73D8 *frames = &g->frames[0][0];
        int index = player->x7978;
        int count = counts[index];
        if (count < 12) {
            ((Record_C73D8 *)((char *)frames + (index * 0x240 + count * 0x30)))->value = value;
            COPY_VECTOR(((Record_C73D8 *)((char *)frames + player->x7978 * 0x240))[counts[player->x7978]].pos0, vec0);
            COPY_VECTOR(((Record_C73D8 *)((char *)frames + player->x7978 * 0x240))[counts[player->x7978]].pos1, vec1);
            counts[player->x7978]++;
        }
    }
}

/* ---- 001C74D8 ---- */
typedef struct Obj_C74D8 {
    char pad0[0x88];
    int x88;
} Obj_C74D8;

extern Obj_C74D8 *func_001C74D8(Obj_C74D8 *obj);
Obj_C74D8 *func_001C74D8(Obj_C74D8 *obj)
{
    obj->x88 = 0;
    return obj;
}

/* ---- 001C74E8 ---- */
typedef int s32_C74E8; typedef float f32_C74E8;
struct Obj_C74E8 {
    char pad0[0x88];
    int x88;
    int x8C;
    char pad90[0x14];
    int xA4;
    char padA8[0x18];
    int xC0;
    int xC4;
    int xC8;
    int xCC;
    int xD0;
    int xD4;
    int xD8;
    int xDC;
    int xE0;
    int xE4;
    int xE8;
    char padEC[0x4];
    float xF0;
    float xF4;
    float xF8;
    float xFC;
    float x100;
    float x104;
    float x108;
    char pad10C[0x4];
    int x110;
    int x114;
    int x118;
    int x11C;
    int x120;
    int x124;
    int x128;
    int x12C;
    int x130;
    char pad134[0x4];
    int x138;
    int x13C;
    char pad140[0x18];
    int x158;
    int x15C;
    int x160;
    int x164;
    int x168;
    int x16C;
    char pad170[0x24];
    int x194;
    char pad198[0x4];
    int x19C;
    char pad1A0[0x4];
    int x1A4;
};
int func_001BF758(void *, s32_C74E8, int);                 /* extern */
extern void func_001D11A8();                                  /* extern */
s32_C74E8 func_00222B08(int, int);                            /* extern */
s32_C74E8 func_00238F48();                                /* extern */
extern s32_C74E8 D_0029E4FC;
extern s32_C74E8 D_0029E500;
extern s32_C74E8 D_0029E504;
extern s32_C74E8 D_0029E508;
extern s32_C74E8 D_0029E50C;
extern s32_C74E8 D_0029E510;
extern s32_C74E8 D_0029E514;
extern s32_C74E8 D_0029E518;
extern s32_C74E8 D_0029E51C;
extern s32_C74E8 D_0029E520;
extern s32_C74E8 D_0029E524;
extern f32_C74E8 D_0029E528;
extern s32_C74E8 D_0029E52C;
extern s32_C74E8 D_0029E530;
extern s32_C74E8 D_0029E534;

extern void func_001C74E8(struct Obj_C74E8 *arg0, s32_C74E8 arg1);
void func_001C74E8(struct Obj_C74E8 *arg0, s32_C74E8 arg1) {
    f32_C74E8 temp_f0;
    f32_C74E8 temp_f1;
    s32_C74E8 *var_t0;
    s32_C74E8 var_t3;

    arg0->x88 = arg1;
    arg0->xA4 = 0;
    arg0->xC0 = 0;
    arg0->xC4 = 0;
    arg0->xC8 = 0;
    arg0->xDC = 0;
    arg0->xE0 = 0;
    arg0->xE8 = 0;
    arg0->xE4 = 0;
    arg0->xCC = 0;
    func_001D11A8(arg0);
    arg0->x8C = 0;
    D_0029E500 = 0x23;
    D_0029E4FC = 0x23;
    D_0029E504 = 0x34;
    D_0029E508 = 0x34;
    arg0->xD4 = 0;
    arg0->xD8 = 0;
    arg0->x19C = func_00238F48();
    arg0->x19C = func_00238F48();
    arg0->x110 = func_00238F48();
    arg0->x114 = func_00238F48();
    arg0->x11C = func_00238F48();
    arg0->x120 = func_00238F48();
    arg0->x124 = func_00238F48();
    arg0->x128 = func_00238F48();
    arg0->x12C = func_00238F48();
    arg0->x130 = func_00238F48();
    arg0->x13C = func_00238F48();
    arg0->x118 = func_00238F48();
    arg0->x138 = func_00238F48();
    arg0->xD0 = func_00238F48();
    arg0->xF0 = (f32_C74E8) func_00222B08(6, 0xA);
    arg0->x108 = (f32_C74E8) func_00222B08(2, 6);
    arg0->xF4 = (f32_C74E8) func_00222B08(6, 0xA);
    arg0->xF8 = (f32_C74E8) func_00222B08(0x19, 0x23);
    temp_f1 = (f32_C74E8) func_00222B08(8, 0xC);
    arg0->x100 = 2.0f;
    arg0->xFC = temp_f1;
    temp_f0 = (f32_C74E8) func_00222B08(4, 8);
    arg0->x158 = 0;
    arg0->x15C = 1;
    arg0->x104 = temp_f0;
    arg0->x160 = 0;
    arg0->x164 = 1;
    D_0029E50C = 0x1C2;
    D_0029E510 = 0x226;
    var_t3 = 8;
    D_0029E514 = 0x1F4;
    var_t0 = (int *)((char *)arg0 + 0x190);
    D_0029E518 = 0x1770;
    D_0029E51C = 0x3B6;
    D_0029E520 = 0x47E;
    D_0029E524 = 0x60E;
    D_0029E528 = 1.2f;
    D_0029E52C = 0x32;
    D_0029E530 = 0xF;
    D_0029E534 = 0x34;
    arg0->x1A4 = 1;
    do {
        *var_t0 = 0;
        var_t3 -= 1;
        var_t0 -= 1;
    } while (var_t3 >= 0);
    arg0->x194 = 0;
    arg0->x16C = 0;
    func_001BF758((char *)arg0 + 0x140, arg1, 9);
    arg0->x168 = func_00238F48();
}

/* ---- 001C7788 ---- */
typedef struct { char pad[0x7A5C]; int x7A5C; } State_C7788;
typedef struct { char pad0[0x88]; State_C7788 *x88; char pad8C[0x64]; float xF0; } Object_C7788;
extern void func_001C7788__c1(Object_C7788 *object) asm("func_001C7788");
void func_001C7788__c1(Object_C7788 *object) {
    State_C7788 *state = object->x88;
    if (state->x7A5C == 1) {
        func_001C7A60__c1((char *)state + 0x7720, 0x18, object->xF0);
        return;
    }
    func_001C7A60__c1((char *)state + 0x7720, 0x19, object->xF0);
}

/* ---- 001C77D0 ---- */
extern int D_004BD8A0[];
extern void func_001DB6B0__c1(void *, int, int, int *, int *, int) asm("func_001DB6B0");
extern void func_001C77D0(int unused, int arg);
void func_001C77D0(int unused, int arg) {
    int count, value;
    func_001DB6B0__c1((void *)arg, 20, 70, &count, &value, 0);
    if (count > 0) func_00201B68(D_004BD8A0[0], 0, (u128__c1)0x4445505F46425F52UL, count, value, 0, 0);
}

/* ---- 001C7848 ---- */
typedef struct { char pad[0x7A5C]; int x7A5C; } State_C7848;
typedef struct { char pad0[0x88]; State_C7848 *x88; char pad8C[0x64]; float xF0; } Object_C7848;
extern void func_001C7848__c1(Object_C7848 *object) asm("func_001C7848");
void func_001C7848__c1(Object_C7848 *object) {
    State_C7848 *state = object->x88;
    if (state->x7A5C == 1) {
        func_001C7A60__c1((char *)state + 0x7720, 0xA, object->xF0);
        return;
    }
    func_001C7A60__c1((char *)state + 0x7720, 2, object->xF0);
}

/* ---- 001C7890 ---- */
struct Inner_C7890 { char pad0[0x7720]; char x7720[0x33c]; int x7A5C; };
struct Obj_C7890 { char pad0[0x88]; struct Inner_C7890 *x88; };
extern void func_001C7890__c1(struct Obj_C7890 *obj) asm("func_001C7890");
void func_001C7890__c1(struct Obj_C7890 *obj) {
    struct Inner_C7890 *inner = obj->x88;
    if (inner->x7A5C == 1) func_001C7A60__c1(inner->x7720, 7, 5.0f);
}

/* ---- 001C78D0 ---- */
struct Obj_C78D0 { char pad[0x88]; char *x88; };
extern float func_00138580(void *);
extern void func_001C78D0__c1(struct Obj_C78D0 *obj) asm("func_001C78D0");
void func_001C78D0__c1(struct Obj_C78D0 *obj) {
    if (*(int *)(obj->x88 + 0x7A5C) == 1 && func_00138580(obj->x88 + 0x40C0) > 15.0f) {
        func_001C7A60__c1(obj->x88 + 0x7720, 8, 5.0f);
    }
}

/* ---- 001C7940 ---- */
struct Inner_C7940 { char pad0[0x7720]; char x7720[0x33c]; int x7A5C; };
struct Obj_C7940 { char pad0[0x88]; struct Inner_C7940 *x88; };
extern void func_001C7940__c1(struct Obj_C7940 *obj) asm("func_001C7940");
void func_001C7940__c1(struct Obj_C7940 *obj) {
    struct Inner_C7940 *inner = obj->x88;
    if (inner->x7A5C == 1) func_001C7A60__c1(inner->x7720, 23, 5.0f);
}

/* ---- 001C7980 ---- */
struct Inner_C7980 { char pad0[0x7720]; char x7720[0x33c]; int x7A5C; };
struct Obj_C7980 { char pad0[0x88]; struct Inner_C7980 *x88; };
extern void func_001C7980__c1(struct Obj_C7980 *obj) asm("func_001C7980");
void func_001C7980__c1(struct Obj_C7980 *obj) {
    struct Inner_C7980 *inner = obj->x88;
    if (inner->x7A5C == 1) func_001C7A60__c1(inner->x7720, 29, 5.0f);
}

/* ---- 001C79C0 ---- */
typedef struct Obj_C79C0 {
    char pad0[0x88];
    char *x88;
} Obj_C79C0;


extern void func_001C79C0__c1(Obj_C79C0 *self) asm("func_001C79C0");
void func_001C79C0__c1(Obj_C79C0 *self)
{
    char *p = self->x88;

    if (*(int *)(p + 0x7A5C) == 1) {
        func_001C7A60__c1(p + 0x7720, 5, 1.0f);
    } else {
        func_001C7A60__c1(p + 0x7720, 14, 1.0f);
    }
}

/* ---- 001C7A10 ---- */
struct Obj_C7A10 { char pad0[0x88]; char *x88; char pad8C[0x68]; float xF4; };
extern void func_001C7A10__c1(struct Obj_C7A10 *obj) asm("func_001C7A10");
void func_001C7A10__c1(struct Obj_C7A10 *obj) {
    func_001C7A60__c1(obj->x88 + 0x7720, 0, obj->xF4);
}

/* ---- 001C7A38 ---- */
struct Obj_C7A38 { char pad0[0x88]; char *x88; char pad8C[0x7C]; float x108; };
extern void func_001C7A38__c1(struct Obj_C7A38 *obj) asm("func_001C7A38");
void func_001C7A38__c1(struct Obj_C7A38 *obj) {
    func_001C7A60__c1(obj->x88 + 0x7720, 0x22, obj->x108);
}

/* ---- 001C7A60 ---- */
typedef struct Rider_C7A60 {char pad[0x7A5C]; int x7A5C;} Rider_C7A60;
typedef struct Obj_C7A60 {char pad[0x88]; Rider_C7A60 *x88; char p8C[0x80]; int x10C,x110,x114,x118,x11C,x120,x124,x128,x12C,x130,x134,x138,x13C; int x140;} Obj_C7A60;
extern int func_00238F48();
extern void func_001BF7E8(void*,int,int,int);
extern void func_001C7A60__c1(Obj_C7A60 *obj,int id,float duration) asm("func_001C7A60");
void func_001C7A60__c1(Obj_C7A60 *obj,int id,float duration){
 int time;
 switch(id){
 case 0: time=obj->x110;break;
 case 7: time=obj->x114;break;
 case 8: time=obj->x118;break;
 case 9: time=obj->x11C;break;
 case 2:case 10:case 24:case 25:time=obj->x120;break;
 case 5:case 14:time=obj->x124;break;
 case 1:case 11:time=obj->x128;break;
 case 4:case 13:time=obj->x12C;break;
 case 6:case 15:time=obj->x130;break;
 case 23:time=obj->x10C;break;
 case 29:time=obj->x134;break;
 case 34:time=obj->x138;break;
 case 3:case 12:case 16:time=obj->x13C;break;
 default:time=func_00238F48();break;
 }
 if((float)(func_00238F48()-time)>duration*60.0f){
 if(obj->x88->x7A5C==1)func_001BF7E8(&obj->x140,id,D_0029D5B8,D_0029D5BC);
 else func_001BF7E8(&obj->x140,id,D_0029D5B0,D_0029D5B4);
 }
}

/* ---- 001C7BA8 ---- */
struct Obj_C7BA8 {
    char pad0[0x10C];
    int x10C;
    int x110;
    int x114;
    int x118;
    int x11C;
    int x120;
    int x124;
    int x128;
    int x12C;
    int x130;
    int x134;
    int x138;
    int x13C;
};

extern int func_00238F48();

extern void func_001C7BA8(struct Obj_C7BA8 *obj, unsigned int id);
void func_001C7BA8(struct Obj_C7BA8 *obj, unsigned int id)
{
    switch (id) {
    case 0:
        obj->x110 = func_00238F48();
        break;
    case 7:
        obj->x114 = func_00238F48();
        break;
    case 8:
        obj->x118 = func_00238F48();
        break;
    case 9:
        obj->x11C = func_00238F48();
        break;
    case 2:
    case 10:
    case 24:
    case 25:
        obj->x120 = func_00238F48();
        break;
    case 5:
    case 14:
        obj->x124 = func_00238F48();
        break;
    case 1:
    case 11:
        obj->x128 = func_00238F48();
        break;
    case 4:
    case 13:
        obj->x12C = func_00238F48();
        break;
    case 6:
    case 15:
        obj->x130 = func_00238F48();
        break;
    case 23:
        obj->x10C = func_00238F48();
        break;
    case 29:
        obj->x134 = func_00238F48();
        break;
    case 34:
        obj->x138 = func_00238F48();
        break;
    case 3:
    case 12:
    case 16:
        obj->x13C = func_00238F48();
        break;
    }
}

/* ---- 001C7CB8 ---- */


typedef struct Obj__c1 {
    char pad0[0x58];
    float x58;
    char pad5C[0x74 - 0x5C];
    int x74, x78, x7C, x80, x84;
    char *x88;
    char pad8C[8];
    int x94;
    float x98;
    int x9C;
    float xA0;
    char padA4[0xCC - 0xA4];
    unsigned int xCC;
    int xD0;
    unsigned int xD4, xD8;
    char padDC[4];
    unsigned int xE0;
    unsigned int xE4, xE8;
    unsigned int xEC;
    char padF0[8];
    float xF8, xFC;
    char pad100[0x40];
    char x140[0x18];
    float *x158;
    int x15C;
    float *x160;
    int x164;
    char pad168[4];
    int x16C;
    int x170[9];
    float x194;
    int x198, x19C, x1A0, x1A4;
} Obj__c1;

int D_0029D5C0 = 30;
int D_0029D5C4 = 20;   /* $gp at entry: defined earlier in this TU */
int D_0029D5C8 = 45;
int D_0029D5CC = 250;
int D_0029D5D0 = 800;
int D_0029D5D4 = 300;
int D_0029D5D8 = 1000;
int D_0029D5DC = 25;
int D_0029D5E0 = 60;
int D_0029D5E4 = 100;
float D_0029D5E8 = 7.3f;
float D_0029D5EC = 9.8f;
float D_0029D5F0 = 0.3f;
float D_0029D5F4 = 0.9f;
int D_0029D5F8 = 16000;
int D_0029D5FC = 32767;
int D_0029D600 = 900;
int D_0029D604 = 1024;
int D_0029D608 = 25;
int D_0029D60C = 12000;
int D_0029D610 = 4;
int D_0029D614 = 140;
int D_0029D618 = 900;
int D_0029D61C = 0;
int D_0029D620 = 50;
extern int D_004BD8A0[];
extern char D_004BD840[];
extern void func_001DB6B0__c1(void *p, int a, int b, int *outA, int *outB, int c) asm("func_001DB6B0");
extern int func_00238F48();
extern void func_00208BF8__c1(void *a, void *b) asm("func_00208BF8");

/* "E_FIRE_L" packed little-endian into a 128-bit value */
#define NAME_E_FIRE_L ((u128__c1)0x4C5F455249465F45LL)
/* "B_METAL_GRIND" packed little-endian into a 128-bit value */
#define NAME_B_METAL_GRIND (((u128__c1)0x444E495247LL << 64) | (u128__c1)0x5F4C4154454D5F42LL)
/* "E_GLASS_BR" packed little-endian into a 128-bit value */
#define NAME_E_GLASS_BR (((u128__c1)0x5242LL << 64) | (u128__c1)0x5F5353414C475F45LL)

extern void func_001C7CB8__c1(Obj__c1 *self) asm("func_001C7CB8");
void func_001C7CB8__c1(Obj__c1 *self)
{
    int count, other;

    func_001DB6B0__c1(*(char **)(self->x88 + 0x792C) + 0x10, D_0029D5C4, D_0029D5C8,
                  &count, &other, 0);
    if (count > 0) {
        self->xD0 = func_00238F48();
        func_00201BD0(D_004BD8A0[0], 0, NAME_E_FIRE_L, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xCC));
    }
}

typedef struct Choice { int index; float value; } Choice;
extern u128__c1 func_001C9E68(Obj__c1 *, int);
extern u128__c1 func_001C9A50(Obj__c1 *, int);
extern char D_0028FD10[];
extern int strncmp(const char *, const char *, unsigned int);
extern void func_001C7D68(Obj__c1 *self);
void func_001C7D68(Obj__c1 *self)
{
    Choice choice;
    u128__c1 name;
    char *nameptr = (char *)&name;
    int count, other;
    int elapsed;
    char *owner;
    if (*(float *)((char *)self + 0x98) > *(float *)((char *)self + 0xA0)) {
        choice.index = *(int *)((char *)self + 0x94);
        choice.value = *(float *)((char *)self + 0x98);
    } else {
        choice.index = *(int *)((char *)self + 0x9C);
        choice.value = *(float *)((char *)self + 0xA0);
    }
    name = func_001C9E68(self, choice.index);
    if (strncmp(nameptr, D_0028FD10, 16) != 0) {
        elapsed = func_00238F48() - *(int *)((char *)self + 0x19C);
        owner = self->x88;
        if (!((unsigned char)(*(unsigned long *)(owner + 0x4098) >> 2) & 1) && (float)elapsed > 48.0f) {
            func_001DB6B0__c1(*(char **)(owner + 0x792C) + 16, D_0029D5C4, D_0029D5C8, &count, &other, 0);
            if (count > 0) {
                *(int *)((char *)self + 0x19C) = func_00238F48();
                func_00201B68(D_004BD8A0[0], 0, name, count, other, 0, 0);
            }
        }
    }
}


extern void func_001C7E98(Obj__c1 *self);
void func_001C7E98(Obj__c1 *self)
{
    Choice choice;
    u128__c1 name;
    char *nameptr = (char *)&name;
    int count, other;
    int elapsed;
    char *owner;
    if (*(float *)((char *)self + 0x98) > *(float *)((char *)self + 0xA0)) {
        choice.index = *(int *)((char *)self + 0x94);
        choice.value = *(float *)((char *)self + 0x98);
    } else {
        choice.index = *(int *)((char *)self + 0x9C);
        choice.value = *(float *)((char *)self + 0xA0);
    }
    name = func_001C9A50(self, choice.index);
    if (strncmp(nameptr, D_0028FD10, 16) != 0) {
        elapsed = func_00238F48() - *(int *)((char *)self + 0x198);
        owner = self->x88;
        if (!((unsigned char)(*(unsigned long *)(owner + 0x4098) >> 2) & 1) && (float)elapsed > 48.0f) {
            func_001DB6B0__c1(*(char **)(owner + 0x792C) + 16, D_0029D5C4, D_0029D5C8, &count, &other, 0);
            if (count > 0) {
                *(int *)((char *)self + 0x198) = func_00238F48();
                func_00201B68(D_004BD8A0[0], 0, name, count, other, 0, 0);
            }
        }
    }
}




extern u128__c1 func_001CA278(Obj__c1 *,int);
extern float func_00138580(void *);
extern void func_001C7FC8__c1(Obj__c1 *self) asm("func_001C7FC8");
void func_001C7FC8__c1(Obj__c1 *self) {
 Choice choice; u128__c1 name; char *nameptr=(char *)&name; int count,other; unsigned int handle;
 if (*(float *)((char *)self+0x98) > *(float *)((char *)self+0xa0)) {
 choice.index=*(int *)((char *)self+0x94); choice.value=*(float *)((char *)self+0x98);
 } else {
 choice.index=*(int *)((char *)self+0x9c); choice.value=*(float *)((char *)self+0xa0);
 }
 name=func_001CA278(self,choice.index);
 if(func_00138580(self->x88+0x40c0)>=1.0f && !((unsigned char)(*(unsigned long *)(self->x88+0x4098)>>2)&1)) {
 if(strncmp(nameptr,D_0028FD10,16)!=0 && *(int *)((char *)self+0x154)==choice.index) {
 func_001DB6B0__c1(*(char **)(self->x88+0x792c)+16,D_0029D5C4,D_0029D5C8,&count,&other,0);
 if(count>0) {
 handle=self->xE0;
 if(handle==0) {
 self->xE0=0xffffffff; *(u128__c1 *)((char *)self+0xb0)=name;
 func_00201BD0(D_004BD8A0[0], 0, name, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE0));
 } else if(handle!=0xffffffff && handle!=0) {
 self->xE0=0xffffffff;
 func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE0));
 }
 } else { func_00208BF8__c1(D_004BD840,&self->xE0); }
 } else { goto cancel; }
 } else {
 cancel:
 func_00208BF8__c1(D_004BD840,&self->xE0);
 }
}

extern void func_001C8188(Obj__c1 *obj);
void func_001C8188(Obj__c1 *obj)
{
    func_00208BF8__c1(D_004BD840, &obj->xE0);
}

extern void func_001C81B0(Obj__c1 *self);
void func_001C81B0(Obj__c1 *self)
{
    int out[2];

    func_001DB6B0__c1(*(char **)(self->x88 + 0x792C) + 0x10, D_0029D5C4, D_0029D5C8,
                  &out[0], &out[1], 0);
    if (out[0] > 0) {
        func_00201B68(D_004BD8A0[0], 0, NAME_E_GLASS_BR, out[0], out[1], 0, 0);
    }
}

extern void func_001C8230(Obj__c1 *obj);
void func_001C8230(Obj__c1 *obj)
{
    func_00208BF8__c1(D_004BD840, &obj->xEC);
}

extern void func_001C8258__c1(Obj__c1 *self) asm("func_001C8258");
void func_001C8258__c1(Obj__c1 *self)
{
    int count, other;
    unsigned int handle;
    char *sound;

    sound = *(char **)(self->x88 + 0x792C) + 0x10;
    count = 0;
    other = 0;
    func_001DB6B0__c1(sound, D_0029D5C4, D_0029D5C8, &count, &other, 0);
    if (count > 0) {
        handle = self->xEC;
        if (handle == 0) {
            self->xEC = 0xFFFFFFFF;
            func_00201BD0(D_004BD8A0[0], 0, NAME_B_METAL_GRIND, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xEC));
        } else if (handle != 0xFFFFFFFF && handle != 0) {
            self->xEC = 0xFFFFFFFF;
            func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xEC));
        }
    } else {
        func_00208BF8__c1(D_004BD840, &self->xEC);
    }
}

typedef struct Name8380 { char bytes[16]; } Name8380;
typedef struct Race8380 {
    char pad0[0x50140];
    char *riders[26];
} Race8380;
typedef struct VEntry8380 {
    short delta;
    short x2;
    int (*call)(void *);
} VEntry8380;
typedef struct Virtual8380 { int x0; VEntry8380 *x4; } Virtual8380;

extern Race8380 D_002AF2C0;
extern Mode8380 D_004B25A0__c1 asm("D_004B25A0");
extern char *D_0037ED14__c1[] asm("D_0037ED14");
extern int D_004B28F8[];
extern int D_004BD8A4[];
extern int D_0029D618, D_0029D61C, D_0029D620;
extern int D_0029E514, D_0029E52C, D_0029E530, D_0029E534;
extern float D_0029E528;
extern float func_00222330(void *, void *);
extern int func_001BF770(void *);
extern int func_001DB9B0(int);
extern int func_00138560(void *);
extern float func_0012DF48(void *);
extern void func_001CB638();
extern void func_001C9378__c1(Obj__c1 *, int, float);
extern void func_001C8F60();
extern void func_001C9128();
extern void func_001D11A8();
extern void func_001D12C0__c1();
extern void func_001D13B8__c1();
extern void func_001D1298();
extern void func_001D1528__c1();
extern float *func_00202DB0();

#define INT8380(p, off) (*(int *)((char *)(p) + (off)))
#define FLOAT8380(p, off) (*(float *)((char *)(p) + (off)))
#define PTR8380(p, off) (*(char **)((char *)(p) + (off)))
#define FLAGS8380(p) ((unsigned char)(*(unsigned long *)((p) + 0x4098) >> 2) & 1)
#define SOUND8380_A (((u128__c1)((0x8A98829A8A8CUL << 7) | 0x5FUL) << 64) | (u128__c1)((0x90A8828AA484UL << 15) | 0x5F52UL))
#define SOUND8380_B ((u128__c1)((0x90A8828AA484UL << 15) | 0x5F4BUL))
#define SOUND8380_C (((u128__c1)((0x8A98829AUL << 7) | 0x5FUL) << 64) | (u128__c1)((0x90A8828AA484UL << 15) | 0x5F52UL))
#define SOUND8380_D (((u128__c1)((0xA49282BEUL << 15) | 0x4C45UL) << 64) | (u128__c1)((0x8A90AEB2988CUL << 15) | 0x5F42UL))

/* The EE accumulator and square root have no equivalent GCC 2.95 C sequence. */
extern void func_001C8380__c1(Obj__c1 *self) asm("func_001C8380");
void func_001C8380__c1(Obj__c1 *self)
{
    Choice choice;
    Name8380 name;
    int count = 0, other = 0;
    unsigned int random;
    float length, speed, ratio, distance;
    float *velocity;
    char *pos, *otherPos;
    unsigned int handle;
    Virtual8380 *virtualObj;
    int mode;

    velocity = (float *)(self->x88 + 0x4180);
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(length) : "f"(velocity[0]), "f"(velocity[1]), "f"(velocity[2]));
    speed = length * 0.6818182f;
    if (self->x98 > self->xA0) {
        choice.index = self->x94;
        choice.value = self->x98;
    } else {
        choice.index = self->x9C;
        choice.value = self->xA0;
    }
    if (INT8380(self->x88, 0x7A5C) != 1) {
        func_001C7A60__c1(self, 11, self->xF8);
    } else {
        func_001C7A60__c1(self, 1, self->xF8);
    }
    if (FLOAT8380(self->x88, 0x7AC0) > (float)D_0029D5C0) {
        if (INT8380(self->x88, 0x7A5C) != 1) {
            func_001C7A60__c1(self, 13, self->xFC);
        } else {
            func_001C7A60__c1(self, 4, self->xFC);
        }
    }
    if (INT8380(self->x88, 0x7A5C) != 1) {
        pos = PTR8380(self->x88, 0x792C);
        otherPos = PTR8380(D_002AF2C0.riders[self->x16C], 0x792C);
        distance = FLOAT8380(otherPos, 0x18) - FLOAT8380(pos, 0x18);
        if (distance > 0.0f) {
            if (self->x170[self->x16C] == 1 && distance < 20.0f &&
                func_00222330(pos + 0x10, otherPos + 0x10) < 15.0f) {
                func_001C7A10__c1(self);
            }
            self->x170[self->x16C] = 0;
        } else {
            self->x170[self->x16C] = 1;
        }
        self->x16C++;
        if (self->x16C >= D_004B28F8[0]) {
            self->x16C = 0;
        }
    }
    if ((unsigned int)(INT8380(self->x88, 0x7908) - 35) >= 3U) {
        float *ratioVector = (float *)(self->x88 + 0x2E94);
        ratio = ratioVector[1] / ratioVector[2];
        mode = D_004B25A0__c1.x10C;
        if ((mode != 10 && INT8380(self->x88, 0x7A5C) == 1) ||
            (D_004B25A0__c1.x10C == 10 && D_0037ED14__c1[0] == self->x88)) {
            if (ratio < 0.5f && !func_001BF770(self->x140)) {
                other = func_001DB9B0(INT8380(self->x88, 0x7978));
                count = (int)((1.0f - ratio) * 1024.0f);
                handle = self->xE4;
                if (handle == 0) {
                    switch (INT8380(self->x88, 0x796C)) {
                    case 0x40:
                    case 0x80:
                    case 0x100:
                    case 0x160:
                        self->xE4 = 0xFFFFFFFF;
                        func_00201BD0(D_004BD8A4[0], 0, SOUND8380_A, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE4));
                        break;
                    case 0x200:
                        self->xE4 = 0xFFFFFFFF;
                        func_00201BD0(D_004BD8A4[0], 0, SOUND8380_B, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE4));
                        break;
                    default:
                        self->xE4 = 0xFFFFFFFF;
                        func_00201BD0(D_004BD8A4[0], 0, SOUND8380_C, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE4));
                        break;
                    }
                } else if (handle != 0xFFFFFFFF && handle != 0) {
                    self->xE4 = 0xFFFFFFFF;
                    func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE4));
                }
            } else {
                func_00208BF8__c1(D_004BD840, &self->xE4);
            }
        }
        if (INT8380(self->x88, 0x7A5C) == 1) {
            func_001DB6B0__c1(PTR8380(self->x88, 0x792C) + 0x10, D_0029D5C4, D_0029D5C8, &count, &other, 0);
        } else {
            func_001DB6B0__c1(PTR8380(self->x88, 0x792C) + 0x10, 10, 95, &count, &other, 0);
        }
        if (choice.value > (float)D_0029E514) {
            func_001CB638(self);
        }
        mode = D_004B25A0__c1.x10C;
        if ((mode != 10 && INT8380(self->x88, 0x7A5C) == 1) ||
            (D_004B25A0__c1.x10C == 10 && D_0037ED14__c1[0] == self->x88)) {
            if (count > 0) {
                if (func_00138560(self->x88 + 0x40C0)) {
                    char *contact = self->x88 + 0x40C0;
                    if (*(short *)(contact + 0x4A4) > D_0029E530 &&
                        func_00138580(contact) > (float)D_0029E534 &&
                        self->x1A4 == 1 && self->xE8 == 0) {
                        self->x1A4 = 0;
                        self->x1A0 = func_00238F48();
                        self->xE8 = 0xFFFFFFFF;
                        func_00201BD0(D_004BD8A0[0], 0, SOUND8380_D, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE8));
                    }
                }
                if (self->xE8 != 0xFFFFFFFF && self->xE8 != 0) {
                    if ((float)(func_00238F48() - self->x1A0) > D_0029E528 * 60.0f) {
                        func_00208BF8__c1(D_004BD840, &self->xE8);
                    } else {
                        handle = self->xE8;
                        self->xE8 = 0xFFFFFFFF;
                        func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xE8));
                    }
                }
            } else {
                func_00208BF8__c1(D_004BD840, &self->xE8);
            }
            if (!func_00138560(self->x88 + 0x40C0)) {
                func_00208BF8__c1(D_004BD840, &self->xE8);
                self->x1A4 = 1;
            }
        }
        if (count > 0 && !FLAGS8380(self->x88) &&
            !func_00138560(self->x88 + 0x40C0) && speed > 5.0f) {
            func_001C9378__c1(self, other, (float)count * 0.0009765625f);
        } else {
            func_00208BF8__c1(D_004BD840, &self->xD4);
            func_00208BF8__c1(D_004BD840, &self->xD8);
        }
        if (func_0012DF48(self->x88) != 0.0f && self->x194 == 0.0f && speed > 25.0f) {
            func_001C7E98(self);
        }
        self->x194 = func_0012DF48(self->x88);
        if (func_00138580(self->x88 + 0x40C0) < 1.0f || FLAGS8380(self->x88)) {
            func_00208BF8__c1(D_004BD840, &self->xE0);
        }
    } else {
        if (INT8380(self->x88, 0x7A5C) == 1) {
            func_001DB6B0__c1(PTR8380(self->x88, 0x792C) + 0x10, D_0029D5C4, D_0029D5C8, &count, &other, 0);
        } else {
            func_001DB6B0__c1(PTR8380(self->x88, 0x792C) + 0x10, 10, 95, &count, &other, 0);
        }
        if (!func_00138560(self->x88 + 0x40C0)) {
            if (self->x158 == 0) {
                name = *(Name8380 *)(self->x88 + 0x20);
                self->x158 = func_00202DB0(1, &name);
            } else if (self->x15C != 0) {
                func_001C8F60(self);
            } else if (*self->x158 == 0.0f) {
                self->x15C = 1;
            }
        }
    }
    if (INT8380(self->x88, 0x7BA4) != 0) {
        func_001C8258__c1(self);
    } else {
        func_001C8230(self);
    }
    if (self->xCC != 0xFFFFFFFF && self->xCC != 0) {
        if (func_00238F48() - self->xD0 > 120) {
            func_00208BF8__c1(D_004BD840, &self->xCC);
        } else {
            handle = self->xCC;
            self->xCC = 0xFFFFFFFF;
            func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xCC));
        }
    }
    if (INT8380(self->x88, 0x7A5C) == 1) {
        if (FLOAT8380(self->x88, 0x7AC0) > (float)D_0029E52C) {
            if (self->x7C == 3 || self->x7C == 1) {
                if (self->x7C == 3) {
                    func_001D11A8(self);
                }
                self->x74 = D_0029D618;
                self->x78 = 1024;
                self->x7C = 0;
                self->x80 = func_00238F48();
                self->x84 = (int)self->x58;
            }
        } else if (FLOAT8380(self->x88, 0x7AC0) < 10.0f) {
            if (self->x7C == 2 || self->x7C == 0) {
                self->x74 = D_0029D61C;
                self->x78 = D_0029D620;
                self->x7C = 1;
                self->x80 = func_00238F48();
                self->x84 = (int)self->x58;
            }
        }
        if (self->x7C == 1) {
            func_001D12C0__c1(self);
        } else if (self->x7C == 0) {
            func_001D13B8__c1(self);
        } else if (self->x7C == 3) {
            func_001D1298(self);
        } else {
            func_001D1528__c1(self);
        }
    }
    self->x98 = 0.0f;
    self->xA0 = 0.0f;
    virtualObj = *(Virtual8380 **)(self->x88 + 0x4ECC);
    if (virtualObj->x4[8].call((char *)virtualObj + virtualObj->x4[8].delta) == 5) {
        if (self->x160 == 0) {
            char *nameSource = self->x88;
            nameSource += INT8380(nameSource, 0x6024) * 16;
            name = *(Name8380 *)(nameSource + 0x1E54);
            self->x160 = func_00202DB0(1, &name);
        } else if (self->x164 != 0) {
            func_001C9128(self);
        } else if (*self->x160 == 0.0f) {
            self->x164 = 1;
        }
    } else {
        self->x164 = 1;
    }
}


/* Sound object: x88 = owner (rider), x158 = trigger value, x15C = cleared once the sound plays. */
struct SoundObj {
    char pad0[0x88];
    char *x88;
    char pad8C[0xCC];
    float *x158;
    int x15C;
};
extern void *memcpy(void *, const void *, unsigned int);

extern void func_001C8F60(struct SoundObj *self);
void func_001C8F60(struct SoundObj *self)
{
    u128__c1 name = 0;
    char *nameptr = (char *)&name;
    int count, other;

    memcpy(&name, D_0028FD10, 9);
    count = 0;
    other = 0;
    if ((int)*self->x158) {
        switch (*(int *)(self->x88 + 0x7908)) {
        case 35: /* "E_RIDE_SML_PAD" */
            name = ((u128__c1)0x4441505F4C4DUL << 64) | (u128__c1)0x535F454449525F45UL;
            break;
        case 36: /* "E_RIDE_SML_HOOF" */
            name = ((u128__c1)0x464F4F485F4C4DUL << 64) | (u128__c1)0x535F454449525F45UL;
            break;
        case 37: /* "E_RIDE_BIG_HOOF" */
            name = ((u128__c1)0x464F4F485F4749UL << 64) | (u128__c1)0x425F454449525F45UL;
            break;
        }
    }
    if (strncmp(nameptr, D_0028FD10, 16)) {
        func_001DB6B0__c1(*(char **)(self->x88 + 0x792C) + 16, D_0029D5C4, D_0029D5C8, &count, &other, 0);
        func_00201B68(D_004BD8A0[0], 0, name, count, other, 0, 0);
        self->x15C = 0;
    }
}

/* Rider fields used by func_001C9128 (x88 of the sound object). */
struct Rider9128 {
    char pad0[0x792C];
    char *x792C;
    char pad7930[0x3C];
    int x796C;
    char pad7970[0xEC];
    int x7A5C;
};
/* Sound object as seen by func_001C9128: x160 = size class, x164 = cleared once the sound plays. */
struct SoundObj9128 {
    char pad0[0x88];
    struct Rider9128 *x88;
    char pad8C[0xD4];
    float *x160;
    int x164;
};
extern int D_004BD8A4[];

extern void func_001C9128(struct SoundObj9128 *self);
void func_001C9128(struct SoundObj9128 *self)
{
    u128__c1 name = 0;
    int count, other;
    char *nameptr;

    nameptr = memcpy(&name, D_0028FD10, 9);
    count = 0;
    other = 0;
    if (self->x88->x7A5C == 1) {
        switch ((int)*self->x160) {
        case 1:
            if (self->x88->x796C == 0x200) /* "K_WHOOSH_SM" */
                name = ((u128__c1)0x4D535FLL << 64) | (u128__c1)0x48534F4F48575F4BLL;
            else /* "R_WHOOSH_SM" */
                name = ((u128__c1)0x4D535FLL << 64) | (u128__c1)0x48534F4F48575F52LL;
            break;
        case 2:
            if (self->x88->x796C == 0x200) /* "K_WHOOSH_MED" */
                name = ((u128__c1)0x44454D5FLL << 64) | (u128__c1)0x48534F4F48575F4BLL;
            else /* "R_WHOOSH_MED" */
                name = ((u128__c1)0x44454D5FLL << 64) | (u128__c1)0x48534F4F48575F52LL;
            break;
        case 3:
            if (self->x88->x796C == 0x200) /* "K_WHOOSH_BIG" */
                name = ((u128__c1)0x4749425FLL << 64) | (u128__c1)0x48534F4F48575F4BLL;
            else /* "R_WHOOSH_BIG" */
                name = ((u128__c1)0x4749425FLL << 64) | (u128__c1)0x48534F4F48575F52LL;
            break;
        }
        if (strncmp(nameptr, D_0028FD10, 16)) {
            func_001DB6B0__c1(self->x88->x792C + 16, D_0029D5C4, D_0029D5C8, &count, &other, 0);
            func_00201B68(D_004BD8A4[0], 0, name, count, other, 0, 0);
            self->x164 = 0;
        }
    }
}
extern u128__c1 func_001CA768(Obj__c1 *, int, int);
extern u128__c1 func_001CB078(Obj__c1 *, int);
extern int D_0029E4FC, D_0029E500, D_0029E504, D_0029E508;
extern float D_0029D5A4;
extern void func_001DB920(void *, int *, void *);
extern int func_00202B48(int);

/* EE accumulator and square root instructions require inline assembly. */
extern void func_001C9378__c1(Obj__c1 *self, int other, float scale) asm("func_001C9378");
void func_001C9378__c1(Obj__c1 *self, int other, float scale)
{
    Choice choice;
    u128__c1 name;
    char *nameptr;
    int result;
    int count, volume, pan;
    float length, speed;
    float *velocity;
    unsigned int handle;
    int mode;

    velocity = (float *)(self->x88 + 0x4180);
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(length) : "f"(velocity[0]), "f"(velocity[1]), "f"(velocity[2]));
    speed = length * 0.6818182f;
    nameptr = (char *)&name;
    pan = 0;
    if (self->x98 > self->xA0) {
        choice.index = self->x94;
        choice.value = self->x98;
    } else {
        choice.index = self->x9C;
        choice.value = self->xA0;
    }
    mode = D_004B25A0__c1.x10C;
    if ((mode != 10 && INT8380(self->x88, 0x7A5C) == 1) ||
        (D_004B25A0__c1.x10C == 10 && D_0037ED14__c1[0] == self->x88)) {
        name = func_001CA768(self, choice.index,
            FLOAT8380(self->x88, 0x7A78) != 0.0f || *(unsigned char *)(self->x88 + 0x7A69) != 0);
        if (strncmp(nameptr, D_0028FD10, 16) != 0 && INT8380(self, 0x154) == choice.index) {
            if (FLOAT8380(self->x88, 0x7A78) != 0.0f || *(unsigned char *)(self->x88 + 0x7A69) != 0) {
                func_00208BF8__c1(D_004BD840, &self->xD4);
                if (speed < (float)D_0029E500)
                    count = (int)(scale * 1024.0f * ((speed - 5.0f) / (float)(D_0029E500 - 5)));
                else count = (int)(scale * 1024.0f);
                if (speed < (float)D_0029E504)
                    volume = (int)(((speed - 5.0f) / (float)(D_0029E504 - 5)) * 32767.0f);
                else volume = 32767;
                handle = self->xD8;
                if (handle == 0) {
                    self->xD8 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, name, count, other, 0, volume, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xD8));
                } else if (handle != 0xFFFFFFFF && handle != 0) {
                    self->xD8 = 0xFFFFFFFF;
                    func_00201E08(handle, 5, count, other, 0, volume, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xD8));
                }
            } else {
                func_00208BF8__c1(D_004BD840, &self->xD8);
                if (speed < (float)D_0029E4FC)
                    count = (int)(scale * 1024.0f * ((speed - 5.0f) / (float)(D_0029E4FC - 5)));
                else count = (int)(scale * 1024.0f);
                if (speed < (float)D_0029E508)
                    volume = (int)(((speed - 5.0f) / (float)(D_0029E508 - 5)) * 32767.0f);
                else volume = 32767;
                handle = self->xD4;
                if (handle == 0) {
                    self->xD4 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, name, count, other, 0, volume, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xD4));
                } else if (handle != 0xFFFFFFFF && handle != 0) {
                    self->xD4 = 0xFFFFFFFF;
                    func_00201E08(handle, 5, count, other, 0, volume, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xD4));
                }
            }
        } else {
            func_00208BF8__c1(D_004BD840, &self->xD4);
            func_00208BF8__c1(D_004BD840, &self->xD8);
        }
    } else if (INT8380(self->x88, 0x7A5C) == 3) {
        name = func_001CB078(self, choice.index);
        if (strncmp(nameptr, D_0028FD10, 16) != 0 && INT8380(self, 0x154) == choice.index) {
            if (speed < (float)D_0029E500)
                count = (int)(scale * 1024.0f * ((speed - 5.0f) / (float)(D_0029E500 - 5)));
            else count = (int)(scale * 1024.0f);
            if (speed < (float)D_0029E504)
                volume = (int)(((speed - 5.0f) / (float)(D_0029E504 - 5)) * 32767.0f);
            else volume = 32767;
            if (D_004B25A0__c1.x288 == 1) {
                func_001DB920(PTR8380(self->x88, 0x792C) + 16, &result, (char *)self + 0x90);
                pan = (int)((float)func_00202B48(result) * D_0029D5A4);
            }
            handle = self->xD8;
            if (handle == 0) {
                self->xD8 = 0xFFFFFFFF;
                func_00201BD0(D_004BD8A0[0], 0, name, count, other, 0, volume, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xD8));
            } else if (handle != 0xFFFFFFFF && handle != 0) {
                self->xD8 = 0xFFFFFFFF;
                func_00201E08(handle, 5, count, other, pan, volume, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&self->xD8));
            }
        } else {
            func_00208BF8__c1(D_004BD840, &self->xD8);
        }
    }
    INT8380(self, 0x154) = choice.index;
}


struct Obj99B0 { char pad[0x94]; int x94; float x98; };
extern int func_001C32D0(int);
extern void func_001C99B0(struct Obj99B0 *obj,int value,float threshold);
void func_001C99B0(struct Obj99B0 *obj,int value,float threshold) {
 if(obj->x98<=threshold) {
  int result=func_001C32D0(value);
  obj->x94=result; obj->x98=threshold;
 }
}

struct Obj9A00 { char pad[0x9c]; int x9C; float xA0; };
extern int func_001C32D0(int);
extern void func_001C9A00(struct Obj9A00 *obj,int value,float threshold);
void func_001C9A00(struct Obj9A00 *obj,int value,float threshold) {
 if(obj->xA0<=threshold) {
  int result=func_001C32D0(value);
  obj->x9C=result; obj->xA0=threshold;
 }
}

typedef struct Name9 { char s[9]; } Name9;
typedef union Name { u128__c1 v; char s[16]; } Name;
/* sound name for a surface type, "NO_SOUND" (D_0028FD10) when none */
extern u128__c1 func_001C9A50(Obj__c1 *self, int surface);
u128__c1 func_001C9A50(Obj__c1 *self, int surface)
{
    Name name;

    name.v = 0;
    *(Name9 *)name.s = *(Name9 *)D_0028FD10;
    switch (surface) {
    case 3:
    case 35:
    case 67:
    case 99:
    case 131:
    case 163:
    case 195:
    case 227:
    case 259:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 2:
    case 98:
    case 162:
    case 194:
    case 226:
    case 258:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F534152475F42ULL; /* "B_GRAS_BRAKE" */
        break;
    case 40:
    case 72:
    case 232:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F474452425F42ULL; /* "B_BRDG_BRAKE" */
        break;
    case 6:
    case 38:
    case 70:
    case 102:
    case 134:
    case 166:
    case 198:
    case 230:
    case 262:
        name.v = ((u128__c1)0x454B41ULL << 64) | (u128__c1)0x52425F4F32485F42ULL; /* "B_H2O_BRAKE" */
        break;
    case 39:
    case 71:
    case 263:
        name.v = ((u128__c1)0x454B41ULL << 64) | (u128__c1)0x52425F44554D5F42ULL; /* "B_MUD_BRAKE" */
        break;
    case 82:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4156414C5F42ULL; /* "B_LAVA_BRAKE" */
        break;
    case 133:
    case 261:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F574F4E535F42ULL; /* "B_SNOW_BRAKE" */
        break;
    case 264:
    case 268:
    case 281:
    case 282:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B4152545F42ULL; /* "B_TRAK_BRAKE" */
        break;
    case 273:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F434E4F435F42ULL; /* "B_CONC_BRAKE" */
        break;
    case 280:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F444E41535F42ULL; /* "B_SAND_BRAKE" */
        break;
    case 0:
    case 5:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 27:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 160:
    case 168:
    case 169:
    case 170:
    case 171:
    case 172:
    case 173:
    case 174:
    case 175:
    case 177:
    case 178:
    case 179:
    case 186:
    case 187:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 192:
    case 197:
    case 200:
    case 202:
    case 204:
    case 205:
    case 206:
    case 209:
    case 212:
    case 218:
    case 219:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 128:
    case 136:
    case 138:
    case 140:
    case 141:
    case 142:
    case 143:
    case 144:
    case 145:
    case 147:
    case 153:
    case 154:
    case 155:
    case 156:
    case 158:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 64:
    case 66:
    case 73:
    case 74:
    case 76:
    case 77:
    case 78:
    case 79:
    case 81:
    case 91:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 96:
    case 104:
    case 105:
    case 106:
    case 109:
    case 110:
    case 111:
    case 113:
    case 117:
    case 121:
    case 122:
    case 123:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 32:
    case 42:
    case 45:
    case 46:
    case 47:
    case 51:
    case 52:
    case 55:
    case 59:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 224:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 241:
    case 243:
    case 244:
    case 250:
    case 251:
    case 252:
    case 253:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 256:
    case 265:
    case 266:
    case 267:
    case 269:
    case 270:
    case 271:
    case 276:
    case 283:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F4B434F525F42ULL; /* "B_ROCK_BRAKE" */
        break;
    case 1:
    case 33:
    case 56:
    case 65:
    case 88:
    case 97:
    case 161:
    case 193:
    case 199:
    case 225:
    case 257:
    case 288:
    case 289:
    case 301:
    case 305:
    case 315:
        name.v = ((u128__c1)0x454B4152ULL << 64) | (u128__c1)0x425F545249445F42ULL; /* "B_DIRT_BRAKE" */
        break;
    }
    return name.v;
}
extern u128__c1 func_001C9E68(Obj__c1 *self, int surface);
u128__c1 func_001C9E68(Obj__c1 *self, int surface)
{
    Name name;

    name.v = 0;
    *(Name9 *)name.s = *(Name9 *)D_0028FD10;
    switch (surface) {
    case 3:
    case 35:
    case 67:
    case 131:
    case 163:
    case 195:
    case 227:
    case 259:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 2:
    case 98:
    case 162:
    case 194:
    case 226:
    case 258:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F534152475F42ULL; /* "B_GRAS_SKID" */
        break;
    case 40:
    case 72:
    case 232:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F474452425F42ULL; /* "B_BRDG_SKID" */
        break;
    case 6:
    case 38:
    case 70:
    case 102:
    case 134:
    case 166:
    case 198:
    case 230:
    case 262:
        name.v = ((u128__c1)0x4449ULL << 64) | (u128__c1)0x4B535F4F32485F42ULL; /* "B_H2O_SKID" */
        break;
    case 39:
    case 71:
    case 263:
        name.v = ((u128__c1)0x4449ULL << 64) | (u128__c1)0x4B535F44554D5F42ULL; /* "B_MUD_SKID" */
        break;
    case 82:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4156414C5F42ULL; /* "B_LAVA_SKID" */
        break;
    case 280:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F444E41535F42ULL; /* "B_SAND_SKID" */
        break;
    case 273:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F434E4F435F42ULL; /* "B_CONC_SKID" */
        break;
    case 264:
    case 268:
    case 281:
    case 282:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B4152545F42ULL; /* "B_TRAK_SKID" */
        break;
    case 133:
    case 261:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F574F4E535F42ULL; /* "B_SNOW_SKID" */
        break;
    case 0:
    case 5:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 27:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 160:
    case 168:
    case 169:
    case 170:
    case 171:
    case 172:
    case 173:
    case 174:
    case 175:
    case 177:
    case 178:
    case 179:
    case 186:
    case 187:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 192:
    case 197:
    case 200:
    case 202:
    case 204:
    case 205:
    case 206:
    case 209:
    case 212:
    case 218:
    case 219:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 128:
    case 136:
    case 138:
    case 140:
    case 141:
    case 142:
    case 143:
    case 144:
    case 145:
    case 147:
    case 153:
    case 154:
    case 155:
    case 156:
    case 158:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 64:
    case 66:
    case 73:
    case 74:
    case 76:
    case 77:
    case 78:
    case 79:
    case 81:
    case 91:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 96:
    case 104:
    case 105:
    case 106:
    case 109:
    case 110:
    case 111:
    case 113:
    case 117:
    case 121:
    case 122:
    case 123:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 32:
    case 42:
    case 45:
    case 46:
    case 47:
    case 51:
    case 52:
    case 55:
    case 59:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 224:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 241:
    case 243:
    case 244:
    case 250:
    case 251:
    case 252:
    case 253:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 256:
    case 265:
    case 266:
    case 267:
    case 269:
    case 270:
    case 271:
    case 276:
    case 283:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID" */
        break;
    case 1:
    case 33:
    case 56:
    case 65:
    case 88:
    case 97:
    case 161:
    case 193:
    case 199:
    case 225:
    case 257:
    case 288:
    case 289:
    case 301:
    case 305:
    case 315:
        name.v = ((u128__c1)0x44494BULL << 64) | (u128__c1)0x535F545249445F42ULL; /* "B_DIRT_SKID" */
        break;
    }
    return name.v;
}
extern u128__c1 func_001CA278(Obj__c1 *self, int surface);
u128__c1 func_001CA278(Obj__c1 *self, int surface)
{
    Name name;

    name.v = 0;
    *(Name9 *)name.s = *(Name9 *)D_0028FD10;
    switch (surface) {
    case 3:
    case 35:
    case 67:
    case 99:
    case 131:
    case 163:
    case 195:
    case 227:
    case 259:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 2:
    case 98:
    case 162:
    case 194:
    case 226:
    case 258:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F534152475F42ULL; /* "B_GRAS_SKID_L" */
        break;
    case 40:
    case 72:
    case 232:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F474452425F42ULL; /* "B_BRDG_SKID_L" */
        break;
    case 6:
    case 38:
    case 70:
    case 102:
    case 134:
    case 166:
    case 198:
    case 230:
    case 262:
        name.v = ((u128__c1)0x4C5F4449ULL << 64) | (u128__c1)0x4B535F4F32485F42ULL; /* "B_H2O_SKID_L" */
        break;
    case 82:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4156414C5F42ULL; /* "B_LAVA_SKID_L" */
        break;
    case 39:
    case 71:
    case 263:
        name.v = ((u128__c1)0x4C5F4449ULL << 64) | (u128__c1)0x4B535F44554D5F42ULL; /* "B_MUD_SKID_L" */
        break;
    case 280:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F444E41535F42ULL; /* "B_SAND_SKID_L" */
        break;
    case 273:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F434E4F435F42ULL; /* "B_CONC_SKID_L" */
        break;
    case 264:
    case 268:
    case 281:
    case 282:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B4152545F42ULL; /* "B_TRAK_SKID_L" */
        break;
    case 133:
    case 261:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F574F4E535F42ULL; /* "B_SNOW_SKID_L" */
        break;
    case 0:
    case 5:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 27:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 160:
    case 168:
    case 169:
    case 170:
    case 171:
    case 172:
    case 173:
    case 174:
    case 175:
    case 177:
    case 178:
    case 179:
    case 186:
    case 187:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 192:
    case 197:
    case 200:
    case 202:
    case 204:
    case 205:
    case 206:
    case 209:
    case 212:
    case 218:
    case 219:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 128:
    case 136:
    case 138:
    case 140:
    case 141:
    case 142:
    case 143:
    case 144:
    case 145:
    case 147:
    case 153:
    case 154:
    case 155:
    case 156:
    case 158:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 64:
    case 66:
    case 73:
    case 74:
    case 76:
    case 77:
    case 78:
    case 79:
    case 81:
    case 91:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 96:
    case 104:
    case 105:
    case 106:
    case 109:
    case 110:
    case 111:
    case 113:
    case 117:
    case 121:
    case 122:
    case 123:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 32:
    case 42:
    case 45:
    case 46:
    case 47:
    case 51:
    case 52:
    case 55:
    case 59:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 224:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 241:
    case 243:
    case 244:
    case 250:
    case 251:
    case 252:
    case 253:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 256:
    case 265:
    case 266:
    case 267:
    case 269:
    case 270:
    case 271:
    case 276:
    case 283:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F4B434F525F42ULL; /* "B_ROCK_SKID_L" */
        break;
    case 1:
    case 33:
    case 56:
    case 65:
    case 88:
    case 97:
    case 161:
    case 193:
    case 199:
    case 225:
    case 257:
    case 288:
    case 289:
    case 301:
    case 305:
    case 315:
        name.v = ((u128__c1)0x4C5F44494BULL << 64) | (u128__c1)0x535F545249445F42ULL; /* "B_DIRT_SKID_L" */
        break;
    }
    return name.v;
}
extern u128__c1 func_001CA768(Obj__c1 *self, int surface, int flag);
u128__c1 func_001CA768(Obj__c1 *self, int surface, int flag)
{
    Name name;

    name.v = 0;
    *(Name9 *)name.s = *(Name9 *)D_0028FD10;
    switch (surface) {
    case 2:
    case 66:
    case 98:
    case 162:
    case 194:
    case 226:
    case 258:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F534152475F42ULL; /* "B_GRAS_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F534152475F42ULL; /* "B_GRAS_COAST" */
        break;
    case 3:
    case 35:
    case 67:
    case 99:
    case 131:
    case 163:
    case 195:
    case 227:
    case 259:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 5:
    case 133:
    case 197:
    case 261:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F574F4E535F42ULL; /* "B_SNOW_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F574F4E535F42ULL; /* "B_SNOW_COAST" */
        break;
    case 6:
    case 38:
    case 70:
    case 102:
    case 134:
    case 166:
    case 198:
    case 230:
    case 262:
        if (flag) name.v = ((u128__c1)0x4C4144ULL << 64) | (u128__c1)0x45505F4F32485F42ULL; /* "B_H2O_PEDAL" */
        else name.v = ((u128__c1)0x545341ULL << 64) | (u128__c1)0x4F435F4F32485F42ULL; /* "B_H2O_COAST" */
        break;
    case 40:
    case 72:
    case 104:
    case 106:
    case 136:
    case 168:
    case 170:
    case 232:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F474452425F42ULL; /* "B_BRDG_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F474452425F42ULL; /* "B_BRDG_COAST" */
        break;
    case 81:
    case 113:
    case 145:
    case 177:
    case 209:
    case 273:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F434E4F435F42ULL; /* "B_CONC_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F434E4F435F42ULL; /* "B_CONC_COAST" */
        break;
    case 122:
    case 154:
    case 186:
    case 200:
    case 218:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F464F4F525F42ULL; /* "B_ROOF_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F464F4F525F42ULL; /* "B_ROOF_COAST" */
        break;
    case 121:
    case 153:
    case 264:
    case 268:
    case 281:
    case 282:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B4152545F42ULL; /* "B_TRAK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B4152545F42ULL; /* "B_TRAK_COAST" */
        break;
    case 144:
        if (flag) name.v = ((u128__c1)0x4C4144ULL << 64) | (u128__c1)0x45505F4543495F42ULL; /* "B_ICE_PEDAL" */
        else name.v = ((u128__c1)0x545341ULL << 64) | (u128__c1)0x4F435F4543495F42ULL; /* "B_ICE_COAST" */
        break;
    case 82:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4156414C5F42ULL; /* "B_LAVA_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4156414C5F42ULL; /* "B_LAVA_COAST" */
        break;
    case 39:
    case 71:
    case 263:
        if (flag) name.v = ((u128__c1)0x4C4144ULL << 64) | (u128__c1)0x45505F44554D5F42ULL; /* "B_MUD_PEDAL" */
        else name.v = ((u128__c1)0x545341ULL << 64) | (u128__c1)0x4F435F44554D5F42ULL; /* "B_MUD_COAST" */
        break;
    case 88:
    case 280:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F444E41535F42ULL; /* "B_SAND_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F444E41535F42ULL; /* "B_SAND_COAST" */
        break;
    case 315:
        if (flag) name.v = ((u128__c1)0x4C5F45434EULL << 64) | (u128__c1)0x45465F5243535F42ULL; /* "B_SCR_FENCE_L" */
        else name.v = ((u128__c1)0x4C5F45434EULL << 64) | (u128__c1)0x45465F5243535F42ULL; /* "B_SCR_FENCE_L" */
        break;
    case 0:
    case 1:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 27:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 32:
    case 42:
    case 45:
    case 46:
    case 47:
    case 51:
    case 52:
    case 55:
    case 56:
    case 59:
    case 76:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 64:
    case 73:
    case 74:
    case 77:
    case 78:
    case 79:
    case 91:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 96:
    case 105:
    case 109:
    case 110:
    case 111:
    case 117:
    case 123:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 128:
    case 138:
    case 140:
    case 141:
    case 142:
    case 143:
    case 147:
    case 155:
    case 156:
    case 158:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 160:
    case 169:
    case 171:
    case 172:
    case 173:
    case 174:
    case 175:
    case 178:
    case 179:
    case 187:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 192:
    case 202:
    case 204:
    case 205:
    case 206:
    case 212:
    case 219:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 224:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 241:
    case 243:
    case 244:
    case 250:
    case 251:
    case 252:
    case 253:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 256:
    case 265:
    case 266:
    case 267:
    case 269:
    case 270:
    case 271:
    case 276:
    case 283:
        if (flag) name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F4B434F525F42ULL; /* "B_ROCK_PEDAL" */
        else name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F4B434F525F42ULL; /* "B_ROCK_COAST" */
        break;
    case 33:
    case 65:
    case 97:
    case 161:
    case 193:
    case 199:
    case 225:
    case 257:
    case 288:
    case 289:
    case 301:
    case 305:
        if (!flag) name.v = ((u128__c1)0x5453414FULL << 64) | (u128__c1)0x435F545249445F42ULL; /* "B_DIRT_COAST" */
        else name.v = ((u128__c1)0x4C414445ULL << 64) | (u128__c1)0x505F545249445F42ULL; /* "B_DIRT_PEDAL" */
        break;
    }
    return name.v;
}
extern u128__c1 func_001CB078(Obj__c1 *self, int surface);
u128__c1 func_001CB078(Obj__c1 *self, int surface)
{
    Name name;

    name.v = 0;
    *(Name9 *)name.s = *(Name9 *)D_0028FD10;
    switch (surface) {
    case 2:
    case 66:
    case 98:
    case 162:
    case 194:
    case 226:
    case 258:
        name.v = ((u128__c1)0x454449525F53ULL << 64) | (u128__c1)0x4152475F49415F42ULL; /* "B_AI_GRAS_RIDE" */
        break;
    case 3:
    case 35:
    case 67:
    case 99:
    case 131:
    case 163:
    case 195:
    case 227:
    case 259:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 5:
    case 133:
    case 197:
    case 261:
        name.v = ((u128__c1)0x454449525F57ULL << 64) | (u128__c1)0x4F4E535F49415F42ULL; /* "B_AI_SNOW_RIDE" */
        break;
    case 6:
    case 38:
    case 70:
    case 102:
    case 134:
    case 166:
    case 198:
    case 230:
    case 262:
        name.v = ((u128__c1)0x454449525FULL << 64) | (u128__c1)0x4F32485F49415F42ULL; /* "B_AI_H2O_RIDE" */
        break;
    case 40:
    case 72:
    case 104:
    case 136:
    case 168:
    case 232:
        name.v = ((u128__c1)0x454449525F47ULL << 64) | (u128__c1)0x4452425F49415F42ULL; /* "B_AI_BRDG_RIDE" */
        break;
    case 81:
    case 113:
    case 145:
    case 177:
    case 209:
    case 273:
        name.v = ((u128__c1)0x454449525F43ULL << 64) | (u128__c1)0x4E4F435F49415F42ULL; /* "B_AI_CONC_RIDE" */
        break;
    case 122:
    case 154:
    case 186:
    case 200:
    case 218:
        name.v = ((u128__c1)0x454449525F46ULL << 64) | (u128__c1)0x4F4F525F49415F42ULL; /* "B_AI_ROOF_RIDE" */
        break;
    case 121:
    case 153:
    case 264:
    case 268:
    case 281:
    case 282:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x4152545F49415F42ULL; /* "B_AI_TRAK_RIDE" */
        break;
    case 144:
        name.v = ((u128__c1)0x454449525FULL << 64) | (u128__c1)0x4543495F49415F42ULL; /* "B_AI_ICE_RIDE" */
        break;
    case 82:
        name.v = ((u128__c1)0x454449525F41ULL << 64) | (u128__c1)0x56414C5F49415F42ULL; /* "B_AI_LAVA_RIDE" */
        break;
    case 39:
    case 71:
    case 263:
        name.v = ((u128__c1)0x454449525FULL << 64) | (u128__c1)0x44554D5F49415F42ULL; /* "B_AI_MUD_RIDE" */
        break;
    case 88:
    case 280:
        name.v = ((u128__c1)0x454449525F44ULL << 64) | (u128__c1)0x4E41535F49415F42ULL; /* "B_AI_SAND_RIDE" */
        break;
    case 315:
        name.v = ((u128__c1)0x4C5F45434EULL << 64) | (u128__c1)0x45465F5243535F42ULL; /* "B_SCR_FENCE_L" */
        break;
    case 0:
    case 1:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 27:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 32:
    case 42:
    case 45:
    case 46:
    case 47:
    case 51:
    case 52:
    case 55:
    case 56:
    case 59:
    case 76:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 64:
    case 73:
    case 74:
    case 77:
    case 78:
    case 79:
    case 91:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 96:
    case 105:
    case 106:
    case 109:
    case 110:
    case 111:
    case 117:
    case 123:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 128:
    case 138:
    case 140:
    case 141:
    case 142:
    case 143:
    case 147:
    case 155:
    case 156:
    case 158:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 160:
    case 169:
    case 170:
    case 171:
    case 172:
    case 173:
    case 174:
    case 175:
    case 178:
    case 179:
    case 187:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 192:
    case 202:
    case 204:
    case 205:
    case 206:
    case 212:
    case 219:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 224:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 241:
    case 243:
    case 244:
    case 250:
    case 251:
    case 252:
    case 253:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 256:
    case 257:
    case 265:
    case 266:
    case 267:
    case 269:
    case 270:
    case 271:
    case 276:
    case 283:
        name.v = ((u128__c1)0x454449525F4BULL << 64) | (u128__c1)0x434F525F49415F42ULL; /* "B_AI_ROCK_RIDE" */
        break;
    case 33:
    case 65:
    case 97:
    case 161:
    case 193:
    case 199:
    case 225:
    case 288:
    case 289:
    case 301:
    case 305:
        name.v = ((u128__c1)0x454449525F54ULL << 64) | (u128__c1)0x5249445F49415F42ULL; /* "B_AI_DIRT_RIDE" */
        break;
    }
    return name.v;
}

typedef struct ColObj {
    char pad0[0x88];
    char *x88;
    char pad8C[0x8];
    int x94;
    float x98;
    int x9C;
    float xA0;
    char padA4[0xC4];
    int x168;
} ColObj;

extern int D_0029E518;
extern int D_004B2840__c1[] asm("D_004B2840");

#define NAME(hi, lo) (((u128__c1)hi##LL << 64) | (u128__c1)lo##LL)
#define NAME_B_COL_ST_POLE   NAME(0x454C4F505F, 0x54535F4C4F435F42)
#define NAME_B_COL_TREE_SML  NAME(0x4C4D535F4545, 0x52545F4C4F435F42)
#define NAME_B_COL_TREE_BIG  NAME(0x4749425F4545, 0x52545F4C4F435F42)
#define NAME_B_COL_WOODSTRC  NAME(0x43525453444F, 0x4F575F4C4F435F42)
#define NAME_B_COL_DEER      NAME(0x5245, 0x45445F4C4F435F42)
#define NAME_B_COL_HELI      NAME(0x494C, 0x45485F4C4F435F42)
#define NAME_B_COL_HAY       NAME(0x59, 0x41485F4C4F435F42)
#define NAME_B_COL_AUTO      NAME(0x4F54, 0x55415F4C4F435F42)
#define NAME_B_COL_FENCE     NAME(0x45434E, 0x45465F4C4F435F42)

extern void func_001CB638(ColObj *self) asm("func_001CB638");
void func_001CB638(ColObj *self)
{
    Choice choice;
    int count, other;

    count = 0;
    other = 0;
    if ((unsigned char)(*(unsigned long *)(self->x88 + 0x4098) >> 2) & 1) {
        return;
    }
    func_001DB6B0__c1(*(char **)(self->x88 + 0x792C) + 0x10, D_0029D5C4, D_0029D5C8,
                  &count, &other, 0);
    if (self->x98 > self->xA0) {
        choice.index = self->x94;
        choice.value = self->x98;
    } else {
        choice.index = self->x9C;
        choice.value = self->xA0;
    }
    if (count > 0) {
        if (choice.value < (float)D_0029E518) {
            count = (float)count * (choice.value / (float)D_0029E518);
        }
        switch (choice.index) {
        case 13: case 14: case 45: case 46: case 77: case 78: case 109: case 110:
        case 117: case 141: case 142: case 173: case 205: case 237: case 269:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_ST_POLE, count, other, 0, 0);
            break;
        case 15: case 47: case 79: case 111: case 170: case 239: case 271:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_TREE_SML, count, other, 0, 0);
            break;
        case 10: case 42: case 74: case 106: case 175: case 202: case 234: case 266:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_TREE_BIG, count, other, 0, 0);
            break;
        case 168: case 172: case 204: case 236: case 268:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_WOODSTRC, count, other, 0, 0);
            break;
        case 23:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_DEER, count, other, 0, 0);
            break;
        case 174: case 206: case 238: case 270:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_HELI, count, other, 0, 0);
            break;
        case 20: case 52: case 178: case 212: case 244: case 276:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_HAY, count, other, 0, 0);
            break;
        case 51: case 147: case 156: case 158: case 171: case 179: case 235: case 243:
        case 252: case 267:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_AUTO, count, other, 0, 0);
            break;
        case 315:
            func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_FENCE, count, other, 0, 0);
            break;
        case 136:
            if (D_004B2840__c1[0] == 0x16) {
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_WOODSTRC, count, other, 0, 0);
            }
            break;
        }
    }
}


/* .data holds a word equal to an address inside this function's jump table (entry 114);
   splat named it D_00292238, so the symbol must exist. The .align 4 keeps the previous
   unit's trailing padding inside this object. */
__asm__(".section .rodata
.align 4
.section .text
.globl D_00292238
D_00292238 = 0x00292238");
#define NAME_K_CRASH_ALL NAME(0x4C4C41, 0x5F48534152435F4B)
#define NAME_B_BRDG_CRASH NAME(0x48534152, 0x435F474452425F42)
#define NAME_B_SNOW_CRASH NAME(0x48534152, 0x435F574F4E535F42)
#define NAME_B_H2O_CRASH NAME(0x485341, 0x52435F4F32485F42)
#define NAME_B_LAVA_CRASH NAME(0x48534152, 0x435F4156414C5F42)
#define NAME_B_MUD_CRASH NAME(0x485341, 0x52435F44554D5F42)
#define NAME_B_DIRT_CRASH NAME(0x48534152, 0x435F545249445F42)
#define NAME_B_CONC_CRASH NAME(0x48534152, 0x435F434E4F435F42)
#define NAME_B_GRAS_CRASH NAME(0x48534152, 0x435F534152475F42)
#define NAME_B_ROOF_CRASH NAME(0x48534152, 0x435F464F4F525F42)
#define NAME_B_SAND_CRASH NAME(0x48534152, 0x435F444E41535F42)
#define NAME_B_TRAK_CRASH NAME(0x48534152, 0x435F4B4152545F42)
#define NAME_B_ROCK_CRASH NAME(0x48534152, 0x435F4B434F525F42)
#define NAME_R_GR_FEM_BIG NAME(0x4749425F, 0x4D45465F52475F52)
#define NAME_R_GR_MAL_BIG NAME(0x4749425F, 0x4C414D5F52475F52)
#define NAME_R_GR_FEM_MED NAME(0x44454D5F, 0x4D45465F52475F52)
#define NAME_R_GR_FEM_SML NAME(0x4C4D535F, 0x4D45465F52475F52)
#define NAME_R_GR_MAL_MED NAME(0x44454D5F, 0x4C414D5F52475F52)
#define NAME_R_GR_MAL_SML NAME(0x4C4D535F, 0x4C414D5F52475F52)
#define NAME_E_BF_SML_ANIMAL NAME(0x4C414D494E415F, 0x4C4D535F46425F45)
#define NAME_E_BF_BIG_ANIMAL NAME(0x4C414D494E415F, 0x4749425F46425F45)

extern int D_004BD8A4[];
/* func_001C7A60__c1 declared above */
extern int func_00222B08(int, int);
extern void func_001BFD50(void *, int);


typedef struct CrashOwner {
    char pad0[0x7908];
    int x7908;
    char pad790C[0x20];
    char *x792C;
    char pad7930[0x3C];
    int x796C;
    char pad7970[0xEC];
    int x7A5C;
} CrashOwner;

typedef struct CrashObj {
    char pad0[0x88];
    CrashOwner *x88;
    char pad8C[8];
    int x94;
    float x98;
    int x9C;
    float xA0;
    char padA4[0x5C];
    float x100;
    char pad104[0x64];
    int x168;
} CrashObj;

extern void func_001CBA18__c1(CrashObj *self, int flag) asm("func_001CBA18");
void func_001CBA18__c1(CrashObj *self, int flag)
{
    Choice choice;
    int count, other;
    unsigned int random;

    count = 0;
    other = 0;
    if (self->x88->x7A5C == 1) {
        func_001C7A60__c1((Obj__c1 *)self, 15, self->x100);
    } else {
        func_001C7A60__c1((Obj__c1 *)self, 6, self->x100);
    }
    func_001DB6B0__c1(self->x88->x792C + 0x10,
                  D_0029D5C4, D_0029D5C8, &count, &other, 0);
    if (self->x98 > self->xA0) {
        choice.index = self->x94;
        choice.value = self->x98;
    } else {
        choice.index = self->x9C;
        choice.value = self->xA0;
    }
    if (count > 0) {
        self->x168 = func_00238F48();
        if ((unsigned int)(self->x88->x7908 - 0x23) >= 3U) {
            if (flag != 1) {
                if (self->x88->x796C == 0x200) {
                    func_00201B68(D_004BD8A4[0], 0, NAME_K_CRASH_ALL, count, other, 0, 0);
                }
                switch (choice.index) {
            case 13: case 14: case 45: case 46: case 77: case 78: case 109: case 110:
            case 117: case 141: case 142: case 173: case 205: case 237: case 269:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_ST_POLE, count, other, 0, 0);
                break;
            case 15: case 47: case 79: case 111: case 170: case 239: case 271:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_TREE_SML, count, other, 0, 0);
                break;
            case 10: case 42: case 74: case 106: case 175: case 202: case 234: case 266:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_TREE_BIG, count, other, 0, 0);
                break;
            case 12: case 40: case 72: case 76: case 104: case 136: case 232:
                if (D_004B2840__c1[0] == 0x16) {
                    func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_WOODSTRC, count, other, 0, 0);
                } else {
                    func_00201B68(D_004BD8A0[0], 0, NAME_B_BRDG_CRASH, count, other, 0, 0);
                }
                break;
            case 168: case 172: case 204: case 236: case 268:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_WOODSTRC, count, other, 0, 0);
                break;
            case 5: case 133: case 144: case 197: case 261:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_SNOW_CRASH, count, other, 0, 0);
                break;
            case 6: case 38: case 70: case 102: case 134: case 166: case 198: case 230:
            case 262:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_H2O_CRASH, count, other, 0, 0);
                break;
            case 82:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_LAVA_CRASH, count, other, 0, 0);
                break;
            case 39: case 71: case 263:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_MUD_CRASH, count, other, 0, 0);
                break;
            case 23:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_DEER, count, other, 0, 0);
                break;
            case 33: case 65: case 97: case 161: case 193: case 199: case 225: case 257:
            case 289:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_DIRT_CRASH, count, other, 0, 0);
                break;
            case 177: case 209: case 273:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_CONC_CRASH, count, other, 0, 0);
                break;
            case 98: case 162: case 194: case 226: case 258:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_GRAS_CRASH, count, other, 0, 0);
                break;
            case 186: case 200: case 218:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_ROOF_CRASH, count, other, 0, 0);
                break;
            case 174: case 206: case 238: case 270:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_HELI, count, other, 0, 0);
                break;
            case 20: case 52: case 178: case 212: case 244: case 276:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_HAY, count, other, 0, 0);
                break;
            case 51: case 147: case 156: case 158: case 171: case 179: case 235: case 243:
            case 252: case 267:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_AUTO, count, other, 0, 0);
                break;
            case 280:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_SAND_CRASH, count, other, 0, 0);
                break;
            case 264: case 281: case 282:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_TRAK_CRASH, count, other, 0, 0);
                break;
            case 315:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_COL_FENCE, count, other, 0, 0);
                break;
            case 0: case 1: case 2: case 3: case 26: case 27: case 32: case 35:
            case 55: case 56: case 59: case 64: case 66: case 67: case 73: case 81:
            case 88: case 91: case 96: case 99: case 105: case 113: case 121: case 122:
            case 123: case 128: case 131: case 138: case 140: case 143: case 145: case 153:
            case 154: case 155: case 160: case 163: case 169: case 187: case 192: case 195:
            case 219: case 224: case 227: case 233: case 241: case 250: case 251: case 253:
            case 256: case 259: case 265: case 283:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_ROCK_CRASH, count, other, 0, 0);
                break;
            case 288: case 301: case 305:
                func_00201B68(D_004BD8A0[0], 0, NAME_B_DIRT_CRASH, count, other, 0, 0);
                break;
                }
            }
            func_001DB6B0__c1(self->x88->x792C + 0x10,
                          7, 20, &count, &other, 0);
            random = func_00222B08(0, 3);
            if (choice.value > 5000.0f) {
                switch (self->x88->x796C) {
                case 0x40: case 0x80: case 0x100: case 0x160:
                    func_00201B68(D_004BD8A4[0], 0, NAME_R_GR_FEM_BIG, count, other, 0, 0);
                    break;
                default:
                    func_00201B68(D_004BD8A4[0], 0, NAME_R_GR_MAL_BIG, count, other, 0, 0);
                    break;
                }
            } else if (random < 2U) {
                switch (self->x88->x796C) {
                case 0x40: case 0x80: case 0x100: case 0x160:
                    if (choice.value > 2500.0f) {
                        func_00201B68(D_004BD8A4[0], 0, NAME_R_GR_FEM_MED, count, other, 0, 0);
                    } else {
                        func_00201B68(D_004BD8A4[0], 0, NAME_R_GR_FEM_SML, count, other, 0, 0);
                    }
                    break;
                default:
                    if (choice.value > 2500.0f) {
                        func_00201B68(D_004BD8A4[0], 0, NAME_R_GR_MAL_MED, count, other, 0, 0);
                    } else {
                        func_00201B68(D_004BD8A4[0], 0, NAME_R_GR_MAL_SML, count, other, 0, 0);
                    }
                    break;
                }
            }
        } else {
            if (self->x88->x7908 == 0x24) {
                func_00201B68(D_004BD8A0[0], 0, NAME_E_BF_SML_ANIMAL, count, other, 0, 0);
            } else {
                func_00201B68(D_004BD8A0[0], 0, NAME_E_BF_BIG_ANIMAL, count, other, 0, 0);
            }
            func_001BFD50((char *)self + 0x140, self->x88->x7908);
        }
    }
}
extern int D_004BD8A4[];

#define NAME_K_CRASH_ALL     NAME(0x4C4C41, 0x5F48534152435F4B)
#define NAME_B_BRDG_CRASH    NAME(0x48534152, 0x435F474452425F42)
#define NAME_B_SNOW_CRASH    NAME(0x48534152, 0x435F574F4E535F42)
#define NAME_B_H2O_CRASH     NAME(0x485341, 0x52435F4F32485F42)
#define NAME_B_LAVA_CRASH    NAME(0x48534152, 0x435F4156414C5F42)
#define NAME_B_MUD_CRASH     NAME(0x485341, 0x52435F44554D5F42)
#define NAME_B_DIRT_CRASH    NAME(0x48534152, 0x435F545249445F42)
#define NAME_B_ROCK_CRASH    NAME(0x48534152, 0x435F4B434F525F42)
#define NAME_B_CONC_CRASH    NAME(0x48534152, 0x435F434E4F435F42)
#define NAME_B_GRAS_CRASH    NAME(0x48534152, 0x435F534152475F42)
#define NAME_B_ROOF_CRASH    NAME(0x48534152, 0x435F464F4F525F42)
#define NAME_B_SAND_CRASH    NAME(0x48534152, 0x435F444E41535F42)
#define NAME_B_TRAK_CRASH    NAME(0x48534152, 0x435F4B4152545F42)
#define NAME_E_BF_SML_ANIMAL NAME(0x4C414D494E415F, 0x4C4D535F46425F45)
#define NAME_E_BF_BIG_ANIMAL NAME(0x4C414D494E415F, 0x4749425F46425F45)

#define PLAY_CRASH(name) func_00201B68(D_004BD8A0[0], 0, name, count, other, 0, 0)

/* Crash sound for the surface hit hardest; at most one every 18 ticks, volume scaled by
   speed below 1000. */
extern void func_001CC520(ColObj *self, float speed);
void func_001CC520(ColObj *self, float speed)
{
    Choice choice;
    int count, other;

    count = 0;
    other = 0;
    if ((float)(func_00238F48() - self->x168) > 18.0f) {
        func_001DB6B0__c1(*(char **)(self->x88 + 0x792C) + 0x10, D_0029D5C4, D_0029D5C8,
                      &count, &other, 0);
        if (self->x98 > self->xA0) {
            choice.index = self->x94;
            choice.value = self->x98;
        } else {
            choice.index = self->x9C;
            choice.value = self->xA0;
        }
        if (speed < 1000.0f) {
            count = (float)count * 0.001f * speed;
        }
        if (count > 0) {
            self->x168 = func_00238F48();
            if ((unsigned int)(*(int *)(self->x88 + 0x7908) - 35) >= 3) {
                if (*(int *)(self->x88 + 0x796C) == 0x200) {
                    func_00201B68(D_004BD8A4[0], 0, NAME_K_CRASH_ALL, count, other, 0, 0);
                }
                switch (choice.index) {
                case 13: case 14: case 45: case 46: case 77: case 78: case 109: case 110:
                case 117: case 141: case 142: case 173: case 205: case 237: case 269:
                    PLAY_CRASH(NAME_B_COL_ST_POLE);
                    break;
                case 15: case 47: case 79: case 111: case 170: case 239: case 271:
                    PLAY_CRASH(NAME_B_COL_TREE_SML);
                    break;
                case 10: case 42: case 74: case 106: case 175: case 202: case 234: case 266:
                    PLAY_CRASH(NAME_B_COL_TREE_BIG);
                    break;
                case 12: case 40: case 72: case 76: case 104: case 136: case 232:
                    if (D_004B2840__c1[0] == 0x16) {
                        PLAY_CRASH(NAME_B_COL_WOODSTRC);
                    } else {
                        PLAY_CRASH(NAME_B_BRDG_CRASH);
                    }
                    break;
                case 168: case 172: case 204: case 236: case 268:
                    PLAY_CRASH(NAME_B_COL_WOODSTRC);
                    break;
                case 5: case 133: case 144: case 197: case 261:
                    PLAY_CRASH(NAME_B_SNOW_CRASH);
                    break;
                case 6: case 38: case 70: case 102: case 134: case 166: case 198: case 230:
                case 262:
                    PLAY_CRASH(NAME_B_H2O_CRASH);
                    break;
                case 82:
                    PLAY_CRASH(NAME_B_LAVA_CRASH);
                    break;
                case 39: case 71: case 263:
                    PLAY_CRASH(NAME_B_MUD_CRASH);
                    break;
                case 23:
                    PLAY_CRASH(NAME_B_COL_DEER);
                    break;
                case 33: case 65: case 97: case 161: case 193: case 199: case 225: case 257:
                case 289:
                    PLAY_CRASH(NAME_B_DIRT_CRASH);
                    break;
                case 3: case 35: case 67: case 99: case 131: case 163: case 195: case 227:
                case 259:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 177: case 209: case 273:
                    PLAY_CRASH(NAME_B_CONC_CRASH);
                    break;
                case 98: case 162: case 194: case 226: case 258:
                    PLAY_CRASH(NAME_B_GRAS_CRASH);
                    break;
                case 186: case 200: case 218:
                    PLAY_CRASH(NAME_B_ROOF_CRASH);
                    break;
                case 174: case 206: case 238: case 270:
                    PLAY_CRASH(NAME_B_COL_HELI);
                    break;
                case 20: case 52: case 178: case 212: case 244: case 276:
                    PLAY_CRASH(NAME_B_COL_HAY);
                    break;
                case 51: case 147: case 156: case 158: case 171: case 179: case 235: case 243:
                case 252: case 267:
                    PLAY_CRASH(NAME_B_COL_AUTO);
                    break;
                case 280:
                    PLAY_CRASH(NAME_B_SAND_CRASH);
                    break;
                case 264: case 281: case 282:
                    PLAY_CRASH(NAME_B_TRAK_CRASH);
                    break;
                case 315:
                    PLAY_CRASH(NAME_B_COL_FENCE);
                    break;
                case 0: case 1: case 2: case 26: case 27:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 32: case 55: case 56: case 59:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 64: case 66: case 73: case 81: case 88: case 91:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 96: case 105: case 113: case 121: case 122: case 123:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 128: case 138: case 140: case 143: case 145: case 153: case 154: case 155:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 160: case 169: case 187:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 192: case 219:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 224: case 233: case 241: case 250: case 251: case 253:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 256: case 265: case 283:
                    PLAY_CRASH(NAME_B_ROCK_CRASH);
                    break;
                case 288: case 301: case 305:
                    PLAY_CRASH(NAME_B_DIRT_CRASH);
                    break;
                }
            } else if (*(int *)(self->x88 + 0x7908) == 36) {
                PLAY_CRASH(NAME_E_BF_SML_ANIMAL);
            } else {
                PLAY_CRASH(NAME_E_BF_BIG_ANIMAL);
            }
        }
    }
}

extern int D_0029E50C;
extern int D_0029E510;
extern int D_0029E51C;
extern int D_0029E520;
extern int D_0029E524;
extern int D_004BD8A4[];
extern int func_00222B08(int, int);
extern void func_001BFEA0(void *, int);

#define SFX_K_GR_MED (((u128__c1)0x0UL << 64) | (u128__c1)0x44454D5F52475F4BUL)
#define SFX_K_GR_BIG (((u128__c1)0x0UL << 64) | (u128__c1)0x4749425F52475F4BUL)
#define SFX_K_GR_SML (((u128__c1)0x0UL << 64) | (u128__c1)0x4C4D535F52475F4BUL)
#define SFX_K_LAND_ALL (((u128__c1)0x4C4CUL << 64) | (u128__c1)0x415F444E414C5F4BUL)
#define SFX_B_MUD_BUMP (((u128__c1)0x504DUL << 64) | (u128__c1)0x55425F44554D5F42UL)
#define SFX_B_H2O_BUMP (((u128__c1)0x504DUL << 64) | (u128__c1)0x55425F4F32485F42UL)
#define SFX_B_MUD_SUSP (((u128__c1)0x5053UL << 64) | (u128__c1)0x55535F44554D5F42UL)
#define SFX_B_H2O_SUSP (((u128__c1)0x5053UL << 64) | (u128__c1)0x55535F4F32485F42UL)
#define SFX_B_LAVA_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F4156414C5F42UL)
#define SFX_B_CONC_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F434E4F435F42UL)
#define SFX_B_SAND_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F444E41535F42UL)
#define SFX_B_ROOF_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F464F4F525F42UL)
#define SFX_B_BRDG_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F474452425F42UL)
#define SFX_B_TRAK_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F4B4152545F42UL)
#define SFX_B_ROCK_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F4B434F525F42UL)
#define SFX_B_GRAS_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F534152475F42UL)
#define SFX_B_DIRT_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F545249445F42UL)
#define SFX_B_SNOW_BUMP (((u128__c1)0x504D55UL << 64) | (u128__c1)0x425F574F4E535F42UL)
#define SFX_B_LAVA_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F4156414C5F42UL)
#define SFX_B_CONC_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F434E4F435F42UL)
#define SFX_B_SAND_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F444E41535F42UL)
#define SFX_B_ROOF_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F464F4F525F42UL)
#define SFX_B_BRDG_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F474452425F42UL)
#define SFX_B_TRAK_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F4B4152545F42UL)
#define SFX_B_ROCK_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F4B434F525F42UL)
#define SFX_B_GRAS_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F534152475F42UL)
#define SFX_B_DIRT_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F545249445F42UL)
#define SFX_B_SNOW_SUSP (((u128__c1)0x505355UL << 64) | (u128__c1)0x535F574F4E535F42UL)
#define SFX_R_GR_MAL_MED (((u128__c1)0x44454D5FUL << 64) | (u128__c1)0x4C414D5F52475F52UL)
#define SFX_R_GR_FEM_MED (((u128__c1)0x44454D5FUL << 64) | (u128__c1)0x4D45465F52475F52UL)
#define SFX_R_GR_MAL_BIG (((u128__c1)0x4749425FUL << 64) | (u128__c1)0x4C414D5F52475F52UL)
#define SFX_R_GR_FEM_BIG (((u128__c1)0x4749425FUL << 64) | (u128__c1)0x4D45465F52475F52UL)
#define SFX_R_GR_MAL_SML (((u128__c1)0x4C4D535FUL << 64) | (u128__c1)0x4C414D5F52475F52UL)
#define SFX_R_GR_FEM_SML (((u128__c1)0x4C4D535FUL << 64) | (u128__c1)0x4D45465F52475F52UL)
#define SFX_B_MUD_LAND_BG (((u128__c1)0x47425F444EUL << 64) | (u128__c1)0x414C5F44554D5F42UL)
#define SFX_B_H2O_LAND_BG (((u128__c1)0x47425F444EUL << 64) | (u128__c1)0x414C5F4F32485F42UL)
#define SFX_B_MUD_LAND_SM (((u128__c1)0x4D535F444EUL << 64) | (u128__c1)0x414C5F44554D5F42UL)
#define SFX_B_H2O_LAND_SM (((u128__c1)0x4D535F444EUL << 64) | (u128__c1)0x414C5F4F32485F42UL)
#define SFX_E_RIDE_SML_PAD (((u128__c1)0x4441505F4C4DUL << 64) | (u128__c1)0x535F454449525F45UL)
#define SFX_B_LAVA_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F4156414C5F42UL)
#define SFX_B_CONC_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F434E4F435F42UL)
#define SFX_B_SAND_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F444E41535F42UL)
#define SFX_B_ROOF_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F464F4F525F42UL)
#define SFX_B_BRDG_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F474452425F42UL)
#define SFX_B_TRAK_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F4B4152545F42UL)
#define SFX_B_ROCK_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F4B434F525F42UL)
#define SFX_B_GRAS_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F534152475F42UL)
#define SFX_B_DIRT_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F545249445F42UL)
#define SFX_B_SNOW_LAND_BG (((u128__c1)0x47425F444E41UL << 64) | (u128__c1)0x4C5F574F4E535F42UL)
#define SFX_B_LAVA_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F4156414C5F42UL)
#define SFX_B_CONC_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F434E4F435F42UL)
#define SFX_B_SAND_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F444E41535F42UL)
#define SFX_B_ROOF_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F464F4F525F42UL)
#define SFX_B_BRDG_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F474452425F42UL)
#define SFX_B_TRAK_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F4B4152545F42UL)
#define SFX_B_ROCK_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F4B434F525F42UL)
#define SFX_B_GRAS_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F534152475F42UL)
#define SFX_B_DIRT_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F545249445F42UL)
#define SFX_B_SNOW_LAND_SM (((u128__c1)0x4D535F444E41UL << 64) | (u128__c1)0x4C5F574F4E535F42UL)
#define SFX_E_RIDE_BIG_HOOF (((u128__c1)0x464F4F485F4749UL << 64) | (u128__c1)0x425F454449525F45UL)
#define SFX_E_RIDE_SML_HOOF (((u128__c1)0x464F4F485F4C4DUL << 64) | (u128__c1)0x535F454449525F45UL)


extern void func_001CCFB8(Obj__c1 *self, float impact) asm("func_001CCFB8");
void func_001CCFB8(Obj__c1 *self, float impact)
{
    int suspend = 0, bump = 0, landSmall = 0, landBig = 0;
    Choice choice;
    int count, other;
    int disabled;
    char *owner = *(char *volatile *)&self->x88;

    *(volatile int *)&count = 0;
    disabled = (unsigned char)(*(unsigned long *)(owner + 0x4098) >> 2) & 1;
    other = 0;
    if (disabled)
        return;
    if (impact < (float)D_0029E510)
        return;
    func_001DB6B0__c1(*(char **)(owner + 0x792C) + 0x10,
                  D_0029D5C4, D_0029D5C8, &count, &other, 0);
    if (*(float *)((char *)self + 0x98) > *(float *)((char *)self + 0xA0)) {
        choice.index = *(int *)((char *)self + 0x94);
        choice.value = *(float *)((char *)self + 0x98);
    } else {
        choice.index = *(int *)((char *)self + 0x9C);
        choice.value = *(float *)((char *)self + 0xA0);
    }
    if (count <= 0)
        return;
    if (*(short *)(self->x88 + 0x4566) < 61) {
        if (impact < (float)D_0029E510)
            count = 0;
        if (impact > (float)D_0029E510 && impact < (float)D_0029E51C) {
            suspend = 1;
        } else if (impact >= (float)D_0029E51C && impact < (float)D_0029E520) {
            bump = 1;
            count = D_0029E50C + (int)((float)(1024 - D_0029E50C) *
                ((impact - (float)D_0029E51C) / (float)(D_0029E520 - D_0029E51C)));
        } else if (impact >= (float)D_0029E520 && impact < (float)D_0029E524) {
            landSmall = 1;
            count = D_0029E50C + (int)((float)(1024 - D_0029E50C) *
                ((impact - (float)D_0029E520) / (float)(D_0029E524 - D_0029E520)));
        } else if (impact >= (float)D_0029E524) {
            landBig = 1;
            count = 1024;
        }
    } else {
        landBig = 1;
        count = 1024;
    }
    if (count > 0) {
        if (*(int *)(self->x88 + 0x796C) == 0x200) {
            func_00201B68(D_004BD8A4[0], 0, SFX_K_LAND_ALL, count, other, 0, 0);
        }
        if ((unsigned int)(*(int *)(self->x88 + 0x7908) - 35) >= 3) {
            switch (choice.index) {
            case 0x5:
            case 0x85:
            case 0x90:
            case 0xC5:
            case 0x105:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SNOW_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SNOW_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SNOW_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SNOW_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0xB1:
            case 0xD1:
            case 0x111:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_CONC_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_CONC_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_CONC_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_CONC_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x3:
            case 0x23:
            case 0x43:
            case 0x63:
            case 0x83:
            case 0xA3:
            case 0xC3:
            case 0xE3:
            case 0x103:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x6:
            case 0x26:
            case 0x46:
            case 0x66:
            case 0x86:
            case 0xA6:
            case 0xC6:
            case 0xE6:
            case 0x106:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x52:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_LAVA_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_LAVA_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_LAVA_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_LAVA_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x27:
            case 0x47:
            case 0x107:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x21:
            case 0x41:
            case 0x61:
            case 0xA1:
            case 0xC1:
            case 0xC7:
            case 0xE1:
            case 0x101:
            case 0x121:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x28:
            case 0x48:
            case 0x4C:
            case 0x68:
            case 0x88:
            case 0xE8:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_BRDG_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_BRDG_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_BRDG_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_BRDG_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x62:
            case 0xA2:
            case 0xC2:
            case 0xE2:
            case 0x102:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_GRAS_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_GRAS_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_GRAS_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_GRAS_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0xBA:
            case 0xC8:
            case 0xDA:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROOF_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROOF_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROOF_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROOF_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x99:
            case 0x108:
            case 0x10C:
            case 0x119:
            case 0x11A:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_TRAK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_TRAK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_TRAK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_TRAK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x118:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SAND_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SAND_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SAND_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_SAND_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x0:
            case 0x1:
            case 0x2:
            case 0xA:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x14:
            case 0x17:
            case 0x1A:
            case 0x1B:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x20:
            case 0x2A:
            case 0x2D:
            case 0x2E:
            case 0x2F:
            case 0x33:
            case 0x34:
            case 0x37:
            case 0x38:
            case 0x3B:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x40:
            case 0x42:
            case 0x49:
            case 0x4A:
            case 0x4D:
            case 0x4E:
            case 0x4F:
            case 0x51:
            case 0x58:
            case 0x5B:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x60:
            case 0x69:
            case 0x6A:
            case 0x6D:
            case 0x6E:
            case 0x6F:
            case 0x71:
            case 0x75:
            case 0x79:
            case 0x7A:
            case 0x7B:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x80:
            case 0x8A:
            case 0x8C:
            case 0x8D:
            case 0x8E:
            case 0x8F:
            case 0x91:
            case 0x93:
            case 0x9A:
            case 0x9B:
            case 0x9C:
            case 0x9E:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0xA0:
            case 0xA8:
            case 0xA9:
            case 0xAA:
            case 0xAB:
            case 0xAC:
            case 0xAD:
            case 0xAE:
            case 0xAF:
            case 0xB2:
            case 0xB3:
            case 0xBB:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0xC0:
            case 0xCA:
            case 0xCC:
            case 0xCD:
            case 0xCE:
            case 0xD4:
            case 0xDB:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0xE0:
            case 0xE9:
            case 0xEA:
            case 0xEB:
            case 0xEC:
            case 0xED:
            case 0xEE:
            case 0xEF:
            case 0xF1:
            case 0xF3:
            case 0xF4:
            case 0xFA:
            case 0xFB:
            case 0xFC:
            case 0xFD:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x100:
            case 0x109:
            case 0x10A:
            case 0x10B:
            case 0x10D:
            case 0x10E:
            case 0x10F:
            case 0x114:
            case 0x11B:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_ROCK_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x120:
            case 0x12D:
            case 0x131:
            case 0x13B:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_DIRT_LAND_BG, count, other, 0, 0);
                }
                break;
            }
        } else {
            switch (choice.index) {
            case 0x6:
            case 0x26:
            case 0x46:
            case 0x66:
            case 0x86:
            case 0xA6:
            case 0xC6:
            case 0xE6:
            case 0x106:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_H2O_LAND_BG, count, other, 0, 0);
                }
                break;
            case 0x27:
            case 0x47:
            case 0x107:
                if (suspend) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_SUSP, count, other, 0, 0);
                }
                else if (bump) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_BUMP, count, other, 0, 0);
                }
                else if (landSmall) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_LAND_SM, count, other, 0, 0);
                }
                else if (landBig) {
                    func_00201B68(D_004BD8A0[0], 0, SFX_B_MUD_LAND_BG, count, other, 0, 0);
                }
                break;
            }
            if (landSmall || landBig) {
                switch (*(int *)(self->x88 + 0x7908)) {
                case 35:
                    func_00201B68(D_004BD8A0[0], 0, SFX_E_RIDE_SML_PAD, count, other, 0, 0);
                    break;
                case 36:
                    func_00201B68(D_004BD8A0[0], 0, SFX_E_RIDE_SML_HOOF, count, other, 0, 0);
                    break;
                case 37:
                    func_00201B68(D_004BD8A0[0], 0, SFX_E_RIDE_BIG_HOOF, count, other, 0, 0);
                    break;
                }
            }
        }
        func_001DB6B0__c1(*(char **)(self->x88 + 0x792C) + 0x10,
                      7, 20, &count, &other, 0);
        if (impact < (float)D_0029E520) {
            if (func_00222B08(0, 6) == 0) {
                if (*(int *)(self->x88 + 0x796C) == 0x200) {
                    func_00201B68(D_004BD8A4[0], 0, SFX_K_GR_SML, count, other, 0, 0);
                } else {
                switch (*(int *)(self->x88 + 0x796C)) {
                case 0x40:
                case 0x80:
                case 0x100:
                case 0x160:
                    func_00201B68(D_004BD8A4[0], 0, SFX_R_GR_FEM_SML, count, other, 0, 0);
                    break;
                default:
                    func_00201B68(D_004BD8A4[0], 0, SFX_R_GR_MAL_SML, count, other, 0, 0);
                    break;
                }
                }
            }
        } else if (impact > (float)D_0029E520 && impact < (float)D_0029E524) {
            if (func_00222B08(0, 6) == 2) {
                if (*(int *)(self->x88 + 0x796C) == 0x200) {
                    func_00201B68(D_004BD8A4[0], 0, SFX_K_GR_MED, count, other, 0, 0);
                } else {
                switch (*(int *)(self->x88 + 0x796C)) {
                case 0x40:
                case 0x80:
                case 0x100:
                case 0x160:
                    func_00201B68(D_004BD8A4[0], 0, SFX_R_GR_FEM_MED, count, other, 0, 0);
                    break;
                default:
                    func_00201B68(D_004BD8A4[0], 0, SFX_R_GR_MAL_MED, count, other, 0, 0);
                    break;
                }
                }
            }
            if (func_00222B08(0, 6) == 2) {
                int type = *(int *)(self->x88 + 0x7908);
                if (type >= 35) {
                    if (type < 38)
                        func_001BFEA0((char *)self + 0x140, type);
                }
            }
        } else if (impact > (float)D_0029E524) {
            if (func_00222B08(0, 4) == 2) {
                if (*(int *)(self->x88 + 0x796C) == 0x200) {
                    func_00201B68(D_004BD8A4[0], 0, SFX_K_GR_BIG, count, other, 0, 0);
                } else {
                switch (*(int *)(self->x88 + 0x796C)) {
                case 0x40:
                case 0x80:
                case 0x100:
                case 0x160:
                    func_00201B68(D_004BD8A4[0], 0, SFX_R_GR_FEM_BIG, count, other, 0, 0);
                    break;
                default:
                    func_00201B68(D_004BD8A4[0], 0, SFX_R_GR_MAL_BIG, count, other, 0, 0);
                    break;
                }
                }
            }
            if (func_00222B08(0, 4) == 2) {
                int type = *(int *)(self->x88 + 0x7908);
                if (type >= 35) {
                    if (type < 38)
                        func_001BFEA0((char *)self + 0x140, type);
                }
            }
        }
    }
}



extern void func_001CF450(int *out);
void func_001CF450(int *out)
{
    *out = func_00238F48(out);
}



extern int D_004BD8A0[];
extern void func_001CF478__c1(int *timer,int age,int object,int type) asm("func_001CF478");
void func_001CF478__c1(int *timer,int age,int object,int type) {
 int amount,extra;
 if ((float)(func_00238F48()-*timer)>18.0f) {
  func_001DB6B0__c1(object,11,28,&amount,&extra,0);
  if(amount>0 && type==0x1E46) {
   if(age<30) amount=(int)((float)amount * 0.033333335f * (float)age);
   if(amount>1024) amount=1024;
   else if(amount<0) amount=0;
   func_00201B68(D_004BD8A0[0], 0, ((u128__c1)0x504D << 64) | (u128__c1)0x495F454E4F435F45UL, amount, extra, 0, 0);
   *timer=func_00238F48();
  }
 }
}


extern int D_004BD8A0[];

extern void func_001CF5C8(float speed, int *time, void *sound, int kind);
#if defined(__mips__)
void func_001CF5C8(float speed, int *time, void *sound, int kind)
#else
void SharedCall001CF5C8(float speed, int * time, void * sound, int kind)
#endif
{

    int out[2];
    int elapsed = func_00238F48();
    int selected = kind;
    if ((float)(elapsed - *time) > 18.0f) {
        func_001DB6B0__c1(sound, 11, 28, &out[0], &out[1], 0);
        if (speed < 10.0f) {
            out[0] = (int)((float)out[0] * 0.1f * speed);
        }
        if (out[0] > 0) {
            if (selected == 0x1E46)
                func_00201B68(D_004BD8A0[0], 0, ((u128__c1)0x504D << 64) | (u128__c1)0x495F454E4F435F45LL, out[0], out[1], 0, 0);
            *time = func_00238F48();
        }
    }
}



extern void func_001CF6F8(int *out);
void func_001CF6F8(int *out) {
    *out = func_00238F48();
}

#define NAME_BIG_001CF720 (((u128__c1)0x4C414D494E415FLL<<64)|(u128__c1)0x4749425F46425F45LL)
#define NAME_PED_001CF720 (u128__c1)0x4445505F46425F52LL
#define NAME_SML_001CF720 (((u128__c1)0x4C414D494E415FLL<<64)|(u128__c1)0x4C4D535F46425F45LL)
extern int D_004BD8A0[];

extern void func_001CF720(void *self,void *obj,int type);
void func_001CF720(void *self,void *obj,int type) {
 int count,other;
 func_001DB6B0__c1(obj,20,70,&count,&other,0);
 if(count>0) {
 if((unsigned int)(type-2)<2) func_00201B68(D_004BD8A0[0], 0, NAME_BIG_001CF720, count, other, 0, 0);
 else if(type==0) func_00201B68(D_004BD8A0[0], 0, NAME_PED_001CF720, count, other, 0, 0);
 else func_00201B68(D_004BD8A0[0], 0, NAME_SML_001CF720, count, other, 0, 0);
 }
}

#define NAME_BIG_001CF858 (((u128__c1)0x4C414D494E415FUL<<64)|(u128__c1)0x4749425F46425F45UL)
#define NAME_SML_001CF858 (((u128__c1)0x4C414D494E415FUL<<64)|(u128__c1)0x4C4D535F46425F45UL)

extern int D_004BD8A0[];

extern void func_001CF858(int *timer,void *obj,int type,float speed);
void func_001CF858(int *timer,void *obj,int type,float speed) {
    int count,other;
    if((float)(func_00238F48()-*timer)>18.0f) {
        func_001DB6B0__c1(obj,20,70,&count,&other,0);
        if(speed<20.0f) count=(int)((float)count*0.05f*speed);
        if(count>0) {
            if((unsigned int)(type-2)<2) func_00201B68(D_004BD8A0[0], 0, NAME_BIG_001CF858, count, other, 0, 0);
            else func_00201B68(D_004BD8A0[0], 0, NAME_SML_001CF858, count, other, 0, 0);
            *timer=func_00238F48();
        }
    }
}



struct Obj001CFA00 {
    int x0;
    int x4;
    int x8;
};


#if defined(__mips__)
extern void func_001CFA00__c1(struct Obj001CFA00 *obj, int arg) asm("func_001CFA00");
#else

#endif

void func_001CFA00__c1(struct Obj001CFA00 *obj, int arg)
{
    obj->x0 = 0;
    obj->x0 = func_00202DB0(1, arg);
    obj->x4 = 1;
    obj->x8 = arg;
}

/* sound names packed into 128-bit values */
#define NAME_E_DOG_001CFA48 (u128__c1)0x474F445F45LL
#define NAME_E_CAT_001CFA48 (u128__c1)0x5441435F45LL
#define NAME_R_BF_DIRT_001CFA48 (((u128__c1)0x54LL << 64) | (u128__c1)0x5249445F46425F52LL)

extern int D_004B2840__c1[] asm("D_004B2840");
extern int D_004BD8A0[];



extern void func_001CFA48(void *self, void *obj, int type);
void func_001CFA48(void *self, void *obj, int type)
{
    int count;
    int other;

    func_001DB6B0__c1(obj, 0x14, 0x46, &count, &other, 0);
    if (count > 0) {
        if (type == 7 && D_004B2840__c1[0] == 0x14) {
            func_00201B68(D_004BD8A0[0], 0, NAME_E_DOG_001CFA48, count, other, 0, 0);
        } else if (type == 11) {
            func_00201B68(D_004BD8A0[0], 0, NAME_E_CAT_001CFA48, count, other, 0, 0);
        } else {
            func_00201B68(D_004BD8A0[0], 0, NAME_R_BF_DIRT_001CFA48, count, other, 0, 0);
        }
    }
}

/* sound names packed little-endian into 128-bit values */
#define NAME_E_STEP_BIG_HOOF_001CFB60 (((u128__c1)0x464F4F485F4749LL << 64) | (u128__c1)0x425F504554535F45LL)
#define NAME_E_STEP_SML_HOOF_001CFB60 (((u128__c1)0x464F4F485F4C4DLL << 64) | (u128__c1)0x535F504554535F45LL)
#define NAME_E_STEP_SML_PAD_001CFB60 (((u128__c1)0x4441505F4C4DLL << 64) | (u128__c1)0x535F504554535F45LL)
#define NAME_E_STEP_BIG_PAD_001CFB60 (((u128__c1)0x4441505F4749LL << 64) | (u128__c1)0x425F504554535F45LL)

typedef struct Step {
    float *x0;
    int x4;
    int x8;
} Step;

extern char D_0028FD10[];
extern int D_004BD8A0[];


extern int strncmp(const char *, const char *, unsigned int);

extern void func_001CFB60__c1(Step *step, int obj, int mode, int kind) asm("func_001CFB60");
void func_001CFB60__c1(Step *step, int obj, int mode, int kind)
{
    u128__c1 name = 0;
    char *nameptr = (char *)&name;
    int count;
    int other;

    __builtin_memcpy(nameptr, D_0028FD10, 9);
    count = 0;
    other = 0;
    if (mode == 5 || mode == 6) {
        if (step->x0 == 0) {
            step->x0 = func_00202DB0(1, step->x8);
            return;
        }
        if (step->x4 != 0 && *step->x0 != 0.0f) {
            switch (kind) {
            case 1:
                name = NAME_E_STEP_BIG_HOOF_001CFB60;
                break;
            case 2:
                name = NAME_E_STEP_SML_HOOF_001CFB60;
                break;
            case 4:
                name = NAME_E_STEP_BIG_HOOF_001CFB60;
                break;
            case 5:
                name = NAME_E_STEP_SML_HOOF_001CFB60;
                break;
            case 6:
                name = NAME_E_STEP_BIG_HOOF_001CFB60;
                break;
            case 7:
                name = NAME_E_STEP_SML_PAD_001CFB60;
                break;
            case 8:
                name = NAME_E_STEP_SML_HOOF_001CFB60;
                break;
            case 3:
            case 9:
                name = NAME_E_STEP_BIG_PAD_001CFB60;
                break;
            }
            if (strncmp(nameptr, D_0028FD10, 16) != 0) {
                func_001DB6B0__c1(obj, 20, 180, &count, &other, 0);
                if (count > 0) {
                    func_00201B68(D_004BD8A0[0], 0, name, count, other, 0, 0);
                }
                step->x4 = 0;
            }
            return;
        }
        if (*step->x0 != 0.0f) {
            return;
        }
    }
    step->x0 = 0;
    step->x4 = 1;
}

extern int *func_001CFED0(int *p);
int *func_001CFED0(int *p)
{
    *p = 0;
    return p;
}

struct Obj001CFEE0 { int x0; char pad4[8]; float xC; int x10, x14, x18; };

extern void func_001CFEE0(struct Obj001CFEE0 *obj, int value);
void func_001CFEE0(struct Obj001CFEE0 *obj, int value) {
    obj->x0 = value;
    obj->x10 = func_00238F48();
    obj->xC = 5.0f;
    obj->x14 = 0;
    obj->x18 = 0;
}


struct State_CFF { char pad[0x16D4]; char *body; };
struct Obj_CFF { struct State_CFF *state; int kind; int x8; char padC[8]; int active; unsigned int handle; };

#define NAME_B_SCR_FENCE_L_001CFF20 (((u128__c1)0x4C5F45434ELL << 64) | (u128__c1)0x45465F5243535F42LL)

extern void func_001CFF20__c1(struct Obj_CFF *obj) asm("func_001CFF20");
void func_001CFF20__c1(struct Obj_CFF *obj)
{
    int count, other;
    unsigned int handle;
    if (obj->active) {
        switch (obj->kind) {
        case 0x1B: case 0x3B: case 0x5B: case 0x7B: case 0x9B:
        case 0xBB: case 0xDB: case 0xFB: case 0x11B: case 0x13B:
            func_001DB6B0__c1(obj->state->body + 16, D_0029D5C4, D_0029D5C8,
                          &count, &other, 0);
            handle = obj->handle;
            if (handle == 0) {
                obj->handle = 0xFFFFFFFF;
                func_00201BD0(D_004BD8A0[0], 0, NAME_B_SCR_FENCE_L_001CFF20, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->handle));
            } else if (handle != 0xFFFFFFFF && handle != 0) {
                obj->handle = 0xFFFFFFFF;
                func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->handle));
            }
            break;
        default:
            func_00208BF8__c1(D_004BD840, &obj->handle);
            break;
        }
    } else {
        func_00208BF8__c1(D_004BD840, &obj->handle);
    }
    obj->x8 = 0;
    obj->active = 0;
}

struct Obj001D00F0 { char *owner; char pad4[12]; int x10; };

#if defined(__mips__)
extern void func_001D00F0(struct Obj001D00F0 *self, int surface);
#else

#endif

void func_001D00F0(struct Obj001D00F0 *self, int surface)
{
    int count, other;
    unsigned int kind = func_001C32D0(surface);
    if (((unsigned char)(*(unsigned long *)(*(char **)(self->owner + 0x16D0) + 0x4098) >> 2) & 1) &&
        (float)(func_00238F48() - self->x10) > 12.0f) {
        func_001DB6B0__c1(*(char **)(self->owner + 0x16D4) + 16, D_0029D5C4, D_0029D5C8, &count, &other, 0);
        if (count > 0) {
            self->x10 = func_00238F48();
            switch (kind) {
            case 2:
            case 66:
            case 98:
            case 162:
            case 194:
            case 226:
            case 258:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x5353UL << 64) | (u128__c1)0x4152475F46425F52UL), count, other, 0, 0); /* R_BF_GRASS */
                break;
            case 6:
            case 38:
            case 70:
            case 102:
            case 134:
            case 166:
            case 198:
            case 230:
            case 262:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x5245UL << 64) | (u128__c1)0x5441575F46425F52UL), count, other, 0, 0); /* R_BF_WATER */
                break;
            case 52:
            case 178:
            case 244:
            case 276:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x0UL << 64) | (u128__c1)0x5941485F46425F52UL), count, other, 0, 0); /* R_BF_HAY */
                break;
            case 14:
            case 46:
            case 174:
            case 206:
            case 238:
            case 270:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x49UL << 64) | (u128__c1)0x4C45485F46425F52UL), count, other, 0, 0); /* R_BF_HELI */
                break;
            case 51:
            case 147:
            case 156:
            case 158:
            case 171:
            case 179:
            case 235:
            case 243:
            case 252:
            case 267:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x4FUL << 64) | (u128__c1)0x5455415F46425F52UL), count, other, 0, 0); /* R_BF_AUTO */
                break;
            case 186:
            case 200:
            case 218:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x46UL << 64) | (u128__c1)0x4F4F525F46425F52UL), count, other, 0, 0); /* R_BF_ROOF */
                break;
            case 12:
            case 168:
            case 172:
            case 204:
            case 236:
            case 268:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x435552545344UL << 64) | (u128__c1)0x4F4F575F46425F52UL), count, other, 0, 0); /* R_BF_WOODSTRUC */
                break;
            case 40:
            case 104:
            case 136:
            case 232:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x454744UL << 64) | (u128__c1)0x4952425F46425F52UL), count, other, 0, 0); /* R_BF_BRIDGE */
                break;
            case 133:
            case 197:
            case 261:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x57UL << 64) | (u128__c1)0x4F4E535F46425F52UL), count, other, 0, 0); /* R_BF_SNOW */
                break;
            case 141:
            case 173:
            case 205:
            case 237:
            case 269:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x454C4F50UL << 64) | (u128__c1)0x5F54535F46425F52UL), count, other, 0, 0); /* R_BF_ST_POLE */
                break;
            case 209:
            case 273:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x4554455243UL << 64) | (u128__c1)0x4E4F435F46425F52UL), count, other, 0, 0); /* R_BF_CONCRETE */
                break;
            case 153:
            case 264:
            case 281:
            case 282:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x4BUL << 64) | (u128__c1)0x4152545F46425F52UL), count, other, 0, 0); /* R_BF_TRAK */
                break;
            case 10:
            case 15:
            case 42:
            case 47:
            case 74:
            case 79:
            case 106:
            case 111:
            case 138:
            case 143:
            case 170:
            case 175:
            case 202:
            case 234:
            case 239:
            case 266:
            case 271:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x45UL << 64) | (u128__c1)0x4552545F46425F52UL), count, other, 0, 0); /* R_BF_TREE */
                break;
            case 1:
            case 33:
            case 65:
            case 97:
            case 161:
            case 193:
            case 199:
            case 225:
            case 257:
            case 289:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x54UL << 64) | (u128__c1)0x5249445F46425F52UL), count, other, 0, 0); /* R_BF_DIRT */
                break;
            case 82:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x41UL << 64) | (u128__c1)0x56414C5F46425F52UL), count, other, 0, 0); /* R_BF_LAVA */
                break;
            case 39:
            case 71:
            case 263:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x0UL << 64) | (u128__c1)0x44554D5F46425F52UL), count, other, 0, 0); /* R_BF_MUD */
                break;
            case 280:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x44UL << 64) | (u128__c1)0x4E41535F46425F52UL), count, other, 0, 0); /* R_BF_SAND */
                break;
            case 315:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x4543UL << 64) | (u128__c1)0x4E45465F46425F52UL), count, other, 0, 0); /* R_BF_FENCE */
                break;
            case 0:
            case 3:
            case 5:
            case 13:
            case 20:
            case 23:
            case 26:
            case 27:
            case 32:
            case 35:
            case 45:
            case 55:
            case 56:
            case 59:
            case 64:
            case 67:
            case 72:
            case 73:
            case 76:
            case 77:
            case 78:
            case 81:
            case 88:
            case 91:
            case 96:
            case 99:
            case 105:
            case 109:
            case 110:
            case 113:
            case 117:
            case 121:
            case 122:
            case 123:
            case 128:
            case 131:
            case 140:
            case 142:
            case 144:
            case 145:
            case 154:
            case 155:
            case 160:
            case 163:
            case 169:
            case 177:
            case 187:
            case 192:
            case 195:
            case 212:
            case 219:
            case 224:
            case 227:
            case 233:
            case 241:
            case 250:
            case 251:
            case 253:
            case 256:
            case 259:
            case 265:
            case 283:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x4BUL << 64) | (u128__c1)0x434F525F46425F52UL), count, other, 0, 0); /* R_BF_ROCK */
                break;
            case 288:
            case 301:
            case 305:
                func_00201B68(D_004BD8A0[0], 0, (((u128__c1)0x54UL << 64) | (u128__c1)0x5249445F46425F52UL), count, other, 0, 0); /* R_BF_DIRT */
                break;
            }
        }
    }
}


extern int D_004BD8A4[];
#define P_001D06C0(p,o) (*(char **)((char *)(p)+(o)))
#define I_001D06C0(p,o) (*(int *)((char *)(p)+(o)))
extern void func_001D06C0(char **p,int kind) asm("func_001D06C0");
void func_001D06C0(char **p,int kind) {
    int value,extra;
    func_001DB6B0__c1(P_001D06C0(*p,0x16D4)+0x10,12,30,&value,&extra,0);
    if (value>0) {
        if (kind==5 || kind==0 || kind==3 || kind==8) {
            if (I_001D06C0(P_001D06C0(*p,0x16D0),0x796C)==0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353494DUL<<64)|0x5F48434E55505F4BUL), value, extra, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353494DUL<<64)|0x5F48434E55505F52UL), value, extra, 0, 0);
        }
        else if (kind==3 || kind==8) {
            if (I_001D06C0(P_001D06C0(*p,0x16D0),0x796C)==0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353494DUL<<64)|0x5F4B434954535F4BUL), value, extra, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353494DUL<<64)|0x5F4B434954535F52UL), value, extra, 0, 0);
        }
        else if (kind==6 || kind==1) {
            if (I_001D06C0(P_001D06C0(*p,0x16D0),0x796C)==0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x535349UL<<64)|0x4D5F4B43494B5F4BUL), value, extra, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x535349UL<<64)|0x4D5F4B43494B5F52UL), value, extra, 0, 0);
        }
        else if (kind==7 || kind==2) {
            if (I_001D06C0(P_001D06C0(*p,0x16D0),0x796C)==0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353UL<<64)|0x494D5F5049485F4BUL), value, extra, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353UL<<64)|0x494D5F5049485F52UL), value, extra, 0, 0);
        }
        else if (kind==4 || kind==9) {
            if (I_001D06C0(P_001D06C0(*p,0x16D0),0x796C)==0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353494D5F54UL<<64)|0x4F425F4F32485F4BUL), value, extra, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x5353494D5F54UL<<64)|0x4F425F4F32485F52UL), value, extra, 0, 0);
        }
    }
}

struct HitState001D0A58 { char pad0[0x796C]; int x796C; char pad7970[0xEC]; int x7A5C; };
struct HitOwner001D0A58 { char pad[0x16D0]; struct HitState001D0A58 *x16D0; char *x16D4; };
struct Hit001D0A58 { struct HitOwner001D0A58 *owner; int x4, x8; float xC; };
extern void func_001D0A58__c1(struct Hit001D0A58 *self, int kind, int strength) asm("func_001D0A58");
void func_001D0A58__c1(struct Hit001D0A58 *self, int kind, int strength)
{
    int count = 0, other = 0;
    struct HitState001D0A58 *owner;
    struct HitOwner001D0A58 *parent;
    unsigned int random;
    owner = self->owner->x16D0;
    if (owner->x7A5C == 1)
        func_001C7A60__c1((Obj__c1 *)((char *)owner + 0x7720), 12, self->xC);
    func_001DB6B0__c1(P_001D06C0(self->owner, 0x16D4) + 16, 12, 30, &count, &other, 0);
    if (count > 0) {
        if (kind == 5 || kind == 0) {
            if (I_001D06C0(P_001D06C0(self->owner, 0x16D0), 0x796C) == 0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x0UL << 64) | (u128__c1)0x48434E55505F4BUL), count, other, 0, 0);
            else if (strength > 2400) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x474CUL << 64) | (u128__c1)0x5F48434E55505F52UL), count, other, 0, 0);
            else if (strength > 1450) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x44454DUL << 64) | (u128__c1)0x5F48434E55505F52UL), count, other, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4C4D53UL << 64) | (u128__c1)0x5F48434E55505F52UL), count, other, 0, 0);
        }
        else if (kind == 8 || kind == 3) {
            if (I_001D06C0(P_001D06C0(self->owner, 0x16D0), 0x796C) == 0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x544948UL << 64) | (u128__c1)0x5F4B434954535F4BUL), count, other, 0, 0);
            else if (strength > 2400) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x474CUL << 64) | (u128__c1)0x5F48434E55505F52UL), count, other, 0, 0);
            else if (strength > 1450) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x44454DUL << 64) | (u128__c1)0x5F48434E55505F52UL), count, other, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4C4D53UL << 64) | (u128__c1)0x5F48434E55505F52UL), count, other, 0, 0);
        }
        else if (kind == 6 || kind == 1) {
            if (I_001D06C0(P_001D06C0(self->owner, 0x16D0), 0x796C) == 0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x0UL << 64) | (u128__c1)0x4B43494B5F4BUL), count, other, 0, 0);
            else if (strength > 2400) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x47UL << 64) | (u128__c1)0x4C5F4B43494B5F52UL), count, other, 0, 0);
            else if (strength > 1450) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4445UL << 64) | (u128__c1)0x4D5F4B43494B5F52UL), count, other, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4C4DUL << 64) | (u128__c1)0x535F4B43494B5F52UL), count, other, 0, 0);
        }
        else if (kind == 7 || kind == 2) {
            if (I_001D06C0(P_001D06C0(self->owner, 0x16D0), 0x796C) == 0x200) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x0UL << 64) | (u128__c1)0x5049485F4BUL), count, other, 0, 0);
            else if (strength > 2400) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x0UL << 64) | (u128__c1)0x474C5F5049485F52UL), count, other, 0, 0);
            else if (strength > 1450) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x44UL << 64) | (u128__c1)0x454D5F5049485F52UL), count, other, 0, 0);
            else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4CUL << 64) | (u128__c1)0x4D535F5049485F52UL), count, other, 0, 0);
        }
        random = func_00222B08(0, 2);
        if (strength > 1450 && random < 2) {
            func_001DB6B0__c1(P_001D06C0(self->owner, 0x16D4) + 16, 7, 20, &count, &other, 0);
            switch (I_001D06C0(P_001D06C0(self->owner, 0x16D0), 0x796C)) {
            case 0x40: case 0x80: case 0x100: case 0x160:
                if (strength > 2400) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4749425FUL << 64) | (u128__c1)0x4D45465F52475F52UL), count, other, 0, 0);
                else if (strength > 1450) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x44454D5FUL << 64) | (u128__c1)0x4D45465F52475F52UL), count, other, 0, 0);
                else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4C4D535FUL << 64) | (u128__c1)0x4D45465F52475F52UL), count, other, 0, 0);
                break;
            default:
                if (strength > 2400) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4749425FUL << 64) | (u128__c1)0x4C414D5F52475F52UL), count, other, 0, 0);
                else if (strength > 1450) func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x44454D5FUL << 64) | (u128__c1)0x4D45465F52475F52UL), count, other, 0, 0);
                else func_00201B68(D_004BD8A4[0], 0, (((u128__c1)0x4C4D535FUL << 64) | (u128__c1)0x4C414D5F52475F52UL), count, other, 0, 0);
                break;
            }
        }
    }
}

struct Obj001D1150 {
    char pad0[0x4];
    int x4;
    float x8;
    char padC[0x14 - 0xC];
    int x14;
};

extern int func_001C32D0(int id);

extern void func_001D1150(struct Obj001D1150 *obj, int id, float priority);
void func_001D1150(struct Obj001D1150 *obj, int id, float priority)
{
    if (obj->x8 <= priority) {
        obj->x4 = func_001C32D0(id);
        obj->x8 = priority;
    }
    obj->x14 = 1;
}

typedef struct Obj001D11A8 {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    char pad34[0x44 - 0x34];
    int x44;
    int x48;
    int x4C;
    int x50;
    float x54;
    float x58;
    float x5C;
    float x60;
    float x64;
    float x68;
    int x6C;
    int x70;
    int x74;
    int x78;
    int x7C;
} Obj001D11A8;



extern void func_001D11A8(Obj001D11A8 *obj);
void func_001D11A8(Obj001D11A8 *obj)
{
    obj->x0 = 0;
    obj->x44 = 0;
    obj->x48 = 0;
    obj->x4C = 0;
    obj->x50 = 0;
    obj->x4 = 1;
    obj->x8 = 1;
    obj->x10 = func_00238F48();
    obj->xC = func_00238F48();
    obj->x54 = 16383.0f;
    obj->x5C = D_0029D5E8;
    obj->x60 = D_0029D5EC;
    obj->x64 = D_0029D5F0;
    obj->x68 = D_0029D5F4;
    obj->x14 = D_0029D5F8;
    obj->x18 = D_0029D5FC;
    obj->x1C = D_0029D600;
    obj->x20 = D_0029D604;
    obj->x24 = D_0029D608;
    obj->x28 = D_0029D60C;
    obj->x2C = D_0029D610;
    obj->x30 = D_0029D614;
    obj->x78 = D_0029D620;
    obj->x58 = D_0029D61C;
    obj->x74 = D_0029D61C;
    obj->x6C = 0;
    obj->x70 = 0;
    obj->x7C = 3;
}

extern char D_004BD840[];
extern void func_00208BF8__c1(void *list, void *item) asm("func_00208BF8");

extern void func_001D1298(void *item);
void func_001D1298(void *item) {
    func_00208BF8__c1(D_004BD840, item);
}

typedef struct Obj001D12C0 {
    unsigned int x0;
    char pad4[0x54 - 0x4];
    float x54;
    float x58;
    char pad5C[0x78 - 0x5C];
    int x78;
    int x7C;
    int x80;
    int x84;
} Obj001D12C0;




extern void func_001D12C0__c1(Obj001D12C0 *obj) asm("func_001D12C0");
void func_001D12C0__c1(Obj001D12C0 *obj)
{
    int now;
    unsigned int handle;

    now = func_00238F48();
    obj->x58 = (int)((float)obj->x84 - (obj->x84 - obj->x78) * 0.033333335f * (now - obj->x80));
    handle = obj->x0;
    if (handle != 0xFFFFFFFF && handle != 0) {
        obj->x0 = 0xFFFFFFFF;
        func_00201E08(handle, 0x15, obj->x58, 0, 0, obj->x54, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(obj));
    }
    if (obj->x58 <= obj->x78) {
        obj->x7C = 3;
    }
}

typedef struct Obj001D13B8 { unsigned int x0; char pad4[0x50]; float x54,x58; char pad5C[0x18]; int x74,x78,x7C,x80,x84; } Obj001D13B8;

extern int D_004BD8A0[];

extern void func_001D13B8__c1(Obj001D13B8 *obj) asm("func_001D13B8");
void func_001D13B8__c1(Obj001D13B8 *obj) {
 int now = func_00238F48();
 obj->x58 = (float)(int)((float)obj->x84 + (float)(obj->x74-obj->x84)*0.013888889f*(float)(now-obj->x80));
 if (obj->x0 == 0) {
  obj->x0 = 0xFFFFFFFF;
  func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x4C5F444E << 64) | (u128__c1)0x4957504D554A5F42UL, obj->x74, 0, 0, (int)obj->x54, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)obj));
 } else if (obj->x0 != 0xFFFFFFFF && obj->x0 != 0) {
  unsigned int id = obj->x0;
  obj->x0 = 0xFFFFFFFF;
  func_00201E08(id, 0x15, (int)obj->x58, 0, 0, (int)obj->x54, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(obj));
 }
 if ((float)obj->x74 <= obj->x58) obj->x7C=2;
}

typedef struct Obj001D1528 { unsigned int x0; int x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C,x30,x34,x38,x3C,x40; float x44,x48,x4C,x50,x54,x58,x5C,x60,x64,x68,x6C,x70; int x74,x78; } Obj001D1528;

extern float func_00222A68(float,float);
extern int D_004BD8A0[];

extern void func_001D1528__c1(Obj001D1528 *s) asm("func_001D1528");
void func_001D1528__c1(Obj001D1528 *s) {
 float unused[3]; int now; float value,max; unsigned int handle;
 unused[0]=0;unused[1]=0;unused[2]=0; now=func_00238F48();
 if(s->x70*60.0f<(float)(now-s->xC)) {
 if(s->x4==1)s->x4=0;else s->x4=1;
 s->x70=func_00222A68(s->x5C,s->x60);s->xC=func_00238F48();
 s->x40=func_00222B08(s->x14,s->x18);if(s->x4==0)s->x40=-s->x40;s->x44=(float)s->x40/(s->x70*60.0f);
 s->x38=func_00222B08(s->x1C,s->x20);if(s->x4==0)s->x38=-s->x38;s->x4C=(float)s->x38/(s->x70*60.0f);
 }
 now=func_00238F48(); if(s->x6C*60.0f<(float)(now-s->x10)) {
 if(s->x8==1)s->x8=0;else s->x8=1;
 s->x6C=func_00222A68(s->x64,s->x68);s->x10=func_00238F48();
 s->x3C=func_00222B08(s->x24,s->x28);if(s->x8==0)s->x3C=-s->x3C;s->x48=(float)s->x3C/(s->x6C*60.0f);
 s->x34=func_00222B08(s->x2C,s->x30);if(s->x8==0)s->x34=-s->x34;s->x50=(float)s->x34/(s->x6C*60.0f);
 }
 max=(float)s->x78; value=s->x58+(s->x4C+s->x50);s->x58=value;
 if(max<value)s->x58=max;else if(value<(float)s->x74)s->x58=(float)s->x74;
 value=s->x54+(s->x44+s->x48);s->x54=value;if(value>32767.0f)s->x54=32767.0f;else if(value<0.0f)s->x54=0.0f;
 handle=s->x0;if(handle==0){s->x0=0xFFFFFFFF;func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x4C5F444E<<64)|(u128__c1)0x4957504D554A5F42UL, (int)s->x58, 0, 0, (int)s->x54, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)s));}
 else if(handle!=0xFFFFFFFF && handle!=0){s->x0=0xFFFFFFFF;func_00201E08(handle, 21, (int)s->x58, 0, 0, (int)s->x54, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(s));}
}

struct Obj001D1888 { char pad0[0x40]; int x40; };
extern void func_001BE9D0(void *, int, int, int, int, int, int);
extern void func_001D1888(struct Obj001D1888 *obj);
void func_001D1888(struct Obj001D1888 *obj) {
    obj->x40 = 0;
    func_001BE9D0(obj, 0x4C, 1, D_0029D5CC, D_0029D5D0, 2, 5);
}

INCLUDE_ASM("asm/nonmatchings/text", func_001D18C0);
typedef struct Obj001D18C8 { char pad0[0x40]; unsigned int x40; int x44; } Obj001D18C8;
typedef struct Event001D18C8 { int x0; char pad4[0x2C]; } Event001D18C8;
typedef struct Global001D18C8 { char pad0[0x260]; Event001D18C8 events[3][12]; char pad920[0xB40 - 0x920]; int counts[4]; } Global001D18C8;
#define GLOBAL18C8 (*(Global001D18C8 *)D_00398C40)
extern int func_00202B48(int) asm("func_00202B48");
extern void func_001BEA48__c1() asm("func_001BEA48");
static inline Mode8380 *game18C8(void) { return &D_004B25A0__c1; }
static inline int players18C8(void) { return game18C8()->x288; }
static inline void clearFlags18C8(int *flags) { int i; for (i = 0; i < players18C8(); i++) flags[i] = 0; }

extern void func_001D18C8__c1(Obj001D18C8 *obj, int id) asm("func_001D18C8");
void func_001D18C8__c1(Obj001D18C8 *obj, int id) {
    int j, i;
    int volume = 0;
    int x, y, n;
    {
        int flags[players18C8()];
        clearFlags18C8(flags);
        i = 0;
        if (i < players18C8()) {
            Global001D18C8 *all = (Global001D18C8 *)D_00398C40;
            do {
                for (j = 0; j < all->counts[i]; j++) {
                    int event = GLOBAL18C8.events[i][j].x0;
                    if (event == 0x645 || event == 0x64A || event == 0x650 ||
                        event == 0x652 || event == 0x659 || event == 0x667 ||
                        event == 0x669 || event == 0x66F || event == 0x671)
                        flags[i] = 1;
                }
                i++;
            } while (i < D_004B25A0__c1.x288);
        }
        func_001DB6B0__c1((void *)id, D_0029D5CC, D_0029D5D0, &x, &y, (int)flags);
        if (x > 0) {
            if (players18C8() == 1) {
                func_001DB920((void *)id, &n, &obj->x44);
                if (!obj->x40) n = 0;
                volume = (int)(func_00202B48(n) * D_0029D5A0);
            }
            if (!obj->x40) {
                obj->x40 = 0xFFFFFFFF;
                func_00201BD0(D_004BD8A0[0], 0, (u128__c1)0x4C5F494C45485F45UL, x, y, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x40));
            } else if (obj->x40 != 0xFFFFFFFF && obj->x40 != 0) {
                unsigned int handle = obj->x40;
                obj->x40 = 0xFFFFFFFF;
                func_00201E08(handle, 13, x, y, volume, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x40));
            }
        } else
            func_00208BF8__c1(D_004BD840, &obj->x40);
        if (game18C8()->x2A0 == 20)
            func_001BEA48__c1(obj, id);
    }
}
INCLUDE_ASM("asm/nonmatchings/text", func_001D1C00);
struct Obj001D1C08 { int x0,x4; char pad8[24]; int x20[4]; int x30; char pad34[12]; u128__c1 x40,x50; int x60[4]; };
extern void func_001D1C08(struct Obj001D1C08 *p,int a,int b,u128__c1 c,u128__c1 d);
void func_001D1C08(struct Obj001D1C08 *p,int a,int b,u128__c1 c,u128__c1 d) {
 int i;
 p->x4=a; p->x40=c; p->x50=d; p->x30=b; p->x0=0;
 for(i=0;i<4;i++) { p->x20[i]=0; p->x60[i]=1; }
}

INCLUDE_ASM("asm/nonmatchings/text", func_001D1C50);
#ifdef NONMATCHING
/* Non-matching: retail builds the state base in $v0 then copies it to $t3, and cse reassociates
   (state+0xB40)+i*4 / (state+0x240)+i*0x240 operand order and schedules the inner-loop setup differently;
   best diff 14/347 words */
/* Sound trigger for one object: looks for arg0->x4 in each rider's event list (D_00398C40), derives a pan
   (`value`) from which riders found it, then starts/updates the arg0->x50 sound and may fire arg0->x40. */
typedef struct {
    char pad[0x30];
} Sub1C58;

typedef struct {
    char pad0[0x40C0];
    char x40C0;
    char pad1[0x7AC0 - 0x40C1];
    float x7AC0;
} Racer1C58;

typedef struct {
    char pad[0x50130];
    Racer1C58 *x50130[4];
} Global1C58;

typedef struct {
    unsigned int x0;   /* sound handle */
    int x4;            /* object id searched in the event lists */
    char pad0[0x18];
    int x20[4];        /* per-rider "skip" flag, cleared at the end */
    int x30;           /* threshold compared with rider x7AC0 */
    char pad1[0xC];
    u128 x40;          /* one-shot sound name */
    u128 x50;          /* loop sound name */
    int x60[4];        /* per-rider "may trigger" latch */
} Obj1C58;

typedef struct {
    char pad0[0xB40];
    int counts[4];
} StateCounts1C58;

extern int D_00398EA0[];
extern char D_004B25A0_chars_1C58[] asm("D_004B25A0");
extern float func_001385A8(void *);
extern void func_00201B00(int, int, u128, int, int, int, int);

void func_001D1C58(Obj1C58 *arg0) {
    int arr[4];
    long found;
    int even;
    int odd;
    int count;
    int value;
    float score;
    Racer1C58 *r;
    int flag2C;
    int flag30;
    int i;
    int i2;
    int i3;
    int i4;
    int j;
    int k;
    StateCounts1C58 *state;
    Mode8380 *mode;
    int offset;
    int offset4;
    Sub1C58 *sub;
    int *cnt;
    int *fld;
    char *rp;

    found = 0;
    even = 0;
    odd = 0;
    value = 0;
    score = 0.0f;
    r = 0;
    flag2C = 0;
    flag30 = 0;

    for (k = 3; k >= 0; k--) {
        arr[k] = 0;
    }

    for (i = 0; i < ((Mode8380 *)D_004B25A0_chars_1C58)->x288; i++) {
        offset4 = i * 4;
        do { state = (StateCounts1C58 *)D_00398C40; } while (0);
        if (((Obj1C58 *)((char *)arg0 + offset4))->x20[0] == 0) {
            j = 0;
            if (state->counts[i] > 0) {
            offset = i * 0x240;
            mode = &D_004B25A0__c1;
            do {} while (0);
            cnt = &state->counts[i];
            sub = (Sub1C58 *)(offset + (unsigned int)((char *)state + 0x240));
            fld = (int *)(offset + (unsigned int)D_00398EA0);
            do {
                if (*fld == arg0->x4) {
                    found = 1;
                    arr[i] = 1;
                    if (mode->x288 >= 2) {
                        if (!(i & 1)) {
                            even = 1;
                        } else {
                            odd = 1;
                        }
                        break;
                    }
                    func_001DB6B0__c1(sub, 0x3E8, 0x5DC, &count, &value, 0);
                    even = 1;
                    odd = 1;
                }
                j++;
                sub++;
                fld += 0xC;
            } while (j < *cnt);
            }
        }
    }

    if (found != 0) {
        if (D_004B25A0__c1.x288 >= 2) {
            if (even != 0) {
                if (odd != 0) {
                    value = 0;
                } else {
                    value = 0x10E;
                }
            } else if (odd != 0) {
                value = 0x5A;
            } else {
                value = 0x10E;
            }
        }

        for (i2 = 0; i2 < D_004B25A0__c1.x288; i2++) {
            if (arr[i2] == 1) {
                r = ((Global1C58 *)&D_002AF2C0)->x50130[i2];
                if (r != 0) {
                    rp = (char *)r + 0x40C0;
                    if (func_001385A8(rp) > 0.008f) {
                        if ((float)arg0->x30 < r->x7AC0) {
                            flag2C = 1;
                        } else if (func_00138580(rp) > D_0029D5DC && arg0->x60[i2] == 1) {
                            flag30 = 1;
                            score = func_001385A8(rp);
                        }
                    }
                }
                arg0->x60[i2] = 0;
            } else {
                arg0->x60[i2] = 1;
            }
        }

        if (flag2C != 0) {
            unsigned int handle = arg0->x0;
            if (handle == 0) {
                arg0->x0 = 0xFFFFFFFF;
                func_00201BD0(D_004BD8A0[0], 0, arg0->x50, 0x400, value, 0, 0,
                              func_00208C50, (SoundCallbackData)(long)arg0);
            } else if (handle != 0xFFFFFFFF && handle != 0) {
                arg0->x0 = 0xFFFFFFFF;
                func_00201E08(handle, 5, 0x400, value, 0, 0, func_00208C50, (SoundCallbackData)(long)arg0);
            }
        } else {
            do { func_00208BF8__c1(&D_004BD840, arg0); } while (0);
        }

        if (flag30 != 0) {
            if (value >= 0xB && value <= 0xB3) {
                value = 0x87;
            } else {
                if (value < 0xB5 || value > 0x15D) {
                    goto end;
                }
                value = 0xE1;
            }
            rp = (char *)r + 0x40C0;
            if (func_00138580(rp) < D_0029D5E0) {
                count = (int)((func_00138580(rp) - (float)D_0029D5DC) /
                              (float)(D_0029D5E0 - D_0029D5DC) * 1024.0f);
            } else {
                count = 0x400;
            }
            func_00201B00(D_004BD8A0[0], 0, arg0->x40, count, value, 0,
                          (int)(score * 65535.0f + -32768.0f));
        }
    } else {
        func_00208BF8__c1(&D_004BD840, arg0);
        for (i3 = 0; i3 < D_004B25A0__c1.x288; i3++) {
            arg0->x60[i3] = 1;
        }
    }

end:
    for (i4 = 0; i4 < D_004B25A0__c1.x288; i4++) {
        arg0->x20[i4] = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_001D1C58);
#endif
typedef struct {int x0,x4,x8; int ids[5]; u128__c1 name;} Row;
typedef struct {char pad[0x14]; unsigned short x14; char pad16[4]; short x1a,x1c,x1e;} Info;
extern char D_004BD8C0[];
extern int func_00163B70(void *);
extern int func_00163BE0(void *,int);
extern Info *func_00163D40(void *,int);
extern void func_001D21C8__c1(unsigned int rows) asm("func_001D21C8");
void func_001D21C8__c1(unsigned int rows) {
 int id=-1, i=0, count=func_00163B70(D_004BD8C0);
 if(count>0) {do {
 int j, offset, next=i+1; short kind; Row *r;
 u128__c1 *names=(u128__c1 *)(rows+0x20);
 id=func_00163BE0(D_004BD8C0,id);
 j=0;
 if(D_004B25A0__c1.x288>0) { do { ((Row *)rows)[i].ids[j]=id; j++; } while(j<D_004B25A0__c1.x288); }
 offset=i*0x30; r=(Row *)(rows+offset); r->x0=0;
 r->x4=func_00163D40(D_004BD8C0,id)->x1a;
 r->x8=func_00163D40(D_004BD8C0,id)->x1c;
 kind=func_00163D40(D_004BD8C0,id)->x1e;
 if(kind==0) {
 r->x4*=3; r->x8*=3; *(u128__c1 *)((char *)names+offset)=(((u128__c1)0x4C5FUL << 64) | (u128__c1)0x4D41455254535F45UL); } else if(kind==1) { *(u128__c1 *)((char *)names+offset)=(((u128__c1)0x4C5F4D41455254UL << 64) | (u128__c1)0x535F4156414C5F45UL); } else if(kind==2) { *(u128__c1 *)((char *)names+offset)=(((u128__c1)0x4C5F4CUL << 64) | (u128__c1)0x4C41464F32485F45UL); } else if(kind==3) { *(u128__c1 *)((char *)names+offset)=(((u128__c1)0x4C5F4D414552UL << 64) | (u128__c1)0x54535F4543495F45UL);
 }
 i=next;
 }while(i<count); }
}

#ifdef NONMATCHING
/* Non-matching: global-alloc keeps rows in $s6 and spills i (retail spills rows, i in $s6), s4/s5 swapped,
   mixer loop uses branches instead of beql chains, call-arg scheduling;
   best diff 208/292 words */
/* Positional loop sounds for the rows built by func_001D21C8: for every row, func_001D2AB8 gives each rider's
   volume/pan against the node segments before and after the rider's node; these are mixed into one
   volume/pan and the row's sound is started (func_00201BD0) or updated (func_00201E08), or stopped. */
struct Obj001D28B8;
extern void func_001D28B8(struct Obj001D28B8 *, int);
extern void func_001D2AB8();

typedef struct { char pad[0x50130]; char *riders[26]; } Race2428;
typedef struct { int volume, pan; } Pair2428;

/* same layout as Row (func_001D21C8) */
typedef struct { unsigned int x0; int x4, x8; int ids[5]; u128 name; } Row2428;
extern void func_001D2428__c1(Row2428 *rows) asm("func_001D2428");
void func_001D2428__c1(Row2428 *rows)
{
    int count;
    int before, beforePan, after, afterPan;
    register int i = 0;

    count = func_00163B70(D_004BD8C0);
    before = 0;
    beforePan = 0;
    after = 0;
    afterPan = 0;
    {
        int results[D_004B25A0__c1.x288 * 2]; /* per rider: volume, pan */
        if (count > 0) {
            do {
                int next;
                int j;
                int volume, pan;
                Row2428 *row;
                func_001D28B8((struct Obj001D28B8 *)rows, i);
                j = 0;
                next = i + 1;
                if (D_004B25A0__c1.x288 > 0) {
                    int offset = 0;
                    int *indexBase = rows[0].ids;
                    int *curId = rows[i].ids;
                    do {
                        int *second;
                        /* segment ending at the rider's current node (unless node flag bit 0) */
                        if (!(func_00163D40(D_004BD8C0, *curId)->x14 & 1)) {
                            char *rider = ((Race2428 *)&D_002AF2C0)->riders[j];
                            void *position = *(char **)(rider + 0x792C) + 0x10;
                            func_001D2AB8((void *)rows, position, func_00163D40(D_004BD8C0, *curId),
                                func_00163D40(D_004BD8C0, *curId - 1), &before, &beforePan, rows[i].x4, rows[i].x8);
                        } else {
                            before = 0;
                            beforePan = 0;
                        }
                        /* segment starting at the rider's current node (unless node flag bit 1) */
                        second = (int *)((char *)indexBase + i * 48 + offset);
                        if (!((func_00163D40(D_004BD8C0, *second)->x14 >> 1) & 1)) {
                            char *rider = ((Race2428 *)&D_002AF2C0)->riders[j];
                            void *position = *(char **)(rider + 0x792C) + 0x10;
                            func_001D2AB8((void *)rows, position,
                                func_00163D40(D_004BD8C0, *second),
                                func_00163D40(D_004BD8C0, *second + 1),
                                &after, &afterPan, rows[i].x4, rows[i].x8);
                        } else {
                            after = 0;
                            afterPan = 0;
                        }
                        /* keep the louder of the two segments */
                        results[j * 2] = before <= after ? after : before;
                        { int chosenPan;
                        int *panOut = &results[j * 2 + 1];
                        if (after < before) {
                            j++;
                            chosenPan = beforePan;
                        } else {
                            j++;
                            chosenPan = afterPan;
                        }
                        *panOut = chosenPan; }
                        curId++;
                        offset += 4;
                    } while (j < D_004B25A0__c1.x288);
                }
                if (D_004B25A0__c1.x288 == 1) {
                    volume = results[0];
                    pan = results[1];
                } else {
                    /* several riders: the first rider's volume pans to 270, the loudest of the others to 90 */
                    int first = 0;
                    int firstCheck = 1;
                    int rest = 0;
                    if (D_004B25A0__c1.x288 > 0) {
                        Pair2428 *p = (Pair2428 *)results;
                        int remaining = D_004B2828[0];
                        do {
                            /* both operands are 0/1, so & == && here */
                            if (firstCheck & (first < p->volume)) {
                                first = p->volume;
                            } else {
                                if (p->volume > rest)
                                    rest = p->volume;
                            }
                            firstCheck = 0;
                            p++;
                            remaining--;
                        } while (remaining != 0);
                    }
                    volume = rest;
                    pan = 90;
                    if (first != 0) {
                        if (rest == 0) {
                            volume = first;
                            pan = 270;
                        } else if (rest < first) {
                            volume = first;
                            pan = (rest * 90 / first) + 270;
                        } else {
                            pan = 90 - (first * 90 / rest);
                        }
                    }
                }
                if (volume > 0) {
                    unsigned int handle;
                    row = &rows[i];
                    handle = row->x0;
                    if (handle == 0) {
                        row->x0 = 0xffffffffU;
                        func_00201BD0(D_004BD898__c1[2], 0, row->name, volume, pan, 0, 0,
                                      func_00208C50, (SoundCallbackData)(long)row);
                    } else {
                        if (handle != 0xFFFFFFFF && handle != 0) {
                        row->x0 = 0xffffffffU;
                        func_00201E08(handle, 5, volume, pan, 0, 0,
                                      func_00208C50, (SoundCallbackData)(long)row);
                        }
                    }
                } else {
                    func_00208BF8__c1(D_004BD840, &rows[i]);
                }
                i = next;
            } while (i < count);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_001D2428);
#endif
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Node { char pad0[0x14]; unsigned short x14; } Node;
typedef struct Body { char pad0[0x10]; Vec pos; } Body;
typedef struct Rider { char pad0[0x792C]; Body * volatile x792C; } Rider;
typedef struct Global { char pad0[0x130]; Rider *riders[1]; } Global;
typedef struct State { char pad0[0x288]; int count; } State;
typedef struct Obj001D28B8 { char pad0[0xC]; int indices[4][12]; } Obj001D28B8;

extern Global D_002FF2C0;

extern char D_004BD8C0[];



static inline Vec *GetPos(Rider *rider)
{
    return &rider->x792C->pos;
}

extern void func_001D28B8(Obj001D28B8 *obj, int row);
void func_001D28B8(Obj001D28B8 *obj, int row)
{
    int i = 0;
    int *index;
    Rider **riders;
    Global *global;
    int offset;
    int *base;

    if (D_004B25A0__c1.x288 > 0) {
        offset = row * 48;
        global = &D_002FF2C0;
        base = obj->indices[0];
        index = (int *)(offset + (unsigned int)base);
        riders = global->riders;
        do {
            float previous = 32767.0f, current, next = 32767.0f;
            int prev = -1, following = -1;

            if (!(func_00163D40(D_004BD8C0, *index)->x14 & 1))
                prev = *index - 1;
            if (!((func_00163D40(D_004BD8C0, *index)->x14 >> 1) & 1))
                following = *index + 1;
            current = func_00222330(GetPos(*riders), func_00163D40(D_004BD8C0, *index));
            if (prev != -1) {
                previous = func_00222330(GetPos(*riders), func_00163D40(D_004BD8C0, prev));
            }
            if (following != -1) {
                next = func_00222330(GetPos(*riders), func_00163D40(D_004BD8C0, following));
            }
            if (previous < current && previous < next)
                *index = prev;
            else if (next < current) {
                if (next < previous)
                    *index = following;
            }
            i++;
            riders++;
            index++;
        } while (i < D_004B25A0__c1.x288);
    }
}

typedef struct Vec2AB8 {
    float x, y, z, w;
} Vec2AB8 __attribute__((aligned(16)));

static inline void VecSub2AB8(Vec2AB8 *out, Vec2AB8 *a, Vec2AB8 *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecAdd2AB8(Vec2AB8 *out, Vec2AB8 *a, Vec2AB8 *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecScale2AB8(Vec2AB8 *out, Vec2AB8 *v, float s)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "mfc1 $2, %2\n"
        "qmtc2.ni $2, $vf12\n"
        "vmulx.xyz $vf11, $vf11, $vf12x\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline float VecDot2AB8(Vec2AB8 *a, Vec2AB8 *b)
{
    float r;
    __asm__ volatile (
        "mula.s %1, %2\n"
        "madda.s %3, %4\n"
        "madd.s %0, %5, %6\n"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

struct Body2AB8 { char pad[0x10]; Vec2AB8 x10; char pad2[0x30]; Vec2AB8 x50; };
struct Rider2AB8 { char pad[0x792c]; struct Body2AB8 *x792c; };
struct Global2AB8 { char pad[0x2b4]; struct Rider2AB8 *x2b4; };
extern struct Global2AB8 D_0037EA60;
extern int D_004B2828[];
extern int func_00208E40(Vec2AB8 *,Vec2AB8 *,int,int);
#if defined(__mips__)
extern int func_00208C60(Vec2AB8 *,float,Vec2AB8 *);
#else
#include "sound_math.h"
#define func_00208C60 SoundHeadingBetween
#endif
extern float func_00222640(Vec2AB8 *);
static inline struct Rider2AB8 *get_rider2AB8(struct Global2AB8 *g) { return g->x2b4; }

extern void func_001D2AB8(void *unused,Vec2AB8 *pos,Vec2AB8 *end,Vec2AB8 *start,int *out,int *out2,int arg6,int arg7);
void func_001D2AB8(void *unused,Vec2AB8 *pos,Vec2AB8 *end,Vec2AB8 *start,int *out,int *out2,int arg6,int arg7) {
    Vec2AB8 d,p,scaled,projected; Vec2AB8 *other=0; float t;
    VecSub2AB8(&d,end,start); VecSub2AB8(&p,pos,start);
    t=VecDot2AB8(&p,&d)/VecDot2AB8(&d,&d);
    if(t>0.0f && t<1.0f) {VecScale2AB8(&scaled,&d,t);VecAdd2AB8(&projected,&scaled,start);*out=func_00208E40(&projected,pos,arg6,arg7);other=&projected;}
    else if(t<0.0f) {*out=func_00208E40(start,pos,arg6,arg7);other=end;}
    else if(t>1.0f) {*out=func_00208E40(end,pos,arg6,arg7);other=start;}
    if(D_004B2828[0]==1) { float len=func_00222640(&get_rider2AB8(&D_0037EA60)->x792c->x50); *out2=func_00208C60(&get_rider2AB8(&D_0037EA60)->x792c->x10,len,other); }
}
INCLUDE_ASM("asm/nonmatchings/text", func_001D2C68);
typedef struct Obj001D2C70 { unsigned int x0; int x4; char pad8[8]; u128__c1 x10; int x20[4]; } Obj001D2C70;
extern void func_001D2C70(Obj001D2C70 *obj, int value, u128__c1 name);
void func_001D2C70(Obj001D2C70 *obj, int value, u128__c1 name) {
    int i;
    int *p;
    obj->x4 = value;
    obj->x10 = name;
    obj->x0 = 0;
    p = &obj->x20[3];
    for (i = 3; i >= 0; i--) *p-- = 0;
}

INCLUDE_ASM("asm/nonmatchings/text", func_001D2CB0);
extern void func_001D2CB8__c1(Obj001D2C70 *obj) asm("func_001D2CB8");
void func_001D2CB8__c1(Obj001D2C70 *obj)
{
    int found[4];
    int any = 0, even = 0, odd = 0;
    int i, j, k, angle;
    unsigned int offset;
    unsigned int handle;
    Global_C73D8 *table, *t;

    for (k = 0; k < 4; k++) found[k] = 0;
    i = 0;
    if (i < D_004B25A0__c1.x288) {
        table = (Global_C73D8 *)D_00398C40;
        for (; i < D_004B25A0__c1.x288; i++) {
            if (obj->x20[i] == 0) {
                t = table;
                j = 0;
                if (j < t->counts[i]) {
                    offset = i * sizeof(int);
                    for (; j < t->counts[i]; j++) {
                        if (table->frames[i][j].value == obj->x4) {
                            *(int *)((char *)found + offset) = 1;
                            any = 1;
                            if (D_004B25A0__c1.x288 >= 2) {
                                if (!(i & 1)) even = 1;
                                else odd = 1;
                                break;
                            }
                            even = 1;
                            odd = 1;
                        }
                    }
                }
            }
        }
    }
    if (any) {
        if (even && odd) angle = 0;
        else if (odd) angle = 90;
        else angle = 270;
        handle = obj->x0;
        if (handle == 0) {
            obj->x0 = 0xFFFFFFFF;
            func_00201BD0(D_004BD8A0[0], 0, obj->x10, 0x400, angle, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)obj));
        } else if (handle != 0xFFFFFFFF && handle != 0) {
            obj->x0 = 0xFFFFFFFF;
            func_00201E08(handle, 5, 0x400, angle, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)obj));
        }
    } else {
        func_00208BF8__c1(D_004BD840, obj);
    }
    for (k = 0; k < 4; k++) obj->x20[k] = 0;
    /* never true; combine deletes it after flow, but the empty block it leaves
       turns off haifa's region (interblock) scheduling, as in retail */
    if (((unsigned char *)found)[0] & 0x100) found[0] = 0;
}
struct S_001D2F08 {
    int x0;
};

extern void func_001D2F08(struct S_001D2F08 *s);
void func_001D2F08(struct S_001D2F08 *s) {
    s->x0 = 0;
}

extern char D_004BD840[];
extern void func_00208BF8__c1(void *list, void *item) asm("func_00208BF8");

extern void func_001D2F10(void *item);
void func_001D2F10(void *item) {
    func_00208BF8__c1(D_004BD840, item);
}

extern char D_004BD840[];
extern int D_004BD8A0[];


extern void func_00208BF8__c1(void *a, void *b) asm("func_00208BF8");


/* "E_BIRDFLAP_L" packed little-endian into a 128-bit value */
#define NAME_E_BIRDFLAP_L_001D2F38 (((u128__c1)0x4C5F5041LL << 64) | (u128__c1)0x4C46445249425F45LL)

extern void func_001D2F38__c1(unsigned int *handlep, void *sound) asm("func_001D2F38");
void func_001D2F38__c1(unsigned int *handlep, void *sound)
{
    int count, other;
    unsigned int handle;

    func_001DB6B0__c1(sound, 0x78, 0x118, &count, &other, 0);
    if (count > 0) {
        handle = *handlep;
        if (handle == 0) {
            *handlep = 0xFFFFFFFF;
            func_00201BD0(D_004BD8A0[0], 0, NAME_E_BIRDFLAP_L_001D2F38, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)handlep));
        } else if (handle != 0xFFFFFFFF && handle != 0) {
            *handlep = 0xFFFFFFFF;
            func_00201E08(handle, 0xD, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)handlep));
        }
    } else {
        func_00208BF8__c1(D_004BD840, handlep);
    }
}

extern void func_001BE9D0(void *, int, int, int, int, int, int);
extern void func_001D3040(int *obj);
void func_001D3040(int *obj) {
    obj[0] = 0;
    func_001BE9D0((char *)obj + 0x10, 30, 6, 650, 2200, 2, 5);
}

extern char D_004BD840[];
extern void func_00208BF8__c1(void *list, void *item) asm("func_00208BF8");

extern void func_001D3080(void *item);
void func_001D3080(void *item) {
    func_00208BF8__c1(D_004BD840, item);
}

typedef struct Obj001D30A8 { unsigned int x0; char pad[12]; char x10[16]; } Obj001D30A8;
extern int D_004BD8A0[];
extern char D_004BD840[];


extern void func_00208BF8__c1(void *, void *) asm("func_00208BF8");
extern void func_001BEA48__c1(void *, void *) asm("func_001BEA48");
extern void func_001D30A8__c1(Obj001D30A8 *obj, void *sound) asm("func_001D30A8");
void func_001D30A8__c1(Obj001D30A8 *obj, void *sound) {
 int count, other;
 unsigned int handle;
 if(D_004B25A0__c1.x2A0==3) func_001DB6B0__c1(sound,300,1150,&count,&other,0);
 else func_001DB6B0__c1(sound,175,800,&count,&other,0);
 if(count>0) {
  handle=obj->x0;
  if(handle==0) {
   obj->x0=0xffffffff;
   func_00201BD0(D_004BD8A0[0], 0, ((u128__c1)0x4CUL<<64)|(u128__c1)0x5F4E494152545F45UL, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)obj));
  } else if(handle!=0xffffffff && handle!=0) {
   obj->x0=0xffffffff;
   func_00201E08(handle, 5, count, other, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)obj));
  }
  if(D_004B25A0__c1.x2A0==3) func_001BEA48__c1(obj->x10,sound);
 } else func_00208BF8__c1(D_004BD840,obj);
}

INCLUDE_ASM("asm/nonmatchings/text", func_001D3218);


extern void func_001D3228(int *out);
void func_001D3228(int *out) {
    *out = func_00238F48();
}

/* "E_BLDRBNCE_SML" packed into a 128-bit name */
#define NAME_E_BLDRBNCE_SML_001D3250 (((u128__c1)0x4C4D535F4543LL << 64) | (u128__c1)0x4E4252444C425F45LL)

extern int D_004B2840__c1[] asm("D_004B2840");
extern int D_004BD8A0[];




extern void func_001D3250__c1(int *lastTime, int arg) asm("func_001D3250");
void func_001D3250__c1(int *lastTime, int arg)
{
    int count;
    int other;

    if ((float)(func_00238F48() - *lastTime) > 48.0f) {
        if (D_004B2840__c1[0] == 3) {
            func_001DB6B0__c1(arg, D_0029D5D4, D_0029D5D8, &count, &other, 0);
        } else {
            func_001DB6B0__c1(arg, 100, 300, &count, &other, 0);
        }
        if (count > 0) {
            *lastTime = func_00238F48();
            func_00201B68(D_004BD8A0[0], 0, NAME_E_BLDRBNCE_SML_001D3250, count, other, 0, 0);
        }
    }
}
/* ---- merged from src/unsorted/001D3360.c ---- */
extern void func_001D3360(void);
void func_001D3360(void) {
}
INCLUDE_ASM("asm/nonmatchings/text", func_001D3368);
/* ---- merged from src/unsorted/001D3370.c ---- */
typedef struct Obj__c3 { char pad[0x40]; int x40; int x44; int x48; } Obj__c3;
extern void func_001BE9D0(void *, int, int, int, int, int, int);
extern void func_001D3370(Obj__c3 *obj, int value);
void func_001D3370(Obj__c3 *obj, int value) {
    obj->x48 = value;
    obj->x40 = 0;
    obj->x44 = 0;
    func_001BE9D0(obj, 42, 1, 80, 170, 2, 5);
}
/* ---- merged from src/unsorted/001D33B0.c ---- */
typedef unsigned int u128__c4 __attribute__((mode(TI)));

/* g++ 2.95 vtable entry: this-adjustment, index, function. */
typedef struct VtblEntry { short delta; short index; char *(*fn)(void *); } VtblEntry;
typedef struct Vehicle {
    VtblEntry *vtbl;
    int x4;
    char pad8[0x1A0 - 0x8];
    float x1A0;
    float x1A4;
} Vehicle;

/* Vehicle virtual 4: the name/position block the sound calls offset by 16. */
#define VCALL_GET(v) ((v)->vtbl[4].fn((char *)(v) + (v)->vtbl[4].delta))

typedef struct Ent {
    char pad0[0x40];
    unsigned int x40;
    unsigned int x44;
    Vehicle *x48;
} Ent;

#define NAME(hi, lo) (((u128__c4)(hi) << 64) | (u128__c4)(lo))
#define NAME_E_SNOWCAT_L     NAME(0x4C5F54ULL, 0x4143574F4E535F45ULL)
#define NAME_E_SNOWCAT_IDLE  NAME(0x454C44495F54ULL, 0x4143574F4E535F45ULL)
#define NAME_E_TRUCK_L       NAME(0x4CULL, 0x5F4B435552545F45ULL)
#define NAME_E_TRUCK_IDLE    NAME(0x454C4449ULL, 0x5F4B435552545F45ULL)
#define NAME_E_JEEP_L        ((u128__c4)0x4C5F5045454A5F45ULL)
#define NAME_E_COPCAR_L      NAME(0x4C5FULL, 0x524143504F435F45ULL)
#define NAME_E_COPCAR_IDLE   NAME(0x454C44495FULL, 0x524143504F435F45ULL)
#define NAME_E_CAR_L         ((u128__c4)0x204C5F5241435F45ULL)
#define NAME_S_CAR_IDLE      NAME(0x454CULL, 0x44495F5241435F53ULL)
#define NAME_E_BUS_L         ((u128__c4)0x204C5F5355425F45ULL)
#define NAME_E_MINECART_L    NAME(0x4C5F5452ULL, 0x4143454E494D5F45ULL)

extern int D_004B2840__c4[] asm("D_004B2840");
extern int D_004BD8A0[];
extern char D_004BD840[];

extern void func_001DB6B0__c4(char *sound, int a, int b, int *count, int *other, int flag) asm("func_001DB6B0");
extern void func_001BEA48__c4(Ent *ent, char *sound) asm("func_001BEA48");
extern void func_00208BF8__c4(void *a, void *b) asm("func_00208BF8");

extern void func_001D33B0__c4(Ent *ent, int type) asm("func_001D33B0");
void func_001D33B0__c4(Ent *ent, int type)
{
    int count;
    int other;
    int level;
    unsigned int handle;
    u128__c4 name;

    if (ent->x48 == 0 || VCALL_GET(ent->x48) == 0) {
        return;
    }

    switch (type) {
    case 0x4EC:
        if (D_004B2840__c4[0] == 5) {
            func_001DB6B0__c4(VCALL_GET(ent->x48) + 16, 120, 280, &count, &other, 0);
        } else {
            func_001DB6B0__c4(VCALL_GET(ent->x48) + 16, 80, 170, &count, &other, 0);
        }
        break;
    case 0x56E:
    case 0x584:
    case 0x58A:
    case 0x590:
    case 0x596:
    case 0x59C:
    case 0x5A2:
    case 0x5A8:
    case 0x5AE:
    case 0x5B4:
        func_001DB6B0__c4(VCALL_GET(ent->x48) + 16, 80, 170, &count, &other, 0);
        break;
    case 0x574:
    case 0x580:
        func_001DB6B0__c4(VCALL_GET(ent->x48) + 16, 45, 100, &count, &other, 0);
        break;
    case 0x514:
    case 0x516:
    case 0x57A:
        func_001DB6B0__c4(VCALL_GET(ent->x48) + 16, 120, 280, &count, &other, 0);
        break;
    }

    if (count > 0) {
        level = (int)(ent->x48->x1A0 / ent->x48->x1A4 * 32767.0f);
        if (type == 0x584) {
            func_001BEA48__c4(ent, VCALL_GET(ent->x48) + 16);
        }
        handle = ent->x40;
        if (handle == 0) {
            switch (type) {
            case 0x514:
            case 0x516:
                if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_SNOWCAT_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                } else {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_SNOWCAT_IDLE, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            case 0x4EC:
                if (D_004B2840__c4[0] == 5) {
                    if (ent->x48->x4 == 2) {
                        ent->x40 = 0xFFFFFFFF;
                        func_00201BD0(D_004BD8A0[0], 0, NAME_E_TRUCK_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                    } else {
                        ent->x40 = 0xFFFFFFFF;
                        func_00201BD0(D_004BD8A0[0], 0, NAME_E_TRUCK_IDLE, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                    }
                } else if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_JEEP_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            case 0x584:
                if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_COPCAR_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                } else {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_COPCAR_IDLE, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            case 0x58A:
            case 0x590:
            case 0x596:
            case 0x59C:
            case 0x5A2:
            case 0x5A8:
            case 0x5AE:
            case 0x5B4:
                if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_CAR_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                } else {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_S_CAR_IDLE, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            case 0x55A:
            case 0x56E:
                if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_BUS_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            case 0x574:
            case 0x580:
                if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_MINECART_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            case 0x57A:
                if (ent->x48->x4 == 2) {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_TRUCK_L, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                } else {
                    ent->x40 = 0xFFFFFFFF;
                    func_00201BD0(D_004BD8A0[0], 0, NAME_E_TRUCK_IDLE, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
                }
                break;
            }
        } else if (handle != 0xFFFFFFFF && handle != 0) {
            ent->x40 = 0xFFFFFFFF;
            func_00201E08(handle, 13, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&ent->x40));
        }
    } else {
        func_00208BF8__c4(D_004BD840, &ent->x40);
        func_00208BF8__c4(D_004BD840, &ent->x44);
    }
}
/* ---- merged from src/unsorted/001D3D10.c ---- */
struct S_001D3D10 {
    int x0;
};

extern void func_001D3D10(struct S_001D3D10 *s);
void func_001D3D10(struct S_001D3D10 *s) {
    s->x0 = 0;
}
INCLUDE_ASM("asm/nonmatchings/text", func_001D3D18);
/* ---- merged from src/unsorted/001D3D20.c ---- */
typedef unsigned int u128__c6 __attribute__((mode(TI)));

extern char D_004BD840[];
extern int D_004BD8A0[];

extern void func_001DB6B0__c6(void *sound, int a, int b, int *count, int *other, int flag) asm("func_001DB6B0");
extern void func_00208BF8__c6(void *a, void *b) asm("func_00208BF8");

extern struct { char pad[0x2a0]; int x2a0; } D_004B25A0__c6 asm("D_004B25A0");
extern void func_001D3D20__c6(unsigned int *handlep, void *sound, float value,float max) asm("func_001D3D20");
#if defined(__mips__)
void func_001D3D20__c6(unsigned int *handlep, void *sound, float value,float max)
#else
void SharedCall001D3D20(unsigned int * handlep, void * sound, float value, float max)
#endif
{

 int count=0,other=0;
 unsigned int handle;
 if(D_004B25A0__c6.x2a0==19) {
  func_001DB6B0__c6(sound,45,90,&count,&other,0);
  if(count>0 && value>0.01f) {
   int level=(int)((value/max)*32767.0f);
   handle=*handlep;
   if(handle==0) {
    *handlep=0xffffffff;
    func_00201BD0(D_004BD8A0[0], 0, ((u128__c6)0x204C5F5452UL<<64)|(u128__c6)0x4143464C4F475F45UL, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)handlep));
   } else if(handle!=0xffffffff && handle!=0) {
    *handlep=0xffffffff;
    func_00201E08(handle, 13, count, other, 0, level, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)handlep));
   }
  } else func_00208BF8__c6(D_004BD840,handlep);
 }
}
/* ---- merged from src/unsorted/001D3EA8.c ---- */
struct Obj__c7 {
    char pad0[0x10];
    int x10;
    int x14;
    int x18;
};
extern char D_004BD840[];
extern void func_00208BF8__c7(void *, void *) asm("func_00208BF8");
extern void func_001D3EA8__c7(struct Obj__c7 *obj) asm("func_001D3EA8");
void func_001D3EA8__c7(struct Obj__c7 *obj) {
    obj->x18 = 0;
    func_00208BF8__c7(D_004BD840, &obj->x10);
}
/* ---- merged from src/unsorted/001D3ED8.c ---- */
typedef unsigned int u128__c8 __attribute__((mode(TI)));

struct Obj__c8 {
    u128__c8 x0;
    char pad10[0x14 - 0x10];
    float x14;
    int x18;
    int x1C;
};

extern int func_00238F48(void);

extern void func_001D3ED8(struct Obj__c8 *obj, u128__c8 *value, float x14);
void func_001D3ED8(struct Obj__c8 *obj, u128__c8 *value, float x14)
{
    obj->x14 = x14;
    obj->x18 = 1;
    obj->x1C = func_00238F48();
    obj->x0 = *value;
}
/* ---- merged from src/unsorted/001D3F20.c ---- */
typedef unsigned int u128__c9 __attribute__((mode(TI)));
extern void func_001DB6B0__c9(int,int,int,int*,int*,int) asm("func_001DB6B0");
extern int D_004BD8A0[];
extern void func_001D3F20(int unused,int arg);
void func_001D3F20(int unused,int arg) {
 int values[2];
 func_001DB6B0__c9(arg,60,300,&values[0],&values[1],0);
 if(values[0]>0) func_00201B68(D_004BD8A0[0], 0, ((u128__c9)0x54524154ULL<<64)|0x535F455249465F45ULL, values[0], values[1], 0, 0);
}
/* ---- merged from src/unsorted/001D3FA0.c ---- */
typedef unsigned int u128__c10 __attribute__((mode(TI)));
struct Obj__c10 { char pad[16]; unsigned int x10; float x14; int x18,x1C; };
extern int func_00238F48(void);
extern void func_001D3EA8__c10(struct Obj__c10*) asm("func_001D3EA8");
extern void func_001DB6B0__c10(struct Obj__c10*,int,int,int*,int*,int) asm("func_001DB6B0");
extern int D_004BD8A0[];
extern char D_004BD840[];
extern void func_00208BF8__c10(void*,unsigned int*) asm("func_00208BF8");
extern void func_001D3FA0__c10(struct Obj__c10 *obj) asm("func_001D3FA0");
void func_001D3FA0__c10(struct Obj__c10 *obj) {
 int volume=0,pan=0;
 if(obj->x18) {
 if((float)(func_00238F48()-obj->x1C)<obj->x14*60.0f) {
 func_001DB6B0__c10(obj,60,300,&volume,&pan,0);
 if(volume>0) {
 unsigned int handle=obj->x10;
 if(handle==0) {
 obj->x10=0xffffffffU;
 func_00201BD0(D_004BD8A0[0], 0, (u128__c10)0x4C5F455249465F45UL, volume, pan, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x10));
 } else if(handle!=0xffffffffU && handle!=0) {
 obj->x10=0xffffffffU;
 func_00201E08(handle, 13, volume, pan, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x10));
 }
 } else func_00208BF8__c10(D_004BD840,&obj->x10);
 } else func_001D3EA8__c10(obj);
 }
}
/* ---- merged from src/unsorted/001D40F8.c ---- */
struct Entry { char pad0[0x20]; int x20; char pad24[0xC]; };
struct Group { struct Entry entries[12]; };
struct Obj__c11 { struct Group groups[4]; int counts[4]; };
extern unsigned int D_004B2840__c11[] asm("D_004B2840");
extern void func_001D21C8__c11(void *) asm("func_001D21C8");
extern void func_001BE0C8(void *, int);
extern void func_001BE3D8(void *, int);
extern void func_001D53C0(void *);
extern void func_001D6390(void *);
extern void func_001D5C30(void *);
extern void func_001D6D68(void *);
extern void func_001D7968(void *);
extern void func_001D9038(void *);
extern void func_001D8618(void *);
extern void func_001D9A40(void *);
extern void func_001DAEE8(void *);
extern void func_001D8F18(void *);
extern void func_001D7838(void *);
extern void func_001D40F8(struct Obj__c11 *obj);
void func_001D40F8(struct Obj__c11 *obj) {
 char *p=(char *)obj; int i,j;
 func_001D21C8__c11(p+0xC30);
 switch(D_004B2840__c11[0]) {
 case 1:
 case 7:
 case 13:
 func_001D53C0(p+0x1810); break;
 case 5:
 case 14:
 case 22:
 func_001D6390(p+0x1C50); break;
 case 0:
 case 6:
 case 24:
 func_001D5C30(p+0x1410); break;
 case 2:
 case 8:
 case 10:
 func_001D6D68(p+0x2B70); break;
 case 3:
 case 4:
 case 9:
 func_001D7968(p+0x2280); break;
 case 11:
 case 15:
 case 16:
 func_001D9038(p+0x3570); break;
 case 12:
 case 21:
 case 26:
 func_001D8618(p+0x3B70); break;
 case 17:
 case 18:
 case 20:
 func_001D9A40(p+0x4520); break;
 case 19:
 case 25:
 func_001DAEE8(p+0x41E0); break;
 case 23:
 func_001D8F18(p+0x5040); break;
 case 29:
 case 30:
 case 31:
 case 32:
 func_001D7838(p+0x50F0); break;
 }
 for(i=0;i<4;i++) {
 for(j=0;j<12;j++) obj->groups[i].entries[j].x20=0;
 }
 for(j=0;j<4;j++) obj->counts[j]=0;
 func_001BE0C8(p+0xFF0,3);
 func_001BE3D8(p+0x11F4,3);
}
/* ---- merged from src/unsorted/001D4288.c ---- */
typedef struct { char pad[0x2a0]; unsigned int x2A0; char pad2[0x41c-0x2a4]; int x41C; } Game;
typedef struct { char pad[0x18]; int x18; int x1C; } Item;
typedef struct { char pad[0x20]; int x20; char pad24[0xc]; } Entry;
typedef struct { int counts[4]; Item items[25]; } Sub;
typedef struct {
    Entry rows[4][12];  /* 0x000 */
    Sub sub;            /* 0x900 */
} Obj__c12;
extern Game D_004B25A0__c12 asm("D_004B25A0");
extern int D_0043F348[];
extern void func_001D7840(void *);
extern void func_001D2428(void *);
extern void func_001D58F0(void *);
extern void func_001D6A38(void *);
extern void func_001D6090(void *);
extern void func_001D7488(void *);
extern void func_001D8190(void *);
extern void func_001D9750(void *);
extern void func_001D8CA0(void *);
extern void func_001DAC50(void *);
extern void func_001DB280(void *);
extern void func_001D9000(void *);
extern void func_001D3FA0__c12(Item *) asm("func_001D3FA0");
extern void func_001BE168(void *);
extern void func_001BE468(void *);

static inline void ClearRows(int *counts, Obj__c12 *obj)
{
    int i, j, k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < obj->sub.counts[i]; j++)
            obj->rows[i][j].x20 = 0;
    }
    for (k = 3; k >= 0; k--)
        counts[k] = 0;
}

extern void func_001D4288(Obj__c12 *obj);
void func_001D4288(Obj__c12 *obj)
{
    unsigned int mode = D_004B25A0__c12.x2A0;
    Sub *sub;
    Item *item;
    int n;

    if (mode - 29 < 4)
        func_001D7840((char *)obj + 0x50f0);
    if ((unsigned int)(D_0043F348[0] - 1) < 3 && D_004B25A0__c12.x41C) {
        func_001D2428((char *)obj + 0xc30);
        switch (mode) {
        case 1: case 7: case 13: func_001D58F0((char *)obj + 0x1810); break;
        case 5: case 14: case 22: func_001D6A38((char *)obj + 0x1c50); break;
        case 0: case 6: case 24: func_001D6090((char *)obj + 0x1410); break;
        case 2: case 8: case 10: func_001D7488((char *)obj + 0x2b70); break;
        case 3: case 4: case 9: func_001D8190((char *)obj + 0x2280); break;
        case 11: case 15: case 16: func_001D9750((char *)obj + 0x3570); break;
        case 12: case 21: case 26: func_001D8CA0((char *)obj + 0x3b70); break;
        case 17: case 18: case 20: func_001DAC50((char *)obj + 0x4520); break;
        case 19: case 25: func_001DB280((char *)obj + 0x41e0); break;
        case 23: func_001D9000((char *)obj + 0x5040); break;
        case 29: case 30: case 31: case 32: func_001D7840((char *)obj + 0x50f0); break;
        }

        sub = &obj->sub;
        for (n = 0; n < 25; n++) {
            item = &sub->items[n];
            if (item->x18 == 1)
                func_001D3FA0__c12(item);
        }
        ClearRows(obj->sub.counts, obj);
        func_001BE168((char *)obj + 0xff0);
        func_001BE468((char *)obj + 0x11f4);
    }
}
INCLUDE_ASM("asm/nonmatchings/text", func_001D44C8);
/* ---- merged from src/unsorted/001D44D0.c ---- */
typedef unsigned int u128__c13 __attribute__((mode(TI)));
extern int D_004BD898__c13[] asm("D_004BD898");
extern void func_001DB6B0__c13(int,int,int,int *,int *,int) asm("func_001DB6B0");
extern void func_001D44D0(int unused,int value);
void func_001D44D0(int unused,int value) {
 int out[2]; out[0]=0;out[1]=0;
 func_001DB6B0__c13(value,45,240,&out[0],&out[1],0);
 if(out[0]>0) func_00201B68(D_004BD898__c13[0], 0, ((u128__c13)0x544E554F4D4552ULL<<64)|(u128__c13)0x5F4B434955515F50ULL, out[0], out[1], 0, 0);
}
/* ---- merged from src/unsorted/001D4568.c ---- */
typedef unsigned int u128__c14 __attribute__((mode(TI)));
extern void func_001DB6B0__c14(int,int,int,int*,int*,int) asm("func_001DB6B0");
extern int func_001DB9B0(int);
extern int D_004B26AC[],D_004BD898__c14[] asm("D_004BD898");
extern void func_001D4568(int unused,int arg);
void func_001D4568(int unused,int arg) {
 int value=func_001DB9B0(arg);
 if(D_004B26AC[0]!=10) func_00201B68(D_004BD898__c14[0], 0, ((u128__c14)0x454B4F5453ULL<<64)|0x5F524554454D5F50ULL, 1024, value, 0, 0);
}
/* ---- merged from src/unsorted/001D45E8.c ---- */
typedef unsigned int u128__c15 __attribute__((mode(TI)));
extern int D_004BD8A0[];
extern void func_001DB6B0__c15(void *,int,int,int *,int *,int) asm("func_001DB6B0");
extern void func_001D45E8(void *unused,void *p);
void func_001D45E8(void *unused,void *p) {
 int count=0; int value=0;
 func_001DB6B0__c15(p,125,800,&count,&value,0);
 if(count>0) func_00201B68(D_004BD8A0[0], 0, ((u128__c15)0x45544954<<64) | (u128__c15)0x47414C4154535F45UL, count, value, 0, 0);
}
/* ---- merged from src/unsorted/001D4670.c ---- */
typedef unsigned int u128__c16 __attribute__((mode(TI)));
extern void func_001DB6B0__c16(int,int,int,int*,int*,int) asm("func_001DB6B0");
extern int D_004BD8A0[];
extern void func_001D4670(int unused,int arg);
void func_001D4670(int unused,int arg) {
 int values[2]; values[0]=0; values[1]=0;
 func_001DB6B0__c16(arg,125,800,&values[0],&values[1],0);
 if(values[0]>0) func_00201B68(D_004BD8A0[0], 0, ((u128__c16)0x47ULL<<64)|0x5245424543495F45ULL, values[0], values[1], 0, 0);
}
/* ---- merged from src/unsorted/001D46F0.c ---- */
typedef unsigned int u128__c17 __attribute__((mode(TI)));
extern int D_004BD8A0[];
extern void func_001DB6B0__c17(int,int,int,int *,int *,int) asm("func_001DB6B0");
extern void func_001D46F0(int unused,int value);
void func_001D46F0(int unused,int value) {
 int out[2]; out[0]=0;out[1]=0;
 func_001DB6B0__c17(value,125,800,&out[0],&out[1],0);
 if(out[0]>0) func_00201B68(D_004BD8A0[0], 0, ((u128__c17)0x52425F45544954ULL<<64)|(u128__c17)0x47414C4154535F45ULL, out[0], out[1], 0, 0);
}
/* ---- merged from src/unsorted/001D4788.c ---- */
typedef unsigned int u128__c18 __attribute__((mode(TI)));

/* "R_H2O_BOT_HIT" packed into a 128-bit name */
#define NAME_R_H2O_BOT_HIT (((u128__c18)0x5449485F54LL << 64) | (u128__c18)0x4F425F4F32485F52LL)

extern int D_004BD8A4[];

extern void func_001DB6B0__c18(int, int, int, int *, int *, int) asm("func_001DB6B0");

extern void func_001D4788(int unused, int arg);
void func_001D4788(int unused, int arg)
{
    int count, value;

    count = 0;
    value = 0;
    func_001DB6B0__c18(arg, 0x2D, 0x96, &count, &value, 0);
    if (count > 0) {
        func_00201B68(D_004BD8A4[0], 0, NAME_R_H2O_BOT_HIT, count, value, 0, 0);
    }
}
/* ---- merged from src/unsorted/001D4818.c ---- */
struct Obj__c19 { char pad0[0x5100]; int x5100; int x5104; int x5108; };
extern void func_001D4818(struct Obj__c19 *obj, int key, int value);
void func_001D4818(struct Obj__c19 *obj, int key, int value) {
    if (key == 0x65D) obj->x5100 = value;
    else if (key == 0x65E) obj->x5104 = value;
    else if (key == 0x65F) obj->x5108 = value;
}
/* ---- merged from src/unsorted/001D4850.c ---- */
typedef struct Obj__c20 { char pad[0x5100]; int x5100; int x5104; int x5108; } Obj__c20;
extern int func_001D4850(Obj__c20 *obj, int id);
int func_001D4850(Obj__c20 *obj, int id) {
    if (id == 0x65D) return obj->x5100;
    if (id == 0x65E) return obj->x5104;
    if (id == 0x65F) return obj->x5108;
    return 0;
}
/* ---- merged from src/unsorted/001D4890.c ---- */
typedef unsigned int u128__c21 __attribute__((mode(TI)));
extern int D_004BD898__c21[] asm("D_004BD898");
extern void func_001D4890(void);
void func_001D4890(void) {
    func_00201B68(D_004BD898__c21[0], 0, ((u128__c21)0x544e494f << 64) | (u128__c21)0x504b434548435f50UL, 0x400, 0, 0, 0);
}
/* ---- merged from src/unsorted/001D48F0.c ---- */
typedef unsigned int u128__c22 __attribute__((mode(TI)));

struct Player__c22 {
    char pad0[0x7A5C];
    int x7A5C;
};

struct Obj__c22 {
    char pad0[0x88];
    struct Player__c22 *x88;
};

extern int D_004B26AC[];
extern struct Player__c22 *D_0037ED14__c22[] asm("D_0037ED14");
extern int D_004BD898__c22[] asm("D_004BD898");

#define NAME_P_METER_UP (((u128__c22)0x5055 << 64) | (u128__c22)0x5F524554454D5F50)

extern void func_001D48F0(struct Obj__c22 *obj) asm("func_001D48F0");
void func_001D48F0(struct Obj__c22 *obj)
{
    int mode;
    mode = D_004B26AC[0];
    if (mode != 10 && obj->x88->x7A5C == 1) {
        goto play;
    }
    if (D_004B26AC[0] == 10 && D_0037ED14__c22[0] == obj->x88) {
play:
        func_00201B68(D_004BD898__c22[0], 0, NAME_P_METER_UP, 0x400, 0, 0, 0);
    }
}
/* ---- merged from src/unsorted/001D4990.c ---- */
typedef unsigned int u128__c23 __attribute__((mode(TI)));

struct Player__c23 {
    char pad0[0x7A5C];
    int x7A5C;
};

struct Obj__c23 {
    char pad0[0x88];
    struct Player__c23 *x88;
};

extern int D_004B26AC[];
extern struct Player__c23 *D_0037ED14__c23[] asm("D_0037ED14");
extern int D_004BD898__c23[] asm("D_004BD898");

#define NAME_P_METERS_DOWN (((u128__c23)0x4E574F44 << 64) | (u128__c23)0x5F524554454D5F50)

extern void func_001D4990(struct Obj__c23 *obj) asm("func_001D4990");
void func_001D4990(struct Obj__c23 *obj)
{
    int mode;
    mode = D_004B26AC[0];
    if (mode != 10 && obj->x88->x7A5C == 1) {
        goto play;
    }
    if (D_004B26AC[0] == 10 && D_0037ED14__c23[0] == obj->x88) {
play:
        func_00201B68(D_004BD898__c23[0], 0, NAME_P_METERS_DOWN, 0x400, 0, 0, 0);
    }
}
/* ---- merged from src/unsorted/001D4A30.c ---- */
typedef unsigned int u128__c24 __attribute__((mode(TI)));
extern int D_004BD898__c24[] asm("D_004BD898");
extern void func_001D4A30(void);
void func_001D4A30(void) {
    func_00201B68(D_004BD898__c24[0], 0, ((u128__c24)0x59454e << 64) | (u128__c24)0x4f4d485341435f50UL, 0x400, 0, 0, 0);
}
/* ---- merged from src/unsorted/001D4A90.c ---- */
typedef unsigned int u128__c25 __attribute__((mode(TI)));
extern int D_004BD898__c25[] asm("D_004BD898");
extern void func_001DB6B0__c25(int,int,int,int *,int *,int) asm("func_001DB6B0");
extern void func_001D4A90(void *self, unsigned int index, int arg);
void func_001D4A90(void *self, unsigned int index, int arg) {
 int x=0,y=0;
 func_001DB6B0__c25(arg,45,240,&x,&y,0);
 switch(index) {
 case 0:
 func_00201B68(D_004BD898__c25[0], 0, ((u128__c25)0x454EUL<<64)|0x4F5F455059545F50UL, x, y, 0, 0);
 break;
 case 1:
 func_00201B68(D_004BD898__c25[0], 0, ((u128__c25)0x4F57UL<<64)|0x545F455059545F50UL, x, y, 0, 0);
 break;
 case 2:
 func_00201B68(D_004BD898__c25[0], 0, ((u128__c25)0x45455248UL<<64)|0x545F455059545F50UL, x, y, 0, 0);
 break;
 case 3: case 4:
 func_00201B68(D_004BD898__c25[0], 0, ((u128__c25)0x52554FUL<<64)|0x465F455059545F50UL, x, y, 0, 0);
 break;
 case 5:
 func_00201B68(D_004BD898__c25[0], 0, ((u128__c25)0x455649UL<<64)|0x465F455059545F50UL, x, y, 0, 0);
 break;
 }
}
/* ---- merged from src/unsorted/001D4C48.c ---- */
typedef struct {
    int x0;
    int x4;
    int x8;
} Obj__c26;

extern void func_001D4C48(Obj__c26 *p);
void func_001D4C48(Obj__c26 *p)
{
    p->x0 = 0;
    p->x4 = 0;
    p->x8 = 0;
}
typedef struct State_1D4C58 { char pad[0x288]; int count; char pad2[0x41c-0x28c]; int active; } State_1D4C58;
typedef struct Entry_1D4C58 { int code; char pad[0x2c]; } Entry_1D4C58;
typedef struct List_1D4C58 { char pad[0x260]; Entry_1D4C58 entries[3][12]; char pad2[0x220]; int counts[4]; } List_1D4C58;
typedef struct Info_1D4C58 { char pad[0xfc]; unsigned char enabled; } Info_1D4C58;
extern State_1D4C58 D_004B25A0_1D4C58 __asm__("D_004B25A0");
extern List_1D4C58 D_00398C40_1D4C58 __asm__("D_00398C40");
extern Info_1D4C58 D_004C26C0;
extern int D_0043F348[], D_004BD8A0[];
extern int func_00222B08(int, int);
extern int D_0029D5E4;
extern void func_001DB6B0_1D4C58(float *, int, int, int *, int *, int *) __asm__("func_001DB6B0");
void func_001D4C58(void) {
 float pos[4]; int volume=1024, other=0;
 int i,j,k; List_1D4C58 *list;
 int flags[D_004B25A0_1D4C58.count];
 if((unsigned)(D_0043F348[0]-1)<3 && D_004B25A0_1D4C58.active) {
   pos[0]=8100.0f; pos[1]=500.0f; pos[2]=-2600.0f;
   for(k=0;k<D_004B25A0_1D4C58.count;k++) flags[k]=0;
   i=0;
   if(i<D_004B25A0_1D4C58.count) {
    list=&D_00398C40_1D4C58;
    do {
     for(j=0;j<list->counts[i];j++) {
       int code=list->entries[i][j].code;
       if(code==0x645 || code==0x652 || code==0x669) flags[i]=1;
     }
    } while(++i<D_004B25A0_1D4C58.count);
   }
   func_001DB6B0_1D4C58(pos,3000,8000,&volume,&other,flags);
   if(D_004C26C0.enabled==1 && func_00222B08(0,100)<D_0029D5E4 && volume>0) {
     func_00201B68(D_004BD8A0[0], 0, ((u128)0x474e49<<64)|(u128)0x4e544847494c5f45UL, volume, other, 0, 0);
   }
 }
}
/* ---- func_001D4E80: weather ambience (rain/snow loop, rain backdrop, lightning) ---- */
typedef struct Ambience_1D4E80 {
    int x0;
    unsigned int loop;          /* rain/snow loop sound: 0 none, ~0 starting, else handle */
    unsigned int back;          /* rain backdrop sound, same states */
} Ambience_1D4E80;
typedef struct Weather_1D4E80 {
    char pad0[0xD0];
    float score[4];             /* weather strength per player */
    int kind[4];                /* weather kind per player, 2 = snow */
} Weather_1D4E80;
/* the lightning source sits at D_004C26C0 + 0x80 and the rain backdrop source at + 0x90;
   retail addresses both through their own symbols */
typedef struct Storm_1D4E80 {
    float pos[4];
    char pad10[0x6C];
    unsigned char lightning;    /* lightning enabled (D_004C26C0 + 0xFC) */
} Storm_1D4E80;
extern Weather_1D4E80 D_004C26C0__w asm("D_004C26C0");
extern Storm_1D4E80 D_004C2740__w asm("D_004C2740");
extern float D_004C2750__w[] asm("D_004C2750");
extern int D_004B29BC[];
extern void func_001DB6B0__w(float *pos, int nearDist, int farDist, int *volume, int *pan, int flags)
    asm("func_001DB6B0");
extern void func_00201BD0__w(int bank, int b, u128 name, int volume, int pan, int f, int g,
                             SoundCompletionCallback done, SoundCallbackData handle) asm("func_00201BD0");
extern void func_00201E08__w(unsigned int handle, int a, int volume, int pan, int e, int f,
                             SoundCompletionCallback done, SoundCallbackData out) asm("func_00201E08");
extern void func_00208BF8__w(void *bank, unsigned int *handle) asm("func_00208BF8");

/* sound names packed little-endian into 128 bits */
#define NAME_E_SNOW_STORM_L (((u128)0x4C5F4D524F54LL << 64) | (u128)0x535F574F4E535F45LL)
#define NAME_E_RAIN_HVY_L   (((u128)0x4C5F5956LL << 64) | (u128)0x485F4E4941525F45LL)
#define NAME_E_RAINBACK_L   (((u128)0x4C5F4B43LL << 64) | (u128)0x41424E4941525F45LL)
#define NAME_E_LIGHTNING    (((u128)0x474E49LL << 64) | (u128)0x4E544847494C5F45LL)

/* returns 1 while some weather is active */
int func_001D4E80(Ambience_1D4E80 *amb)
{
    int volume = 0x400;
    int pan = 0;
    int right = 0;
    int left = 0;
    int kind = 0;
    int active = 0;
    int i;
    Weather_1D4E80 *weather;
    float value;
    float strength = 0.0f;
    unsigned int handle;
    Storm_1D4E80 *storm;
    int *kinds;

    if ((unsigned int)(D_0043F348[0] - 1) >= 3 || D_004B29BC[0] == 0) {
        return 0;
    }

    /* strongest weather over six slots; there are only four scores, so the last two read
       kind[0] and kind[1] as floats (indexing through kind matches retail's addressing) */
    for (i = 0; i < 6; i++) {
        weather = &D_004C26C0__w;
        kinds = weather->kind;
        value = ((float *)kinds)[i - 4];
        if (strength < value) {
            kind = kinds[i];
            strength = value;
        }
    }

    if (strength > 0.0f) {
        active = 1;

        /* split screen: pan towards the side(s) whose players have weather */
        if (D_004B25A0__c1.x288 >= 2) {
            Weather_1D4E80 *w = &D_004C26C0__w;
            Weather_1D4E80 *w2;

            if (w->score[1] > 0.0f || w->score[3] > 0.0f) {
                right = 1;
            }
            w2 = &D_004C26C0__w;
            if (w2->score[0] > 0.0f || w2->score[2] > 0.0f) {
                left = 1;
            }

            if (right) {
                if (left) {
                    pan = 0;
                } else {
                    pan = 270;
                }
            } else if (left) {
                pan = 90;
            }
        } else {
            pan = 0;
        }

        volume = (int)((float)volume * strength) + 200;
        if (volume > 0x400) {
            volume = 0x400;
        }

        handle = amb->loop;
        if (handle == 0) {
            amb->loop = 0xFFFFFFFFU;
            if (kind == 2) {
                func_00201BD0__w(D_004BD8A0[0], 0, NAME_E_SNOW_STORM_L, volume, pan, 0, 0,
                                 func_00208C50, (long)&amb->loop);
            } else {
                func_00201BD0__w(D_004BD8A0[0], 0, NAME_E_RAIN_HVY_L, volume, pan, 0, 0,
                                 func_00208C50, (long)&amb->loop);
            }
        } else if (handle != 0xFFFFFFFFU && handle != 0) {
            amb->loop = 0xFFFFFFFFU;
            func_00201E08__w(handle, 0xD, volume, pan, 0, 0, func_00208C50, (long)&amb->loop);
        }
    } else {
        func_00208BF8__w(D_004BD840, &amb->loop);
    }

    if (D_004B25A0__c1.x2A0 != 0xF) {
        func_001DB6B0__w(D_004C2750__w, 1400, 2000, &volume, &pan, 0);
        if (volume > 0) {
            if (kind != 2 &&
                D_004B25A0__c1.x2A0 != 0x13 &&
                D_004B25A0__c1.x2A0 != 0xC &&
                D_004B25A0__c1.x2A0 != 3) {
                handle = amb->back;
                if (handle == 0) {
                    amb->back = 0xFFFFFFFFU;
                    func_00201BD0__w(D_004BD8A0[0], 0, NAME_E_RAINBACK_L, volume, pan, 0, 0,
                                     func_00208C50, (long)&amb->back);
                } else if (handle != 0xFFFFFFFFU && handle != 0) {
                    amb->back = 0xFFFFFFFFU;
                    func_00201E08__w(handle, 0xD, volume, pan, 0, 0, func_00208C50,
                                     (long)&amb->back);
                }
            }

            storm = &D_004C2740__w;
            func_001DB6B0__w(storm->pos, 150, 2000, &volume, &pan, 0);
            if (storm->lightning == 1) {
                if (func_00222B08(0, 100) < D_0029D5E4 && volume > 0) {
                    func_00201B68(D_004BD8A0[0], 0, NAME_E_LIGHTNING, volume, pan, 0, 0);
                }
            }
        } else {
            func_00208BF8__w(D_004BD840, &amb->back);
        }
    } else {
        func_001DB6B0__w(D_004C2740__w.pos, 1500, 4200, &volume, &pan, 0);
        if (D_004C2740__w.lightning == 1) {
            if (func_00222B08(0, 100) < D_0029D5E4 && volume > 0) {
                func_00201B68(D_004BD8A0[0], 0, NAME_E_LIGHTNING, volume, pan, 0, 0);
            }
        }
    }

    return active;
}
#undef NAME_E_SNOW_STORM_L
#undef NAME_E_RAIN_HVY_L
#undef NAME_E_RAINBACK_L
#undef NAME_E_LIGHTNING
