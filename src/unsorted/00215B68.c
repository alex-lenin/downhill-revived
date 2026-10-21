/* UNIT 0x00215B68 0x00218720 .sdata=0x29DA8C:0x29DAAC */
#include "vif_transforms.h"
#include "particle_descriptor.h"
#include "matrix_packets.h"
#include "soft_double.h"
#include "scene.h"
#include "include_asm.h"
typedef unsigned int u128 __attribute__((mode(TI)));

int D_0029DA8C=0;
int D_0029DA90=0,D_0029DA94=0;
int D_0029DA98 = 0;
int D_0029DA9C = 1;
int D_0029DAA0 = 0;
float D_0029DAA4 = 0.0f;

typedef struct Mtx { float f[16]; } Mtx __attribute__((aligned(16)));
typedef struct Entry217160 { char pad[0x40]; } Entry217160;
extern Mtx *D_0029E1A0, *D_0029E1A4, *D_0029E1A8, *D_0029E1AC;
extern Entry217160 *D_0029E1B0;
extern int *D_0029E1B4;

/* declared without prototypes: callers pass different pointer types */
extern void *func_0023CF78();
extern int func_0021B7E8();
extern void func_0021B8A8();
extern void func_00216AE0();
extern void func_00216EF0();
extern int func_00217138();
extern Entry217160 *func_00217160();
extern void func_00217100();

extern void func_00263B38(void *);

extern int func_0023D608(void);
extern int func_0022FD90(void);

typedef SceneNode Obj;
typedef struct Node { Obj *x0; struct Node *next; struct Node *prev; } Node;
typedef struct List { int x0; Node *next; Node *prev; int xC; } List;
/* List iteration goes through inlined iterator helpers (by-value iterators passed by
   address, like the inlined C++ list iterator of 00216980.cpp); a plain pointer loop
   allocates the list head and end pointer differently. */
typedef struct ListIter { Node *node; } ListIter;
extern List D_004D0318;
static inline ListIter ListBegin(List *list) { ListIter it; it.node = list->next; return it; }
static inline ListIter ListEnd(List *list) { ListIter it; it.node = (Node *)list; return it; }
static inline int IterNe(const ListIter *a, const ListIter *b) { return a->node != b->node; }
static inline Obj **IterGet(const ListIter *a) { return &a->node->x0; }
static inline void IterNext(ListIter *a) { a->node = a->node->next; }
extern void func_0022F870(int a0, void *a1);
extern void func_00216F48(float f);

/* ---- merged from src/unsorted/00215B68.c ---- */
typedef unsigned int u128__c1 __attribute__((mode(TI)));
typedef struct Packet { int x0,x4; char *cur; int xC; char *x10; int x14; } Packet;
extern void (*D_0029DA94__c1)(int) asm("D_0029DA94");
extern int D_0029E1A0__c1 asm("D_0029E1A0"),D_0029E1A4__c1 asm("D_0029E1A4"),D_0029E1A8__c1 asm("D_0029E1A8"),D_0029E1AC__c1 asm("D_0029E1AC"),D_0029E1B0__c1 asm("D_0029E1B0"),D_0029E1B4__c1 asm("D_0029E1B4");
extern int D_0029E1B8,D_0029E1C0,D_0029E1C8,D_0029E1CC,D_0029E1E4;
extern Packet *D_0029E1DC,*D_0029E1E0;
extern char *D_0029E1E8__c1 asm("D_0029E1E8"),*D_0029E1EC__c1 asm("D_0029E1EC");
extern int D_0029E1F0,D_0029E1F8__c1 asm("D_0029E1F8"),D_0029E1FC;
extern char D_00269C50[],D_004F3480[],D_00528360[],D_00528980[],D_00580180[],D_005D7980[],D_0061E190[],D_0061FBC4[],D_006201D0[];
extern char D_0061D980[][0x810],D_0061F9C0[][0x204];
extern int D_00620290[];
extern void func_00216980(int) asm("func_00216980"),func_00217DA8(void) asm("func_00217DA8");
extern Packet D_00528900;
extern int func_0025B000(int);
extern void func_0025B068(int) asm("func_0025B068"),func_00216DD0(void) asm("func_00216DD0"),func_0022F570(Packet *,int,int) asm("func_0022F570"),func_0022F588(Packet *,int,void *) asm("func_0022F588"),func_0022F600(int) asm("func_0022F600"),func_0020EE30(void *) asm("func_0020EE30"),func_00230058(int) asm("func_00230058"),func_00228040(int *,int *,int) asm("func_00228040"),func_00216D28__c1(Packet *,int) asm("func_00216D28"),func_0021CB98(int,int,void *,int,float,float) asm("func_0021CB98"),func_0021D950(void *,int) asm("func_0021D950"),func_00216690(int) asm("func_00216690"),func_002167C0(int) asm("func_002167C0"),func_00216718(int) asm("func_00216718"),func_00217C00(int,int) asm("func_00217C00"),func_0021B500(void *) asm("func_0021B500"),func_0022F5E0(Packet *,Packet *) asm("func_0022F5E0"),func_00228078(int,int) asm("func_00228078"),func_00216BB8__c1(Packet *,int,int,int,int) asm("func_00216BB8"),func_0025A960(int) asm("func_0025A960"),func_0021E9A8(void) asm("func_0021E9A8");
extern float func_0020EB30(int) asm("func_0020EB30"),func_0020EE10(int) asm("func_0020EE10");
extern int func_0023CF78__c1(int) asm("func_0023CF78"),func_0021E3C8(void) asm("func_0021E3C8"),func_0022FD90__c1(void) asm("func_0022FD90"),func_00242FA0(void) asm("func_00242FA0"),func_00217D88(void) asm("func_00217D88");
extern char *func_0021E0B8(Packet *) asm("func_0021E0B8"),*func_0021E130(Packet *) asm("func_0021E130"),*func_0022E9A0(int,int *) asm("func_0022E9A0");
#define ADDR(x) (((int)(x)&0xfffffff)|0x30000000)
typedef union DmaTag { u128__c1 q; int w[4]; } DmaTag;
#define CLEAR(p) (((DmaTag *)(p))->q=0)
#define WORD(p,n) (((DmaTag *)(p))->w[n])
extern void func_00215B68(int start,int count,int arg2,int arg3) asm("func_00215B68");
void func_00215B68(int start,int count,int arg2,int arg3) {
 Packet *packet1,*packet2,*packet3,*packet5,*packet6;
 int chain; int i; int a,b,c; char *tail,*next; Packet *packet;
 D_0029E1DC=(Packet *)0x70000300;
 D_0029E1E0=(Packet *)0x70000290;
 *(int *)0x700002ac=1;
 func_0025B000(1);
 *(int *)0x700002b4=0; *(int *)0x700002b8=0;
 func_00216DD0();
 func_0022F570(D_0029E1DC,ADDR(D_00528980),ADDR(D_00580180));
 func_0022F570(D_0029E1E0,ADDR(D_005D7980),ADDR(D_005D7980));
 func_0022F588(D_0029E1E0,ADDR(D_0061D980[0]),D_0061F9C0[0]);
 func_0022F600(0x70000000);
 D_0029E1A0__c1=0x70000400; D_0029E1A4__c1=0x70000900;
 D_0029E1A8__c1=0x70000e00; D_0029E1AC__c1=0x70001300;
 D_0029E1B0__c1=0x70001d00; D_0029E1B4__c1=0x70000350;
 *(int *)0x7000006c=0; *(int *)0x70000068=0;
 for(i=start;i<count+start;i++) {
  if(D_0029DA94__c1) D_0029DA94__c1(i);
  D_0029E1B8=0;
  D_0029E1EC__c1=D_006201D0+i*32;
  D_0029E1F0=D_00620290[i];
  D_0029E1C0=0;
  if(count>=3) func_0020EE30(D_004F3480);
  *(float *)0x700001d0=func_0020EB30(i);
  func_00230058(i); func_00228040(&D_0029E1F8__c1,&D_0029E1FC,i);
  func_00216D28__c1(D_0029E1DC,i);
  {
   Packet *packet=D_0029E1DC;
   CLEAR(packet->cur);
   WORD(packet->cur,0)=0x70000000;
  }
  D_0029E1E8__c1=D_00528360+i*240;
  a=func_0023CF78__c1(i); b=func_0021E3C8(); c=func_0022FD90__c1();
  func_0021CB98(a,b,D_0029E1E8__c1,c,func_0020EE10(i),1.0f);
  *(int *)0x700000e0=func_00242FA0();
  __asm__ volatile("sync\n\tsync.p" ::: "memory");
  func_0021D950(D_0029E1E8__c1+32,i);
  func_00216690(i); func_002167C0(i); func_00216718(i); func_00217C00(4,i);
  chain=0; D_0029E1E4=0;
  D_0029E1CC=*(int *)0x70000068;
  D_0029E1C8=*(int *)0x7000006c;
  if((D_0029E1C8 || D_0029E1CC) && (D_0029E1E4=1,D_0029E1CC)) {
   func_0021B500(D_0029E1E0->x10+0x800);
   func_0022F5E0(D_0029E1E0,D_0029E1DC);
   tail=func_0021E0B8(D_0029E1E0);
   if(!tail) tail=D_0029E1E0->x10;
  } else tail=func_0021E130(D_0029E1DC);
  func_00228078(D_0029E1FC,i);
  if(i==5) { next=0; } else { next=func_0022E9A0(i,&chain); }
  if(next) {
   D_0029E1E4=1;
   if(tail) {
    CLEAR(tail); WORD(tail,0)=0x20000000; WORD(tail,1)=chain&0xfffffff;
   } else {
    packet1=D_0029E1DC; CLEAR(packet1->cur); WORD(packet1->cur,0)=0x20000000;
    WORD(packet1->cur,1)=chain&0xfffffff; packet1->cur+=16;
   }
   CLEAR(next); packet2=D_0029E1DC; WORD(next,0)=0x20000000; WORD(next,1)=(int)packet2->cur&0xfffffff;
   CLEAR(packet2->cur); WORD(packet2->cur,0)=0x70000000;
  } else if(tail) {
   D_0029E1E4=1;
   CLEAR(tail); packet3=D_0029E1DC; WORD(tail,0)=0x20000000; WORD(tail,1)=(int)packet3->cur&0xfffffff;
   CLEAR(packet3->cur); WORD(packet3->cur,0)=0x70000000;
  }
  if(i+1<count) {
   if(next) {
    {
     Packet *packet4=D_0029E1DC;
     CLEAR(packet4->cur); WORD(packet4->cur,0)=0x50000000;
    WORD(packet4->cur,1)=(int)D_00269C50&0xfffffff;
    WORD(packet4->cur,2)=0x11000000; packet4->cur+=16;
    }
   }
   packet5=D_0029E1DC; CLEAR(packet5->cur); WORD(packet5->cur,0)=0x70000000;
   func_00216BB8__c1(packet5,i+1,arg2,arg3,1);
   packet6=D_0029E1DC; CLEAR(packet6->cur); WORD(packet6->cur,0)=0x70000000;
   func_0022F588(D_0029E1E0,ADDR(D_0061E190+i*0x810),D_0061FBC4+i*0x204);
  }
 }
 packet=D_0029E1DC;
 D_00528900.x0=packet->x0; D_00528900.x4=packet->x4; D_00528900.cur=packet->cur;
 D_00528900.xC=packet->xC; D_00528900.x10=packet->x10; D_00528900.x14=packet->x14;
 func_0025A960(0);
 if(func_00217D88()==0) func_0021E9A8(); else func_0025B068(1);
}

extern void func_002161B8(int arg0, int arg1, int arg2, int arg3);
void func_002161B8(int arg0, int arg1, int arg2, int arg3)
{
    int i;
    int next;
    int t68;
    int t6C;
    int a, b, c;
    Packet *pkt;

    D_0029E1DC = (Packet *)0x70000300;
    D_0029E1E0 = (Packet *)0x70000290;
    *(int *)0x700002AC = 1;
    func_0025B000(1);
    *(int *)0x700002B4 = 0;
    *(int *)0x700002B8 = 0;
    func_00216DD0();
    func_0022F570(D_0029E1DC, ADDR(D_00528980), ADDR(D_00580180));
    func_0022F570(D_0029E1E0, ADDR(D_005D7980), ADDR(D_005D7980));
    func_0022F588(D_0029E1E0, ADDR(D_0061D980[0]), D_0061F9C0[0]);
    func_0022F600(0x70000000);
    func_00216BB8__c1(D_0029E1DC, arg0, arg2, arg3, 0);

    D_0029E1A0__c1 = 0x70000400;
    D_0029E1A4__c1 = 0x70000900;
    D_0029E1A8__c1 = 0x70000E00;
    D_0029E1AC__c1 = 0x70001300;
    D_0029E1B0__c1 = 0x70001D00;
    D_0029E1B4__c1 = 0x70000350;
    *(int *)0x7000006C = 0;
    *(int *)0x70000068 = 0;

    for (i = arg0; i < arg1 + arg0; i = next) {
        if (D_0029DA94__c1 != 0) {
            D_0029DA94__c1(i);
        }

        D_0029E1EC__c1 = D_006201D0 + i * 0x20;
        D_0029E1F0 = D_00620290[i];
        D_0029E1B8 = 0;
        D_0029E1C0 = 0;
        if (arg1 >= 3) {
            func_0020EE30(D_004F3480);
        }

        *(float *)0x700001D0 = func_0020EB30(i);
        func_00230058(i);
        func_00228040(&D_0029E1F8__c1, &D_0029E1FC, i);
        func_00216D28__c1(D_0029E1DC, i);

        {
            Packet *pkt = D_0029E1DC;
            CLEAR(pkt->cur);
            WORD(pkt->cur, 0) = 0x70000000;
        }
        D_0029E1E8__c1 = D_00528360 + i * 0xF0;
        a = func_0023CF78__c1(i);
        b = func_0021E3C8();
        c = func_0022FD90__c1();
        func_0021CB98(a, b, D_0029E1E8__c1, c, func_0020EE10(i), 1.0f);

        *(int *)0x700000E0 = func_00242FA0();
        __asm__ volatile ("sync\n\tsync.p");
        func_00216980(i);

        t68 = *(int *)0x70000068;
        t6C = *(int *)0x7000006C;
        D_0029E1CC = t68;
        D_0029E1C8 = t6C;
        D_0029E1E4 = 0;
        if (t6C || t68) {
            D_0029E1E4 = 1;
            if (t68) {
                func_0021B500(D_0029E1E0->x10 + 0x800);
                func_0022F5E0(D_0029E1E0, D_0029E1DC);
                {
                    char *tail = D_0029E1E0->x10;
                    Packet *pkt = D_0029E1DC;
                    CLEAR(tail);
                    WORD(tail, 0) = 0x20000000;
                    WORD(tail, 1) = (int)pkt->cur & 0x0FFFFFFF;
                    CLEAR(pkt->cur);
                    WORD(pkt->cur, 0) = 0x70000000;
                }
            }
        }

        next = i + 1;
        if (next < arg1) {
            Packet *pkt = D_0029E1DC;
            CLEAR(pkt->cur);
            WORD(pkt->cur, 0) = 0x70000000;
            func_00216BB8__c1(pkt, next, arg2, arg3, 0);
            {
                Packet *pkt2 = D_0029E1DC;
                CLEAR(pkt2->cur);
                WORD(pkt2->cur, 0) = 0x70000000;
            }
            func_0022F588(D_0029E1E0, ADDR(D_0061D980[i + 1]), D_0061F9C0[i + 1]);
        }
    }

    pkt = D_0029E1DC;
    D_00528900.x0 = pkt->x0;
    D_00528900.x4 = pkt->x4;
    D_00528900.cur = pkt->cur;
    D_00528900.xC = pkt->xC;
    D_00528900.x10 = pkt->x10;
    D_00528900.x14 = pkt->x14;
    func_0025A960(0);
    func_00217DA8();
    func_0021E9A8();
}
/* ---- merged from src/unsorted/00216690.c ---- */
typedef struct { int x0; int *x4; } State;
extern State *D_0029E1E8__c2 asm("D_0029E1E8");
extern int D_0029E200;
extern int func_0023D608(void);

extern void func_0021B8A8__c2(int *,int *,int) asm("func_0021B8A8");
extern void func_00216690(int arg);
void func_00216690(int arg) {
 int buffer[4];
 if(D_0029E200 && func_0023D608()<3) {
  int *p=D_0029E1E8__c2->x4;
  if(p && *p) {
   PrepareVifTransformsDiscard(p,D_0029E1E8__c2,buffer);
   *(int **)0x70000038=D_0029E1E8__c2->x4;
   func_0021B8A8__c2(D_0029E1E8__c2->x4,buffer,arg);
  }
 }
}
/* ---- merged from src/unsorted/00216718.c ---- */
extern int D_0029E1A0__c3 asm("D_0029E1A0"), D_0029E1A4__c3 asm("D_0029E1A4"), D_0029E1A8__c3 asm("D_0029E1A8"), D_0029E1AC__c3 asm("D_0029E1AC");
extern char *D_0029E1E8__c3 asm("D_0029E1E8");


extern void *func_0022FD90__c3(void) asm("func_0022FD90");
extern int func_0021B8A8__c3(void *, void *, int) asm("func_0021B8A8");
extern void func_00216718(int arg);
void func_00216718(int arg) {
    if (*(int *)D_0029E1E8__c3) {
        *(int *)0x70000038 = 0;
        IdentityMatrixAddressLegacy(D_0029E1A0__c3);
        CopyMatrixAddressLegacy(D_0029E1A4__c3, D_0029E1E8__c3 + 0x70);
        CopyMatrixAddressLegacy(D_0029E1A8__c3, D_0029E1E8__c3 + 0xb0);
        CopyMatrixAddressLegacy(D_0029E1AC__c3, func_0022FD90__c3());
        func_0021B8A8__c3(D_0029E1E8__c3, D_0029E1E8__c3 + 0x20, arg);
    }
}
void func_002167C0(int idx)
{
    Obj *o;
    ListIter it;
    ListIter end;
    int stack[4];

    if (func_0023D608() >= 3) {
        func_0022F870(0, (void *)0x70000000);
    }
    if (func_0023D608() == 2) {
        func_00216F48(2.0f);
    }
    for (it = ListBegin(&D_004D0318); end = ListEnd(&D_004D0318), IterNe(&it, &end); IterNext(&it)) {
        int mask = ~(1 << idx);
        float one = 1.0f;
        o = *IterGet(&it);
        if (o->x4 == 0) continue;
        if (o->resourceId == 0) continue;
        if (((o->x6 >> idx) & 1) == 0) continue;
        o->x5 &= mask;
        if (func_0021B7E8(*IterGet(&it), stack) != 0) {
            if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 41)) { *(float *)0x700003A0 = one / (*IterGet(&it))->x30.x; }
            if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 44)) { D_0029DAA4 = one; }
            else if (D_0029DAA0) { D_0029DAA4 = one; }
            func_0021B8A8(*IterGet(&it), stack, idx);
            if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 44)) { D_0029DAA4 = 0.0f; }
            else if (D_0029DAA0) { D_0029DAA4 = 0.0f; }
        }
    }
    if (func_0023D608() == 2) {
        func_00216F48(1.0f);
    }
}

extern List D_004D0338;

void func_00216980(int idx)
{
    Obj *o;
    ListIter it;
    ListIter end;
    int stack[4];

    for (it = ListBegin(&D_004D0338); end = ListEnd(&D_004D0338), IterNe(&it, &end); IterNext(&it)) {
        int mask = ~(1 << idx);
        float one = 1.0f;
        o = *IterGet(&it);
        if (o->x4 == 0) continue;
        if (o->resourceId == 0) continue;
        if (((o->x6 >> idx) & 1) == 0) continue;
        o->x5 &= mask;
        if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 41)) { *(float *)0x700003A0 = one / (*IterGet(&it))->x30.x; }
        if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 46)) { func_00216AE0(*IterGet(&it), idx); }
        if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 44)) { *(volatile float *)&D_0029DAA4 = one; }
        if (func_0021B7E8(*IterGet(&it), stack) != 0) { func_0021B8A8(*IterGet(&it), stack, idx); }
        if ((*IterGet(&it))->flags & ((SceneNodeFlags)0x8000 << 44)) { D_0029DAA4 = 0.0f; }
    }
}


typedef union Vector216AE0 {
    u128 q;
    float v[4];
} Vector216AE0;

typedef struct Matrix216AE0 {
    u128 r[4];
} Matrix216AE0;

typedef struct Obj216AE0 {
    char pad0[0x10];
    Vector216AE0 x10;
    char pad20[0x20];
    Matrix216AE0 x40;
} Obj216AE0;

/* quadword copies as the game's vector/matrix copy macros */
#define COPY_VECTOR_AE0(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX_AE0(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
        : "=m"((dst).r[0]), "=m"((dst).r[1]), "=m"((dst).r[2]), "=m"((dst).r[3]) \
        : "r"(src) : "$8", "$9", "$10", "$11")

extern Matrix216AE0 *func_0023CF58(void *src);

static inline void ApplyMatrix(Vector216AE0 *out, Matrix216AE0 *m, Vector216AE0 *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(m), "r"(in) : "memory");
}

void func_00216AE0(Obj216AE0 *obj, void *src)
{
    Vector216AE0 out;
    Vector216AE0 prev;
    Matrix216AE0 *m;

    m = &obj->x40;
    COPY_MATRIX_AE0(*m, func_0023CF58(src));
    COPY_VECTOR_AE0(prev, &obj->x10);
    COPY_VECTOR_AE0(obj->x10, func_0023CF78(src));
    ApplyMatrix(&out, m, &prev);
    obj->x10.v[0] += out.v[0];
    obj->x10.v[1] += out.v[1];
    obj->x10.v[2] += out.v[2];
}


typedef struct Packet216BB8 {
    char pad0[0x8];
    char *cur;
} Packet216BB8;

extern char *func_0023D5F0(void *);
extern int func_0021F3F8(void);
extern void func_0021F210(void *, char *, int, int, int);

void func_00216BB8(Packet216BB8 *pkt, void *tex, int alt, int arg3, int big)
{
    u128 *dst;
    u128 *src;
    char *data;
    int i;

    dst = (u128 *)pkt->cur;
    if (big) {
        ((unsigned int *)dst)[0] = 0x10000017;
    } else {
        ((unsigned int *)dst)[0] = 0x10000011;
    }
    ((unsigned int *)dst)[1] = 0;
    ((unsigned int *)dst)[2] = 0x11000000;
    if (big) {
        ((unsigned int *)dst)[3] = 0x50000017;
    } else {
        ((unsigned int *)dst)[3] = 0x50000011;
    }
    dst++;
    data = func_0023D5F0(tex);
    if (func_0021F3F8()) {
        func_0021F210(tex, data, alt == 0, arg3, func_0023D608());
    } else {
        func_0021F210(tex, data, alt == 0, 0, func_0023D608());
    }
    src = (u128 *)(alt == 0 ? data + 0x80 : data + 0x1F0);
    for (i = 0; i < 23; i++) {
        *dst = *src;
        if (i == 0) {
            if (big) {
                *(unsigned short *)dst = 0x8016;
            } else {
                *(unsigned short *)dst = 0x8010;
            }
        }
        dst++;
        src++;
    }
    if (big) {
        pkt->cur += 0x180;
    } else {
        pkt->cur += 0x120;
    }
}

struct Packet216D28 { int x0, x4, x8, xc; long x10, x18, x20, x28; };
struct Obj216D28 { char pad[8]; struct Packet216D28 *x8; };
extern int func_0023DF58(int);
extern long func_0020EC30(int);
void func_00216D28(struct Obj216D28 *obj, int arg) {
    struct Packet216D28 *p = obj->x8;
    int value;
    p->x0 = 0x50000002;
    value = func_0023DF58(arg);
    p->xc = 0x50000002;
    p->x10 = ((SceneNodeFlags)0x8000 << 45) | 0x8001;
    p->x4 = value & 0x0fffffff;
    p->x8 = 0;
    p->x18 = 14;
    p->x20 = func_0020EC30(arg);
    p->x28 = 0x3d;
    obj->x8 = (struct Packet216D28 *)((char *)obj->x8 + 0x30);
}

extern char D_006202B0[];

void func_00216DD0(void)
{
    *(volatile unsigned int *)0x10009020 = 0;
    *(volatile unsigned int *)0x10009030 = (unsigned int)D_006202B0 & 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000E010 = 2;
    *(volatile unsigned int *)0x10009000 = 0x145;
}

typedef VifTransformSource Obj216E28;

typedef VifTransformState State216E28;



extern void func_00223950(void *, float *, float *);

int func_00216E28(Obj216E28 *obj, State216E28 *state, Vector4 *out) {
    Vector4 pos;
    float *matrix;

    pos.x = state->x20 + obj->x10;
    pos.y = state->x24 + obj->x14;
    pos.z = state->x28 + obj->x18;
#if !defined(__mips__)
    /* EE only initializes xyz; native quad loads must not read outside a Vector3. */
    pos.w = 0.0f;
#endif
    matrix = obj->matrix;
    func_00221888((const Matrix4 *)matrix, &pos, out);
    func_00223A38((Matrix4 *)D_0029E1A0, (const Matrix4 *)matrix);
    func_00223950(D_0029E1A4, matrix, state->x70);
    func_00223950(D_0029E1A8, matrix, state->xB0);
    func_00216EF0(D_0029E1AC, matrix, func_0022FD90());
    return 1;
}

void func_00216EF0(float *out, const float *a, const float *b) {
    __asm__ volatile (
        "lqc2       $vf14, 0x0(%1)
	"
        "lqc2       $vf15, 0x10(%1)
	"
        "lqc2       $vf16, 0x20(%1)
	"
        "lqc2       $vf11, 0x0(%2)
	"
        "lqc2       $vf12, 0x10(%2)
	"
        "lqc2       $vf13, 0x20(%2)
	"
        "vmulax.xyzw ACC, $vf11, $vf14x
	"
        "vmadday.xyzw ACC, $vf12, $vf14y
	"
        "vmaddz.xyzw $vf17, $vf13, $vf14z
	"
        "vmulax.xyzw ACC, $vf11, $vf15x
	"
        "vmadday.xyzw ACC, $vf12, $vf15y
	"
        "vmaddz.xyzw $vf18, $vf13, $vf15z
	"
        "vmulax.xyzw ACC, $vf11, $vf16x
	"
        "vmadday.xyzw ACC, $vf12, $vf16y
	"
        "vmaddz.xyzw $vf19, $vf13, $vf16z
	"
        "sqc2       $vf17, 0x0(%0)
	"
        "sqc2       $vf18, 0x10(%0)
	"
        "sqc2       $vf19, 0x20(%0)
	"
        "sqc2       $vf0, 0x30(%0)
	"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}

extern float D_0029DAA8;

void func_00216F48(float v) {
    D_0029DAA8 = v;
}


INCLUDE_ASM("asm/nonmatchings/text", func_00216F50);

typedef struct Node216F58 {
    char pad0[8];
    short parent;
    char padA[6];
    float matrix[4][4];
} Node216F58;

typedef struct Model216F58 {
    char pad0[0x14];
    float **matrices;
} Model216F58;


void func_00216F58(short *node, short *depth, Model216F58 *model, float *offset)
{
    float local[4][4] __attribute__((aligned(16)));
    float world[4][4] __attribute__((aligned(16)));
    Node216F58 *info;
    float *parent;

    info = (Node216F58 *)func_00217138(*node);
    func_00221F20(local);
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(local[3]) : "r"(local[3]), "r"(offset));
    parent = (float *)func_00217160(*depth);
    func_00221930(world, local, parent);
    (*depth)++;
    func_00217100((float *)world, *depth);
    if (info->parent >= 0) {
        func_00221930(model->matrices[info->parent], info->matrix, world);
    }
    (*node)--;
}

typedef struct { char p[8]; short x8; char q[6]; float matrix[16]; } Obj217038;
typedef struct { char p[0x14]; void **x14; } Parent217038;
void func_00217038(short *a,short *b,Parent217038 *p,int o) { float matrix[16]; void *v; Obj217038 *n=(Obj217038 *)func_00217138(*a); v=func_00217160(*b); func_00221930(matrix,(void*)(o+0x10),v); ++*b; func_00217100(matrix,*b); if(n->x8>=0) func_00221930(p->x14[n->x8],n->matrix,matrix); --*a; }


/* Retail pads func_00217038 (ends 0x2170F4) with zeros up to 0x217100: 8 bytes
   more than the function alignment gives, so they are emitted here. */
static int pad_002170F8[2] __attribute__((section(".text"))) = {0, 0};

typedef int Vector217100 __attribute__((mode(TI)));
typedef struct Matrix217100 { Vector217100 x0, x10, x20, x30; } Matrix217100;
void func_00217100(const Matrix217100 *src, int index)
{
    Matrix217100 *dst;
    int base;
    int off;
    base = (int)D_0029E1B0;
    off = index << 6;
    off += base;
    dst = (Matrix217100 *)off;
    /* 64-byte matrix copy (as sceVu0CopyMatrix) */
    __asm__ volatile (
        "lq $8, 0x0(%1)\n\t"
        "lq $9, 0x10(%1)\n\t"
        "lq $10, 0x20(%1)\n\t"
        "lq $11, 0x30(%1)\n\t"
        "sq $8, 0x0(%0)\n\t"
        "sq $9, 0x10(%0)\n\t"
        "sq $10, 0x20(%0)\n\t"
        "sq $11, 0x30(%0)"
        : : "r"(dst), "r"(src)
        : "$8", "$9", "$10", "$11");
}

int func_00217138(unsigned int index) {
    if (index >= 2U) return 0;
    return D_0029E1B4[index];
}



Entry217160 *func_00217160(unsigned int i)
{
    if (i >= 0x40) {
        return 0;
    }
    return &D_0029E1B0[i];
}

/* Sony libvu0 matrix copy uses four quadword loads followed by stores. */
#define COPY_MATRIX_180(dst,src) __asm__ volatile ("lq $8,0(%1)
	lq $9,16(%1)
	lq $10,32(%1)
	lq $11,48(%1)
	sq $8,0(%0)
	sq $9,16(%0)
	sq $10,32(%0)
	sq $11,48(%0)" : : "r"(dst), "r"(src) : "$8","$9","$10","$11","memory")
struct Obj217180 { int x0,x4,x8,xC; float x10,x14,x18,x1C,x20,x24,x28; };
extern void func_00263B28(float *);

extern void func_00221CB8(Mtx *);
extern void func_00221C20(Mtx *,Mtx *,float);
void func_00217180(struct Obj217180 *obj,int index) {
 Mtx matrix;
 float pos[4];
 int next;
 if(obj->x8) {
 func_00221F20(&matrix);
 matrix.f[0]=obj->x20; matrix.f[5]=obj->x24; matrix.f[10]=obj->x28;
 func_00263B38(pos);
 pos[0]=(pos[0]+obj->x10)/obj->x20;
 pos[1]=(pos[1]+obj->x14)/obj->x24;
 pos[2]=(pos[2]+obj->x18)/obj->x28;
 func_00263B28(pos);
 next=index+1;
 func_00221A60(D_0029E1A0+next,&matrix,D_0029E1A0+index);
 if(obj->xC) {
 COPY_MATRIX_180((Mtx *)((next<<6)+(unsigned int)D_0029E1A4),(Mtx *)((index<<6)+(unsigned int)D_0029E1A4));
 COPY_MATRIX_180((Mtx *)((next<<6)+(unsigned int)D_0029E1A8),(Mtx *)((index<<6)+(unsigned int)D_0029E1A8));
 COPY_MATRIX_180((Mtx *)((next<<6)+(unsigned int)D_0029E1AC),(Mtx *)((index<<6)+(unsigned int)D_0029E1AC));
 return;
 }
 func_00221A60(D_0029E1A4+next,&matrix,D_0029E1A4+index);
 func_00221A60(D_0029E1A8+next,&matrix,D_0029E1A8+index);
 matrix.f[0]=1.0f/obj->x20; matrix.f[5]=1.0f/obj->x24; matrix.f[10]=1.0f/obj->x28;
 func_00221A60(D_0029E1AC+next,&matrix,D_0029E1AC+index);
 func_00221CB8(D_0029E1A4+next);
 func_00221CB8(D_0029E1A8+next);
 func_00221CB8(D_0029E1AC+next);
 func_00221C20(D_0029E1AC+next,D_0029E1AC+next,32.0f);
 }
}


#ifdef NONMATCHING
/* Non-matching: obj/index saved registers swapped (s1/s2), x40/x44 loads swapped, x20 kept in f2 by a mov.s instead of loading it there; best diff 54/241 words */
/* Animated scale/translate node (sibling of struct Obj217180): x10 = weight channel,
   x12..x16 = scale channels, x18..x1C = translation channels (-1/negative = fixed). */
struct Obj2173C0 {
    int x0;
    int x4;                         /* enabled */
    char pad8[8];
    short x10, x12, x14, x16, x18, x1A, x1C, x1E;
    float x20, x24, x28, x2C;       /* scale */
    float x30, x34, x38, x3C;       /* translation */
    float x40, x44, x48, x4C;
    float x50, x54, x58;
};
struct Anim2173C0 { int x0; float *x4; /* channel values */ };

/* Returns 0 when the node is disabled or its weight is 0, else pushes the scaled
   matrices for level index+1 (the four D_0029E1A0..AC stacks) and returns 1. */
int func_002173C0(struct Obj2173C0 *obj, int index, struct Anim2173C0 *anim) {
    int nextoff;
    int next;
    Mtx matrix;
    float pos[4];
    float **values;
    float weight;
    float scale;

    values = anim ? &anim->x4 : 0;
    if (obj->x10 == -1) {
        weight = 1.0f;
    } else {
        weight = (*values)[obj->x10];
    }

    if (weight == 0.0f) return 0;
    if (values == 0) return 0;
    if (obj->x4 != 0) {
        if (obj->x12 >= 0) obj->x20 = (*values)[obj->x12];
        if (obj->x14 >= 0) obj->x24 = (*values)[obj->x14];
        if (obj->x16 >= 0) obj->x28 = (*values)[obj->x16];
        if (obj->x18 >= 0) obj->x30 = (*values)[obj->x18];
        if (obj->x1A >= 0) obj->x34 = (*values)[obj->x1A];
        if (obj->x1C >= 0) obj->x38 = (*values)[obj->x1C];

        next = index + 1;
        func_00221F20(&matrix);
        index <<= 6;                /* byte offset of matrix [index] */
        matrix.f[0] = obj->x20;
        matrix.f[5] = obj->x24;
        matrix.f[10] = obj->x28;
        nextoff = next << 6;
        func_00263B38(pos);
        pos[0] = ((pos[0] + (obj->x30 + obj->x50)) / obj->x20) + obj->x40;
        pos[1] = ((pos[1] + (obj->x34 + obj->x54)) / obj->x24) + obj->x44;
        pos[2] = ((pos[2] + (obj->x38 + obj->x58)) / obj->x28) + obj->x48;
        func_00263B28(pos);
        func_00221A60((Mtx *)((unsigned int)D_0029E1A0 + nextoff), &matrix, (Mtx *)((unsigned int)D_0029E1A0 + index));

        /* uniform scale: the other three stacks are copied unchanged */
        scale = obj->x20;
        weight = obj->x20;
        weight -= obj->x24;
        weight = __builtin_fabsf(weight);
        if (weight < 0.001f) {
            weight = __builtin_fabsf(scale - obj->x28);
            if (weight < 0.001f) {
                COPY_MATRIX_180((Mtx *)(nextoff + (unsigned int)D_0029E1A4), (Mtx *)(index + (unsigned int)D_0029E1A4));
                COPY_MATRIX_180((Mtx *)(nextoff + (unsigned int)D_0029E1A8), (Mtx *)(index + (unsigned int)D_0029E1A8));
                COPY_MATRIX_180((Mtx *)(nextoff + (unsigned int)D_0029E1AC), (Mtx *)(index + (unsigned int)D_0029E1AC));
                return 1;
            }
        }
        func_00221A60(D_0029E1A4 + next, &matrix, (Mtx *)((unsigned int)D_0029E1A4 + index));
        func_00221A60(D_0029E1A8 + next, &matrix, (Mtx *)((unsigned int)D_0029E1A8 + index));
        matrix.f[0] = 1.0f / obj->x20;
        matrix.f[5] = 1.0f / obj->x24;
        matrix.f[10] = 1.0f / obj->x28;
        func_00221A60(D_0029E1AC + next, &matrix, (Mtx *)((unsigned int)D_0029E1AC + index));
        func_00221CB8(D_0029E1A4 + next);
        func_00221CB8(D_0029E1A8 + next);
        func_00221CB8(D_0029E1AC + next);
        func_00221C20(D_0029E1AC + next, D_0029E1AC + next, 32.0f);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_002173C0);
#endif


typedef struct Vec217788 { float x, y, z, w; } Vec217788 __attribute__((aligned(16)));

struct Light217788 {
    union {
        long flags;
        struct { long low:7; unsigned long index:32; long high:25; } bits;
    } u;
    float x8;
    char padC[4];
    Vec217788 x10;
    Vec217788 x20;
    float x30, x34, x38;
    float x3C;
};

struct Contribution217788 {
    Vec217788 x0;
    int x10;
    float x14, x18, x1C, x20, x24, x28;
    short x2C, x2E;
};

extern struct Contribution217788 *D_0029E1F8;
extern int D_0029E1FC;
/* soft-float double helpers (float->double, compare, subtract, double->float) */





#define VADD_217788(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" \
                      : "=m"(dst) : "r"(a), "r"(b))

#define APPLY_MATRIX_217788(dst, v, m) \
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 16(%2)\n\tlqc2 $vf14, 32(%2)\n\t" \
                      "vmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\t" \
                      "sqc2 $vf11, %0" : "=m"(dst) : "r"(v), "r"(m))

/* out = v / |v|, returns |v| */
static inline float Normalize217788(Vec217788 *out, Vec217788 *v) {
    float x = v->x, y = v->y, z = v->z, len, inv;
    int one;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(one) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline float Dot217788(float *a, float *b) {
    float ax = a[0], bx = b[0], ay = a[1], by = b[1], az = a[2], bz = b[2], result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(result) : "f"(ax), "f"(bx), "f"(ay), "f"(by), "f"(az), "f"(bz));
    return result;
}

void func_00217788(struct Light217788 *light, void *obj, int index) {
    Vec217788 position, delta, direction, axis;
    Vec217788 *other;
    float length, dot, weight, factor, scale, angle, distance, axisDot;
    float clamped, contribution;
    long flags;

    if ((light->u.flags & 0x300000000UL) == 0x300000000UL && D_0029E1FC < 74 && D_0029E1F8) {
        func_00263B38(&position);
        VADD_217788(position, &position, &light->x10);
        APPLY_MATRIX_217788(delta, &position, (Vec217788 *)(index * 64 + (int)D_0029E1A0));
        other = func_0023CF78(obj);
        VADD_217788(D_0029E1F8->x0, &delta, other);
        distance = Normalize217788(&direction, &delta);
        if (light->u.flags & 0x8000000000UL) length = 100.0f;
        else length = distance;
        other = (Vec217788 *)func_0023CF58(obj);
        dot = Dot217788((float *)(other + 1), (float *)&direction);
        if (dot > 0.0f) {
            __asm__ volatile ("sqrt.s %0, %1" : "=&f"(weight) : "f"(light->x8));
            weight /= length;
            D_0029E1F8->x14 = weight;
            D_0029E1F8->x18 = weight * dot;
            if (weight > 0.00390625f) {
                flags = light->u.flags;
                if (flags & 0x800000000UL) {
                    APPLY_MATRIX_217788(axis, &light->x20, (Vec217788 *)(index * 64 + (int)D_0029E1A0));
                    axisDot = Dot217788((float *)&direction, (float *)&axis);
                    scale = 1.0f / light->x3C;
                    angle = -axisDot;
                    if (flags & 0x1000000000UL) {
                        /* fabs() done in double precision */
                        double value = func_001FD300(angle);
                        if (func_001FCDA0(value, 0.0) < 0) value = func_001FC810(0.0, value);
                        angle = func_001FD010(value);
                    }
                    /* The do/while(0) (a macro-style block) is needed: its loop notes
                       double the allocation weight of the values inside, which gives
                       retail's register choice. */
                    do {
                        factor = angle * scale - scale + 1.0f;
                        contribution = D_0029E1F8->x18;
                        __asm__ volatile ("max.s %0, %1, %2" : "=f"(clamped) : "0"(0.0f), "f"(factor));
                    } while (0);
                    D_0029E1F8->x18 = contribution * clamped;
                }
                D_0029E1F8->x10 = 0;
                D_0029E1F8->x20 = light->x30;
                D_0029E1F8->x24 = light->x34;
                D_0029E1F8->x28 = light->x38;
                D_0029E1F8->x2C = ((unsigned short *)&light->u.flags)[2] & 0x80;
                D_0029E1F8->x2E = light->u.bits.index & 0x7FF;
                D_0029E1F8++;
                D_0029E1FC++;
            }
        }
    }
}


typedef struct Vec4217AA0 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4217AA0;

typedef struct Matrix217AA0 {
    Vec4217AA0 row[4];
} Matrix217AA0;

typedef ParticleEmitterDescription Obj217AA0;




/* 64-byte matrix copy (as sceVu0CopyMatrix) */
static inline void CopyMatrix(Matrix217AA0 *dst, Matrix217AA0 *src)
{
    __asm__ volatile ("lq $8, 0x0(%4)\n\t"
                      "lq $9, 0x10(%4)\n\t"
                      "lq $10, 0x20(%4)\n\t"
                      "lq $11, 0x30(%4)\n\t"
                      "sq $8, %0\n\t"
                      "sq $9, %1\n\t"
                      "sq $10, %2\n\t"
                      "sq $11, %3"
                      : "=m"(dst->row[0]), "=m"(dst->row[1]), "=m"(dst->row[2]), "=m"(dst->row[3])
                      : "r"(src)
                      : "$8", "$9", "$10", "$11");
}

static inline void ApplyRotation(Vec4217AA0 *v, Matrix217AA0 *m, Vec4217AA0 *out)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\t"
                      "lqc2 $vf12, 0(%2)\n\t"
                      "lqc2 $vf13, 0x10(%2)\n\t"
                      "lqc2 $vf14, 0x20(%2)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\t"
                      "vmadday.xyz ACC, $vf13, $vf11y\n\t"
                      "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "r"(m));
}

static inline void AddVector(Vec4217AA0 *out, Vec4217AA0 *a, Vec4217AA0 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\t"
                      "lqc2 $vf12, 0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

void func_00217AA0(Obj217AA0 *obj, int id, int index)
{
    Vec4217AA0 v;
    Vec4217AA0 tmp;

    if (obj->x58.all & (1L << 36)) {
        if (obj->x58.all & (1L << 32)) {
            CopyMatrix((Matrix217AA0 *)obj->x60, &((Matrix217AA0 *)D_0029E1A0)[index]);
            func_00263B38(&v);
            ApplyRotation(&v, &((Matrix217AA0 *)D_0029E1A0)[index], &tmp);
            AddVector((Vec4217AA0 *)obj->xA0, &tmp, func_0023CF78(id));
            obj->xA0[3] = 1.0f;
        }
    } else {
        func_00263B38(&v);
        obj->x58.bits.active =
            SpawnParticleDescription(&v, &((Matrix217AA0 *)D_0029E1A0)[index], obj, func_0023CF78(id)) != -1;
        obj->xA0[3] = 1.0f;
    }
}

typedef struct ObjC00 { int x0; unsigned char x4; } ObjC00;
typedef struct BufferC00 { char x[0xF0]; } BufferC00;
extern char D_00528360[];
extern int D_0029E1B8, D_0029E1BC, D_0029E1C0;
extern void *D_0029E1E8;
extern float func_0021D720(int);
extern void func_0021DC08(void);
extern void func_0021DC50(void *, int, int, int);
extern void func_0021DF38(int);
extern void func_0021E068(void);
extern ObjC00 *func_0021E3D8(int);
extern ObjC00 *func_0021E410(int);
extern void func_0021E440(void *, int);
void func_00217C00(int index, int arg1) {
    char local[0x10];
    int i;
    ObjC00 *obj;
    *(int *)0x70000088 = 1;
    D_0029E1B8 = 0;
    D_0029E1C0 = 0;
    D_0029E1E8 = (char *)D_00528360 + index * 240;
    *(ObjC00 **)0x70000038 = 0;
    func_0021DC08();
    i = 0;
    do {
        obj = func_0021E3D8(i);
        if (obj && obj->x4 && obj->x0) {
            *(ObjC00 **)0x70000038 = obj;
            D_0029E1BC = 999999;
            func_0021E440(D_0029E1E8 + 0x20, i);
            func_0021DC50(D_0029E1E8 + 0x20, index, arg1, i);
            func_00216E28((void *)obj, D_0029E1E8, (void *)local);
            func_0021B8A8(obj, local, index);
            D_0029DAA4 = func_0021D720(i);
            obj = func_0021E410(i);
            if (obj) {
                func_00216E28((void *)obj, D_0029E1E8, (void *)local);
                func_0021B8A8(obj, local, index);
            }
            func_0021DF38(i);
            D_0029DAA4 = 0.0f;
        }
        i++;
    } while (i < 4);
    func_0021E068();
    *(int *)0x70000088 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text", func_00217D88);

void func_00217DA8(void) {
    __asm__ volatile ("1: bc0f 1b");
}

/* Retail has 16 bytes of zero padding at 0x17DCC-0x17DE0 between func_00217DA8
   (which compiles to 0x28 bytes) and func_00217DE0. In a single-function unit
   the linker fills this gap, but in this merged TU it must be emitted here so
   that func_00217DE0 and all following functions land at retail addresses. */
static int pad_00217DCC[4] __attribute__((section(".text"))) = {0, 0, 0, 0};

int func_00217DE0(void) {
    int status, other;
    __asm__ volatile ("cfc2.i %0, $vi2" : "=r"(status));
    __asm__ volatile ("cfc2.i %0, $vi5" : "=r"(other));
    if (status == 0) return other != 0 ? 2 : 1;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text", func_00217E08);

typedef struct Vec217E10 { float x, y, z, w; } Vec217E10 __attribute__((aligned(16)));
typedef struct World217E10 { char pad[0x20]; Vec217E10 x20; } World217E10;
typedef struct Obj217E10 {
    int x0, x4, x8;
    unsigned char xC, xD, xE;
    char padF[0x11];
    int x20[1];
} Obj217E10;
typedef struct Result217E10 { char pad[0x28]; unsigned long x28; } Result217E10;
extern Vec217E10 *D_0029E1EC;
extern int D_0029E1F0;
extern void func_00242970(int, Vec217E10 *, Vec217E10 *, Result217E10 *);
extern int func_00242C80(int, Vec217E10 *);
void func_00217E10(Obj217E10 *obj, int *out, int index) {
    Vec217E10 original, neg;
    Result217E10 result;
    Vec217E10 pos, transformed;
    unsigned char i = 0;
    int valid = 1;
    int j;

    /* The vector argument arrives in $vf4. An output-less volatile asm makes
       cse forget valid == 1, so the first loop test is kept (as in retail). */
    __asm__ volatile ("sqc2 $vf4, %0" : : "m"(original));
    neg.x = -original.x;
    neg.y = -original.y;
    neg.z = -original.z;
    for (; valid && i < obj->xC; i++) {
        if (obj->xE) {
            for (j = 0; j < D_0029E1F0; j++) {
                pos.x = ((World217E10 *)D_0029E1E8)->x20.x + D_0029E1EC[j].x;
                pos.y = ((World217E10 *)D_0029E1E8)->x20.y + D_0029E1EC[j].y;
                pos.z = ((World217E10 *)D_0029E1E8)->x20.z + D_0029E1EC[j].z;
                func_00221888((float *)(D_0029E1A0 + index), &pos.x, &transformed.x);
                pos.x = neg.x + transformed.x;
                pos.y = neg.y + transformed.y;
                pos.z = neg.z + transformed.z;
                func_00242970(obj->x20[i], &neg, &pos, &result);
                if ((result.x28 & 0x0000FFFF0000FFFFUL) != 0x20000000000UL) valid = 0;
            }
        } else if (func_00242C80(obj->x20[i], &neg) != 0x200) {
            valid = 0;
        }
    }
    if (valid) {
        if (obj->x4) *out = obj->x4;
    } else if (obj->x8) {
        *out = obj->x8;
    }
}

float D_0029DAA8=1.0f;
typedef union Packet218038 { u128 all; unsigned int word[4]; } Packet218038;
typedef struct Slot218038 { int x0, x4, x8, xC; char pad10[0xE0]; } Slot218038;
extern int D_0029E200;
extern float D_0029E1D4;
extern unsigned char D_0029E204;
extern char D_00269C50[], D_0026CF80[];
extern void func_00208EE0(void);
extern void func_00242F10(void);
extern void func_0025A960(int);
extern int func_002433A8(int, int);
#define HW(a) (*(volatile unsigned int *)(a))
#define SCR(a) (scratch[((a) - 0x70000000) / 4])
static __inline__ void initScratch(int *scratch) {
    SCR(0x70000000) = 0x30000000; SCR(0x70000004) = 0; SCR(0x70000008) = 0; SCR(0x7000000C) = 0x1000; SCR(0x70000010) = 0; SCR(0x70000014) = 0x20000000; SCR(0x70000018) = 0x50505050; SCR(0x7000001C) = 0x31000000; SCR(0x70000020) = 0x800000; SCR(0x70000024) = 0x800000; SCR(0x70000028) = 0x800000; SCR(0x7000002C) = 0x800000;
}
void func_00218038(void) {
    Slot218038 *p = (Slot218038 *)D_00528360;
    Slot218038 *end = p + 6;
    Packet218038 *packet;
    int *scratch;
    D_0029DAA8 = 1.0f; D_0029DA98 = 0; D_0029DA9C = 1; D_0029E200 = 1; D_0029E1D4 = 5.0f; D_0029DAA4 = 0.0f; D_0029DAA0 = 0;
    do { p->x4 = 0; p->x8 = 0; p->xC = 0; p->x0 = 0; p++; } while ((int)p < (int)end);
    D_0029E204 = 0; D_0029DA8C = 0;
    func_00208EE0(); func_00242F10(); func_0025A960(0); func_002433A8(0, 0);
    scratch = (int *)0x70000000;
    initScratch(scratch);
    HW(0x10009020) = 3; HW(0x10009010) = 0xF0000000; HW(0x1000E010) = 2; HW(0x10009000) = 0x101;
    func_002433A8(0, 0);
    SCR(0x70000000) = 0x30000000; SCR(0x70000014) = 0x20000000; SCR(0x7000001C) = 0x31000000;
    HW(0x10008020) = 3; HW(0x10008010) = 0xF0000000; HW(0x1000E010) = 1; HW(0x10008000) = 0x101;
    func_002433A8(0, 0);
    {
        unsigned int address = (unsigned int)D_00269C50 & 0xFFFFFFF;
        HW(0x10008020) = 0; HW(0x10008030) = (unsigned int)D_0026CF80 & 0xFFFFFFF; HW(0x1000E010) = 1; HW(0x10008000) = 0x145;
        packet = (Packet218038 *)D_006202B0;
        packet[0].all = 0;
        ((Packet218038 *)D_006202B0)[0].word[0] = 0x50000000;
        packet[0].word[1] = address;
        packet[1].all = 0;
        packet[1].word[0] = (unsigned int)scratch;
    }
}


typedef struct Source { int flags; char pad4[0x18]; float x1C; } Source;
typedef struct Obj00218278 { Source *x0; int x4; float x8; } Obj00218278;
void func_00218278(Obj00218278 *obj, Source *source) {
    obj->x0 = source;
    if (source && obj && ((source->flags & 0x3F) == 1)) {
        obj->x8 = source->x1C;
    }
}

struct Obj002182A8 {
    char pad0[0x4];
    unsigned char x4;
};

void func_002182A8(struct Obj002182A8 *obj, unsigned char value)
{
    if (obj != 0) {
        obj->x4 = value;
    }
}

extern int D_0029DA90;

void func_002182C0(int v) {
    D_0029DA90 = v;
}

extern int D_0029DA94;

void func_002182C8(int v) {
    D_0029DA94 = v;
}

typedef SceneNode Obj182D0;
#define ZERO182D0(o) __asm__ volatile("sqc2 $vf0, " #o "(%0)" : : "r"(p) : "memory")
void func_002182D0(Obj182D0 *p) {
 p->resourceId=0; p->x4=0; p->b57=0; p->x5=0; p->b56=0;
 ZERO182D0(0xA0); func_00221F20(&p->matrix);
 ZERO182D0(0x10); ZERO182D0(0x30);
 p->b62=0; p->b63=1; p->x6=-1; p->b59=0; p->b60=1; p->b58=0;
 p->x84=32767; p->x86=32767; p->x88=-32767; p->x8A=-32767; p->xB6=-1;
 p->x20=50.0f; p->x24=50.0f; p->x28=50.0f; p->x2C=1.0f; p->xB4=0;
 ZERO182D0(0x90); func_00221F20(&p->xC0);
 p->x8=50.0f; p->xB8=1.0f; p->xC=800.0f; p->xBC=0xFFFFFFFF; p->b61=0;
}

typedef struct { int flags; char pad4[4]; unsigned char x8; char pad9[2]; unsigned char xB; } Obj00218498;
void func_00218498(Obj00218498 *obj, int value) {
    if (obj && (obj->flags & 0x3F) == 6 && value < obj->xB) {
        obj->x8 = value;
    }
}

INCLUDE_ASM("asm/nonmatchings/text", func_002184C8);

typedef struct Vec4C00 {
    float x, y, z, w;
} Vec4C00 __attribute__((aligned(16)));

extern unsigned char D_0029E204;
extern char D_00528300[];
extern Vec4C00 D_00528340;
extern char D_00279A20[];
extern void func_0023CFA0(void *a, void *arg);
extern void func_0023CFE0(Vec4C00 *v, void *arg);
extern void func_0023D1F0(Vec4C00 *v, void *arg);

void func_002184D0(void *matrix, Vec4C00 *pos, void *arg) {
    Vec4C00 *offset;

    if (matrix != 0) {
        D_0029E204 = 1;
        __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(D_00528340) : "r"(matrix), "r"(pos));
        offset = (Vec4C00 *)func_0023CF78(arg);
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(D_00528340) : "r"(&D_00528340), "r"(offset));
        func_00221930(D_00528300, D_00279A20, matrix);
    } else if (D_0029E204 != 0) {
        D_0029E204 = 0;
        func_0023CFA0(D_00528300, arg);
        func_0023CFE0(&D_00528340, arg);
        func_0023D1F0(&D_00528340, arg);
    }
}

extern char D_00528360[];
extern int func_0021E3C8(void);
extern float func_0020EE10(int);
extern void func_0021CB98(int,int,void *,int,float,float);
void func_002185C0(int *obj,int mode) {
    int buffer[4];
    int a,b,c;
    if(*obj) {
        D_0029E1A0=(Mtx *)0x70000400;
        D_0029E1A4=(Mtx *)0x70000900;
        D_0029E1A8=(Mtx *)0x70000E00;
        D_0029E1AC=(Mtx *)0x70001300;
        D_0029E1B0=(Entry217160 *)0x70001D00;
        D_0029E1B4=(int *)0x70000350;
        D_0029DA9C=mode;
        D_0029E1E8=D_00528360;
        *(int **)0x70000038=obj;
        D_0029DA98=1;
        D_0029DAA4=1.0f;
        a=(int)func_0023CF78(0);
        b=func_0021E3C8();
        c=func_0022FD90();
        func_0021CB98(a,b,D_0029E1E8,c,func_0020EE10(0),1.0f);
        __asm__ volatile ("sync\n\tsync.p");
        func_00216E28((void *)obj,D_0029E1E8,(void *)buffer);
        func_0021B8A8(obj,buffer,0);
        D_0029DA9C=1;
        D_0029DA98=0;
        D_0029DAA4=0.0f;
    }
}
