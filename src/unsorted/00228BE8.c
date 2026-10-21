/* UNIT 0x00228BE8 0x0022E560 .rodata=0x298FE0:0x2990D0 .sdata=0x29DB60:0x29DBC8 */
#include "particle_spray.h"
#include "particle_descriptor.h"
#include "particle.h"
#include "include_asm.h"

unsigned long D_0029DB60 = 0x74CBB1; /* random seed */
int D_0029DB68 = 70;
float D_0029DB6C = 3.0f;
float D_0029DB70 = 3.0f;
float D_0029DB74 = 3.25f;
float D_0029DB78 = 3.25f;
float D_0029DB7C = 6.25f;
float D_0029DB80 = 6.0f;
float D_0029DB84 = 255.0f;
float D_0029DB88 = 255.0f;
float D_0029DB8C = -1000.0f;
float D_0029DB90 = 64.0f;
float D_0029DB94 = 128.0f;
float D_0029DB98 = 128.0f;
float D_0029DB9C = -1000.0f;
float D_0029DBA0 = 64.0f;
unsigned char D_0029DBA4 = 1;
unsigned char D_0029DBA5 = 0;
float D_0029DBA8 = 0.0f;
int D_0029DBAC = 0;
int D_0029DBB0 = 0;
/* spawn colour cycle */
unsigned char D_0029DBB8[8] = { 0x0C, 0x79, 0x69, 0x5A, 0x0C, 0x39, 0x29, 0x12 };
float D_0029DBC0 = 0.03125f;
float D_0029DBC4 = 255.0f;

/* func_00228BE8: reset the particle system state */
typedef struct Cfg__bbe8 {
    unsigned char c[5][4];
    short s[4];
    signed char x1C[4];
} Cfg__bbe8;

typedef struct Wind__bbe8 {
    char pad0[4];
    float x4;
    float x8;
    int xC;
    char pad10[0x3C];
    float x4C;
} Wind__bbe8;

typedef struct Tune__bbe8 {
    char pad0[0xC];
    float xC;
    float x10;
    char pad14[0x2C];
} Tune__bbe8;

typedef struct Type__bbe8 {
    char pad0[0x1A];
    short x1A;
    char pad1C[0xC];
    float x28;
    float x2C;
    char pad30[0x18];
    float x48;
    char pad4C[4];
} Type__bbe8;

typedef struct Size__bbe8 {
    unsigned int flags;
    char pad4[0x1C];
} Size__bbe8;

typedef struct Obj__bbe8 {
    char pad0[0x2848];
    int *x2848;
    char pad284C[0x14];
} Obj__bbe8;

typedef struct Gs__bbe8 {
    char pad0[0x188];
    int x188;
    short x18C;
    short x18E;
} Gs__bbe8;

typedef struct Image__bbe8 {
    char pad0[0x70];
} Image__bbe8;

extern int D_0029E314, D_0029E30C, D_0029E308;
extern Cfg__bbe8 D_0027EB70__bbe8 asm("D_0027EB70");
extern char D_00679280[][0x1340];
extern void *D_0067DF80[];
extern Gs__bbe8 D_0067DF90__bbe8 asm("D_0067DF90");
extern Obj__bbe8 D_0067E180__bbe8[] asm("D_0067E180");
extern char D_0071F980__bbe8[] asm("D_0071F980");
extern int D_0071FC80__bbe8[] asm("D_0071FC80");
extern Image__bbe8 D_00732600[];
extern Wind__bbe8 D_0027A4B0__bbe8[] asm("D_0027A4B0");
extern Tune__bbe8 D_0027BFC0__bbe8[] asm("D_0027BFC0");
extern Type__bbe8 D_0027D300__bbe8[] asm("D_0027D300");
extern Size__bbe8 D_00279B00__bbe8[] asm("D_00279B00");

extern void *func_00224B88(void *, int);
extern void func_002432B8(void *, int, int *, int);
extern void func_00237C78(int, int, int);
extern void sceGsSetDefStoreImage(void *, int, int, int, int, int, int, int);
extern void func_0023D650(int, int *, int *);
extern void func_00237D20(int, int);
extern float func_002487A0(float);

void func_00228BE8(int enabled)
{
    int i;
    int w, h;

    D_0029DBA5 = enabled;
    if (enabled == 0) return;

    for (i = 0; i < 4; i++) {
        D_0067DF80[i] = func_00224B88(D_00679280[i], i);
    }

    D_0027EB70__bbe8.s[0] = 0x32;
    D_0027EB70__bbe8.s[1] = -0x82;
    D_0027EB70__bbe8.s[2] = -0x12;
    D_0027EB70__bbe8.s[3] = 0x76;
    D_0027EB70__bbe8.x1C[0] = 0x46;
    D_0027EB70__bbe8.x1C[1] = 0;
    D_0027EB70__bbe8.x1C[2] = -0x1C;
    D_0027EB70__bbe8.x1C[3] = -0x1C;
    D_0027EB70__bbe8.c[0][0] = 0x82;
    D_0027EB70__bbe8.c[1][0] = 0x46;
    D_0027EB70__bbe8.c[2][0] = 0x28;
    D_0027EB70__bbe8.c[3][0] = 0x3C;
    D_0027EB70__bbe8.c[4][0] = 0x14;
    D_0027EB70__bbe8.c[0][1] = 0x78;
    D_0027EB70__bbe8.c[1][1] = 0x4B;
    D_0027EB70__bbe8.c[2][1] = 0x1E;
    D_0027EB70__bbe8.c[3][1] = 0x28;
    D_0027EB70__bbe8.c[4][1] = 0x14;
    D_0027EB70__bbe8.c[0][2] = 0x6E;
    D_0027EB70__bbe8.c[1][2] = 0x50;
    D_0027EB70__bbe8.c[2][2] = 0x14;
    D_0027EB70__bbe8.c[3][2] = 0x14;
    D_0027EB70__bbe8.c[4][2] = 0x14;

    D_0029DB6C = 3.0f;
    D_0029DB70 = 3.0f;
    D_0029DB7C = 6.25f;
    D_0029DB9C = -1000.0f;
    D_0029DBA0 = 64.0f;
    D_0029DB94 = 128.0f;
    D_0029DB74 = 3.25f;
    D_0029DB78 = 3.25f;
    D_0029DB80 = 6.25f;
    D_0029DB98 = 128.0f;
    D_0029DB84 = 255.0f;
    D_0029DB88 = 255.0f;
    D_0029DB8C = -1000.0f;
    D_0029DB90 = 64.0f;
    D_0029DB68 = 70;
    D_0029E314 = 1;
    D_0029DBA8 = 8.60444450378418f;
    D_0029DBAC = 0;

    for (i = 63; i >= 0; i--) {
        if (D_0029DBA4 == 0 && D_0067E180__bbe8[63 - i].x2848 != 0) {
            *D_0067E180__bbe8[63 - i].x2848 = 0;
        }
        D_0067E180__bbe8[63 - i].x2848 = 0;
    }

    func_002432B8(D_0071F980__bbe8, 0x40, &D_0029E30C, 0);
    *(int *)(D_0071F980__bbe8 + 0x14) = -1;
    D_0067DF90__bbe8.x18E = 0xE0;
    D_0067DF90__bbe8.x18C = 0x200;
    D_0029E308 = -1;
    D_0067DF90__bbe8.x188 = 0;
    func_00237C78(0xC, 0xFF000, 0x40);
    func_00237C78(0xD, 0xFE000, 0x40);

    for (i = 0; i < 0x4A; i++) {
        sceGsSetDefStoreImage(&D_00732600[i], 0, 8, 0x30, 0, 0, 4, 4);
    }

    func_0023D650(0, &w, &h);
    func_00237D20(w, h);

    if (D_0029DBA4) {
        Wind__bbe8 *winds = D_0027A4B0__bbe8;

        for (i = 0; i < 0x4D; i++) {
            if (winds[i].xC > 0) {
                winds[i].x4C = 1.0f / winds[i].xC;
            } else {
                winds[i].x4C = 0.0f;
            }
            D_0027BFC0__bbe8[i].x10 = 0.5f - func_002487A0(D_0027BFC0__bbe8[i].x10 * 0.00872664619f) * 0.5f;
            if ((D_00279B00__bbe8[i].flags & ~0x70) == 0x8003) {
                D_0027D300__bbe8[i].x48 = (D_0027D300__bbe8[i].x28 - D_0027D300__bbe8[i].x2C) / D_0027D300__bbe8[i].x1A;
                winds[i].x4 = 1.0f - winds[i].x4;
                winds[i].x8 = 1.0f - winds[i].x8;
                D_0027D300__bbe8[i].x1A += (int)(1.0f / D_0027BFC0__bbe8[i].xC);
            }
        }
    }

    D_0029DBA4 = 0;
    for (i = 27; i >= 0; i--) {
        D_0071FC80__bbe8[i] = 0;
    }
}


typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vec { u128 q; float f[4]; } Vec;
typedef struct Mat { Vec r[4]; } Mat;

typedef ParticleEmitterDescription Desc;

typedef struct Obj {
    char pad0[0x2600];
    Vec x2600;
    char pad2610[0x20];
    Vec x2630;
    char pad2640[0x40];
    Vec x2680;
    Vec x2690;
    Vec x26A0;
    Mat x26B0;
    char pad26F0[0x20];
    void *x2710;
    void *x2714;
    char pad2718[8];
    int x2720;
    short x2724;
    short x2726;
    char pad2728[4];
    float x272C;
    float x2730;
    Vec *x2734;
    Mat *x2738;
    float x273C;
    float x2740;
    float x2744;
    char pad2748[4];
    float x274C;
    float x2750;
    float x2754;
    char pad2758[0xC];
    float x2764;
    char pad2768[0x10];
    float x2778;
    int x277C;
    float x2780[4];
    float x2790;
    char pad2794[0x28];
    float x27BC;
    int x27C0;
    char pad27C4[4];
    float x27C8;
    float x27CC;
    float x27D0;
    float x27D4;
    short x27D8;
    short x27DA;
    char pad27DC[0x14];
    float x27F0;
    float x27F4;
    char pad27F8[0x28];
    float x2820;
    char pad2824[7];
    unsigned char x282B;
    char pad282C[0x20];
    Desc *x284C;
    char pad2850[0x10];
} Obj;

struct TypeInfo { int flags; int x4; float x8, xC, x10, x14; char pad18[0x38]; };
struct SizeInfo { char pad0[0x10]; float x10; char pad14[0xC]; };
struct Entry { char pad0[6]; unsigned short x6; char pad8[0x14]; float x1C, x20, x24; char pad28[8]; float x30, x34; char pad38[8]; };

extern struct TypeInfo D_0027D300[];
extern struct SizeInfo D_00279B00[];
extern struct Entry D_0027BFC0[];
extern Obj D_0067E180[];
extern Desc *D_0071FD30[];




extern unsigned char func_0022C350(Vec *, float);
extern int func_0022C468(int, float);
extern void func_00225550(Obj *), func_0022A280(Obj *), func_0022A2C0(Obj *), func_002256F0(Obj *);
extern void func_0022CAB0(Obj *);
extern float func_002487A0(float);

#define COPY_QUAD(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MTX4(dst, src) __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" : "=m"((dst)->r[0]), "=m"((dst)->r[1]), "=m"((dst)->r[2]), "=m"((dst)->r[3]) : "r"(src) : "$8", "$9", "$10", "$11")

static __inline__ void VuAdd(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static __inline__ void VuScale(Vec *out, Vec *v, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static __inline__ void VuAddScaled(Vec *out, Vec *a, Vec *b, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static __inline__ void VuApplyMatrix(Vec *out, Mat *m, Vec *v) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\t"
                      "vmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "r"(m));
}

static __inline__ void VuSinCos(Vec *angles, Vec *s, Vec *c) {
    __asm__ volatile ("lqc2 $vf14, 0x0(%0)\n\tvcallms 0x978\n\tnop\n\tqmfc2.i $2, $vf14\n\tsqc2 $vf15, 0x0(%2)\n\tsq $2, 0x0(%1)"
                      : : "r"(angles), "r"(s), "r"(c) : "$2", "memory");
}

static __inline__ float FSqrt(float x) {
    float r;
 __asm__ volatile ("sqrt.s %0, %1" : "=&f"(r) : "f"(x));
    return r;
}

static __inline__ float FMin(float a, float b) {
    float r;
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

/* ---- merged from src/unsorted/00229400.c ---- */
typedef struct Linked__c1 {
    char pad0[0x58];
    unsigned long x58;
} Linked__c1;

typedef struct Flags__c1 {
    unsigned long low : 6;
    unsigned long rest : 58;
} Flags__c1;

typedef struct Obj__c1 {
    char pad0[0x2860];
} Obj__c1;

typedef struct Node__c1 {
    int next;
    char pad4[0x8];
} Node__c1;

extern Flags__c1 *D_0071FC80__c1[] asm("D_0071FC80");
extern Linked__c1 *D_0071FD30__c1[] asm("D_0071FD30");
extern int D_0029E308__c1 asm("D_0029E308");
extern Obj__c1 D_0067E180__c1[] asm("D_0067E180");
extern Node__c1 D_0071F988__c1[] asm("D_0071F988");
extern void func_002293C8__c1(Obj__c1 *) asm("func_002293C8");

extern float *D_0029E2F0,*D_0029E2F4,*D_0029E2F8,*D_0029E2FC,*D_0029E300,*D_0029E304;
extern int D_0029E310;
extern char D_00678880[],D_00678D80[];
extern struct InitState229050 { char pad[0x1AC]; short x1AC; } D_0067DF90;
extern float D_0027EB10[], D_0027EB40[];
extern unsigned char D_0027EB70[],D_0027EB90[];
extern int D_00720270[], D_00720280[];
struct InitObj229050 { char pad[0x2770]; int x2770; char p2774[0x9C]; int x2810; char p2814[0x30]; int x2844; char p2848[8]; int x2850; char p2854[12]; };
extern struct InitObj229050 D_00723A80[];
extern int func_0023D608(void);
extern void func_00225DB0(struct InitObj229050 *,int);
extern void func_00237918(void*,int,int,int,int,int,float**,float**,float**);
extern void func_00238728(void),func_00227DD0(void);
void func_00229050(void) {
 int i; struct InitObj229050 *p; int *head;
 if (D_0029DBA5) {
  for(i=0;i<func_0023D608();i++) {
   p=&D_00723A80[i]; p->x2850=3; p->x2810=0x8029; p->x2770=0x1000; p->x2844=0;
   func_00225DB0(p,i); D_00720270[i]=D_00720280[i]=0;
  }
  i=0;
  func_00237918(D_00678880,0xFF000,0x40,0x19,0x1A,D_0067DF90.x1AC,&D_0029E2F8,&D_0029E2F0,&D_0029E2F4);
  func_00237918(D_00678D80,0xFE000,0x40,0x19,0x1A,D_0067DF90.x1AC,&D_0029E304,&D_0029E2FC,&D_0029E300);
  func_00238728(); func_00227DD0();
  
  D_0029DB74=3.25f; D_0029DB78=3.25f; D_0029DB80=6.25f; D_0029DB88=255;
  head=(int*)D_0029E2F8; head[-4]=D_0029DB68*4;
  D_0029E2F8[0]=D_0029DB6C; D_0029E2F8[1]=D_0029DB70; D_0029E2F8[2]=D_0029DB7C;
  head=(int*)D_0029E304; head[-4]=D_0029DB68*4;
  D_0029E304[0]=D_0029DB74; D_0029E304[1]=D_0029DB78; D_0029E304[2]=D_0029DB80;
  D_0029E2F8[4]=D_0029DB94; D_0029E2F8[5]=D_0029DB98/D_0029DB94; D_0029E2F8[6]=D_0029DB9C; D_0029E2F8[7]=D_0029DBA0;
  D_0029E304[4]=D_0029DB84; D_0029E304[5]=D_0029DB88/D_0029DB84; D_0029E304[6]=D_0029DB8C; D_0029E304[7]=D_0029DB90;
  do { D_0029E2FC[i]=D_0027EB40[i]; D_0029E2F0[i]=D_0027EB10[i]; i++; } while(i<12);
  i=0; do { D_0029E300[i]=D_0027EB90[i]; D_0029E2F4[i]=D_0027EB70[i]; i++; } while (i<20);
  D_0029E310=0; D_0029DBB0=0;
 }
}

/* UNIT 0x002293C8 0x00229400 */
typedef struct { char pad0[0x58]; unsigned long x58; } Linked293C8;
typedef struct { char pad0[0x284c]; Linked293C8 *x284C; } Obj293C8;
void func_002293C8(Obj293C8 *obj) {
    Linked293C8 *linked = obj->x284C;
    if (linked) {
        linked->x58 &= ~(1UL << 36);
        obj->x284C = 0;
    }
}

extern void func_00229400__c1(void) asm("func_00229400");
void func_00229400__c1(void)
{
    int i;
    int n;
    Linked__c1 **lp;
    Flags__c1 **pp;

    for (n = 0; n < 28; n++) {
        Flags__c1 *f = D_0071FC80__c1[n];
        if (f != 0) {
            f->low = 0;
        }
    }
    i = D_0029E308__c1;
    if (i != -1) {
        do {
            func_002293C8__c1(&D_0067E180__c1[i]);
            if (i == -1) {
                break;
            }
            i = D_0071F988__c1[i].next;
        } while (i != -1);
    }
    n = D_0029DBB0;
    if (n > 0) {
        lp = D_0071FD30__c1;
        do {
            (*lp)->x58 &= ~(1UL << 36);
            lp++;
        } while (--n != 0);
    }
}
/* ---- merged from src/unsorted/00229508.c ---- */
typedef unsigned int u128__c2 __attribute__((mode(TI)));

typedef struct Mtx__c2 {
    u128__c2 row[4];
} Mtx__c2;

typedef struct Obj__c2 {
    u128__c2 pos;
    Mtx__c2 mtx;
    char pad50[0x90];
    u128__c2 *xE0;
    Mtx__c2 *xE4;
    char padE8[0x4];
    void *xEC;
    char padF0[0x14];
    u128__c2 *x104;
    Mtx__c2 *x108;
} Obj__c2;

/* dst = m1 * m0 (sceVu0MulMatrix) */
extern void func_00221930__c2(Mtx__c2 *dst, Mtx__c2 *m0, Mtx__c2 *m1) asm("func_00221930");
extern void func_00221888__c2(void *m, void *in, void *out) asm("func_00221888");

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* sceVu0ApplyMatrix-style, xyz only: out = m * in */
static __inline__ void ApplyMatrix(u128__c2 *out, Mtx__c2 *m, u128__c2 *in)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "lqc2 $vf13, 0x10(%2)\n\t"
        "lqc2 $vf14, 0x20(%2)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*out) : "r"(in), "r"(m));
}

static __inline__ void AddVector(u128__c2 *out, u128__c2 *a, u128__c2 *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "vadd.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, 0x0(%0)"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}

/* sceVu0CopyMatrix */
static __inline__ void CopyMatrix(Mtx__c2 *dst, Mtx__c2 *src)
{
    __asm__ volatile (
        "lq $8, 0x0(%4)
\n\t"
        "lq $9, 0x10(%4)
\n\t"
        "lq $10, 0x20(%4)
\n\t"
        "lq $11, 0x30(%4)
\n\t"
        "sq $8, %0
\n\t"
        "sq $9, %1
\n\t"
        "sq $10, %2
\n\t"
        "sq $11, %3"
        : "=m"(dst->row[0]), "=m"(dst->row[1]), "=m"(dst->row[2]), "=m"(dst->row[3])
        : "r"(src) : "$8", "$9", "$10", "$11");
}

void func_00229508(Obj__c2 *obj, void *vec)
{
    u128__c2 v;

    if (obj->xE4 != 0) {
        ApplyMatrix(&v, obj->xE4, obj->x104);
        func_00221930__c2(&obj->mtx, obj->x108, obj->xE4);
        AddVector(&obj->pos, &v, obj->xE0);
    } else {
        CopyMatrix(&obj->mtx, obj->x108);
        COPY_VECTOR(obj->pos, obj->x104);
    }
    if (obj->xEC != 0) {
        func_00221888__c2(obj->xEC, vec, vec);
    }
}
/* ---- func_002295D0: particle emitter spawn ---- */
typedef struct Emitter__p {
    char pad0[0x2500];
    unsigned char x2500[0x110];
    Vec x2610;
    Vec x2620;
    Vec x2630;
    Vec x2640;
    Vec x2650;
    Vec x2660;
    char pad2670[0x10];
    Vec x2680;
    char pad2690[0x10];
    Vec x26A0;
    Mat x26B0;
    Vec x26F0;
    Vec x2700;
    char pad2710[4];
    Mat *x2714;
    char pad2718[8];
    union {
        struct { unsigned int flags; short x2724; short x2726; } w;
        unsigned long l;  /* bits 22-23: mode */
    } x2720;
    float x2728;
    float x272C;
    float x2730;
    char pad2734[8];
    float x273C;
    float x2740;
    float x2744;
    float x2748;
    float x274C;
    float x2750;
    float x2754;
    char pad2758[0x10];
    float x2768;
    unsigned short x276C;
    unsigned short x276E;
    int x2770;
    char pad2774[0xC];
    Vec x2780;
    char pad2790[0x30];
    int x27C0;
    char pad27C4[4];
    float x27C8;
    float x27CC;
    char pad27D0[0xA];
    short x27DA;
    float x27DC;
    char pad27E0[0x30];
    unsigned int x2810;
    char pad2814[0x1C];
    Vec *x2830;
    Vec *x2834;
    short *x2838;
    char pad283C[0x14];
    int x2850;
    int x2854;
    char pad2858[2];
    short x285A;
} Emitter__p;

extern unsigned long D_0029DB60__p asm("D_0029DB60");
extern unsigned char D_0029DBB8[8];
extern Vec D_0027A4A0__p asm("D_0027A4A0");
extern Vec D_0027BCD0;
extern void func_0023F138(Emitter__p *, Vec *, int);


extern void func_0022A2C0__p(Emitter__p *) asm("func_0022A2C0");
extern void func_00222C38(Vec *, float);
extern void func_00223628(Vec *, Vec *);
extern void *memmove(void *, const void *, unsigned int);

static __inline__ void VuSub__p(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

#define VU_SETX(vf, f) __asm__ volatile ("qmfc2.ni $2, " vf "\n\tmfc1 $2, %0\n\tqmtc2.ni $2, " vf : : "f"(f) : "$2")
#define VU_LOAD(vf, p) __asm__ volatile ("lqc2 " vf ", 0x0(%0)" : : "r"(p))
#define VU_STORE(vf, p) __asm__ volatile ("sqc2 " vf ", 0x0(%0)" : : "r"(p) : "memory")

#define RAND_STEP() (D_0029DB60__p = D_0029DB60__p * 0x41C64E6D + 12345)

typedef union { unsigned int i; float f; } IntFloat__p;

static __inline__ float BitsFloat__p(unsigned int i) {
    union { unsigned int i; float f; } u;
    u.i = i;
    return u.f;
}

/* LCG: a float in [0,1) built from the top mantissa bits of the seed */
static __inline__ float Rand01__p(void) {
    union { unsigned int i; float f; } u;
    RAND_STEP();
    u.i = ((D_0029DB60__p >> 9) & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

void func_002295D0(Emitter__p *obj, int count)
{
    Vec pt;
    Vec v10;
    Vec v20;
    Vec v30;
    Vec v40;
    Vec v50;
    Vec sc;
    float len;
    float step;
    float r;
    float alpha;
    float invAlpha;
    float s, c, radius, z;
    int n, i;
    int locked, end;
    int randZ, randY, is800A, is8002, spin, randW, hasLife, isType3, first;
    int is8006;
    Vec *dst0, *dst1;
    int v;

    v10.q = 0;
    pt.f[3] = count;
    if (obj->x2770 & 0x400) {
        VuAddScaled(&v10, &obj->x2780, &D_0027A4A0__p, obj->x2780.f[3]);
    }
    if (obj->x2720.w.flags & 0xC00) {
        if (!(obj->x2720.w.flags & 0x1000)) {
            COPY_QUAD(obj->x2680, &obj->x2630);
        }
        func_00229508((Obj__c2 *)&obj->x2630, &v10);
        obj->x2720.w.flags &= ~0x400;
    }
    if (obj->x2720.w.x2724 == 0) {
        if (obj->x2720.w.flags & 0x200000) {
            v20.q = obj->x2700.q;
        } else {
            VuSub__p(&v20, &obj->x2630, &obj->x2680);
        }
        VuScale(&v30, &v20, obj->x2748 / pt.f[3]);
        if (obj->x2720.w.flags & 0x8000) {
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                              : "=&f"(len) : "f"(v30.f[0]), "f"(v30.f[1]), "f"(v30.f[2]));
            if (D_0029DBA8 < len) {
                return;
            }
        }
        obj->x2768 += obj->x272C * pt.f[3];
        locked = (obj->x2720.l & 0xC00000) == 0x400000;
        if (locked) {
            obj->x2768 += obj->x2768;
            VuApplyMatrix(&v40, obj->x2714 ? obj->x2714 : &obj->x26B0, &D_0027BCD0);
            VU_LOAD("$vf20", &v40);
        }
        n = obj->x2768;
        obj->x2768 -= (float)n;
        if (obj->x2850 + n > obj->x2854) {
            n = obj->x2854 - obj->x2850;
        }
        if (n != 0) {
            unsigned int type;

            type = obj->x2810 & ~0x70;
            if (type == 0x8003) {
                if (obj->x276C == 0x38) {
                    func_0023F138(obj, &v10, count);
                } else if (obj->x276C == 0x3D) {
                    EmitParticleSprayLegacy(obj, &v10, count);
                } else {
                    VuSub__p(&v30, &v30, &v10);
                    EmitParticleSprayVariantLegacy(obj, &v30, count);
                }
                return;
            }
            if (type == 4) {
                obj->x285A = n;
                func_0022A2C0__p(obj);
            } else {
                unsigned int kind;

                i = obj->x2850;
                end = i + n;
                if (obj->x276C == 0x3B && i != 0) {
                    end = n;
                    memmove(obj->x2830 + n, obj->x2830, i * sizeof(Vec));
                    i = 0;
                    memmove(obj->x2834 + n, obj->x2834, obj->x2850 * sizeof(Vec));
                    memmove(obj->x2838 + n, obj->x2838, obj->x2850 * sizeof(short));
                    memmove(obj->x2500 + n, obj->x2500, obj->x2850);
                }
                step = 1.0f / n;
                pt.f[0] = step;
                VuSub__p(&v40, &v30, &v10);
                randZ = 1;
                if (obj->x2744 == obj->x2740) randZ = 0;
                randY = 1;
                if (obj->x2754 == obj->x2750) randY = 0;
                kind = obj->x2810 & ~0x70;
                spin = obj->x2720.w.flags & 1;
                is8006 = kind == 0x8006;
                is800A = kind == 0x800A;
                is8002 = kind == 0x8002;
                randW = obj->x27CC > obj->x27C8 && !is800A && !is8006;
                hasLife = 1;
                if (!(obj->x2728 > 0.0f)) hasLife = 0;
                isType3 = obj->x276C == 3;
                first = obj->x2850 == 0;
                if (spin && obj->x2730 > 0.0f) {
                    radius = obj->x2730;
                    __asm__ volatile ("mfc1 $3, %2\n\tqmtc2.ni $3, $vf14\n\tvcallms 0x978\n\tnop\n\tqmfc2.i $3, $vf14\n\tmtc1 $3, %0\n\tqmfc2.ni $3, $vf15\n\tmtc1 $3, %1"
                                      : "=&f"(s), "=&f"(c) : "f"(obj->x27DC * 0.017453292f) : "$3");
                    z = c * radius;
                    obj->x26A0.f[0] = s * radius;
                    obj->x26A0.f[2] = z;
                    obj->x26A0.f[3] = obj->x27DA;
                    obj->x26F0.f[0] = 1.0f;
                    obj->x26F0.f[1] = obj->x273C;
                    obj->x26F0.f[2] = 0.0f;
                    obj->x26F0.f[3] = obj->x26A0.f[3];
                    obj->x2730 = -1.0f;
                    sc.f[0] = s;
                    sc.f[1] = c;
                    obj->x26A0.f[1] = radius;
                }
                if (is8006) {
                    alpha = obj->x27DA;
                } else {
                    alpha = 0.0f;
                }
                invAlpha = 1.0f / obj->x27DA;
                dst0 = obj->x2830 + i;
                dst1 = obj->x2834 + i;
                VU_SETX("$vf11", obj->x273C);
                VU_SETX("$vf12", obj->x274C);
                VU_LOAD("$vf26", &obj->x2640);
                VU_LOAD("$vf28", &obj->x2650);
                VU_LOAD("$vf29", &obj->x2660);
                __asm__ volatile ("vmulx.xyzw $vf27, $vf28, $vf11x\n\tvmulx.xyzw $vf28, $vf28, $vf12x");
                VU_LOAD("$vf25", &obj->x2680);
                VU_LOAD("$vf24", &v20);
                VU_LOAD("$vf23", &v40);
                for (; i < end; i++, dst0++, dst1++) {
                    unsigned int flags;
                    Vec *pv = &v50;

                    if (spin) {
                        VU_LOAD("$vf11", &obj->x26F0);
                        VU_LOAD("$vf12", &obj->x26A0);
                        alpha = obj->x26A0.f[3] - obj->x26F0.f[3];
                        v50.q = obj->x26F0.q;
                        __asm__ volatile ("vsubz.x $vf13, $vf0, $vf12z");
                        obj->x2744 *= obj->x26A0.f[1];
                        obj->x2740 *= obj->x26A0.f[1];
                        __asm__ volatile ("vmuly.yw $vf14, $vf11, $vf12y\n\tvaddx.z $vf13, $vf0, $vf12x");
                        obj->x27CC *= obj->x26A0.f[1];
                        obj->x27C8 *= obj->x26A0.f[1];
                        __asm__ volatile ("vmulaz.xz ACC, $vf13, $vf11z\n\tvmaddx.xz $vf14, $vf12, $vf11x");
                        VU_STORE("$vf14", &obj->x26F0);
                    } else {
                        func_00222C38(pv, obj->x2730);
                        VU_LOAD("$vf11", pv);
                    }
                    r = Rand01__p();
                    if (randZ) {
                        pt.f[2] = (obj->x2744 - obj->x2740) * r + obj->x2740;
                    } else {
                        pt.f[2] = obj->x2740;
                    }
                    if (randY) {
                        if (!(obj->x2720.w.flags & 0x10000)) {
                            r = Rand01__p();
                        }
                        pt.f[1] = (obj->x2754 - obj->x2750) * r + obj->x2750;
                    } else {
                        pt.f[1] = obj->x2750;
                    }
                    VU_LOAD("$vf22", &pt);
                    __asm__ volatile ("vmulax.xyzw ACC, $vf26, $vf11x\n\t"
                                      "vmaddaz.xyzw ACC, $vf29, $vf11z\n\t"
                                      "vmaddy.xyzw $vf13, $vf27, $vf11y\n\t"
                                      "vmaddy.xyzw $vf11, $vf28, $vf11y\n\t"
                                      "vaddax.w ACC, $vf22, $vf0x\n\t"
                                      "vmsubx.w $vf21, $vf22, $vf22x\n\t"
                                      "vaddax.xyz ACC, $vf23, $vf0x\n\t"
                                      "vmaddz.xyzw $vf12, $vf13, $vf22z\n\t"
                                      "vmulz.w $vf21, $vf21, $vf22z");
                    flags = obj->x2720.w.flags;
                    if (locked && (flags & 0x8000000)) {
                        __asm__ volatile ("vadda.xyz ACC, $vf25, $vf20");
                        obj->x2720.w.flags = flags & ~0x8000000;
                    } else {
                        __asm__ volatile ("vadda.xyz ACC, $vf25, $vf0");
                        obj->x2720.w.flags = flags | 0x8000000;
                    }
                    __asm__ volatile ("vmadday.xyz ACC, $vf11, $vf22y\n\t"
                                      "vmaddaw.xyz ACC, $vf13, $vf21w\n\t"
                                      "vmaddx.xyz $vf11, $vf24, $vf22x");
                    VU_STORE("$vf12", dst1);
                    VU_STORE("$vf11", dst0);
                    if (is8006) {
                        RAND_STEP();
                        ((unsigned int *)dst0->f)[3] = (D_0029DB60__p >> 9) & 1;
                    } else if (randW) {
                        dst0->f[3] = (obj->x27CC - obj->x27C8) * Rand01__p() + obj->x27C8;
                    } else if (is800A) {
unsigned int bits; IntFloat__p t;
RAND_STEP(); bits = D_0029DB60__p;
type = ((bits >> 9) & 0x7FFFFF) | 0x40800000;
t.i = type; t.f -= 6.0f; type = t.i;
type = ((unsigned int)type & ~0xF) | ((bits >> 5) & 0xF);
((int *)dst0->f)[3] = type;
                    } else {
                        dst0->f[3] = obj->x27C8;
                    }
                    if (first) {
                        VU_LOAD("$vf30", dst0);
                        VU_LOAD("$vf31", dst0);
                        first = 0;
                    } else {
                        __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tvmini.xyzw $vf30, $vf30, $vf11\n\tvmax.xyzw $vf31, $vf31, $vf11" : : "r"(dst0));
                    }
                    if (hasLife) {
                        obj->x2838[i] = obj->x27DA * obj->x2728 * Rand01__p();
                        if (is8006) {
                            obj->x2838[i] = obj->x27DA - obj->x2838[i];
                            dst1->f[3] = 1.0f / obj->x2838[i];
                        } else {
                            dst1->f[3] = obj->x2838[i];
                        }
                    } else if (isType3) {
                        obj->x2838[i] = 0;
                    } else {
                        obj->x2838[i] = alpha;
                        if (is8006) {
                            dst1->f[3] = invAlpha;
                        } else {
                            dst1->f[3] = alpha;
                        }
                    }
                    if (obj->x27C0 & 0x20) {
                        obj->x2500[i] = (obj->x2500[i - 1] & 2) ? 0 : 2;
                    } else {
                        RAND_STEP();
                        v = (D_0029DB60__p >> 16) & 3;
                        obj->x2500[i] = (v == obj->x2500[i - 1]) ? ((v + 1) & 3) : v;
                    }
                    if (is8002) {
                        ((unsigned int *)dst0->f)[3] = obj->x2500[i] | (((unsigned int *)dst0->f)[3] & ~3);
                    }
                    if (obj->x2770 & 0x2000) {
                        obj->x2500[i] = D_0029DBB8[obj->x276E++ & 7];
                    }
                    pt.f[0] += step;
                }
                func_00223628(&obj->x2610, &obj->x2620);
                obj->x285A = n;
                obj->x2850 += n;
            }
        }
    }
    obj->x2720.w.x2724 = 0;
}
/* ---- merged from src/unsorted/0022A168.c ---- */
typedef struct TuneA__c3 {
    char pad0[0x14C];
    float x14C;
    char pad150[0xC];
    float x15C;
    float x160;
    float x164;
    char pad168[0x4];
    float x16C;
} TuneA__c3;

typedef struct TuneB__c3 {
    char pad0[0x198];
    float x198;
    float x19C;
} TuneB__c3;

/* Player state block at Obj__c3+0x2630 (fields named by offset within it). */
typedef struct State__c3 {
    char pad0[0x8];
    float x8;
    char padC[0x4C];
    float x58;
    char pad5C[0xC];
    float x68;
    char pad6C[0x88];
    short xF4;
    char padF6[0x6];
    float xFC;
    char pad100[0xC];
    float x10C;
    float x110;
    float x114;
    char pad118[0x4];
    float x11C;
    char pad120[0x10];
    float x130;
    float x134;
    char pad138[0x4];
    unsigned short x13C;
    char pad13E[0x1A];
    float x158;
    char pad15C[0x3C];
    float x198;
    float x19C;
} State__c3;

typedef struct Obj__c3 {
    char pad0[0x2630];
    State__c3 x2630;
} Obj__c3;

extern float D_0027A658__c3[] asm("D_0027A658");
extern TuneA__c3 D_0027BFC0__c3 asm("D_0027BFC0");
extern TuneB__c3 D_0027D300__c3 asm("D_0027D300");

extern void func_0022A168__c3(Obj__c3 *obj) asm("func_0022A168");
void func_0022A168__c3(Obj__c3 *obj)
{
    State__c3 *st = &obj->x2630;
    float t, t2, k, s, s20, v;

    if (st->xF4 < 2 && st->x13C == 5) {
        __asm__ volatile ("min.s %0,%1,%2" : "=f"(t) : "f"(obj->x2630.x130), "f"(1.0f));
        t2 = t * t;
        k = t2 * (((t2 - t) - t + 1.0f) * 16.0f);
        s = k + 1.0f;
        s20 = s * 20.0f;
        obj->x2630.x158 = D_0027A658__c3[0] * s;
        obj->x2630.xFC = D_0027BFC0__c3.x14C * k;
        obj->x2630.x10C = D_0027BFC0__c3.x15C * s20;
        obj->x2630.x11C = D_0027BFC0__c3.x16C * s20;
        obj->x2630.x198 = D_0027D300__c3.x198 * s;
        obj->x2630.x19C = D_0027D300__c3.x19C * s;
        obj->x2630.x110 = D_0027BFC0__c3.x160 * s;
        obj->x2630.x114 = D_0027BFC0__c3.x164 * s;
        if (obj->x2630.x130 > 0.5f) {
            v = obj->x2630.x68 + obj->x2630.x134 * 60.0f;
            obj->x2630.x68 = v;
            obj->x2630.x8 = v;
            obj->x2630.x58 = v;
        }
    }
}
/* ---- merged from src/unsorted/0022A280.c ---- */
typedef struct { float x0; int x4; int x8; float xC; } Params__c4;
typedef struct { char pad0[0x26F0]; Params__c4 x26F0; char pad2700[0x2C]; float x272C; char pad2730[0x120]; int x2850; char pad2854[6]; short x285A; } Obj__c4;
extern void func_0022A280__c4(Obj__c4 *obj) asm("func_0022A280");
void func_0022A280__c4(Obj__c4 *obj) {
    Params__c4 *p = &obj->x26F0;
    p->x0 = 1.0f;
    p->xC = 0.6754903f;
    p->x8 = 0;
    p->x4 = 0;
    obj->x2850 = 0;
    obj->x285A = (int)obj->x272C;
}

void func_0022A2C0__p(Emitter__p *obj)
{
    Vec v0, v10, v20, v30, v40;
    Vec *dst, *vel, *a, *b;
    float *pair;
    float inv, delta, range, r, root;
    int i, n;
    if (obj->x285A == 0) return;
    v0.q = obj->x26F0.q;
    i = obj->x2850;
    v10.f[0] = -v0.f[1];
    v10.f[1] = v0.f[0];
    dst = obj->x2830 + i;
    a = dst + obj->x2854 + 1;
    v10.f[2] = 0.0f;
    v10.f[3] = -0.7373688817024231f;
    b = dst + (obj->x2854 + 1) * 2;
    vel = obj->x2834 + i;
    pair = (float *)(obj->x2834 + obj->x2854);
    pair += i * 2;
    VU_LOAD("$vf20", &v0);
    VU_LOAD("$vf21", &v10);
    inv = 1.0f / obj->x27DA;
    delta = 1.0f / (obj->x27DA * (1.0f - obj->x2728)) - inv;
    v30.f[2] = inv - delta;
    v30.f[3] = delta;
    VU_LOAD("$vf29", &v30);
    if (obj->x276C == 0x3A) {
        v20.f[0] = -0.5f;
        v20.f[1] = Rand01__p();
        v20.f[2] = 1.0f / obj->x272C;
        v20.f[3] = -1.5f;
        ((Vec *)((char *)obj + 0x2690))->f[3] = 0.5f;
        VU_LOAD("$vf22", &obj->x26A0);
        VU_LOAD("$vf23", &obj->x26B0.r[0]);
        VU_LOAD("$vf24", &obj->x26B0.r[2]);
        VU_LOAD("$vf25", (char *)obj + 0x2690);
        VU_LOAD("$vf28", &v20);
        __asm__ volatile ("vmulw.xyz  $vf26, $vf23, $vf23w
	vmulw.xyz  $vf27, $vf24, $vf24w
	vmulw.xyz  $vf23, $vf23, $vf22w
	vmulw.xyz  $vf24, $vf24, $vf22w
	vadda.xyz  ACC, $vf25, $vf0
	vmaddaw.xyz ACC, $vf26, $vf25w
	vmaddw.xyz $vf25, $vf27, $vf25w");
        VU_STORE("$vf25", (char *)obj + 0x2600);
        ((Vec *)((char *)obj + 0x2600))->f[3] = (obj->x273C + obj->x2730 + obj->x27C8 * 3.0f) * 0.5f;
    } else {
        v20.f[0] = obj->x2754;
        v20.f[1] = Rand01__p();
        v20.f[2] = obj->x2740;
        v20.f[3] = obj->x2744 - obj->x2740;
        if (obj->x276C == 0x48) {
            VU_LOAD("$vf22", *(Mat **)((char *)obj + 0x2738));
            VU_LOAD("$vf23", &(*(Mat **)((char *)obj + 0x2738))->r[1]);
            VU_LOAD("$vf24", &(*(Mat **)((char *)obj + 0x2738))->r[2]);
            __asm__ volatile ("vsub.xyzw $vf26, $vf0, $vf0");
        } else {
            VU_LOAD("$vf22", &obj->x26B0.r[0]);
            VU_LOAD("$vf23", &obj->x26B0.r[1]);
            VU_LOAD("$vf24", &obj->x26B0.r[2]);
            VU_LOAD("$vf26", &obj->x26A0);
        }
        VU_LOAD("$vf25", (char *)obj + 0x2690);
        VU_LOAD("$vf28", &v20);
        if (obj->x2850) {
            __asm__ volatile ("lqc2 $vf30, 0(%0)\n\tlqc2 $vf31, 0(%1)" : : "r"(&obj->x2610), "r"(&obj->x2620));
        } else {
            __asm__ volatile ("lqc2 $vf30, 0(%0)\n\tlqc2 $vf31, 0(%0)" : : "r"(&obj->x2630));
        }
    }
    range = obj->x27CC - obj->x27C8;
    __asm__ volatile ("vrinit     R, $vf28y
	vmr32.w    $vf18, $vf0");
    for (i = 0; i < obj->x285A; i++, dst++, a++, b++, vel++, pair += 2) {
        __asm__ volatile ("vmove.xyzw $vf19, $vf20
	vmulaw.xyz ACC, $vf20, $vf21w
	vmsubw.xyz $vf20, $vf21, $vf19w
	vmulaw.xyz ACC, $vf21, $vf21w
	vmaddw.xyz $vf21, $vf19, $vf19w");
        r = Rand01__p() * range;
        __asm__ volatile ("sqc2 $vf20, 0(%1)" : "=m"(*a) : "r"(a) : "memory");
        __asm__ volatile ("sqc2 $vf21, 0(%1)" : "=m"(*b) : "r"(b) : "memory");
        a->f[3] = obj->x27C8 + r;
        b->f[3] = obj->x27CC - r;
        if (obj->x276C == 0x3A) {
            __asm__ volatile ("vrnext.y   $vf28, R
	vaddz.x    $vf28, $vf28, $vf28z
	vaddw.y    $vf28, $vf28, $vf28w
	vadda.xyz  ACC, $vf25, $vf0
	vmaddax.xyz ACC, $vf26, $vf28x
	vmaddy.xyz $vf18, $vf27, $vf28y
	vmula.xy   ACC, $vf28, $vf28
	vmadd.xy   $vf29, $vf28, $vf28
	vadda.xyz  ACC, $vf22, $vf0
	vmsubax.xyz ACC, $vf22, $vf29x
	vmsubay.xyz ACC, $vf22, $vf29y
	vmaddax.xyz ACC, $vf23, $vf28x
	vmaddy.xyz $vf19, $vf24, $vf28y");
            r = Rand01__p() * obj->x2744;
        } else {
            func_00222C38(&v40, obj->x2730);
            v40.f[3] = v20.f[2] + Rand01__p() * v20.f[3];
            VU_LOAD("$vf11", &v40);
            r = Rand01__p() * obj->x273C;
            __asm__ volatile ("vmulax.xyz ACC, $vf22, $vf11x
	vmadday.xyz ACC, $vf23, $vf11y
	vmaddz.xyz $vf12, $vf24, $vf11z
	vaddax.xyz ACC, $vf25, $vf0x
	vmaddx.xyz $vf18, $vf12, $vf28x
	vaddax.xyz ACC, $vf26, $vf0x
	vmaddw.xyz $vf19, $vf12, $vf11w
	vmini.xyz  $vf30, $vf30, $vf18
	vmax.xyz   $vf31, $vf31, $vf18");
        }
        __asm__ volatile ("vrnext.y   $vf28, R
	vmulaz.w   ACC, $vf0, $vf29z
	vmaddy.w   $vf19, $vf29, $vf28y");
        __asm__ volatile ("sqrt.s %0, %1" : "=&f"(root) : "f"(1.0f - r * r));
        pair[0] = root;
        root = a->f[3];
        if (root <= b->f[3]) { r = -r; pair[1] = r; }
        else pair[1] = r;
        VU_STORE("$vf18", dst);
        VU_STORE("$vf19", vel);
    }
    if (obj->x276C != 0x3A) func_00223628(&obj->x2610, &obj->x2620);
    VU_STORE("$vf20", &obj->x26F0);
    obj->x2850 += obj->x285A;
    obj->x285A = 0;
}

/* ---- merged from src/unsorted/0022A8A0.c ---- */
typedef unsigned int u128__c5 __attribute__((mode(TI)));
typedef struct Obj__c5 { char pad0[0x2630]; float x2630[4]; char pad2640[0x40]; float x2680[4]; char pad2690[0x10]; float x26A0[4]; char pad26B0[0x50]; float x2700[4]; float *x2710; float *x2714; char pad2718[0xC]; short x2724; char pad2726[6]; float x272C; int x2730; float *x2734; char pad2738[0x34]; unsigned short x276C; char pad276E[0xAE]; unsigned int x281C; } Obj__c5;
typedef struct Table__c5 { float x0,x4,x8,xC; char pad10[0x30]; } Table__c5;
extern Table__c5 D_0027BFC0__c5[] asm("D_0027BFC0");
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, %1\n\tsq $2, %0" : "=m"(dst) : "m"(src) : "$2")
extern void func_0022A8A0__c5(Obj__c5 *p) asm("func_0022A8A0");
void func_0022A8A0__c5(Obj__c5 *p) {
 float temp[4] __attribute__((aligned(16))); float length; float *diff; float *pos;
 *(u128__c5 *)p->x2680=*(u128__c5 *)p->x2630;
 __asm__ volatile("lqc2 $vf11, 0(%2)\n\tlqc2 $vf12, 0(%1)\n\tlqc2 $vf13, 16(%1)\n\tlqc2 $vf14, 32(%1)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(temp[0]) : "r"(p->x2714),"r"(p->x2734));
 pos=p->x2630;
 __asm__ volatile("lqc2 $vf11, %1\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*pos) : "m"(temp[0]),"r"(p->x2710));
 diff=p->x26A0;
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(p->x26A0) : "r"(pos),"r"(p->x2680));
 __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0" : "=&f"(length) : "f"(p->x26A0[0]),"f"(diff[1]),"f"(diff[2]));
 p->x26A0[3]=length;
 if(length>1.6f) p->x2724=1;
 p->x272C=0.0f;
 if(p->x2724==0) {
 if(p->x26A0[3]>0.00390625f && (p->x2710[3]==1.0f || p->x276C==31)) {
 __asm__ volatile("lqc2 $vf11, 0(%2)\n\tmfc1 $2, %1\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(p->x2700) : "f"(p->x2734[3]),"r"(p->x2714+8) : "$2");
 p->x272C=D_0027BFC0__c5[p->x276C].xC*p->x26A0[3];
 }
 } else p->x2724=(unsigned short)p->x2724-1;
 p->x281C=p->x276C==70?0x20405060:0xFF507090;
}
typedef struct {
    char pad0[0x2610];
    Vec x2610, x2620;
    char pad2630[0x50];
    Vec x2680;
    char pad2690[0x10];
    Vec x26A0;
    char pad26B0[0x50];
    Vec x2700;
    void *x2710;
    Mat *x2714;
    char pad2718[8];
    int x2720;
    char pad2724[8];
    float x272C;
    char pad2730[0x38];
    float x2768;
    unsigned short x276C;
    char pad276E[2];
    int x2770;
    char pad2774[0xA8];
    unsigned int x281C;
    char pad2820[0x10];
    Vec *x2830, *x2834;
    unsigned char (*x2838)[4];
    char pad283C[0x14];
    int x2850, x2854;
    short x2858, x285A;
} Emitter__aa28;
typedef struct {
    char pad0[0x448];
    float x448;
    char pad44C[0x10];
    float x45C, x460, x464, x468, x46C, x470, x474;
} Params__aa28;
typedef struct {
    char pad0[0x558];
    float x558, x55C;
    char pad560[0xA];
    short x56A;
    float x56C;
} Type__aa28;
typedef struct {
    char pad0[0x560];
    Vec x560;
} Wind__aa28;
extern Params__aa28 D_0027BFC0__aa28 asm("D_0027BFC0");
extern Type__aa28 D_0027D300__aa28 asm("D_0027D300");
extern Wind__aa28 D_0027A4B0__aa28 asm("D_0027A4B0");
extern Vec D_0027BD30;
extern void func_0022A8A0__aa28(Emitter__aa28 *) asm("func_0022A8A0");

/* Vector operations use the game's VU0 inline helpers. */
void func_0022AA28(Emitter__aa28 *obj, int count)
{
    Vec offset, step, velocity, direction, wind, position, random, jitter;
    unsigned int color;
    int alternating, always;
    float alpha, spread, baseAlpha, minSize, sizeRange;
    float invCount, length, lifeSpread, baseLife, jitterSize;
    Vec *pos, *vel;
    unsigned char (*rgba)[4];
    int first, total, remaining;
    Vec *stepPtr, *jitterPtr;
    int green, blue;

    if (obj->x276C == 70 || obj->x276C == 31)
        func_0022A8A0__aa28(obj);
    color = obj->x281C;
    alpha = ((float)(int)(color >> 24) + 1.0f) * 0.00390625f;
    minSize = D_0027D300__aa28.x558;
    sizeRange = D_0027D300__aa28.x55C - minSize;
    spread = D_0027BFC0__aa28.x474 * alpha;
    baseAlpha = alpha - spread;
    offset.q = 0;
    alternating = (obj->x2720 >> 22) & 1;
    always = (obj->x2720 >> 14) & 1;
    obj->x2768 += obj->x272C / (minSize + sizeRange * (baseAlpha + spread * 0.5f));
    if (alternating || always) {
        obj->x2768 += obj->x2768;
        VuApplyMatrix(&offset, obj->x2714, &D_0027BD30);
    }
    obj->x285A = (int)obj->x2768;
    obj->x2768 -= obj->x285A;
    if (obj->x285A + obj->x2850 > obj->x2854)
        obj->x285A = obj->x2854 - obj->x2850;
    if (obj->x285A == 0)
        return;
    invCount = 1.0f / (float)count;
    VuScale(&step, &obj->x26A0, 1.0f / (float)obj->x285A);
    VuScale(&direction, &obj->x2700, invCount * obj->x26A0.f[3] * D_0027BFC0__aa28.x45C);
    VuScale(&velocity, &obj->x26A0, invCount * D_0027BFC0__aa28.x468);
    velocity.f[3] = D_0027BFC0__aa28.x460;
    if (obj->x2720 & 0x8000) {
        __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
            : "=&f"(length) : "f"(velocity.f[0]), "f"(velocity.f[1]), "f"(velocity.f[2]));
        if (D_0029DBA8 < length)
            return;
    }
    __asm__ volatile ("lqc2 $vf30, 0(%0)
	lqc2 $vf31, 0(%1)" : : "r"(&obj->x2610), "r"(&obj->x2620));
    wind.q = 0;
    if (obj->x2770 & 0x400)
        VuAddScaled(&wind, &D_0027A4B0__aa28.x560, &D_0027A4A0__p, D_0027A4B0__aa28.x560.f[3]);
    jitterSize = D_0027BFC0__aa28.x470;
    lifeSpread = (float)D_0027D300__aa28.x56A * D_0027BFC0__aa28.x448;
    baseLife = (float)D_0027D300__aa28.x56A * (1.0f - D_0027BFC0__aa28.x448);
    pos = obj->x2830 + obj->x2850;
    vel = obj->x2834 + obj->x2850;
    rgba = obj->x2838 + obj->x2850;
    position.q = obj->x2680.q;
    first = obj->x2850 == 0;
    total = obj->x285A;
    VU_LOAD("$vf27", &direction);
    VU_LOAD("$vf28", &wind);
    VU_LOAD("$vf29", &velocity);
    for (remaining = 0; remaining < total;) {
        float a, size;
        a = baseAlpha + spread * Rand01__p();
        size = minSize + sizeRange * a;
        stepPtr = &step;
        VuAdd(&position, &position, stepPtr);
        func_00222C38(&random, 1.0f);
        jitterPtr = &jitter;
        VuScale(jitterPtr, &random, jitterSize);
        VuAdd(pos, &position, jitterPtr);
        if ((alternating && (obj->x2720 & 0x08000000)) || always) {
            VuAdd(pos, pos, &offset);
            obj->x2720 &= ~0x08000000;
        } else {
            obj->x2720 |= 0x08000000;
        }
        pos->f[2] += D_0027D300__aa28.x56C * size;
        pos->f[3] = 0.0f;
        (*rgba)[0] = color;
        green = (color >> 8) & 255;
        (*rgba)[1] = green;
        RAND_STEP();
        blue = (color >> 16) & 0xFC;
        (*rgba)[2] = (unsigned char)((D_0029DB60__p >> 16) & 3) | blue;
        (*rgba)[3] = (int)(a * 255.0f);
        if (first) {
            VU_LOAD("$vf30", pos);
            VU_LOAD("$vf31", pos);
            first = 0;
        } else {
            __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tvmini.xyzw $vf30,$vf30,$vf11\n\tvmax.xyzw $vf31,$vf31,$vf11" : : "r"(pos));
        }
        random.f[3] = size;
        __asm__ volatile ("lqc2 $vf12, 0(%1)\n\tvsuba.xyz ACC,$vf29,$vf28\n\tvmaddaw.xyz ACC,$vf27,$vf12w\n\tvmaddw.xyz $vf11,$vf12,$vf29w\n\tsqc2 $vf11,%0"
            : "=m"(*vel) : "r"(&random));
        vel->f[3] = 1.0f / (baseLife + lifeSpread * a);
        remaining++;
        pos++;
        rgba++;
        vel++;
    }
    obj->x2850 += total;
    obj->x285A = 0;
    func_00223628(&obj->x2610, &obj->x2620);
}

INCLUDE_ASM("asm/nonmatchings/text", func_0022B028);
/* ---- merged from src/unsorted/0022B1F0.c ---- */
typedef unsigned int u128__c6 __attribute__((mode(TI)));
struct Object__c6 { char pad[0x2830]; int x2830, x2834, x2838; char pad2[0x14]; int x2850; };
#if defined(__mips__)
extern void func_0022B028__c6(int, int, int, int *, int, float *) asm("func_0022B028");
#else
#include "particle_update.h"
#endif
extern void func_00223628__c6(void *, void *) asm("func_00223628");
extern void func_0022B1F0__c6(struct Object__c6 *obj, int arg1, float velocityScale, float value, float last) asm("func_0022B1F0");
void func_0022B1F0__c6(struct Object__c6 *obj, int arg1, float velocityScale, float value, float last) {
    union { u128__c6 zero; float v[4]; } values;
    values.zero = 0;
    values.v[0] = value;
    values.v[1] = value;
    values.v[2] = value;
    values.v[3] = last;
    if (obj->x2850) {
#if defined(__mips__)
        func_0022B028__c6(obj->x2830, obj->x2834, obj->x2838, &obj->x2850, arg1, values.v);
#else
        func_0022B028((void *)obj->x2830, (void *)obj->x2834, (void *)obj->x2838, &obj->x2850, (const float *)arg1, values.v, velocityScale);
#endif
        func_00223628__c6((char *)obj + 0x2610, (char *)obj + 0x2620);
    }
}
#ifdef NONMATCHING
/* Non-matching: loop-carried outVel pointer and its +0x10 successor swap $s1/$s2 (local-alloc priority);
   best diff 5/187 words */
/* Particle pool of a D_0067E180 emitter (stride 0x2860), cap = x2854 slots:
 *   x2830[k]            position, .f[3] = normalised age (>= 1.0 means expired)
 *   x2830[cap+1+k]      orientation axis A
 *   x2830[2*(cap+1)+k]  orientation axis B
 *   x2834[k]            velocity (.z gets gravity * dt added each update)
 *   x2834 + 16*cap + 8k { float cos, float sin } spin step; the int view of
 *                       .sin selects which axis is rotated */
typedef struct ParticlePool__b258 {
    char pad0[0x2610];
    Vec boundsMin;              /* 0x2610 */
    Vec boundsMax;              /* 0x2620 */
    char pad2630[0xF0];
    int x2720;                  /* flags */
    short x2724;                /* update delay counter */
    char pad2726[0x3A];
    float x2760;                /* accumulated time */
    float x2764;                /* time scale */
    char pad2768[0x28];
    float x2790;                /* gravity */
    char pad2794[0x9C];
    Vec *x2830;                 /* positions (+ axis vectors after x2854 entries) */
    Vec *x2834;                 /* velocities (+ spin pairs after x2854 entries) */
    char pad2838[0x18];
    int x2850;                  /* live particle count */
    int x2854;                  /* capacity */
} ParticlePool__b258;

/* func_0022B258: advance all particles by `steps` ticks, drop expired ones and
 * compact the survivors to the front; then recompute the bounding box. */
void func_0022B258(ParticlePool__b258 *obj, int steps) {
    int cap, n;
    float dt;
    Vec *pos, *axisA, *axisB, *outPos, *outAxisA, *outAxisB;
    float *spinIn, *vel, *outVel, *spinOut;
    int *spinSel;

    if (obj->x2724 >= 2) {
        obj->x2724 = (unsigned short)obj->x2724 - 1;
        return;
    }

    cap = obj->x2854;
    pos = obj->x2830;
    vel = (float *)obj->x2834;
    spinIn = vel + 4 * cap;
    dt = (float)steps;
    obj->x2760 = obj->x2760 + obj->x2764 * dt;
    axisA = pos + cap + 1;
    axisB = pos + 2 * (cap + 1);
    outPos = pos;
    outAxisA = axisA;
    outAxisB = axisB;
    outVel = vel;
    spinOut = spinIn;
    VU_SETX("$vf18", obj->x2790 * dt);
    __asm__ volatile ("lqc2 $vf30, 0x0(%0)\n\tlqc2 $vf31, 0x0(%0)" : : "r"(pos));
    n = obj->x2850;

    while (n) {
        if (pos->f[3] >= 1.0f) {
            /* expired: drop it */
            obj->x2850--;
            if (obj->x2720 & 0x20000000)
                obj->x2720 = (obj->x2720 & 0xDFFFFFFF) | 0x01000000;
        } else {
            spinSel = (int *)spinIn;
            spinSel++;
            __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, 0x0(%1)" : : "r"(axisA), "r"(axisB));
            __asm__ volatile ("lqc2 $vf14, 0x0(%0)" : : "r"(pos));
            __asm__ volatile ("lqc2 $vf15, 0x0(%0)" : : "r"(vel));
            VU_SETX("$vf16", spinIn[0]);
            VU_SETX("$vf17", spinIn[1]);
            /* pos += vel; vel.z += gravity * dt */
            __asm__ volatile ("vadd.xyzw $vf14, $vf14, $vf15\n\tvaddx.z $vf15, $vf15, $vf18x");
            if (*spinSel > 0) {
                /* rotate axisB: b = b*cos + (a x b)*sin, `steps` times */
                { int j = steps;
                if (steps) do {
                    __asm__ volatile ("vopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf13, $vf12, $vf11\n\tvmulax.xyz ACC, $vf12, $vf16x\n\tvmaddx.xyz $vf12, $vf13, $vf17x");
                } while (--j);
                }
                if (outPos != pos) {
                    COPY_QUAD(*outAxisA, axisA);
                    spinOut[0] = spinIn[0];
                    spinOut[1] = spinIn[1];
                }
                __asm__ volatile ("sqc2 $vf12, 0x0(%0)" : : "r"(outAxisB) : "memory");
            } else {
                /* rotate axisA: a = a*cos + (a x b)*sin, `steps` times */
                { int j = steps;
                if (j) do {
                    __asm__ volatile ("vopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf13, $vf12, $vf11\n\tvmulax.xyz ACC, $vf11, $vf16x\n\tvmaddx.xyz $vf11, $vf13, $vf17x");
                } while (--j);
                }
                if (outPos != pos) {
                    COPY_QUAD(*outAxisB, axisB);
                    spinOut[0] = spinIn[0];
                    spinOut[1] = spinIn[1];
                }
                __asm__ volatile ("sqc2 $vf11, 0x0(%0)" : : "r"(outAxisA) : "memory");
            }
            __asm__ volatile ("sqc2 $vf14, 0x0(%0)" : : "r"(outPos) : "memory");
            __asm__ volatile ("sqc2 $vf15, 0x0(%1)" : "=m"(*(Vec *)outVel) : "r"(outVel) : "memory");
            /* grow the bounding box */
            __asm__ volatile ("vmini.xyzw $vf30, $vf30, $vf14\n\tvmax.xyzw $vf31, $vf31, $vf14");
            outPos++;
            outAxisA++;
            outAxisB++;
            outVel += 4;
            spinOut += 2;
        }
        /* triple nesting kept: it fixes sched2's decrement order */
        do {
            do {
                do { n--; } while (0);
            } while (0);
        } while (0);
        pos++;
        axisA++;
        axisB++;
        vel += 4;
        spinIn += 2;
    }
    func_00223628(&obj->boundsMin, &obj->boundsMax);
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0022B258);
#endif
typedef struct VecB__c13 {float x,y,z; unsigned int flags;} VecB__c13 __attribute__((aligned(16)));
typedef struct Obj__c13 {
 char p0[0x2610];Vec x2610,x2620;
 char p2630[0x2720-0x2630];unsigned int x2720;
 char p2724[0x2778-0x2724];float x2778;int x277c;
 char p2780[0x27da-0x2780];short x27da;
 char p27dc[0x2830-0x27dc];VecB__c13 *x2830,*x2834;unsigned short *x2838;
 char p283c[0x2850-0x283c];int x2850;
} Obj__c13;
extern void func_00223628__c13(Vec *,Vec *) asm("func_00223628");
extern void func_0022B548__c13(Obj__c13 *obj,Vec *offset,unsigned int mask,float dt) asm("func_0022B548");
void func_0022B548__c13(Obj__c13 *obj,Vec *offset,unsigned int mask,float dt) {
 float values[4] __attribute__((aligned(16)));float scale,inv;
 int count;
 VecB__c13 *pos=obj->x2830,*velocity=obj->x2834;unsigned short *life=obj->x2838;
 VecB__c13 *outPos=pos,*outVelocity=velocity;unsigned short *outLife=life;
 int first=1,n;unsigned int oldFlags,flags,keep;int left;
 Vec *min,*max;
 if(obj->x277c)scale=(float)(obj->x277c/obj->x27da);
 else scale=1.0f/(float)obj->x27da;
 inv=1.0f/scale;keep=~mask;
 values[0]=dt;values[1]=scale;values[2]=obj->x2778*inv;values[3]=1.0f-obj->x2778;
 __asm__ volatile("lqc2 $vf28,%0
	lqc2 $vf29,0(%1)" : : "m"(values[0]),"r"(offset));
 n=obj->x2850;
 __asm__ volatile("vsub.w $vf29,$vf0,$vf0" : : "r"(velocity));
 min=&obj->x2610;max=&obj->x2620;
 if(n>0) {count=n;do {
 oldFlags=pos->flags;
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vadda.xyzw ACC,$vf11,$vf29
	vmaddx.xyzw $vf11,$vf12,$vf28x
	vaddax.w ACC,$vf0,$vf0x
	vmsubz.w $vf13,$vf11,$vf28z
	sqc2 $vf11,%0" : "=m"(*outPos) : "r"(pos),"r"(velocity));
 left=*life-1;*life=left;
 if((int)(left<<16)<=0) {
 obj->x2850--;flags=obj->x2720;
 if(flags&0x20000000)obj->x2720=(flags&0xdfffffff)|0x01000000;
 }else {
 *outLife=left;
 __asm__ volatile("vmax.w $vf13,$vf13,$vf28
	vmulw.xyz $vf12,$vf12,$vf13w");
 outPos->flags=(outPos->flags&keep)|(oldFlags&mask);
 __asm__ volatile("sqc2 $vf12,%0" : "=m"(*outVelocity));
 if(first) {__asm__ volatile("vaddx.xyzw $vf30,$vf11,$vf0x
	vaddx.xyzw $vf31,$vf11,$vf0x");first=0;}
 else {__asm__ volatile("vmini.xyzw $vf30,$vf30,$vf11
	vmax.xyzw $vf31,$vf31,$vf11");}
 outPos++;outVelocity++;outLife++;
 }
 count--;pos++;velocity++;life++;
 }while(count);}
 func_00223628__c13(min,max);
}
INCLUDE_ASM("asm/nonmatchings/text", func_0022B708);
/* ---- merged from src/unsorted/0022BEF8.c ---- */
typedef struct Obj__c12 {
    char pad0[0x2600];
    Vec x2600;
    Vec x2610;
    Vec x2620;
    char pad2630[0x124];
    float x2754;
    char pad2758[0x70];
    float x27C8;
    float x27CC;
    float x27D0;
    float x27D4;
    short x27D8;
    short x27DA;
    char pad27DC[0x34];
    unsigned int x2810;
    char pad2814[0x3C];
    int x2850;
} Obj__c12;

void func_0022BEF8(Obj__c12 *obj)
{
    Vec delta;
    float length;

    if (obj->x2850 > 0) {
        VuAdd(&obj->x2600, &obj->x2610, &obj->x2620);
        VuScale(&obj->x2600, &obj->x2600, 0.5f);
        VuSub__p(&delta, &obj->x2620, &obj->x2610);
        switch (obj->x2810 & ~0x70U) {
        case 1:      delta.f[3] = 0.0f; break;
        case 0x8003: delta.f[3] = obj->x27CC + obj->x27D4 + obj->x2754; break;
        case 0x8006: delta.f[3] = obj->x27C8 + obj->x27D4 * obj->x27DA; break;
        case 4:
        case 0x800A: delta.f[3] = obj->x27CC; break;
        case 11:     delta.f[3] = obj->x27CC + obj->x27D4 * obj->x27DA; break;
        case 0x800C: delta.f[3] = obj->x27CC; break;
        default:     delta.f[3] = obj->x2620.f[3]; break;
        }
        __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0"
                          : "=&f"(length) : "f"(delta.f[0]), "f"(delta.f[1]), "f"(delta.f[2]));
        obj->x2600.f[3] = length * 0.5f + delta.f[3];
    }
}
/* ---- merged from src/unsorted/0022C080.c ---- */
typedef struct { char pad[0x2760]; float x2760; char gap0[0x30]; float x2794,x2798,x279C; char gap1[0x14]; float x27B4,x27B8; char gap2[0x40]; float x27FC; char gap3[0x2C]; int x282C; } Obj__c7;
extern unsigned long D_0029DB60__c7 asm("D_0029DB60");
extern void func_0022C080__c7(Obj__c7 *obj) asm("func_0022C080");
void func_0022C080__c7(Obj__c7 *obj) {
 if(obj->x282C & 0x04000000) {
  if(obj->x2760 > obj->x279C) obj->x282C &= ~0x04000000;
  D_0029DB60__c7=D_0029DB60__c7*0x41C64E6D+12345;
  if((D_0029DB60__c7 & 255)>=128U || obj->x27FC < obj->x2794) {
   obj->x27FC+=obj->x27B4;
   if(obj->x2798 < obj->x27FC) obj->x27FC=obj->x2798;
  } else {
   if(obj->x2794+obj->x27B4 < obj->x27FC) obj->x27FC-=obj->x27B4;
  }
 } else {
  if(obj->x2760 <= obj->x279C) obj->x27FC=obj->x2794+obj->x27B4*obj->x2760;
  else obj->x27FC=obj->x2798+obj->x27B8*(obj->x2760-obj->x279C);
 }
 if(obj->x27FC <= 0.0f) obj->x282C=0;
}
/* ---- merged from src/unsorted/0022C1C0.c ---- */
struct Value__c8 { char pad[12]; float xC; };
struct Obj__c8 { char pad[0x2710]; struct Value__c8 *x2710; char pad2714[12]; int x2720; short x2724, x2726; char pad2728[0x44]; unsigned short x276c; char pad276e[0xa2]; int x2810; char pad2814[0x18]; int x282c; char pad2830[0x20]; int x2850; char pad2854[4]; unsigned short x2858; char pad285a[2]; signed char x285c,x285d,x285e; };
struct Entry__c8 { char pad[6]; unsigned short x6; char pad8[0x38]; };
extern struct Entry__c8 D_0027BFC0__c8[] asm("D_0027BFC0");
extern void func_002293C8__c8(struct Obj__c8 *) asm("func_002293C8");
extern int func_0022C1C0__c8(struct Obj__c8 *p, int ticks) asm("func_0022C1C0");
int func_0022C1C0__c8(struct Obj__c8 *p, int ticks) {
 int mode = p->x2810 & 0xFFFFFF8F;
 int flags;
 if (mode == 0x8005) {
  if (!(p->x2720 & 4)) return 0;
  p->x2726 -= ticks;
  return p->x2726 <= 0;
 }
 if (mode == 0x8003) {
  if ((p->x2720 & 4) && p->x2726 <= 0 && p->x285c < 2 && p->x285d < 2 && p->x285e < 2) {
   func_002293C8__c8(p); return 1;
  }
 }
 flags=p->x2720;
 if (flags & 0x10000000) {
  if (p->x2710->xC == 0.0f && p->x2858 == 0) {
   if (p->x2726 > 0) p->x2726 -= ticks;
   else goto clear;
  } else {
   struct Entry__c8 *entry = &D_0027BFC0__c8[p->x276c];
   p->x2726=entry->x6;
   p->x2720 |= 8;
  }
 } else if ((flags & 4) && p->x2724 == 0) {
  p->x2726 -= ticks;
  if (p->x2726 < 0) {
clear:
   p->x2720=flags & 0xFFFFFFF7;
  }
 }
 if (p->x2850 || (p->x2720 & 8)) return 0;
 p->x282c=0;
finish:
 func_002293C8__c8(p);
 return 1;
}
typedef union Vector__c350 { u128 q; float f[4]; } Vector__c350;
extern int func_0023D608(void);
extern void *func_0023CF58(int);
extern void *func_0023CF78(int);
#define COPY_VECTOR_C350(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
unsigned char func_0022C350(Vec *pos, float radius) {
    Vector__c350 origin, offset, point, delta;
    int i = 0;
    int count;
    void *matrix;
    void *translation;
    Vector__c350 *orig = &origin;
    float distance;
    origin.q = pos->q;
    count = func_0023D608();
    offset.q = 0;
    offset.f[1] = radius * 0.5f;
    if (count > 0) do {
        matrix = func_0023CF58(i);
        __asm__ volatile (
            "lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 16(%2)\n\tlqc2 $vf14, 32(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0"
            : "=m"(point) : "r"(&offset), "r"(matrix) : "memory");
        translation = func_0023CF78(i);
        __asm__ volatile (
            "lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
            : "=m"(point) : "r"(translation), "r"(&point) : "memory");
        __asm__ volatile (
            "lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
            : "=m"(delta) : "r"(orig), "r"(&point) : "memory");
        __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
            : "=&f"(distance) : "f"(delta.f[0]),"f"(delta.f[1]),"f"(delta.f[2]));
        if (distance < radius) return 1;
        i++;
    } while (i < count);
    return 0;
}
typedef struct Vec__w1 { float x, y, z, w; } Vec__w1 __attribute__((aligned(16)));

typedef struct Node__w1 {           /* D_0071F980__w1 free/used list entry */
    int used;
    int prev;
    int next;
} Node__w1;

typedef struct Entry__w1 {          /* D_0027BFC0__w1: 0x40 bytes */
    unsigned int flags;
    unsigned short x4;
    short x6;
    char pad8[4];
    float xC;
    char pad10[4];
    Vec__w1 *x14;
    char pad18[8];
    float x20;
    float x24;
    char pad28[8];
    float x30;
    float x34;
    unsigned int x38;
    char pad3C[4];
} Entry__w1;

typedef struct Big__w1 {            /* D_0027A4B0__w1: 0x50 bytes, 16-aligned */
    char pad0[0xC];
    int xC;
    char pad10[0x14];
    float x24;
    float x28;
    float x2C;
    float x30;
    char pad34[0x10];
    float x44;
    float x48;
    float x4C;
} Big__w1 __attribute__((aligned(16)));

typedef struct Scale__w1 {          /* D_0027D300__w1: 0x50 bytes */
    unsigned int flags;
    char pad4[4];
    float x8;
    float xC;
    float x10;
    float x14;
    short x18;
    unsigned short x1A;
    char pad1C[0xC];
    float x28;
    float x2C;
    char pad30[0xC];
    float x3C;
    char pad40[8];
    float x48;
    char pad4C[4];
} Scale__w1;

typedef struct Info__w1 {           /* D_00279B00__w1: 0x20 bytes */
    unsigned int x0;
    char pad4[8];
    int xC;
    float x10;
    int x14;
    int x18;
    int x1C;
} Info__w1;

typedef struct Obj__w1 {            /* D_0067E180__w1: 0x2860 bytes */
    char pad0[0x2710];
    int x2710;
    int x2714;
    int x2718;
    int x271C;
    Entry__w1 entry;                /* 0x2720 */
    float x2760;
    float x2764;
    float x2768;
    short x276C;
    short x276E;
    Big__w1 big;                    /* 0x2770 */
    Scale__w1 scale;                /* 0x27C0 */
    Info__w1 info;                  /* 0x2810 */
    char pad2830[0x10];
    int x2840;
    int x2844;
    int x2848;
    int x284C;
    int x2850;
    char pad2854[8];
    char x285C;
    char x285D;
    char x285E;
    char pad285F[1];
} Obj__w1;



extern int D_0029E308__w1 asm("D_0029E308");
extern int D_0029E30C__w1 asm("D_0029E30C");
extern Node__w1 D_0071F980__w1[] asm("D_0071F980");
extern Obj__w1 D_0067E180__w1[] asm("D_0067E180");
extern Entry__w1 D_0027BFC0__w1[] asm("D_0027BFC0");
extern Big__w1 D_0027A4B0__w1[] asm("D_0027A4B0");
extern Scale__w1 D_0027D300__w1[] asm("D_0027D300");
extern Info__w1 D_00279B00__w1[] asm("D_00279B00");
extern int func_0023D608__w1(void) asm("func_0023D608");
extern void func_0022A280__w1(Obj__w1 *) asm("func_0022A280");

int func_0022C468(int index, float scale)
{
    Obj__w1 *obj;
    int slot;
    int n;
    float f7;
    float f8;
    float f;

    if (D_0029DBA5 != 0) {
    n = func_0023D608__w1();
    if ((unsigned)((D_00279B00__w1[index].x0 & 0x70) >> 4) < (unsigned)n) {
        return -1;
    }
    slot = D_0029E30C__w1;
    if (slot == -1) {
        return -1;
    }
    D_0029E30C__w1 = D_0071F980__w1[slot].next;
    if (D_0071F980__w1[slot].next != -1) {
        D_0071F980__w1[D_0071F980__w1[slot].next].prev = -1;
    }
    if (D_0029E308__w1 != -1) {
        D_0071F980__w1[D_0029E308__w1].prev = slot;
    }
    D_0071F980__w1[slot].next = D_0029E308__w1;
    D_0071F980__w1[slot].prev = -1;
    D_0071F980__w1[slot].used = 1;
    D_0029E308__w1 = slot;
    } else {
        return -1;
    }

    obj = &D_0067E180__w1[slot];
    obj->entry = D_0027BFC0__w1[index];
    obj->big = D_0027A4B0__w1[index];
    obj->scale = D_0027D300__w1[index];
    obj->info = D_00279B00__w1[index];

    if (scale != 1.0f) {
        f7 = 1.0f;
        if (obj->entry.flags & 0x20) {
            __asm__ volatile ("sqrt.s %0, %1" : "=f"(f7) : "f"(scale));
        }
        __asm__ volatile ("sqrt.s %0, %1" : "=f"(f8) : "f"(scale));
        if (obj->info.x1C & 0x3000000) {
            obj->scale.x3C = obj->scale.x3C * f7;
            obj->big.x28 = obj->big.x28 * f7;
        }
        obj->big.xC = (int)((float)obj->big.xC * f7);
        obj->entry.x6 = (short)((float)obj->entry.x6 * f7);
        if (obj->entry.flags & 0x80) {
            obj->entry.x20 = obj->entry.x20 * f8;
            obj->entry.x24 = obj->entry.x24 * f8;
        }
        if (obj->entry.flags & 0x10) {
            obj->entry.xC = obj->entry.xC * f7;
        } else if (obj->entry.flags & 0x40) {
            obj->entry.xC = obj->entry.xC / f7;
        }
        if (obj->entry.flags & 0x100) {
            obj->scale.x18 = (short)((float)obj->scale.x18 * f7);
            obj->scale.x1A = (short)((float)(short)obj->scale.x1A * f7);
            obj->big.xC = (int)((float)obj->big.xC * f7);
            obj->scale.x8 = obj->scale.x8 * scale;
            obj->scale.xC = obj->scale.xC * scale;
            obj->scale.x10 = obj->scale.x10 * f8;
            obj->scale.x14 = obj->scale.x14 * f8;
        }
        if (obj->entry.flags & 0x200) {
            obj->entry.x30 = obj->entry.x30 * scale;
            obj->entry.x34 = obj->entry.x34 * scale;
        }
    }
    if (obj->big.xC > 0) {
        obj->big.x4C = 1.0f / (float)obj->big.xC;
    }
    obj->x2840 = 0;
    obj->x2850 = 0;
    obj->x285E = 0;
    obj->x285D = 0;
    obj->x285C = 0;
    obj->x2710 = 0;
    obj->x2714 = 0;
    obj->x2718 = 0;
    obj->x271C = 0;
    obj->x2760 = 0;
    obj->x2764 = 1.0f / (float)(obj->entry.x6 + ((short)obj->scale.x1A >> 1));
    if (obj->entry.xC < 1.0f) {
        f = 1.0f - obj->entry.xC;
    } else {
        f = obj->x2760;
    }
    obj->x2768 = f;
    obj->x276C = index;
    obj->x276E = 0;
    obj->x2844 = 0;
    obj->x2848 = 0;
    obj->x284C = 0;
    obj->scale.x48 = (obj->scale.x28 - obj->scale.x2C) / (float)(short)obj->scale.x1A;
    if (obj->info.x1C & 0x3000000) {
        if (obj->info.x1C & 0x4000000) {
            obj->scale.x3C = 1.0f;
            obj->big.x44 = (obj->big.x28 - obj->big.x24) * 0.125f;
        } else {
            obj->big.x44 = (obj->big.x28 - obj->big.x24) / obj->big.x2C;
        }
        obj->big.x48 = -obj->big.x28 / obj->big.x30;
    }
    if (obj->entry.x6 != 0) {
        obj->entry.flags = obj->entry.flags | 4;
    } else {
        obj->entry.flags = obj->entry.flags & ~4;
    }
    if ((obj->info.x0 & 0xFFFFFF8F) == 4) {
        func_0022A280__w1(obj);
    }
    return slot;
}

/* ---- merged from src/unsorted/0022CAB0.c ---- */
typedef struct Obj__c9 { char pad0[0x2810]; unsigned int x2810; } Obj__c9;
extern void func_00225068__c9(Obj__c9 *) asm("func_00225068");
extern void func_00224CC8__c9(Obj__c9 *) asm("func_00224CC8");
extern void func_00226658__c9(Obj__c9 *) asm("func_00226658");
extern void func_00225550__c9(Obj__c9 *) asm("func_00225550");
extern void func_002257F8__c9(Obj__c9 *) asm("func_002257F8");
extern void func_00227618__c9(Obj__c9 *) asm("func_00227618");
extern void func_00227A60__c9(Obj__c9 *) asm("func_00227A60");
extern void func_00225B20__c9(Obj__c9 *) asm("func_00225B20");
extern void func_00226FA8__c9(Obj__c9 *) asm("func_00226FA8");
extern void func_00227320__c9(Obj__c9 *) asm("func_00227320");
extern void func_0022CAB0__c9(Obj__c9 *obj) asm("func_0022CAB0");
void func_0022CAB0__c9(Obj__c9 *obj) {
 switch(obj->x2810 & 0xFFFFFF8F) {
 case 0x8002: func_00225068__c9(obj); break;
 case 1: func_00224CC8__c9(obj); break;
 case 11: func_00226658__c9(obj); break;
 case 4: func_00225550__c9(obj); break;
 case 0x800A: func_002257F8__c9(obj); break;
 case 0x8003: func_00227618__c9(obj); break;
 case 0x800D: func_00227A60__c9(obj); break;
 case 0x8006: func_00225B20__c9(obj); break;
 case 0x8007: case 0x8008: func_00226FA8__c9(obj); break;
 case 0x800C: func_00227320__c9(obj); break;
 }
}
/* ---- merged from src/unsorted/0022CC08.c ---- */
typedef unsigned int u128__c10 __attribute__((mode(TI)));
typedef struct Particle__c10 { char pad0[0x2600]; u128__c10 x2600; char pad1[0x80]; u128__c10 x2690; float x26A0[4]; char pad2[0x76]; short x2726; char pad3[0x20]; float x2748; char pad4[8]; float x2754; char pad5[0xD0]; unsigned int x2828; char pad6[0x34]; } Particle__c10;
extern int D_0027D800__c10[] asm("D_0027D800");
extern float D_00279D10__c10[] asm("D_00279D10");
extern Particle__c10 D_0067E180__c10[] asm("D_0067E180");
extern unsigned char func_0022C350__c10(float *, float) asm("func_0022C350");
extern int func_0022C468__c10(int, float) asm("func_0022C468");
extern void func_002226C8__c10(void *, float *) asm("func_002226C8"), func_00221F20__c10(void *) asm("func_00221F20");
extern void func_00225550__c10(Particle__c10 *) asm("func_00225550"), func_0022A280__c10(Particle__c10 *) asm("func_0022A280"), func_0022A2C0__c10(Particle__c10 *) asm("func_0022A2C0"), func_002256F0__c10(Particle__c10 *) asm("func_002256F0");
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static __inline__ void Scale(float *out, float *in, float scale) {
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*(u128__c10 *)out) : "r"(in), "f"(scale) : "$2");
}
static __inline__ float Normalize(float *out, float *v) {
 float x,y,z,length,inv;
 x=v[0]; y=v[1]; z=v[2];
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(length) : "f"(x), "f"(y), "f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(length));
 out[0]=x*inv; out[1]=y*inv; out[2]=z*inv;
 return length;
}
extern int func_0022CC08__c10(u128__c10 *pos, float *dir, unsigned int color, float scale) asm("func_0022CC08");
int func_0022CC08__c10(u128__c10 *pos, float *dir, unsigned int color, float scale) {
 float temp[4];
 u128__c10 copy;
 int index;
 Particle__c10 *p;
 float x,y,z,length,inv;
 if (D_0027D800__c10[0] & 0x4000) {
  *(u128__c10 *)temp = *pos;
  if ((func_0022C350__c10(temp, D_00279D10__c10[0]) ^ 1) & 0xFF) return -1;
 }
 index = func_0022C468__c10(16, 1.0f);
 if (index == -1) return -1;
 p = &D_0067E180__c10[index];
 Scale(p->x26A0, dir, p->x2748 * 0.012222222052514553f);
 p->x2754 *= scale;
 copy = *pos;
 p->x2600 = copy;
 ((float *)&p->x2600)[3] = p->x2754;
 p->x2690 = copy;
 length=Normalize(temp,p->x26A0);
 if(length>0.000001f) func_002226C8__c10((char *)p+0x26B0,temp);
 else func_00221F20__c10((char *)p+0x26B0);
 p->x2828=(color & 0xFFFFFF)|0xFF000000;
 func_00225550__c10(p); func_0022A280__c10(p); p->x2726=0; func_0022A2C0__c10(p); func_002256F0__c10(p);
 return index;
}
/* ---- merged from src/unsorted/0022CDC0.c ---- */
typedef unsigned int u128__c11 __attribute__((mode(TI)));
typedef ParticleQuadMatrix Mat__c11;
struct Type__c11 {int flags;char pad[0x4c];};
struct Info__c11 {char pad[0x10];float x10;char pad2[12];};
struct Offset__c11 {char pad[0x14];void *x14;char pad2[0x28];};
typedef Particle Obj__c11;
extern struct Type__c11 D_0027D300__c11[] asm("D_0027D300");
extern struct Info__c11 D_00279B00__c11[] asm("D_00279B00");
extern struct Offset__c11 D_0027BFC0__c11[] asm("D_0027BFC0");
extern Obj__c11 D_0067E180__c11[] asm("D_0067E180");
extern int func_0022C350__c11(u128__c11 *,float) asm("func_0022C350");
extern int func_0022C468__c11(int,float) asm("func_0022C468");
extern void func_00222C38__c11(u128__c11 *,float) asm("func_00222C38"),func_002226C8__c11(void *,void *) asm("func_002226C8"),func_0022CAB0__c11(void *) asm("func_0022CAB0");
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
int func_0022CDC0(Vector4 *pos,int index,Matrix4 *matrix,int value,float scale,Vector4 *extra) {
 u128__c11 temp;int result;Obj__c11 *obj;Mat__c11 *dest;u128__c11 *newpos;
 if(index[D_0027D300__c11].flags&0x4000) {temp=*(u128__c11 *)pos;if(((func_0022C350__c11(&temp,D_00279B00__c11[index].x10)^1)&0xff)!=0)return -1;}
 result=func_0022C468__c11(index,scale);if(result==-1)return -1;
 obj=&D_0067E180__c11[result];
 if(value) {obj->x2828=value;obj->x27C0|=0x100;}
 if(extra) {obj->x2720=(obj->x2720&0xfffff3ff)|0x200000;obj->x2724=0;obj->x2700=*(u128__c11 *)extra;}
 newpos=&obj->x2690;
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*newpos) : "r"(pos),"r"(D_0027BFC0__c11[index].x14) : "memory");
 COPY_VECTOR(obj->x2630,&obj->x2690);COPY_VECTOR(obj->x2680,&obj->x2690);COPY_VECTOR(obj->x2600,&obj->x2690);
 obj->x2734=&obj->x2690;
 if(obj->x2720&0x40000) {dest=&obj->matrix;func_00222C38__c11(&temp,0.5f);func_002226C8__c11(dest,&temp);obj->x2738=dest;obj->x2720|=0x400;}
 else if(matrix) {Mat__c11 *dest=&obj->matrix;__asm__ volatile ("lq $8, 0x0(%4)
	lq $9, 0x10(%4)
	lq $10, 0x20(%4)
	lq $11, 0x30(%4)
	sq $8, %0
	sq $9, %1
	sq $10, %2
	sq $11, %3" : "=m"(dest->row[0]), "=m"(dest->row[1]), "=m"(dest->row[2]), "=m"(dest->row[3]) : "r"(matrix) : "$8", "$9", "$10", "$11");obj->x2738=&obj->matrix;obj->x2720|=0x400;}
 func_0022CAB0__c11(obj);return result;
}
typedef unsigned int u128__cfb8 __attribute__((mode(TI)));
typedef ParticleQuadMatrix Mat__cfb8;
typedef Particle Particle__cfb8;
struct Type__cfb8 { int flags; char pad[0x4C]; };
struct Size__cfb8 { char pad[0x10]; float radius; char pad2[0xC]; };
struct Default__cfb8 { char pad[0x14]; u128__cfb8 *pos; char pad2[0x28]; };
extern struct Type__cfb8 D_0027D300__cfb8[] asm("D_0027D300");
extern struct Size__cfb8 D_00279B00__cfb8[] asm("D_00279B00");
extern struct Default__cfb8 D_0027BFC0__cfb8[] asm("D_0027BFC0");
extern Particle__cfb8 D_0067E180__cfb8[] asm("D_0067E180");
extern unsigned char func_0022C350__cfb8(float *, float) asm("func_0022C350");
extern int func_0022C468__cfb8(int, float) asm("func_0022C468");
extern void func_00222C38__cfb8(float *, float) asm("func_00222C38");
extern void func_002226C8__cfb8(Mat__cfb8 *, float *) asm("func_002226C8");
extern void func_0022CAB0__cfb8(Particle__cfb8 *) asm("func_0022CAB0");
/* Spawns a particle of `type`; with `direct` the dir/matrix pointers are kept, otherwise copied. */
int func_0022CFB8(Vector4 *pos, void *arg1, int type, Vector4 *dir, float scale, Matrix4 *matrix, int direct, int color) {
 float temp[4]; int index; Particle__cfb8 *p;
 if (D_0027D300__cfb8[type].flags & 0x4000) {
  *(u128__cfb8 *)temp = *(u128__cfb8 *)pos;
  if ((func_0022C350__cfb8(temp, D_00279B00__cfb8[type].radius) ^ 1) & 0xFF) return -1;
 }
 index = func_0022C468__cfb8(type, scale);
 if (index == -1) return -1;
 p = &D_0067E180__cfb8[index];
 if (color) { p->x2828 = color; p->x27C0 |= 0x100; }
 p->x2710 = pos; p->x2714 = arg1;
 p->x2720 |= 0x800;
 p->x2734 = &p->x2690;
 if (dir) {
  if (direct) p->x2734 = dir;
  else { COPY_VECTOR(p->x2690, dir); }
 } else { COPY_VECTOR(p->x2690, D_0027BFC0__cfb8[type].pos); }
 if (matrix) {
  if (direct) p->x2738 = matrix;
  else {
   if (p->x2720 & 0x40000) {
    Mat__cfb8 *m = &p->matrix;
    func_00222C38__cfb8(temp, 0.5f);
    func_002226C8__cfb8(m, temp);
   } else {
    /* sceVu0CopyMatrix; the source rows as "m" operands (4 refs to matrix) keep it in $s0 */
    Mat__cfb8 *dest = &p->matrix;
    __asm__ volatile ("lq $8, %1\n\tlq $9, %2\n\tlq $10, %3\n\tlq $11, %4\n\tsq $8, %0\n\tsq $9, 16(%5)\n\tsq $10, 32(%5)\n\tsq $11, 48(%5)"
     : "=m"(p->matrix)
     : "m"(((Mat__cfb8 *)matrix)->row[0]), "m"(((Mat__cfb8 *)matrix)->row[1]), "m"(((Mat__cfb8 *)matrix)->row[2]), "m"(((Mat__cfb8 *)matrix)->row[3]), "r"(dest)
     : "$8", "$9", "$10", "$11", "memory");
   }
   p->x2738 = &p->matrix;
  }
 }
 func_0022CAB0__cfb8(p);
 return index;
}

#if defined(__mips__)
int func_0022D1B0(Vec *pos, Mat *mat, Desc *d, Vec *ofs)
#else
int func_0022D1B0(struct Vector4 *position, struct Matrix4 *matrix, ParticleEmitterDescription *d, struct Vector4 *offset)
#endif
{
#if !defined(__mips__)
    Vec *pos=(Vec *)position; Mat *mat=(Mat *)matrix; Vec *ofs=(Vec *)offset;
#endif
    Mat rot;
    Mat world;
    Vec p;
    Vec angles;
    Vec s;
    Vec c;
    Vec dir;
    float saved_x8, saved_x38, scale, k, f, size;
    int index, type, n, flags, kind;
    Obj *o;
    Vec *newpos;
    Mat *dest;
    float *v;

    if (d->type == 0x39) return -1;
    if (d->type == 0x4C) {
        angles.f[0] = d->x28 + 1.57079995f;
        angles.f[1] = d->x24;
        angles.f[2] = 0;
        angles.f[3] = 0;
    } else {
        angles.f[0] = d->x28;
        angles.f[1] = d->x24;
        angles.f[2] = 0;
        angles.f[3] = 0;
    }
    VuSinCos(&angles, &s, &c);
    dir.f[0] = s.f[1] * c.f[0];
    dir.f[1] = c.f[1] * c.f[0];
    dir.f[2] = s.f[0];
    func_002226C8(&rot, &dir);
    func_00221930(&world, &rot, mat);
    VuAdd(&p, pos, (Vec *)&d->x10);
    VuApplyMatrix(&p, mat, &p);
    if (ofs) VuAdd(&p, &p, ofs);

    type = d->type;
    if (D_0027D300[type].flags & 0x4000) {
        if ((func_0022C350(&p, D_00279B00[type].x10) ^ 1) & 0xFF) return -1;
    }

    saved_x8 = d->x8;
    saved_x38 = d->x38;
    if (d->type == 0x38) {
        d->x8 = 1.0f;
        d->x38 = 1.0f;
    }
    scale = 1.0f;
    if (d->x8 != 0.0f) scale = d->x8;
    index = func_0022C468(d->type, scale);
    if (index == -1) return -1;
    o = &D_0067E180[index];
    if (o->x27C0 & 0x8000) {
        if (D_0029DBB0 < 0x100) D_0071FD30[D_0029DBB0++] = d;
    } else {
        o->x284C = d;
    }
    o->x2726 = d->x30;

    switch (d->type) {
    case 0x3A:
        COPY_MTX4(&o->x26B0, mat);
        VuAdd(&o->x2690, pos, (Vec *)&d->x10);
        o->x2730 = d->x20;
        o->x273C = d->x38;
        if (d->x2C > 0.0f && d->x2C < 128.0f) o->x272C = d->x2C;
        if (d->x8 > 0.0f) {
            o->x27C8 = d->x8 * 0.5f;
            o->x27CC = d->x8 * 3.0f;
        }
        if (d->xC > 0.0f) o->x2740 = d->xC;
        o->x26B0.r[0].f[3] = o->x2730;
        o->x26B0.r[2].f[3] = o->x273C;
        k = FSqrt(o->x273C * o->x2730 / o->x272C);
        o->x27CC *= k;
        o->x27C8 *= k;
        VuScale(&o->x26A0, &o->x26B0.r[1], o->x2740);
        o->x26A0.f[3] = o->x2740;
        func_00225550(o);
        func_0022A280(o);
        o->x2726 = 0;
        func_0022A2C0(o);
        func_002256F0(o);
        return index;
    case 5:
        v = o->x2780;
        v[0] = -0.5f;
        v[1] = -1.0f;
        v[2] = 3.0f;
        v[3] = 2.0f;
        o->x2820 = 2500.0f;
        o->x272C *= 0.25f;
        break;
    case 0x40:
        D_0029DB74 = 2.0f;
        D_0029DB78 = 1.5f;
        D_0029DB80 = 5.5f;
        break;
    case 0x24:
    case 0x2F:
    case 0x36:
        o->x2726 = D_0027BFC0[d->type].x6;
        break;
    }

    f = d->x38;
    if (f > 0.0f) {
        o->x274C = f;
        o->x273C = f;
    }
    if (d->x3C) o->x27DA = d->x3C;
    if (d->x40 != 0.0f) o->x2790 = d->x40 * -0.0170000009f;
    if (d->x44 > 0.0f) o->x2778 = d->x44;
    n = d->x48;
    if (n) {
        o->x277C = n;
        o->x27BC = 1.0f / n;
    }
    if (d->x4C > 0.0f) o->x27D0 = d->x4C;
    if (d->x50 != 0.0f) o->x27D4 = d->x50;
    if (d->x54) o->x27D8 = d->x54;
    if (d->x34) o->x2724 = d->x34;
    if (o->x2726 > 0) {
        flags = o->x2720;
        if (!(flags & 0x40000000)) {
            o->x2726 = D_0027BFC0[d->type].x6;
            o->x2720 = flags | 0x10000004;
            o->x2764 = 1.0f / (o->x2726 + (o->x27DA >> 1));
        }
    } else {
        o->x2764 = 0.0f;
        o->x2720 = (o->x2720 & 0xFFFFFFFB) | 0x20000000;
        if (!(d->x58.l & 0x100000000)) {
            o->x2600.q = pos->q;
            o->x2600.f[3] = (o->x2744 + o->x27D4) * o->x27DA + o->x27CC + o->x2754;
        }
    }
    if (d->xC > 0.0f) {
        o->x2740 = d->xC * (D_0027BFC0[d->type].x20 / D_0027BFC0[d->type].x24);
        o->x2744 = d->xC;
    }
    if (d->x20 > 0.0f) o->x2730 = (1.0f - func_002487A0(d->x20 * 0.0174532924f)) * 0.5f;
    if (d->x2C > 0.0f) o->x272C = d->x2C;
    dest = &o->x26B0;
    o->x2710 = d->xA0;
    o->x2734 = &o->x2690;
    o->x2738 = dest;
    newpos = o->x2734;
    if (d->x58.l & 0x100000000) {
        o->x2714 = &d->x60;
        COPY_MTX4(dest, &rot);
        COPY_QUAD(o->x2690, &d->x10);
        o->x2720 |= 0x800;
    } else {
        COPY_MTX4(dest, &world);
        COPY_QUAD(o->x2690, &p);
        o->x2720 |= 0x400;
        COPY_QUAD(o->x2630, newpos);
        COPY_QUAD(o->x2680, newpos);
    }

    kind = d->type;
    if (kind == 0x38) {
        if (d->x3C) o->x27DA = d->x3C * 0.670000017f;
        o->x27C8 = D_0027D300[0x38].x8;
        o->x27D0 = D_0027D300[0x38].x10;
        o->x2750 = D_0027BFC0[0x38].x30;
        o->x27CC = D_0027D300[0x38].xC;
        o->x27D4 = D_0027D300[0x38].x14;
        o->x2754 = D_0027BFC0[0x38].x34;
        o->x273C = D_0027BFC0[0x38].x1C;
        d->x8 = saved_x8;
        d->x38 = saved_x38;
        o->x27F0 = FSqrt(saved_x8);
        o->x27F4 = FSqrt(saved_x38);
        o->x2600.f[3] = (o->x2744 + o->x27D4) * o->x27DA + o->x27CC + o->x2754;
    } else if (kind == 0x3C || kind == 0x3D) {
        o->x2740 = d->xC * 0.5f;
        o->x2744 = o->x2740 * 1.10000002f;
        o->x2690.f[2] -= o->x2744 * 8.0f;
        o->x27DA = o->x2744 * 120.0f;
        o->x2680.q = o->x2690.q;
        o->x2630.q = o->x2690.q;
    } else if (kind == 0x3B) {
        VuAddScaled(&o->x2690, newpos, &o->x26B0.r[1], 5.0f);
    } else if (kind == 0x4C) {
        if (d->x3C) o->x27DA *= 60;
        if (d->x54) o->x27D8 *= 60;
        if (d->x48) {
            o->x277C *= 60;
            o->x27BC = 1.0f / o->x277C;
        }
        if (d->x4C != 0.0f) o->x27D0 *= 0.0166666675f;
        if (d->x50 != 0.0f) o->x27D4 *= 0.0166666675f;
        if (d->x2C != 0.0f) o->x272C *= 0.0166666675f;
        if (d->xC != 0.0f) o->x2780[2] = d->xC * 0.0166666675f;
        size = d->x38 == 0.0f ? 1.0f : d->x38;
        o->x2740 = D_0027BFC0[0x4C].x20 * size;
        o->x2744 = D_0027BFC0[0x4C].x24 * size;
        o->x2750 = D_0027BFC0[0x4C].x30 * size;
        o->x2754 = D_0027BFC0[0x4C].x34 * size;
        o->x273C = 0.0f;
        o->x274C = 0.0f;
        o->x2600.f[3] = (o->x27D4 - o->x2740) * o->x27DA + o->x27CC + o->x2754;
        o->x2690.f[2] += o->x27C8 * 0.400000006f;
        if (d->x58.f != 0.0f) o->x282B = FMin(o->x282B * d->x58.f, 255.0f);
    }
    func_0022CAB0(o);
    return index;
}

extern char D_0067E180__0022DD88[] asm("D_0067E180");
extern void *func_0022DD88__0022DD88(int index) asm("func_0022DD88");
void *func_0022DD88__0022DD88(int index) {
    void *result;
    if (index > 0) result = D_0067E180__0022DD88 + index * 0x2860;
    else result = 0;
    return result;
}

struct Link__0022DDB0 {
    struct Obj__0022DDB0 *owner;
};

struct Obj__0022DDB0 {
    char pad0[0x2848];
    struct Link__0022DDB0 *x2848;
};

extern void func_0022DDB0__0022DDB0(struct Obj__0022DDB0 *obj) asm("func_0022DDB0");
void func_0022DDB0__0022DDB0(struct Obj__0022DDB0 *obj) {
    struct Link__0022DDB0 *link = obj->x2848;

    if (link != 0 && link->owner == obj) {
        link->owner = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text", func_0022DDD0);

struct Obj__0022DDD8 { char pad[0x2720]; unsigned int flags; short x2724, x2726; };
extern char D_0067E180__0022DDD8[] asm("D_0067E180");
extern char D_0071F980__0022DDD8[] asm("D_0071F980");
extern void func_002293C8__0022DDD8(struct Obj__0022DDD8 *) asm("func_002293C8");
extern void func_0022DDD8__0022DDD8(struct Obj__0022DDD8 *obj) asm("func_0022DDD8");
void func_0022DDD8__0022DDD8(struct Obj__0022DDD8 *obj) {
 if ((unsigned int)obj >= (unsigned int)D_0067E180__0022DDD8 && (unsigned int)obj < (unsigned int)D_0071F980__0022DDD8) {
 obj->x2726 = 0;
 obj->flags = (obj->flags | 4) & 0xEFFFFFFF;
 func_002293C8__0022DDD8(obj);
 }
}

INCLUDE_ASM("asm/nonmatchings/text", func_0022DE38);

typedef struct Vec__tail { float x, y, z, w; } Vec__tail __attribute__((aligned(16)));
typedef struct Node__tail { int used; int prev; int next; } Node__tail;
typedef struct Value__tail { char pad0[0xC]; int xC; } Value__tail;
typedef struct Obj__tail {
    char pad0[0x2600];
    Vec__tail x2600;
    char pad2610[0x20];
    Vec__tail x2630;
    char pad2640[0x40];
    Vec__tail x2680;
    char pad2690[0x80];
    Value__tail *x2710;
    char pad2714[0xC];
    union { unsigned int x2720; unsigned long x2720_64; } flags;
    char pad2728[0x1C];
    float x2744;
    char pad2748[0xC];
    float x2754;
    char pad2758[0x14];
    unsigned short x276C;
    char pad276E[0x52];
    unsigned int x27C0;
    char pad27C4[0x8];
    float x27CC;
    char pad27D0[0x4];
    float x27D4;
    char pad27D8[0x2];
    short x27DA;
    char pad27DC[0x34];
    unsigned int x2810;
    char pad2814[0x18];
    unsigned int x282C;
    char pad2830[0x20];
    int x2850;
    char pad2854[0x4];
    unsigned short x2858;
    char pad285A[0x6];
} Obj__tail;

extern int D_0029E308__tail asm("D_0029E308");
extern int D_0029E30C__tail asm("D_0029E30C");
extern Obj__tail D_0067E180__tail[] asm("D_0067E180");
extern Node__tail D_0071F980__tail[] asm("D_0071F980");

extern int func_00238F60__tail(void) asm("func_00238F60");
extern void func_00229508__tail(Vec__tail *, Vec__tail *) asm("func_00229508");
extern int func_0022C1C0__tail(Obj__tail *, int) asm("func_0022C1C0");
extern void func_0022DDB0__tail(Obj__tail *) asm("func_0022DDB0");
extern void func_0022B708__tail(Obj__tail *, int) asm("func_0022B708");
extern void func_0022AA28__tail(Obj__tail *, int) asm("func_0022AA28");
extern void func_00239178__tail(Obj__tail *) asm("func_00239178");
extern void func_0023EFB8__tail(Obj__tail *, int) asm("func_0023EFB8");
extern void func_002295D0__tail(Obj__tail *, int) asm("func_002295D0");
extern void func_0022BEF8__tail(Obj__tail *) asm("func_0022BEF8");
extern void func_0022C080__tail(Obj__tail *) asm("func_0022C080");
extern void func_00225458__tail(Obj__tail *) asm("func_00225458");
extern void func_00224F58__tail(Obj__tail *) asm("func_00224F58");
extern void func_002268E0__tail(Obj__tail *) asm("func_002268E0");
extern void func_00225A40__tail(Obj__tail *) asm("func_00225A40");
extern void func_00227840__tail(Obj__tail *) asm("func_00227840");
extern void func_00227C60__tail(Obj__tail *) asm("func_00227C60");
extern void func_002256F0__tail(Obj__tail *) asm("func_002256F0");
extern void func_00225CE8__tail(Obj__tail *) asm("func_00225CE8");
extern void func_00227310__tail(Obj__tail *) asm("func_00227310");
extern void func_00226B90__tail(Obj__tail *) asm("func_00226B90");

#undef COPY_VECTOR
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define LOAD_VECTOR(src) __asm__ volatile ("lq $2, %0" : : "m"(src) : "$2")
#define STORE_VECTOR(dst) __asm__ volatile ("sq $2, %0" : "=m"(dst))

void func_0022DE40(void)
{
    int var_s3;
    int temp_s1;
    int temp_s4;
    int temp_v0;
    int temp_v0_2;
    int temp_v0_3;
    int temp_v0_4;
    unsigned int temp_v1;
    unsigned int temp_v1_2;
    unsigned int temp_v1_4;
    unsigned int temp_v1_5;
    unsigned short temp_v1_3;
    int temp_v0_5;
    Obj__tail *temp_s0;
    Value__tail *temp_v1_6;
    Vec__tail temp;
    float temp_f0;
    float temp_f1;
    float temp_f2;
    float temp_f3;
    float temp_f4;

    if (D_0029DBA5 == 0) return;
    D_0029DBAC += 1;
    temp_s4 = func_00238F60__tail();
    var_s3 = D_0029E308__tail;
    if (var_s3 == -1) return;

    do {
        temp_s1 = var_s3;
        temp_s0 = &D_0067E180__tail[var_s3];
        if (var_s3 != -1) var_s3 = D_0071F980__tail[var_s3].next;
        temp_v1 = temp_s0->flags.x2720;

        if ((temp_v1 & 0x01000000) != 0 && temp_s0->x2858 == 0) {
            if ((temp_v1 & 0xC00) != 0) {
                if ((temp_v1 & 0x1000) == 0) {
                    COPY_VECTOR(temp_s0->x2680, &temp_s0->x2630);
                }
                func_00229508__tail(&temp_s0->x2630, &temp);
                temp_s0->flags.x2720 = temp_s0->flags.x2720 & 0xFFFFFBFF;
            }
            if (func_0022C1C0__tail(temp_s0, temp_s4) != 0) {
                if (temp_s1 != -1 && D_0071F980__tail[temp_s1].used != 0) {
                    temp_v0 = D_0071F980__tail[temp_s1].prev;
                    if (temp_v0 != -1) {
                        D_0071F980__tail[temp_v0].next = D_0071F980__tail[temp_s1].next;
                    } else {
                        D_0029E308__tail = D_0071F980__tail[temp_s1].next;
                    }
                    temp_v0_2 = D_0071F980__tail[temp_s1].next;
                    if (temp_v0_2 != -1) {
                        D_0071F980__tail[temp_v0_2].prev = D_0071F980__tail[temp_s1].prev;
                    }
                    if (D_0029E30C__tail != -1) {
                        D_0071F980__tail[D_0029E30C__tail].prev = temp_s1;
                    }
                    D_0071F980__tail[temp_s1].next = D_0029E30C__tail;
                    D_0071F980__tail[temp_s1].prev = -1;
                    D_0071F980__tail[temp_s1].used = 0;
                    D_0029E30C__tail = temp_s1;
                }
                func_0022DDB0__tail(temp_s0);
            }
        } else {
            func_0022B708__tail(temp_s0, temp_s4);
            if (func_0022C1C0__tail(temp_s0, temp_s4) != 0) {
                if (temp_s1 != -1 && D_0071F980__tail[temp_s1].used != 0) {
                    temp_v0_3 = D_0071F980__tail[temp_s1].prev;
                    if (temp_v0_3 != -1) {
                        D_0071F980__tail[temp_v0_3].next = D_0071F980__tail[temp_s1].next;
                    } else {
                        D_0029E308__tail = D_0071F980__tail[temp_s1].next;
                    }
                    temp_v0_4 = D_0071F980__tail[temp_s1].next;
                    if (temp_v0_4 != -1) {
                        D_0071F980__tail[temp_v0_4].prev = D_0071F980__tail[temp_s1].prev;
                    }
                    if (D_0029E30C__tail != -1) {
                        D_0071F980__tail[D_0029E30C__tail].prev = temp_s1;
                    }
                    D_0071F980__tail[temp_s1].next = D_0029E30C__tail;
                    D_0071F980__tail[temp_s1].prev = -1;
                    D_0071F980__tail[temp_s1].used = 0;
                    D_0029E30C__tail = temp_s1;
                }
                func_0022DDB0__tail(temp_s0);
            } else {
                if ((temp_s0->flags.x2720_64 & 0x100008UL) == 8) {
                    temp_v1_2 = temp_s0->x2810 & 0xFFFFFF8F;
                    switch (temp_v1_2) {
                    case 0xB:
                        func_0022AA28__tail(temp_s0, temp_s4);
                        break;
                    case 0x800C:
                        func_00239178__tail(temp_s0);
                        break;
                    case 0x8003:
                        temp_v1_3 = temp_s0->x276C;
                        if (temp_v1_3 == 8 || temp_v1_3 == 0x2D || temp_v1_3 == 0x37) {
                            func_0023EFB8__tail(temp_s0, temp_s4);
                            break;
                        }
                    default:
                        func_002295D0__tail(temp_s0, temp_s4);
                        break;
                    }
                }

                temp_v1_4 = temp_s0->x27C0;
                if ((temp_v1_4 & 0x600) != 0) {
                    if ((temp_v1_4 & 0x400) != 0) {
                        temp_f1 = temp_s0->x27D4;
                        temp_f0 = temp_s0->x2744;
                        temp_v0_5 = temp_s0->x27DA;
                        temp_f0 = temp_f0 + temp_f1;
                        temp_f3 = temp_s0->x27CC;
                        temp_f4 = (float)temp_v0_5;
                        temp_f2 = temp_s0->x2754;
                        LOAD_VECTOR(temp_s0->x2630);
                        temp_f0 = temp_f0 * temp_f4;
                        STORE_VECTOR(temp_s0->x2600);
                        temp_f0 = temp_f0 + temp_f3;
                        temp_f0 = temp_f0 + temp_f2;
                        temp_s0->x2600.w = temp_f0;
                    }
                } else {
                    func_0022BEF8__tail(temp_s0);
                }

                if ((temp_s0->x282C & 0x03000000) != 0) {
                    func_0022C080__tail(temp_s0);
                }

                if (temp_s0->x2850 != 0) {
                    temp_v1_5 = temp_s0->x2810 & 0xFFFFFF8F;
                    switch (temp_v1_5) {
                    case 0x8002:
                        func_00225458__tail(temp_s0);
                        break;
                    case 0x1:
                        func_00224F58__tail(temp_s0);
                        break;
                    case 0xB:
                        func_002268E0__tail(temp_s0);
                        break;
                    case 0x800A:
                        func_00225A40__tail(temp_s0);
                        break;
                    case 0x8003:
                        func_00227840__tail(temp_s0);
                        break;
                    case 0x800D:
                        func_00227C60__tail(temp_s0);
                        break;
                    case 0x4:
                        func_002256F0__tail(temp_s0);
                        break;
                    case 0x8006:
                        func_00225CE8__tail(temp_s0);
                        break;
                    case 0x8007:
                    case 0x8008:
                        func_00227310__tail(temp_s0);
                        break;
                    case 0x8005:
                        if ((temp_s0->x27C0 & 0x800) == 0) func_00226B90__tail(temp_s0);
                        break;
                    default:
                        break;
                    }
                }
            }

            temp_v1_6 = temp_s0->x2710;
            if (temp_v1_6 != 0) temp_v1_6->xC = 0;
        }
    } while (var_s3 != -1);
}

extern float D_0027EBB0[], D_0027EBC0[], D_0027BF00[], D_0027BF40[];
extern float D_0027A4A0__f[] asm("D_0027A4A0");

extern void func_00222800(float *);

static inline void scale_vector(float *out, float *in, float value)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)
	mfc1 $2,%2
	qmtc2.ni $2,$vf12
	vmulx.xyzw $vf11,$vf11,$vf12x
	sqc2 $vf11,%0"
        : "=m"(*out) : "r"(in), "f"(value) : "$2");
}

#define SIN_COS(in, sine, cosine) \
    __asm__ volatile ("lqc2 $vf14,0(%0)\n\tvcallms 0x978\n\tnop\n\tqmfc2.i $2,$vf14\n\tsqc2 $vf15,0(%2)\n\tsq $2,0(%1)" \
        : : "r"(in), "r"(sine), "r"(cosine) : "$2", "memory")

void func_0022E3D0(void) {
    float a[4] __attribute__((aligned(16)));
    float b[4] __attribute__((aligned(16)));
    float sine_a[4] __attribute__((aligned(16)));
    float sine_b[4] __attribute__((aligned(16)));
    float cosine_a[4] __attribute__((aligned(16)));
    float cosine_b[4] __attribute__((aligned(16)));
    float scale;

    if (D_0029DBA5) {
        scale = (float)(unsigned int)(D_0029DBAC & 0x7FFF);
        scale_vector(a, D_0027EBB0, scale);
        scale_vector(b, D_0027EBC0, scale);
        SIN_COS(a, sine_a, cosine_a);
        SIN_COS(b, sine_b, cosine_b);
        D_0027A4A0__f[0] = sine_a[0] * cosine_a[2] * sine_b[0] * 0.2f + 0.05f;
        D_0027A4A0__f[1] = cosine_a[1] * cosine_a[3] * sine_b[1] * 0.2f + 0.05f;
        func_00221A60(D_0027BF00, D_0027BF00, D_0027BF40);
        D_0027BF00[0] = 0.0f;
        D_0027BF00[4] = -0.96f;
        D_0027BF00[8] = 0.28f;
        func_00222800(D_0027BF00);
    }
}
