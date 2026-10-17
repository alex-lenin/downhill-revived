/* UNIT 0x0019C868 0x0019CFB0 .sdata=0x29D1A8:0x29D1AC */
#include "load_transition.h"
#include "render_wait.h"
#include "ui_region.h"
#include "include_asm.h"
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union { U128 value; struct { int x0,x4,x8,xC; } w; struct { unsigned short x0; unsigned short x2; int x4,x8,xC; } h; } Packet;
float D_0029D1A8 = 1.0f;
extern Packet D_0073A280[];
extern int D_0026F9A8[];
extern void func_0019C670(Packet *,int *,int);
extern void func_0023D650(int,int *,int *);
extern void func_002433A8(int,int);
extern void func_0025A960(int);
/* Builds a small GS packet (color, coordinates) in D_0073A280 and starts GIF DMA (channel 2). */
#if defined(__mips__)
void func_0019C868(int arg0,int index,int alpha,int arg3,int arg4,float x,float y,float width,float height)
#else
void SharedCall0019C868(int arg0, int index, int alpha, int arg3, int arg4, float x, float y, float width, float height)
#endif
{

    int count=0,screenX,screenY,oldCount;
    int left,top,right,bottom,u,v;
    float halfY,halfH,xEnd,yEnd;
    int leftRel,topRel,leftBias,topBias;
    Packet *p1,*p2,*p3,*p4,*p5,*tag;
    func_0019C670(D_0073A280,&count,D_0026F9A8[index]);
    oldCount=count;
    func_0023D650(0,&screenX,&screenY);
    left=(int)x;
    halfY=y*0.5f;
    top=(int)halfY;
    xEnd=x+width;
    right=(int)xEnd;
    halfH=height*0.5f;
    yEnd=halfY+halfH;
    bottom=(int)yEnd;
    u=(int)(D_0029D1A8*383.0f);
    v=(int)(D_0029D1A8*223.0f);
    screenX>>=1;
    screenY>>=1;
    p1=(Packet *)((int)D_0073A280 + (count << 4));
    p1->w.x0=127;
    p1->w.x4=127;
    p1->w.x8=127;
    p1->w.xC=alpha;
    leftRel=left-screenX;
    topRel=top-screenY;
    p2=(Packet *)((int)D_0073A280 + ((count+1) << 4));
    p2->w.x0=16;
    p2->w.x4=16;
    leftBias=leftRel+2048;
    topBias=topRel+2048;
    p3=(Packet *)((int)D_0073A280 + ((count+2) << 4));
    p3->w.x0=leftBias<<4;
    p3->w.x4=topBias<<4;
    p3->w.x8=0;
    p4=(Packet *)((int)D_0073A280 + ((count+3) << 4));
    p4->w.x0=u<<4;
    p4->w.x4=v<<4;
    p5=(Packet *)((int)D_0073A280 + ((count+4) << 4));
    p5->value=0;
    p5->w.x4=(bottom-screenY+2048)<<4;
    p5->w.x8=0;
    p5->w.x0=(right-screenX+2048)<<4;
    D_0073A280[0].value=0;
    D_0073A280[0].w.x0=(count+4)|0x70000000;
    tag=(Packet *)((int)D_0073A280 + ((oldCount-1)<<4));
    count+=5;
    tag->h.x0=0x8001;
    if (!arg3) tag->w.x4^=0x80000;
    if (!arg4) tag->w.x4^=0x200000;
    func_002433A8(0,0);
    func_0025A960(0);
    *(volatile int *)0x1000A020=0;
    *(Packet * volatile *)0x1000A030=D_0073A280;
    *(volatile int *)0x1000E010=4;
    *(volatile int *)0x1000A000=0x145;
}


/* func_0019CB08 */
typedef struct ObjCB08 { int x0, x4; } ObjCB08;

extern void func_00243D80(void *, int, int, int, int, int, int, int);
extern void func_002440A8(void *, int);

/* func_0019CBA8 */
typedef struct {
    char x0;
    char pad1[7];
    char x8;
    char x9;
} State;

typedef struct {
    char pad0[0xA8];
    int xA8;
} Flags;

typedef struct {
    int x0;
    int x4;
    char pad8[8];
} Result;

typedef struct {
    char pad0[0x240];
    char x240[0x5BE4 - 0x240];
    int x5BE4, x5BE8;
} World;

typedef struct {
    int x0;
    int x4;
    int x8;
} GlobalState;

typedef struct {
    char data[0x70];
} Loader;

/* func_0019CCE8 */
typedef struct ObjCCE8 { char pad[0x500]; int x500,x504; char x508; char pad1[0x18]; signed char x521; } ObjCCE8;
typedef struct Global { char pad[0x2b0]; int x2B0; char pad1[8]; int x2BC; } Global;

extern char D_0029D3F0;
extern int D_0029D3F4;
extern World D_00398C40;
extern char D_003F52D0[];
extern GlobalState D_0043EEC0;
extern Result D_004B1F40;
extern Flags D_004B2914;
extern int D_0043EEC8[], D_002B27D4[];
extern int D_0039E828[];
extern Global D_0037EA60;
extern char D_0043DD38[], D_002AF2C0[];


extern void func_001B4AF8(Loader *);
extern void func_001B5A38(Loader *);
extern void func_001B5BA8(Loader *, char *);
extern void func_001B5AF0(void *, int, int);
extern void func_001B4DD0(Loader *);
extern void func_0021F8E0(void);
extern void func_0016C0C8(World *, int);
extern void func_001D4288(char *);
extern void func_001B5130(Loader *);
extern void func_001B5AC8(Loader *);
extern void func_0014DAA8(void *, int);
extern void func_0019D120(ObjCCE8 *);
extern int func_0023D040(int);
extern void func_0019DE20(void *, int);
extern void func_0012DE68(void *, int);
extern void func_00148BE8(void *, int);

void func_0019CB08(ObjCB08 *obj, int index, int arg2) {
    typedef int Quad __attribute__((mode(TI)));
    Quad buffer[6];
    int value;
    WaitForRenderLegacy(&D_004B1F40);
    value = arg2 ? arg2 : obj->x4;
    func_00243D80(buffer, ((short (*)[2])D_0026F9A8)[index][0], 6, 2, 0, 0, 0x180, 0xE0);
    func_0025A960(0);
    func_002440A8(buffer, value);
    func_002433A8(0, 0);
}

static inline void SetLoading(int on)
{
    D_0029D3F0 = on;
}

void func_0019CBA8(State *state, int mode)
{
    Loader loader;
    int i;

    D_004B2914.xA8 = 0;
    SetLoading(1);
    InitializeLoadTransitionDiscard(&loader);
    func_001B4AF8(&loader);
    func_001B5A38(&loader);
    func_001B5BA8(&loader, D_003F52D0);
    D_004B1F40.x4 = 0;
    func_001B5AF0(&loader, 0, 1);
    state->x8 = 1;
    for (i = 1; i >= 0; i--) {
        func_001B4DD0(&loader);
        func_0021F8E0();
        func_0016C0C8(&D_00398C40, 0);
        func_001D4288(D_00398C40.x240);
        func_001B5130(&loader);
    }
    state->x8 = 0;
    func_001B5AC8(&loader);
    state->x0 = D_004B1F40.x4 != 0;
    D_004B2914.xA8 = 1;
    SetLoading(0);
    D_0029D3F4 = D_0043EEC0.x8;
    state->x9 = 1;
}

void func_0019CCE8(ObjCCE8 *o) {
 int previous = D_0043EEC8[0];
 if (D_0029D3F4 != previous) func_0019CBA8((State *)D_0043DD38,D_0029D3F4);
 o->x508=0; D_0039E828[0]=1;
 D_0037EA60.x2B0=func_0023D040(0);
 func_001B5AF0(D_003F52D0,1,0);
 if ((o->x521 = D_0037EA60.x2BC==0)) func_0014DAA8(&D_0037EA60,2);
 o->x500=D_002B27D4[0]; o->x504=0;
 func_0019D120(o);
}

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct BodyCDA0 {
    char pad0[0x50];
    Vec x50;
} BodyCDA0;

typedef struct ObjCDA0 {
    char pad0[0x500];
    BodyCDA0 *x500;
    char pad504[0xC];
    Vec x510;
} ObjCDA0;

typedef struct RiderCDA0 {
    char pad0[0x10];
    Vec x10;
} RiderCDA0;

extern void func_0019D588(ObjCDA0 *obj);

static inline void Sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Add(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Scale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline float Normalize(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
    return len;
}

/* If the direction from the rider to obj is nearly along z, places the rider at
   obj->x510 - 8 * obj->x500->x50. */
void func_0019CDA0(ObjCDA0 *obj)
{
    RiderCDA0 *rider;
    Vec *pos;
    Vec dir;
    Vec back;
    float z;
    float absZ;

    rider = (RiderCDA0 *)func_0023D040(0);
    pos = &obj->x510;
    Sub(&dir, pos, &rider->x10);
    Normalize(&dir);
    /* Zero-byte labels (cf. src/unsorted/001341F8.c): the first keeps the 0.9f constant
       after the normalized stores, the second keeps z live past abs.s so the result gets
       its own register (retail allocation). */
    __asm__ volatile ("L0019CDA0_norm:");
    z = dir.z;
    absZ = __builtin_fabsf(z);
    __asm__ volatile ("L0019CDA0_abs:" : : "f"(z));
    if (0.9f <= absZ) {
        Scale(&back, &obj->x500->x50, -8.0f);
        Add(&rider->x10, &back, pos);
        func_0019D588(obj);
    }
}


void func_0019CE90(unsigned char *obj) {
 D_00398C40.x5BE4 = 0;
 D_00398C40.x5BE8 = 0;
 func_0019DE20(obj, 0);
 func_001B5AF0(D_003F52D0, 5, 0);
 if ((unsigned char)obj[0x521]) {
  func_0012DE68(D_002AF2C0, 1);
  func_0014DAA8(&D_0037EA60, 0);
  func_00148BE8(&D_0037EA60, 0);
 }
}

typedef struct Entry {
    char pad0[0xC];
    short xC;
    char padE[2];
} Entry;

typedef struct Obj {
    char pad0[0x10];
    Entry x10[64];
    char pad410[8];
    int x418;
    char pad41C[0x508 - 0x41C];
    unsigned char x508;
    char pad509[0x520 - 0x509];
    unsigned char x520;
} Obj;


extern void func_00190890(void *);
extern void func_00187E18(void);

void func_0019CF18(Obj *obj, int index)
{
    int type;

    if (obj->x418 != 0) {
        type = (obj->x10 + index)->xC;
        if ((type ^ 30) == 0) {
            func_00190890(D_003F52D0);
            func_00187E18();
            return;
        }
    }
    if (obj->x520) {
        func_0019DE20(obj, 0);
        return;
    }
    func_0019DE20(obj, 1);
    obj->x508 = 1;
    D_0029D1A8 = 1.0f;
}
