/* UNIT 0x0018B3A8 0x0018CF38 .sbss=0x29DE38:0x29DE60 .sdata=0x29CE30:0x29CE68 */
#include "menu_finalize.h"
#include "menu_items.h"
#include "ui_style.h"
#include "animation.h"
#include "include_asm.h"

/* .sdata starts here (8-aligned 0x29CE30): owned by func_0018CE20 below. */
int D_0029CE30 = 0;

/* ---- merged from src/unsorted/0018B3A8.c ---- */
extern "C" {
typedef struct Vec4__c1 { float x, y, z, w; } Vec4__c1;
typedef struct Obj__c1 { int x0; unsigned char x4; char p5[6]; unsigned char xB; char pC[4]; Vec4__c1 pos; } Obj__c1;
union Obj2__c1 { unsigned long flags; struct { char pad0[4]; char x4; } fields; };
typedef struct Entry__c1 { int x0; Obj__c1 *x4; } Entry__c1;
typedef struct Heap__c1 { int x0; int x4; int x8; } Heap__c1;

int D_0029CE34 = 0;
unsigned int D_0029CE38 = 0;

extern void func_00218498__c1(Obj__c1 *, int) asm("func_00218498");
extern void *memcpy(void *, const void *, unsigned int);
extern void *func_00208FB8__c1(void) asm("func_00208FB8");
extern void func_00218278__c1(void *, int) asm("func_00218278");
typedef struct Game__c1 { char p0[0x10C]; int x10C; char p110[0x288 - 0x110]; int x288; } Game__c1;
extern Game__c1 D_004B25A0__c1 asm("D_004B25A0");
extern void func_001BAF10__c1(void *p, int n) asm("func_001BAF10");
extern void func_0018EC08__c1(void *obj) asm("func_0018EC08");
extern Entry__c1 D_003EC540__c1[10][4] asm("D_003EC540");
extern Heap__c1 D_00663B08__c1 asm("D_00663B08");

extern void func_0018B3A8__c1(Obj__c1 *src, Obj__c1 **out) asm("func_0018B3A8");
void func_0018B3A8__c1(Obj__c1 *src, Obj__c1 **out) {
 int size; void *dst; Obj__c1 *o;
 func_00218498__c1(src,1);
 size=src->xB*4+12;
 dst=(void *)D_0029CE38;
 memcpy(dst,src,size);
 D_0029CE38=(D_0029CE38+size+15)&0xfffffff0;
 o=(Obj__c1 *)func_00208FB8__c1(); func_00218278__c1(o,(int)dst); o->x4=1; *out=o;
}

extern void func_0018B448__c1(int value,union Obj2__c1 **out) asm("func_0018B448");
void func_0018B448__c1(int value,union Obj2__c1 **out) {
 union Obj2__c1 *p;
 p=(union Obj2__c1 *)func_00208FB8__c1();
 func_00218278__c1(p,value);
 p->fields.x4=1;
 p->flags &= 0xEFFFFFFFFFFFFFFFUL;
 *out=p;
}


/* A 0x20-byte style record built by func_0018D7D0__c1. */
typedef UiStyle Rec__c1;

typedef struct Label__c1 { short x0; short x2; char p4[3]; char x7; } Label__c1;

typedef struct Panel__c1 {
    void *vptr;
    Rec__c1 *x4;
    Rec__c1 *x8;
    Rec__c1 *xC;
    Rec__c1 *x10;
    char p14[0x438 - 0x14];
    Label__c1 x438;
    char p440[0x45C - 0x440];
    int x45C;
    int x460;
    int x464;
    int x468;
    char p46C[0x478 - 0x46C];
    int x478;
    char p47C[0x4A8 - 0x47C];
    int x4A8;
    char p4AC[0x508 - 0x4AC];
    char x508;
    char p509[0x510 - 0x509];
    void *x510;
    int x514;
    char p518[0x520 - 0x518];
} Panel__c1;

typedef struct PanelB__c1 { Panel__c1 base; char p520[0x20]; } PanelB__c1;

typedef struct PanelC__c1 {
    Panel__c1 base;
    char p520[4];
    int x524[5];
    char p538[0x54C - 0x538];
    Entry__c1 *x54C[5];
    char p560[0x56C - 0x560];
    char x56C;
    char p56D[3];
    int x570;
    char p574[0x5E0 - 0x574];
} PanelC__c1;

typedef struct Box__c1 { char pad0[0x10]; } Box__c1;

typedef struct Profile__c1 {
    char p0[0x45C];
    int x45C;
    char p460[0x570 - 0x460];
    Panel__c1 x570;
    Box__c1 xA90;
    Panel__c1 xAA0;
    PanelB__c1 xFC0;
    PanelC__c1 x1500;
    Panel__c1 x1AE0;
    Box__c1 x2000;
    Box__c1 x2010;
    char p2020[0x20];
} Profile__c1;

typedef struct Marker__c1 { void *vptr; Obj__c1 *x4; char p8[0x18]; } Marker__c1;

typedef struct Ref__c1 { int *vptr; int x4; } Ref__c1;

typedef struct Camera__c1 {
    void *vptr;
    char p4[0x54 - 4];
    int x54;
    Rec__c1 *x58;
} Camera__c1;

typedef struct Listener__c1 {
    int x0;
    void *x4;
    char p8[6];
    char xE;
    char pF;
    void *vptr;
    int x14;
    char x18;
} Listener__c1;

typedef struct Menu__c1 { char p0[0x500]; char x500; } Menu__c1;

typedef struct World__c1 { char p0[0x20]; int x20; char p24[0x12C - 0x24]; unsigned char x12C; } World__c1;

typedef struct Font__c1 { char pad0[0xA0]; } Font__c1;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

int D_0029CE3C = 15;
int D_0029CE40 = 80;
float D_0029CE44 = 4.6f;
float D_0029CE48 = -5.5f;
float D_0029CE4C = 0.0f;
float D_0029CE50 = 0.0f;
float D_0029CE54 = 0.0f;


int D_0029DE38 __attribute__((section(".sbss"))) = 0;
int D_0029DE3C __attribute__((section(".sbss"))) = 0;
int D_0029DE40 __attribute__((section(".sbss"))) = 0;
int D_0029DE44 __attribute__((section(".sbss"))) = 0;
int D_0029DE48 __attribute__((section(".sbss"))) = 0;
int D_0029DE4C __attribute__((section(".sbss"))) = 0;
int D_0029DE50 __attribute__((section(".sbss"))) = 0;
int D_0029DE54 __attribute__((section(".sbss"))) = 0;
int D_0029DE58 __attribute__((section(".sbss"))) = 0;

extern char D_002872A0__c1[] asm("D_002872A0");
extern char D_002872C0__c1[] asm("D_002872C0");
extern char D_002872D8__c1[] asm("D_002872D8");
extern Vec4__c1 D_0026F4C0__c1[] asm("D_0026F4C0");
extern Camera__c1 D_003EB840__c1 asm("D_003EB840");
extern Marker__c1 D_003EB8A0__c1[3][4] asm("D_003EB8A0");
extern Rec__c1 D_003EBA20__c1 asm("D_003EBA20");
extern Rec__c1 D_003EBA40__c1 asm("D_003EBA40");
extern Rec__c1 D_003EBA60__c1 asm("D_003EBA60");
extern Rec__c1 D_003EBA80__c1 asm("D_003EBA80");
extern Rec__c1 D_003EBAA0__c1 asm("D_003EBAA0");
extern Menu__c1 D_003EBAC0__c1 asm("D_003EBAC0");
extern Menu__c1 D_003EBFD0__c1 asm("D_003EBFD0");
extern Profile__c1 D_003EC680__c1[4] asm("D_003EC680");
extern World__c1 D_003F52D0__c1 asm("D_003F52D0");
extern char D_004225A0__c1[] asm("D_004225A0");
extern char D_004C7F50__c1[] asm("D_004C7F50");
extern Font__c1 D_004C8508__c1[] asm("D_004C8508");
extern Ref__c1 D_0029DE60 asm("D_0029DE60");

extern void func_0018B0B8__c1(void) asm("func_0018B0B8");
extern void func_0023D650__c1(int, int *, int *) asm("func_0023D650");
extern void func_0018EBC0__c1(void *, int) asm("func_0018EBC0");
extern Obj__c1 *func_001BA368__c1(void *, int) asm("func_001BA368");
extern Obj__c1 *func_0013AF60__c1(int) asm("func_0013AF60");
extern void func_0018E4D8__c1(void *, void *) asm("func_0018E4D8");
extern void func_0018E398__c1(void *) asm("func_0018E398");
extern void *func_00158638__c1(int) asm("func_00158638");
extern void *func_00158668__c1(int) asm("func_00158668");
extern int func_00193FE8__c1(void *, int) asm("func_00193FE8");
extern void func_0018E408__c1(void *, const void *, int) asm("func_0018E408");
extern void func_0018FEC0__c1(void *, int, int, int, char) asm("func_0018FEC0");
extern void func_0018E638__c1(void *, int, Ref__c1 *) asm("func_0018E638");
extern void func_0018FD70__c1(void *, int) asm("func_0018FD70");
extern void func_0018DFA8__c1(void *, void *) asm("func_0018DFA8");
extern void func_0018FF30__c1(void *, float, float, float) asm("func_0018FF30");
extern void func_0018E720__c1(void *) asm("func_0018E720");
extern void func_0018EA60__c1(void *, void *) asm("func_0018EA60");
extern void func_0018DB20__c1(void *) asm("func_0018DB20");
extern void func_0013CAF0__c1(void *, int, int, int) asm("func_0013CAF0");
extern int func_001F9B98__c1(Font__c1 *) asm("func_001F9B98");
extern int func_001F9BF0__c1(Font__c1 *) asm("func_001F9BF0");
extern void func_0013D750__c1(void *, int, int) asm("func_0013D750");
extern void func_0018EF08__c1(void *) asm("func_0018EF08");
extern void func_0018F160__c1(void *, void *) asm("func_0018F160");
extern void func_0018EE18__c1(void *, void *) asm("func_0018EE18");
extern Obj__c1 *func_00209108__c1(void) asm("func_00209108");
extern void func_001B5AF0__c1(void *, int, int) asm("func_001B5AF0");
extern void func_00190108__c1(void *, void *) asm("func_00190108");
extern void func_00209060__c1(void *) asm("func_00209060");
extern void func_002091A0__c1(void *) asm("func_002091A0");
extern void func_0018B280__c1(void) asm("func_0018B280");
extern void func_0018C590__c1(void);
extern void func_0018C630__c1(void);
extern void func_0018B448__c1(int value, union Obj2__c1 **out);

static __inline__ Font__c1 *FontAt(Font__c1 *fonts, int idx) {
    return &fonts[idx];
}

static __inline__ void Panel_SetMask(Panel__c1 *pn, int bit) {
    pn->x45C = bit;
}

static __inline__ void Panel_Init(Panel__c1 *pn, int id, int style) {
    func_0018E398__c1(pn);
    pn->x4A8 = style;
    pn->x510 = (void *)id;
}

static __inline__ void Panel_Enable(Panel__c1 *pn) {
    pn->x478 = 1;
}

static __inline__ int Panel_Height(Panel__c1 *pn) {
    return pn->x468;
}

static __inline__ void Panel_Place(Panel__c1 *pn, int x, int y) {
    pn->x464 = x;
    pn->x460 = y;
}

static __inline__ void Panel_SetStyle(Panel__c1 *pn, int style) {
    pn->x4A8 = style;
}

static __inline__ void Panel_SetId(Panel__c1 *pn, int id) {
    pn->x510 = (void *)id;
}

static __inline__ void Panel_SetIndex(Panel__c1 *pn, int index) {
    pn->x514 = index;
}

static __inline__ void Panel_SetLink(Panel__c1 *pn, void *link) {
    pn->x510 = link;
}

static __inline__ void PanelC_SetPlayer(PanelC__c1 *pn, int player) {
    pn->x56C = 1;
    pn->x570 = player;
}

static __inline__ void Panel_Reset(Panel__c1 *pn) {
    pn->x508 = 0;
}

static __inline__ void SetLabel(Label__c1 *l, int x, int y) {
    l->x0 = x;
    l->x2 = y;
    l->x7 = 1;
}

static __inline__ void SetPos(Vec4__c1 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

static __inline__ void Listener_Init(Listener__c1 *l) {
    l->vptr = D_002872A0__c1;
    l->x4 = D_004C7F50__c1;
    l->xE = 1;
}

static __inline__ void Menu_SetVisible(Menu__c1 *m, int v) {
    m->x500 = v;
}

extern void func_0018B4C8__c1(void) asm("func_0018B4C8");
void func_0018B4C8__c1(void) {
    int width, height, w = 0;
    Listener__c1 link;
    int count, small;
    Obj__c1 *o = 0, *o2 = 0;

    if (!D_0029DE38) {
        D_003EB840__c1.vptr = D_002872C0__c1;
        D_003EB840__c1.x54 = 0;
        D_003EB840__c1.x58 = 0;
        D_0029DE38 = 1;
    }
    D_003EB840__c1.x54 = 0;
    func_0018B0B8__c1();
    func_0023D650__c1(D_003F52D0__c1.x20, &width, &height);
    w = width;
    if (!D_0029DE40) {
        D_0029DE3C = (w - D_0029CE40 * 2) / 3;
        D_0029DE40 = 1;
    }
    func_0018EBC0__c1(&D_003EBFD0__c1, w);
    count = D_004B25A0__c1.x288;
    func_0018C590__c1();
    small = count < 3;
    func_00218498__c1(func_001BA368__c1(&D_004B25A0__c1, 0x233A), small == 0 ? 2 : 1);
    func_00218498__c1(func_001BA368__c1(&D_004B25A0__c1, 0x2339), small == 0 ? 2 : 1);
    func_00218498__c1(func_0013AF60__c1(0x25B0), 0);
    func_00218498__c1(func_0013AF60__c1(0x25C8), count == 4);
    Listener_Init(&link);
    func_0018E4D8__c1(&D_003EBFD0__c1, &link);

    if (!D_0029DE44) {
        Marker__c1 *m = D_003EB8A0__c1[0];
        int r = 2;
        do {
            Marker__c1 *next = m + 4;
            int c = 3;
            do {
                m->vptr = D_002872D8__c1;
                c--;
                m->x4 = 0;
                m++;
            } while (c != -1);
            r--;
            m = next;
        } while (r != -1);
        D_0029DE44 = 1;
    }
    if (!D_0029DE48) {
        func_0018D7D0((UiStyle *)(void *)(&D_003EBA20__c1), 0x120, 0xA0, -0x64, -0x20, 0x6E, 0x6E, 0x6E, 0x80);
        D_0029DE48 = 1;
    }
    if (!D_0029DE4C) {
        func_0018D7D0((UiStyle *)(void *)(&D_003EBA40__c1), 0x120, 0xA0, -0x64, -0x20, 0xA3, 0x81, 0x38, 0x80);
        D_0029DE4C = 1;
    }
    if (!D_0029DE50) {
        func_0018D7D0((UiStyle *)(void *)(&D_003EBA60__c1), 0x1F4, 0x15E, -0xC5, 0x18, 0x64, 0xA, 0xA, 0x80);
        D_0029DE50 = 1;
    }
    if (!D_0029DE54) {
        func_0018D7D0((UiStyle *)(void *)(&D_003EBA80__c1), 0x120, 0xA0, -0x64, -0x20, 0xA3, 0x81, 0x38, 0x80);
        D_0029DE54 = 1;
    }
    if (!D_0029DE58) {
        func_0018D7D0((UiStyle *)(void *)(&D_003EBAA0__c1), 0x120, 0xA0, -0x64, -0x20, 0x4C, 0x4C, 0x4C, 0x80);
        D_0029DE58 = 1;
    }

    {
    Profile__c1 *p = 0;
    Marker__c1 *m = 0;
    PanelB__c1 *pd = 0;
    Obj__c1 **e = 0;
    int top = 0, col = 0, j = 0;
    float frow = 0.0f;
    /* Always 0, but converted each iteration: the compiler loads the 0 from a
       constant-pool word (0x29CE60) with lwc1 + cvt.s.w. */
    int row = 0;
    Font__c1 *font = 0;
    int id = 0;
    int i;
    int pos = 0, n;

    for (i = 0; i < count; i++) {
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25AA), &D_003EC540__c1[0][i].x4);
        col = i << small;
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25AC), &D_003EC540__c1[6][i].x4);
        SetPos(&D_003EC540__c1[6][i].x4->pos, D_0029CE4C + 2.8f + (float)col * 4.6f + (float)small * 2.5f, -8.0f,
               D_0029CE50 + 0.8f + -0.0f);
        func_0018B448__c1((int)func_001BA368__c1(&D_004B25A0__c1, 0x25A3), (union Obj2__c1 **)&D_003EC540__c1[4][i].x4);
        SetPos(&D_003EC540__c1[4][i].x4->pos, D_0029CE4C + 2.8f + (float)col * 4.6f + (float)small * 2.5f, -8.0f,
               D_0029CE50 + 0.8f + -0.0f);
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25A4), &D_003EC540__c1[3][i].x4);
        SetPos(&D_003EC540__c1[3][i].x4->pos, D_0029CE4C + 2.8f + (float)col * 4.6f + (float)small * 2.5f, -8.0f,
               D_0029CE50 + 0.8f + -0.0f);
        func_0018B448__c1((int)func_001BA368__c1(&D_004B25A0__c1, 0x25A5), (union Obj2__c1 **)&D_003EC540__c1[5][i].x4);
        SetPos(&D_003EC540__c1[5][i].x4->pos, D_0029CE4C + 2.8f + (float)col * 4.6f + (float)small * 2.5f, -8.0f,
               D_0029CE50 + 0.8f + -0.0f);
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25A7), &D_003EC540__c1[2][i].x4);
        SetPos(&D_003EC540__c1[2][i].x4->pos, D_0029CE4C + 3.0f + (float)col * 4.6f + (float)small * 2.45f, -8.0f,
               0.5f);
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25A6), &D_003EC540__c1[1][i].x4);
        SetPos(&D_003EC540__c1[1][i].x4->pos, D_0029CE4C + 3.0f + (float)col * 4.6f + (float)small * 2.45f, -8.0f,
               0.5f);
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25A8), &D_003EC540__c1[7][i].x4);
        SetPos(&D_003EC540__c1[7][i].x4->pos, D_0029CE4C + (float)col * 6.4f + (float)small * 3.4f, -0.0f,
               -0.0f);
        func_0018B3A8__c1(func_001BA368__c1(&D_004B25A0__c1, 0x25A9), &D_003EC540__c1[8][i].x4);
        SetPos(&D_003EC540__c1[8][i].x4->pos, D_0029CE4C + (float)col * 6.4f + (float)small * 3.4f, -0.0f,
               -0.0f);

        D_003EC540__c1[9][i].x4 = (Obj__c1 *)func_00208FB8__c1();
        func_00218278__c1(D_003EC540__c1[9][i].x4, (int)func_001BA368__c1(&D_004B25A0__c1, i + 0x25DF));
        D_003EC540__c1[9][i].x4->x4 = 1;
        ((union Obj2__c1 *)D_003EC540__c1[9][i].x4)->flags &= 0xEFFFFFFFFFFFFFFFUL;
        if (small) {
            COPY_VECTOR(D_003EC540__c1[9][i].x4->pos, &D_0026F4C0__c1[i]);
        }

        p = &D_003EC680__c1[i];
        frow = row;
        p->x45C = 1 << i;
        Panel_SetMask(&p->x570, 1 << i);
        Panel_SetMask(&p->xAA0, 1 << i);
        p->xFC0.base.x45C = 1 << i;
        p->x1500.base.x45C = 1 << i;
        p->x1AE0.x45C = 1 << i;
        Panel_Init(&p->x570, i, 5);
        for (j = 0; j < 14; j++) {
            if (j >= 13) {
                func_0018E408__c1(&p->x570, func_00158638__c1(15), func_00193FE8__c1(D_004225A0__c1, j));
            } else {
                func_0018E408__c1(&p->x570, func_00158668__c1(j), func_00193FE8__c1(D_004225A0__c1, j));
            }
        }
        Panel_Enable(&p->x570);
        p->x570.xC = &D_003EBAA0__c1;
        p->x570.x10 = &D_003EBA80__c1;
        pos = D_0029CE40 + i * D_0029DE3C;
        if (small) {
            pos = (float)(i * (D_0029DE3C + D_0029DE3C) + D_0029CE40) + D_0029DE3C * 0.5f;
        }
        top = D_0029CE3C;
        func_0018FEC0__c1(&D_003EB840__c1, D_0029DE60.vptr[i], pos, top, 1);
        top += 12;
        p->x570.x4 = &D_003EBA40__c1;
        D_003EB840__c1.x58 = &D_003EBA20__c1;
        Panel_Place(&p->x570, pos, top + Panel_Height(&p->x570));
        SetLabel(&p->x570.x438, pos, top);
        func_0018E638__c1(&p->x570, 5, &D_0029DE60);
        Panel_Reset(&p->x570);
        e = &D_003EC540__c1[0][i].x4;
        func_0018FD70__c1(&p->xA90, (*e)->x0);
        func_0018DFA8__c1(&p->x570, &p->xA90);
        func_0018DFA8__c1(&p->x570, &D_003EC540__c1[0][i]);
        m = &D_003EB8A0__c1[0][i];
        m->x4 = *e;
        func_0018FF30__c1(m, (float)col * D_0029CE44 + 4.0f + D_0029CE44 * ((float)small * 0.5f),
                      -8.0f, frow * D_0029CE48 + -1.0f);
        func_0018DFA8__c1(&p->x570, m);

        pd = &p->xFC0;
        func_0018E720__c1(pd);
        func_0018E3A8((struct SharedMenu *)(void *)(pd), 8, &D_0029DE60, 1);
        func_0018EA60__c1(pd, &p->x1500);
        func_0018E3A8((struct SharedMenu *)(void *)(pd), 9, &D_0029DE60, 1);
        func_0018EA60__c1(pd, &p->x1AE0);
        pd->base.x464 = pos;
        pd->base.x460 = top + Panel_Height(&p->x570);
        SetLabel(&p->xFC0.base.x438, pos, top);
        p->xFC0.base.xC = &D_003EBAA0__c1;
        p->xFC0.base.x10 = &D_003EBA80__c1;
        p->xFC0.base.x4 = &D_003EBA40__c1;
        func_0018E638__c1(&p->xFC0, 7, &D_0029DE60);

        func_0018E398__c1(&p->x1AE0);
        func_0018DB20__c1(&p->x1AE0);
        Panel_SetStyle(&p->x1AE0, 5);
        func_0018E408__c1(&p->x1AE0, "", 1);
        func_0018E408__c1(&p->x1AE0, "", 1);
        for (n = 0; n < 17; n++) {
            func_0018E408__c1(&p->x1AE0, "", 1);
        }
        Panel_SetId(&p->x1AE0, i);
        func_0018FD70__c1(&p->x2010, D_003EC540__c1[6][i].x4->x0);
        func_0018DFA8__c1(&p->x1AE0, &p->x2010);
        func_0018FD70__c1(&p->x2000, D_003EC540__c1[3][i].x4->x0);
        func_0018DFA8__c1(&p->x1AE0, &p->x2000);
        func_0018DFA8__c1(&p->x1AE0, &D_003EC540__c1[3][i]);
        func_0018DFA8__c1(&p->x1AE0, &D_003EC540__c1[5][i]);
        func_0018DFA8__c1(&p->x1AE0, &D_003EC540__c1[6][i]);
        func_0018DFA8__c1(&p->x1AE0, &D_003EC540__c1[4][i]);
        func_0018DFA8__c1(&p->x1AE0, &D_003EC540__c1[0][i]);
        func_0018DFA8__c1(&p->x1AE0, &D_003EB8A0__c1[0][i]);
        Panel_Place(&p->x1AE0, pos, top);

        func_0018E398__c1(&p->x1500);
        PanelC_SetPlayer(&p->x1500, i);
        func_0013CAF0__c1(&p->x1500, i * 3 + 0x25D3, i * 3 + 0x25D4, i * 3 + 0x25D5);
        func_0018DFA8__c1(&p->x1500, &D_003EC540__c1[9][i]);
        id = D_004B25A0__c1.x10C;
        font = FontAt(D_004C8508__c1, id);
        if (id == 12 || id == 13 || id == 17 || id == 16 || id == 18 || id == 11 || id == 15 ||
            (id >= 6 && id <= 9)) {
            func_0013D750__c1(&p->x1500, func_001F9B98__c1(FontAt(D_004C8508__c1, 3)), func_001F9BF0__c1(FontAt(D_004C8508__c1, 3)));
        } else {
            func_0013D750__c1(&p->x1500, func_001F9B98__c1(font), func_001F9BF0__c1(font));
        }
        Panel_Place(&p->x1500.base, pos, top + 0x85);
        p->x1500.x524[0] = D_003EC540__c1[1][i].x4->x0;
        p->x1500.x54C[0] = &D_003EC540__c1[1][i];
        p->x1500.x524[1] = D_003EC540__c1[2][i].x4->x0;
        p->x1500.x54C[1] = &D_003EC540__c1[2][i];
        p->x1500.x524[2] = D_003EC540__c1[7][i].x4->x0;
        p->x1500.x54C[2] = &D_003EC540__c1[7][i];
        p->x1500.x524[4] = D_003EC540__c1[8][i].x4->x0;
        p->x1500.x54C[4] = &D_003EC540__c1[8][i];

        Panel_SetIndex(&p->xAA0, i);
        Panel_Place(&p->xAA0, pos, top + 10);
        func_0018E398__c1(&p->xAA0);
        p->xAA0.x10 = &D_003EBA60__c1;
        func_0018E3A8((struct SharedMenu *)(void *)(&p->xAA0), 4, &D_0029DE60, 1);
        Panel_Reset(&p->xAA0);
        func_0018DFA8__c1(&p->xAA0, &D_003EC540__c1[0][i]);
        func_0018DFA8__c1(&p->xAA0, &D_003EB8A0__c1[0][i]);
        Panel_SetLink(&p->xAA0, &p->x1500);
        func_0018EF08__c1(p);
        func_0018F160__c1(p, &p->x570);
        func_0018F160__c1(p, &p->xFC0);
        func_0018F160__c1(p, &p->xAA0);
        func_0018EE18__c1(&D_003EBFD0__c1, p);
    }

    }
    o = (Obj__c1 *)func_00208FB8__c1();
    func_00218278__c1(o, (int)func_001BA368__c1(&D_004B25A0__c1, 0x233B));
    o->x4 = 1;
    o2 = func_00209108__c1();
    func_00218278__c1(o2, (int)func_001BA368__c1(&D_004B25A0__c1, 0x2338));
    o2->x4 = 1;
    ((union Obj2__c1 *)o2)->flags &= 0xEFFFFFFFFFFFFFFFUL;
    func_0018DFA8__c1(&D_003EBFD0__c1, &D_003EB840__c1);
    func_001B5AF0__c1(&D_003F52D0__c1, 1, 0);
    func_001B5AF0__c1(&D_003F52D0__c1, 2, 1);
    Menu_SetVisible(&D_003EBAC0__c1, 1);
    func_00190108__c1(&D_003F52D0__c1, &D_003EBAC0__c1);
    if (D_003F52D0__c1.x12C) {
        func_00209060__c1(o);
        func_002091A0__c1(o2);
        func_0018B280__c1();
        func_0018C630__c1();
    }
}


extern void func_0018C558__c1(void *obj) asm("func_0018C558");
void func_0018C558__c1(void *obj)
{
    func_001BAF10__c1(&D_004B25A0__c1, 2);
    func_0018EC08__c1(obj);
}

static __inline__ void HeapPushMark(Heap__c1 *heap) {
 int prev;
 int *mark;
 prev = heap->x8;
 mark = (int *)((heap->x4 + 3) & ~3);
 heap->x8 = (int)mark;
 heap->x4 = (int)mark + 4;
 *mark = prev;
}

static __inline__ int HeapAlloc(Heap__c1 *heap, int size) {
 int p;
 p = (heap->x4 + 15) & ~15;
 heap->x4 = p + size;
 return p;
}

extern void func_0018C590__c1(void) asm("func_0018C590");
void func_0018C590__c1(void) {
 int i, j;
 Entry__c1 *e;

 for (i = 0; i < 4; i++) {
  e = &D_003EC540__c1[0][i];
  for (j = 9; j >= 0; j--) {
   e->x4 = 0;
   e += 4;
  }
 }
 HeapPushMark(&D_00663B08__c1);
 D_0029CE38 = D_0029CE34 = HeapAlloc(&D_00663B08__c1, 0x1400);
}

extern void func_0018C750__c1(void) asm("func_0018C750");
extern void func_001F67F0__c1(int) asm("func_001F67F0");
/* A heap mark is a word holding the previous mark. */
typedef struct HeapMark__c1 { int prev; } HeapMark__c1;

extern void func_0018C630__c1(void) asm("func_0018C630");
void func_0018C630__c1(void) {
 int i, j;
 HeapMark__c1 *mark;

 D_0029CE34 = 0;
 mark = (HeapMark__c1 *)D_00663B08__c1.x8;
 D_00663B08__c1.x4 = (int)mark;
 D_00663B08__c1.x8 = mark->prev;
 for (i = 0; i < 4; i++) {
  for (j = 0; j < 10; j++) {
   if (D_003EC540__c1[j][i].x4) func_00209060__c1(D_003EC540__c1[j][i].x4);
  }
 }
 func_0018C750__c1();
 func_001F67F0__c1(0);
}
}
/* ---- merged from src/unsorted/0018C6E0.c ---- */
extern "C" {
typedef AnimationHandle Handle__c2;
extern char D_003F52D0__c2[] asm("D_003F52D0");
extern void func_001B5AF0__c2(void *,int,int) asm("func_001B5AF0");
extern void func_0018C6E0__c2(void) asm("func_0018C6E0");
void func_0018C6E0__c2(void) {
 Handle__c2 handle;
 handle.x8 = 0;
 func_00203590(&handle,0x2330,0,0);
 func_002038E0(&handle,1);
 func_00203590(&handle,0x2588,0,3);
 func_002038E0(&handle,1);
 func_001B5AF0__c2(D_003F52D0__c2,0,1);
}
}
/* ---- merged from src/unsorted/0018C750.c ---- */
extern "C" {
typedef AnimationHandle Handle__c3;
extern void func_0018C750__c3(void) asm("func_0018C750");
void func_0018C750__c3(void) {
    Handle__c3 handle;
    handle.x8 = 0;
    func_00203590(&handle, 0x2330, 0, 1);
    func_00203FE0(&handle, 1);
    func_002038E0(&handle, 0);
    func_00203590(&handle, 0x2588, 0, 0);
    func_00203B68(&handle, 1, 1.0f);
    func_002038E0(&handle, 1);
}
}
extern "C" {
typedef struct { int x0, x4, x8; } Entry__w;
typedef struct { int x0; char pad4[0x2040 - 4]; } Entry2040__w;
typedef struct { int x0; char pad4[0x418 - 4]; int x418; char pad41C[0x2040 - 0x41C]; } Info2040__w;
typedef struct { int index; char pad4[0x578 - 4]; unsigned char x578[0x100]; char tail[0x2040 - 0x678]; } State2040__w;
typedef struct { char pad[0xA0]; } Font__w;
typedef struct {
    char pad0[0x10C];
    int x10C;
    int x110;
    char pad114[0x288 - 0x114];
    int x288;
    int x28c;
    int x290[16];
} Game__w;
typedef struct {
    Entry__w x0[2];
    Entry__w x18[2];
    char x30[0x20];
} Group__w;

extern Entry2040__w D_003ECBF0__w[] asm("D_003ECBF0");
extern Info2040__w D_003EE160__w[] asm("D_003EE160");
extern State2040__w D_003EDB80__w[] asm("D_003EDB80");
extern Game__w D_004B25A0__w asm("D_004B25A0");
extern Font__w D_004C8508__w[] asm("D_004C8508");
extern char D_002AF2C0__w[] asm("D_002AF2C0");
extern char D_004225A0__w[] asm("D_004225A0");
static inline Group__w *GetGroup(int mode) {
    int off = mode * 0x50;
    off += (int)D_004225A0__w;
    off += 0x11948;
    return (Group__w *)off;
}

extern void func_001B9AE8__w(void *, int, int, int, int) asm("func_001B9AE8");
extern void *func_001F9B98__w(void *) asm("func_001F9B98");
extern void func_001B97E0__w(void *, int, int, int, int) asm("func_001B97E0");
extern void func_001FABE8__w(void *, void *) asm("func_001FABE8");
extern int func_001FA7C0__w(int) asm("func_001FA7C0");
extern void func_001B9890__w(void *, int, int) asm("func_001B9890");

extern void func_0018C7C8__w(int) asm("func_0018C7C8");
void func_0018C7C8__w(int index)
{
    int state;
    int width;
    int wide;
    int selection;
    int limit;
    Entry__w tmp;
    Entry__w *e;
    int mode;
    int offset;

    state = D_003ECBF0__w[index].x0;
    if (state == 13) {
        state = 15;
    }
    width = (state + 1) << 5;
    D_004B25A0__w.x290[index] = width;
    wide = 0;
    if (width == 0xE0 || width == 0x100 || width == 0x140 || width == 0x180 || width == 0x1A0) {
        wide = 1;
    }
    {
        int off = index * 0x2040;
        off += (int)D_003EE160__w;
        selection = ((Info2040__w *)off)->x0;
        limit = D_003EE160__w[index].x418 - 0x12;
    }
    if (selection >= limit) {
        int n;
        tmp.x0 = index;
        n = selection - limit;
        tmp.x8 = (selection == 1);
        tmp.x4 = n > -1 ? n : 0;
    } else {
        tmp.x0 = index;
        tmp.x8 = selection & 1;
        tmp.x4 = 0;
    }
    func_001B9AE8__w(&D_004B25A0__w, D_004B25A0__w.x290[index], index, tmp.x4, tmp.x8);
    {
        int mode = D_004B25A0__w.x10C;
        int idx = mode;
        if (mode == 12 || mode == 13 || mode == 16 || mode == 18 || mode == 17 || mode == 15 || wide != 0 || (mode >= 6 && mode < 10)) {
            idx = 3;
        }
        {
            Entry__w *items = (Entry__w *)func_001F9B98__w(&D_004C8508__w[idx]);
            State2040__w *st = &D_003EDB80__w[index];
            unsigned char sel = st->x578[st->index];
            e = &items[sel];
        }
    }
    func_001B97E0__w(&D_004B25A0__w, index, e->x0, e->x4, e->x8);
    {
        offset = 0x7BD0;
        offset *= index;
        offset += (int)D_002AF2C0__w;
        func_001FABE8__w((void *)(offset + 0x78D8), e);
    }
    func_001B9890__w(&D_004B25A0__w, index, func_001FA7C0__w(e->x0));
    if (D_004B25A0__w.x288 != 2) {
        return;
    }
    mode = D_004B25A0__w.x10C;
    if (mode < 4) {
        return;
    }
    if (mode >= 10) {
        return;
    }
    {
        int off;
        int m;
        int work;
        int g = mode * 0x50;
        g += (int)D_004225A0__w;
        g += 0x11948;
        off = index * sizeof(Entry__w);

        tmp.x0 = state;
        *(Entry__w *)(off + g) = *e;
        work = D_004B25A0__w.x10C;
        m = work;
        work *= 0x50;
        work += (int)D_004225A0__w;
        work += 0x11948;
        off += work;
        *(Entry__w *)(off + 0x18) = tmp;
        {
            char value = func_001FA7C0__w(e->x0);
            GetGroup(m)->x30[index] = value;
        }
    }
}

}

/* ---- func_0018CAC0 ---- */
extern "C" {
typedef struct ListPanel__c9 {
    char pad0[0x418];
    int x418;
    char pad41C[0x510 - 0x41C];
    int x510;
} ListPanel__c9;
typedef struct Player__c9 { int mode; char pad4[0x2040 - 0x4]; } Player__c9;
typedef struct Progress__c9 { char pad0[0x2188]; int x2188; } Progress__c9;
extern Player__c9 D_003ECBF0__c9[] asm("D_003ECBF0");
extern Progress__c9 D_004325A0__c9 asm("D_004325A0");
extern void func_0018E408__c9(void *, const void *, int) asm("func_0018E408");


void func_0018CAC0(ListPanel__c9 *pn)
{
    int mode, row, special, i, bit, sel, notLast, ok;

    pn->x418 = 0;
    mode = D_003ECBF0__c9[pn->x510].mode;
    if (mode >= 13) {
        mode = 15;
    }
    row = (mode + 1) * 32;
    special = 0;
    if (row == 0xE0 || row == 0x100 || row == 0x140 || row == 0x180 || row == 0x1A0) {
        special = 1;
    }
    for (i = 0; i < 16; i++) {
        const char *label = "";
        notLast = row != 0x200;
        func_0018E408__c9(pn, label, i == mode);
        if (i != mode || special) {
            sel = 0;
        } else {
            sel = notLast;
        }
        func_0018E408__c9(pn, label, sel);
    }
    for (i = 1; i < 18; i++) {
        const char *label = "";
        bit = 1 << i;
        sel = 0;
        if (D_004325A0__c9.x2188 & bit) {
            ok = !special;
            mode = ok + 1; /* dead, but needed: retail keeps ok as its own (hoisted) register */
            sel = ok;
        }
        func_0018E408__c9(pn, label, sel);
    }
    FinalizeMenuLegacy(pn);
}
}
/* ---- merged from src/unsorted/0018CC28.c ---- */
extern "C" {
struct Entry__c4 {
    int x0;
    char pad4[0x2040 - 0x4];
};

extern struct Entry__c4 D_003ECBF0__c4[] asm("D_003ECBF0");

extern int func_0018CC28__c4(int index) asm("func_0018CC28");
int func_0018CC28__c4(int index)
{
    int state = D_003ECBF0__c4[index].x0;
    if (state == 13) {
        state = 15;
    }
    return state;
}
}
/* ---- merged from src/unsorted/0018CC50.c ---- */
extern "C" {
struct Obj__c5 { char pad[0x510]; int x510; };
extern char D_003EE160__c5[] asm("D_003EE160");
extern void func_0018DB20__c5(void *) asm("func_0018DB20");
extern void func_0018F280__c5(struct Obj__c5 *, int) asm("func_0018F280");
extern void func_0018CC50__c5(struct Obj__c5 *obj, int arg1) asm("func_0018CC50");
void func_0018CC50__c5(struct Obj__c5 *obj, int arg1) {
    func_0018DB20__c5(D_003EE160__c5 + obj->x510 * 0x2040);
    func_0018F280__c5(obj, arg1);
}
}
/* ---- merged from src/unsorted/0018CCA8.c ---- */
extern "C" {


extern void func_0018CCA8__c6(void *obj) asm("func_0018CCA8");
void func_0018CCA8__c6(void *obj)
{
    FinalizeMenuLegacy(obj);
}
}
/* ---- merged from src/unsorted/0018CCC8.c ---- */
extern "C" {
typedef struct Vec__c7 {
    float x, y, z, w;
} Vec__c7 __attribute__((aligned(16)));

typedef struct Item__c7 {
    char pad0[0x4];
    unsigned char visible;
    char pad5[0x10 - 0x5];
    Vec__c7 pos;
} Item__c7;

extern const int D_0028CEE8__c7[14] asm("D_0028CEE8");

/* 128-bit vector copy (libvu0-style inline asm) */
static inline void VecCopy(Vec__c7 *out, Vec__c7 *in)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(in) : "$2");
}
extern Item__c7 *D_003F4780__c7[][14] asm("D_003F4780");
extern int D_004B2828__c7[] asm("D_004B2828");
extern Vec__c7 D_0026F4E0__c7[] asm("D_0026F4E0");
extern Vec__c7 D_0026F520__c7[] asm("D_0026F520");

extern void func_0018CCC8__c7(int player, int choice) asm("func_0018CCC8");
void func_0018CCC8__c7(int player, int choice)
{
    int i;
    int mode;
    int slot;

    mode = D_004B2828__c7[0];
    slot = D_0028CEE8__c7[choice];
    for (i = 0; i < 14; i++) {
        if (i == slot) {
            D_003F4780__c7[player][i]->visible = 1;
            if (mode == 2) {
                VecCopy(&D_003F4780__c7[player][i]->pos, &D_0026F520__c7[player]);
            } else {
                VecCopy(&D_003F4780__c7[player][i]->pos, &D_0026F4E0__c7[player]);
            }
        } else {
            D_003F4780__c7[player][i]->visible = 0;
        }
    }
}
}
/* ---- merged from src/unsorted/0018CD78.c ---- */
extern "C" {
typedef struct Obj__c8 {
    int x0;
    char pad4[0x50C];
    int x510;
} Obj__c8;

extern void func_0018CCC8__c8(int, int) asm("func_0018CCC8");
extern void func_0018DC20__c8(Obj__c8 *) asm("func_0018DC20");

extern void func_0018CD78__c8(Obj__c8 *obj) asm("func_0018CD78");
void func_0018CD78__c8(Obj__c8 *obj)
{
    func_0018CCC8__c8(obj->x510, obj->x0);
    func_0018DC20__c8(obj);
}
}
/* ---- merged from src/unsorted/0018CDB0.c ---- */
extern "C" {
typedef struct Item__c9 { int x0; unsigned char x4; } Item__c9;
typedef struct Obj__c9 { char pad[0x510]; int x510; } Obj__c9;
extern Item__c9 *D_003F4780__c9[][14] asm("D_003F4780");
extern void func_0018CDB0__c9(Obj__c9 *obj) asm("func_0018CDB0");
void func_0018CDB0__c9(Obj__c9 *obj) {
    int index = 0;
    do { D_003F4780__c9[obj->x510][index]->x4 = 0; ++index; } while (index < 14);
}
}
/* ---- merged from src/unsorted/0018CDF8.cpp and 0018CE20.cpp (one class A) ---- */
struct Entry__c11 {
    int x0;
    char pad4[0x2040 - 0x4];
};

class Base {
public:
    char pad0[0x45C];
    int x45C;
    char pad460[0x4A8 - 0x460];
    int x4A8;
    char pad4AC[0x4B0 - 0x4AC];
    int x4B0;
    char pad4B4[0x4F8 - 0x4B4];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
};

class A : public Base {
public:
    char pad4FC[0x510 - 0x4FC];
    int x510;
    int x514;
    void func_0018CDF8();
    void func_0018E468();
    void func_0018CE20();
    void func_0018E5A8();
    void func_0018DBA8();
};

void A::func_0018CDF8()
{
    if (x510 == 0) {
        this->func_0018E468();
    }
}

extern struct Entry__c11 D_003ECBF0__c11[] asm("D_003ECBF0");
typedef struct Global__c11 {
    char pad0[0x64];
    int x64;
} Global__c11;

extern Global__c11 D_003F52D0__c11 asm("D_003F52D0");
extern int D_004B2828__c11[] asm("D_004B2828");

extern "C" {
void func_0018CCC8__c11(int index, int state) asm("func_0018CCC8");
void func_001904B8__c11(void *p, int value) asm("func_001904B8");
void func_0018C7C8__c11(int index) asm("func_0018C7C8");
void func_0018C6E0__c11(void) asm("func_0018C6E0");
void func_0013DA40__c11(int value) asm("func_0013DA40");
int func_00190D60__c11(void *p, int a, int b) asm("func_00190D60");
}

static inline int CheckFlag(Global__c11 *g, int index, int mask)
{
    return func_00190D60__c11(&g->x64, index, mask);
}

void A::func_0018CE20()
{
    int count;
    int i;

    func_0018CCC8__c11(x514, D_003ECBF0__c11[x514].x0);
    this->func_0018E5A8();
    count = D_004B2828__c11[0];
    if (++D_0029CE30 == count && x4B0 != 0) {
        func_001904B8__c11(&D_003F52D0__c11, x4B0);
        for (i = 0; i < count; i++) {
            func_0018C7C8__c11(i);
        }
    }
    func_0018C6E0__c11();
    this->func_0018DBA8();
    if (x510 != 0) {
        func_0013DA40__c11(x510);
    }
    x4A8 = 5;
    if (CheckFlag(&D_003F52D0__c11, 5, x45C) != 0) {
        v7();
    }
}
