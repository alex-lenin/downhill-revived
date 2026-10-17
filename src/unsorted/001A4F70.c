/* UNIT 0x001A4F70 0x001A8200 .sdata=0x29D330:0x29D3B0 */
#include "particle_emission.h"
#if defined(__mips__)
#define CallShared001B5DE8_0 func_001B5DE8
#else
#define CallShared001B5DE8_0 Forward001B5DE8_3
#endif
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00239A98_24_func_00239A98 func_00239A98
#else
#define CallScalarBridge00239A98_24_func_00239A98 ScalarBridge00239A98_24
#endif

#if defined(__mips__)
#define CallScalarBridge00239A80_23_func_00239A80 func_00239A80
#else
#define CallScalarBridge00239A80_23_func_00239A80 ScalarBridge00239A80_23
#endif

#if defined(__mips__)
#define CallScalarBridge001D00F0_22_func_001D00F0 func_001D00F0
#else
#define CallScalarBridge001D00F0_22_func_001D00F0 ScalarBridge001D00F0_22
#endif
#include "impulse.h"
#include "spatial.h"
/* Retains the EE argument setup; guarded native permutation remains. */
#include "particle.h"
/* Retains retail call scheduling; guarded PC permutations remain where needed. */
#include "tween.h"
#include "animation_select.h"
#include "include_asm.h"

float D_0029D330 = 30.0f;
float D_0029D334 = 0.95f;
float D_0029D338 = 0.95f;
int tail_0029D33C = 1;
float tail_0029D340 = 1.0f;
float tail_0029D344 = 0.99f;
float D_0029D348 = 0.2f;
float D_0029D34C = 0.4f;
int D_0029D350 = 0;
int tail_0029D354 = 13;
int D_0029D358 = 1;
int D_0029D35C = 1;
int D_0029D360 = 0;
int tail_0029D364 = 0;
int D_0029D368 = 1;
int D_0029D36C = 0;
float D_0029D370 = 0.75f;
float D_0029D374 = 0.025f;
float D_0029D378 = 0.007f;
float D_0029D37C = 0.75f;
float D_0029D380 = 0.75f;
float D_0029D384 = 0.005f;
int D_0029D388 = 6;
float D_0029D38C = 0.4f;
float tail_0029D390 = 1.0f;
float D_0029D394 = 2000.0f;
float D_0029D398 = 0.005f;
float D_0029D39C = 4.0f;
float D_0029D3A0 = 0.4f;
float D_0029D3A4 = 0.1f;

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Mat {
    u128 q[4];
} Mat __attribute__((aligned(16)));

typedef struct Body {
    unsigned long flags;
    char pad8[8];
    Vec x10;
    char pad20[0x20];
    Mat x40;
    char pad80[0x10];
    Vec x90;
} Body;

typedef struct Rider {
    char pad0[0x16D4];
    Body *x16D4;
    char pad16D8[0x2160 - 0x16D8];
    unsigned char x2160;
} Rider;

typedef struct Obj {
    char pad0[0x10];
    Vec x10;
    Vec x20;
    Vec x30;
    char pad40[0x120 - 0x40];
    Vec x120;
    char pad130[0x210 - 0x130];
    Vec x210;
    char pad220[0x3F0 - 0x220];
    Vec x3F0[2];
    char pad410[0x678 - 0x410];
    float x678;
    char pad67C[0xDA0 - 0x67C];
    unsigned char xDA0;
    char padDA1[3];
    Rider *xDA4;
    char padDA8[0xDB0 - 0xDA8];
    Vec xDB0;
    Vec xDC0;
    int xDD0;
    unsigned char xDD4;
    char padDD5[3];
    int xDD8;
    unsigned char xDDC;
    char padDDD[7];
    int xDE4;
    char padDE8[0xDF8 - 0xDE8];
    float xDF8;
} Obj;

extern void func_001A4CE0(Obj *);
extern void func_001A4E08(Obj *);
extern void func_001A5848(Obj *);
extern void func_001A5930(Obj *);
extern void func_001A5E00(Obj *);
extern void func_001A8658(Obj *);
extern void func_001B16C8(void *, void *, void *, Vec *);

extern void func_002185C0(void *, int);

extern void func_00222040(Mat *, void *);
extern void func_00238F68(int);

#if defined(__mips__)
extern void func_00239A80(void *, float, float, float, float);
#else

#endif


#if defined(__mips__)
extern void func_00239A98(void *, float);
#else

#endif

extern void func_00239DA0(void *, int);

static inline void VAdd(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VSub(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VCross(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void VTransform(Vec *dst, Vec *v, Mat *m)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(v), "r"(m));
}

static inline float VLength(Vec *src)
{
    float length;
    float x = src->x;
    float y = src->y;
    float z = src->z;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(length) : "f"(x), "f"(y), "f"(z));
    return length;
}

static inline void NormalizeTo(Vec *dst, Vec *src)
{
    float inv, length, x = src->x, y = src->y, z = src->z;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    dst->x = x * inv;
    dst->y = y * inv;
    dst->z = z * inv;
}

/* ---- merged from src/unsorted/001A5218.c ---- */
typedef struct Vector__c1 { float x,y,z,w; } Vector__c1;
typedef struct Rider__c1 {
 char pad0[0x304]; Vector__c1 x304;
 char pad314[0x16D0-0x314]; char *x16D0; long *x16D4;
 char pad16D8[0x1CB0-0x16D8]; char x1CB0[0x110]; char x1DC0[0x110]; char x1ED0[0x110]; char x1FE0[0x110]; char pad20F0[0x70]; unsigned char x2160;
} Rider__c1;
typedef struct Obj__c1 {
 char pad0[0xDA0]; char xDA0; char padDA1[3]; Rider__c1 *xDA4;
 char padDA8[0xDFC-0xDA8]; void *xDFC; void *xE00;
} Obj__c1;
extern void func_00238F68__c1(int) asm("func_00238F68");



extern void func_00203EF0__c1(Vector__c1 *) asm("func_00203EF0");
extern void func_0022DDD8__c1(void *) asm("func_0022DDD8");
extern void func_001A5218__c1(Obj__c1 *obj) asm("func_001A5218");
/* ---- func_001A4F70: same translation unit as func_001A5218 (shares the .sdata block) ---- */
typedef struct Vec__c11 { float x, y, z, w; } Vec__c11 __attribute__((packed));
typedef struct Effect__c11 {
    char pad0[0x2848];
    struct Effect__c11 **x2848;
} Effect__c11;
typedef struct RiderData__c11 {
    char pad0[0x7908];
    int x7908;
    char pad790C[0x796C - 0x790C];
    int x796C;
} RiderData__c11;
typedef struct Rider__c11 {
    char pad0[0x304];
    Vec__c11 x304;
    char pad314[0x16D0 - 0x314];
    char *x16D0;
    Body *x16D4;
    char pad16D8[0x1CB0 - 0x16D8];
    char x1CB0[0x110];
    char x1DC0[0x110];
    char x1ED0[0x110];
    char x1FE0[0x110];
    char pad20F0[0x2160 - 0x20F0];
    unsigned char x2160;
} Rider__c11;
typedef struct Obj__c11 {
    char pad0[0x10];
    Vec x10;
    char pad20[0xDA0 - 0x20];
    unsigned char xDA0;
    char padDA1[3];
    Rider__c11 *xDA4;
    char padDA8[0xDD0 - 0xDA8];
    int xDD0;
    unsigned char xDD4;
    char padDD5[3];
    int xDD8;
    unsigned char xDDC;
    char padDDD[3];
    int xDE0;
    int xDE4;
    int xDE8;
    unsigned char xDEC;
    char padDED[7];
    int xDF4;
    char padDF8[4];
    Effect__c11 *xDFC;
    Effect__c11 *xE00;
} Obj__c11;

extern Vec D_0043F9E0;
extern void func_001A7880(Obj__c11 *);
extern void func_00203E10(void *, float);
extern Effect__c11 *func_0022DD88(int);

#define COPY_VEC11(dst, src)     __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"((dst)->x) : "r"(src) : "$2", "memory")

void func_001A4F70(Obj__c11 *obj, int mode)
{
    Vec__c11 vec;
    Rider__c11 *rider;
    RiderData__c11 *data;
    char *component;
    int handle;

    if (obj->xDA4 != 0) {
        register int m = mode;
        if (obj->xDA4->x2160) {
        obj->xDEC = 0;
        obj->xDE0 = 0;
        obj->xDD0 = D_0029D388;
        obj->xDA0 = 1;
        obj->xDE4 = 30;
        obj->xDE8 = 0;
        obj->xDD4 = 0;
        obj->xDA4->x16D4->flags &= ~(1UL << 57);
        **(unsigned int **)obj->xDA4->x16D4 =
            (**(unsigned int **)obj->xDA4->x16D4 & 0xFFFC007F) | ((obj->xDF4 & 0x7FF) << 7);
        COPY_VEC11(&D_0043F9E0, &obj->xDA4->x16D4->x10);
        COPY_VEC11(&obj->x10, &obj->xDA4->x16D4->x10);
        func_001A7880(obj);
        obj->xDD8 = 0;
        obj->xDDC = 0;
        SetSecondaryBlendWeightFirst((RiderSecondaryBlend *)((char *)obj->xDA4 + 0x16F0), 1.0f, 32);
        SetBodyBlendWeightFirst((RiderBodyBlend *)(obj->xDA4->x16D0 + 0x47F0), 1.0f, 32);
        rider = obj->xDA4;
        component = rider->x1CB0;
        SetRiderBlendWeightFirst((RiderBlendController *)(rider->x1DC0), 1.0f, 32);
        SetRiderBlendWeightFirst((RiderBlendController *)(component), 1.0f, 32);
        SetRiderBlendWeightFirst((RiderBlendController *)(rider->x1ED0), 1.0f, 32);
        SetRiderBlendWeightFirstDiscard((RiderBlendController *)(rider->x1FE0), 1.0f, 32);
        vec = obj->xDA4->x304;
        func_00203E10(&vec, 0.0f);
        if (m == 0) {
            data = (RiderData__c11 *)obj->xDA4->x16D0;
            if (data->x796C == 0x200) {
                func_0022CFB8((Vector4 *)(&obj->xDA4->x16D4->x10), &obj->xDA4->x16D4->x40, 0x49, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
            } else if (data->x7908 < 0x23) {
                handle = SpawnParticleScaleBeforeDirection((Vector4 *)(&obj->xDA4->x16D4->x10), &obj->xDA4->x16D4->x40, 7, 1.0f, (Vector4 *)(0), (Matrix4 *)(0), (int)(0), (int)(0));
                if (handle != -1) {
                    obj->xE00 = func_0022DD88(handle);
                    obj->xE00->x2848 = &obj->xE00;
                }
                handle = func_0022CFB8((Vector4 *)(&obj->xDA4->x16D4->x10), &obj->xDA4->x16D4->x40, 1, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
                if (handle != -1) {
                    obj->xDFC = func_0022DD88(handle);
                    obj->xDFC->x2848 = &obj->xDFC;
                }
            }
        }
        }
    }
}
void func_001A5218__c1(Obj__c1 *obj) {
 Vector__c1 vector;
 Rider__c1 *rider;
 void *component;
 if (obj->xDA4 && obj->xDA4->x2160) {
  obj->xDA0=0;
  if (D_0029D36C) func_00238F68__c1(60);
  *obj->xDA4->x16D4 &= ~(1UL << 57);
  **(unsigned int **)obj->xDA4->x16D4 &= 0xFFFC007F;
  SetSecondaryBlendWeightFirst((RiderSecondaryBlend *)((char *)obj->xDA4+0x16F0), 1.0f, 0);
  SetBodyBlendWeightFirst((RiderBodyBlend *)(obj->xDA4->x16D0+0x47F0), 1.0f, 0);
  rider=obj->xDA4;
  component=rider->x1CB0;
  SetRiderBlendWeightFirst((RiderBlendController *)(rider->x1DC0), 1.0f, 10);
  SetRiderBlendWeightFirst((RiderBlendController *)(component), 1.0f, 10);
  SetRiderBlendWeightFirst((RiderBlendController *)(rider->x1ED0), 1.0f, 10);
  SetRiderBlendWeightFirstDiscard((RiderBlendController *)(rider->x1FE0), 1.0f, 10);
  vector=obj->xDA4->x304;
  func_00203EF0__c1(&vector);
  func_00202E38((struct AnimationHandle *)(&obj->xDA4->x304),1,1,10.0f);
  if(obj->xDFC) func_0022DDD8__c1(obj->xDFC);
  if(obj->xE00) func_0022DDD8__c1(obj->xE00);
  obj->xDFC=0; obj->xE00=0;
 }
}
typedef struct RiderBody {
    char pad0[0x4B0];
    unsigned long flags;
} RiderBody;



void func_001A53C8(Obj *obj, char *rider)
{
    Vec temp;
    RiderBody *body = (RiderBody *)(rider + 0x40C0);

    body->flags = (body->flags & ~0x20UL) | 0x20UL;
    __asm__ volatile ("lqc2 $vf11,0(%1)
	mfc1 $2,%2
	qmtc2.ni $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0"
                      : "=m"(temp) : "r"(rider + 0x42A0), "f"(D_0029D394) : "$2");
    func_00231C88((ImpulseBody *)(body), (Vector4 *)(&temp));
    obj->xDA4->x16D4->flags &= ~(1UL << 57);
    obj->xDE4 = 0;
}

void func_001A5460(Obj *obj)
{
    Vec temp0;
    Vec normal;
    Vec cross;
    Vec delta0;
    Vec delta1;
    Mat local;
    Mat world;
    char hit[0x30] __attribute__((aligned(16)));
    Vec ray0;
    Vec ray1;
    unsigned long mask;
    unsigned long saved;
    unsigned long saved_bit;
    Rider *rider;
    unsigned long *flags0;
    unsigned long *flags1;
    Vec *offset;
    Mat *localp;
    Mat *worldp;

    obj->xDD8++;
    if (D_0029D36C) {
        func_00238F68(30);
    } else {
        func_00238F68(60);
    }

    rider = obj->xDA4;
    if (rider && rider->x2160 && obj->xDA0 && ((unsigned char *)rider->x16D4)[5]) {
        obj->xDD0++;
        obj->x678 = -D_0029D330;
        obj->xDE4++;

        if (D_0029D350) {
            func_001A5848(obj);
        }

        if (obj->xDDC == 0) {
            func_001A8658(obj);
            func_001A5930(obj);
            if (D_0029D368) {
                mask = ~(1UL << 57);
                flags0 = &obj->xDA4->x16D4->flags;
                saved = *flags0;
                saved_bit = (saved >> 57) & 1;
                *flags0 = saved & mask;
                func_001A5E00(obj);
                flags1 = &obj->xDA4->x16D4->flags;
                *flags1 = (*flags1 & mask) | (saved_bit << 57);
            }
        }

        if (D_0029D360 == 0) {
            CallScalarBridge00239A80_23_func_00239A80(&obj->x20, D_0029D370, D_0029D374, D_0029D378, D_0029D37C);
            CallScalarBridge00239A98_24_func_00239A98(&obj->x20, D_0029D380);
            if (obj->xDDC == 0) {
                func_00239DA0(&obj->x20, 10);

                offset = &obj->x3F0[0];
                VAdd(&obj->xDA4->x16D4->x10, &obj->x10, offset);

                VTransform(&temp0, &obj->xDB0, &obj->xDA4->x16D4->x40);
                VAdd(&obj->xDA4->x16D4->x10, &obj->xDA4->x16D4->x10, &temp0);

                VSub(&normal, &obj->xDA4->x16D4->x10, &obj->xDA4->x16D4->x90);
                obj->xDF8 = VLength(&normal);
                if (obj->xDF8 < D_0029D398 && obj->xDD8 >= 11 && obj->xDD4) {
                    obj->xDDC = 1;
                }

                VSub(&delta0, &obj->x120, &obj->x30);
                VSub(&delta1, &obj->x210, &obj->x120);
                VCross(&cross, &delta0, &delta1);
                NormalizeTo(&normal, &cross);

                func_001B16C8(&obj->xDA4->x16D4->x40, &obj->x120, &obj->x30, &normal);
                localp = &local;
                func_00222040(localp, &obj->xDC0);
                worldp = &world;
                func_00221930((Matrix4 *)(worldp), (Matrix4 *)(localp), (Matrix4 *)(&obj->xDA4->x16D4->x40));
                __asm__ volatile (
                    "lq $8, 0x0(%4)\n\t"
                    "lq $9, 0x10(%4)\n\t"
                    "lq $10, 0x20(%4)\n\t"
                    "lq $11, 0x30(%4)\n\t"
                    "sq $8, %0\n\t"
                    "sq $9, %1\n\t"
                    "sq $10, %2\n\t"
                    "sq $11, %3"
                    : "=m"(obj->xDA4->x16D4->x40.q[0]),
                      "=m"(obj->xDA4->x16D4->x40.q[1]),
                      "=m"(obj->xDA4->x16D4->x40.q[2]),
                      "=m"(obj->xDA4->x16D4->x40.q[3])
                    : "r"(worldp)
                    : "$8", "$9", "$10", "$11");

                VAdd(&ray0, offset + 1, &obj->x10);
                VAdd(&ray1, offset, &obj->x10);
                if (QuerySpatialHitsFloatFirst((Vector4 *)(&ray0), (Vector4 *)(&ray1), 4.0f, 0.0f, 0.0f, (SpatialHit *)(hit), 1)) {
                    func_001A4E08(obj);
                } else {
                    func_001A4CE0(obj);
                }
                func_002185C0(obj->xDA4->x16D4, 0);
            }
        }
    }
}

/* ---- merged from src/unsorted/001A5848.c ---- */
typedef struct Vec__c2 {
    float x, y, z, w;
} Vec__c2 __attribute__((aligned(16)));

typedef struct Node__c2 {
    Vec__c2 pos;
    char pad10[0x40];
} Node__c2;

typedef struct Obj__c2 {
    char pad0[0x10];
    Vec__c2 origin;         /* 0x10 */
    char pad20[0x10];
    Node__c2 nodes[13];     /* 0x30 */
} Obj__c2;

/* dst.xyz = a.xyz + b.xyz (VU0 macro mode) */
static inline void VecAdd(Vec__c2 *dst, Vec__c2 *a, Vec__c2 *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*dst) : "r"(a), "r"(b) : "memory");
}

extern void func_001A5848__c2(Obj__c2 *self) asm("func_001A5848");
void func_001A5848__c2(Obj__c2 *self)
{
    Vec__c2 seg[2];
    Vec__c2 pts[5];
    int i;

    for (i = 0; i < 12; i += 3) {
        VecAdd(&seg[0], &self->nodes[i].pos, &self->origin);
        VecAdd(&seg[1], &self->nodes[i + 1].pos, &self->origin);
        VecAdd(&seg[0], &self->nodes[i + 1].pos, &self->origin);
        VecAdd(&seg[1], &self->nodes[i + 2].pos, &self->origin);
    }
    VecAdd(&pts[0], &self->nodes[12].pos, &self->origin);
    VecAdd(&pts[1], &self->nodes[0].pos, &self->origin);
    VecAdd(&pts[2], &self->nodes[3].pos, &self->origin);
    VecAdd(&pts[3], &self->nodes[6].pos, &self->origin);
    VecAdd(&pts[4], &self->nodes[9].pos, &self->origin);
}
/* ---- merged from src/unsorted/001A5930.c ---- */
typedef struct Vec__c3 { float x, y, z, w; } Vec__c3 __attribute__((aligned(16)));

static inline void VSub__c3(Vec__c3 *dst, Vec__c3 *a, Vec__c3 *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline void VAdd__c3(Vec__c3 *dst, Vec__c3 *a, Vec__c3 *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline void VScale(Vec__c3 *dst, Vec__c3 *src, float f) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(src),"f"(f) : "$2");
}
static inline void VCross__c3(Vec__c3 *dst, Vec__c3 *a, Vec__c3 *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}

static inline void NormalizeTo__c3(Vec__c3 *dst, Vec__c3 *src) {
    float inv, length, x = src->x, y = src->y, z = src->z;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    dst->x = x * inv;
    dst->y = y * inv;
    dst->z = z * inv;
}

static inline float Dot(Vec__c3 *a, Vec__c3 *b) {
    float ax = a->x, bx = b->x, ay = a->y, by = b->y, az = a->z, bz = b->z, result;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(result) : "f"(ax), "f"(bx), "f"(ay), "f"(by), "f"(az), "f"(bz));
    return result;
}

/* plane = normalized dir, plane.w = plane . point */
static inline void MakePlane(Vec__c3 *plane, Vec__c3 *dir, Vec__c3 *point) {
    float inv, length, x = dir->x, y = dir->y, z = dir->z;
    __asm__ volatile ("mula.s %2,%3\n\tmadda.s %4,%5\n\tmadd.s %1,%6,%7\n\tsqrt.s %1,%1\n\t"
                      "lui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(length) : "f"(x), "f"(x), "f"(y), "f"(y), "f"(z), "f"(z) : "$2");
    plane->x = x * inv;
    plane->y = y * inv;
    plane->z = z * inv;
    plane->w = Dot(point, plane);
}

struct Obj__c3 {
    char pad0[0x20];
    char x20[0x10];
    Vec__c3 x30;
    char pad40[0x40];
    Vec__c3 x80;
    char pad90[0x40];
    Vec__c3 xD0;
    char padE0[0x40];
    Vec__c3 x120;
    char pad130[0x40];
    Vec__c3 x170;
    char pad180[0x40];
    Vec__c3 x1C0;
    char pad1D0[0x40];
    Vec__c3 x210;
    char pad220[0x40];
    Vec__c3 x260;
    char pad270[0x90];
    Vec__c3 x300;
    char pad310[0x40];
    Vec__c3 x350;
};

extern void func_0023AF18__c3(void *, Vec__c3 *, Vec__c3 *, int, int) asm("func_0023AF18");

extern void func_001A5930__c3(struct Obj__c3 *obj) asm("func_001A5930");
void func_001A5930__c3(struct Obj__c3 *obj) {
    Vec__c3 tmp, mid, plane, cross;
    Vec__c3 *p170 = &obj->x170;
    Vec__c3 *p300 = &obj->x300;
    Vec__c3 *p260 = &obj->x260;
    Vec__c3 *p210 = &obj->x210;
    Vec__c3 *p350 = &obj->x350;
    Vec__c3 *p30 = &obj->x30;
    Vec__c3 *p1C0 = &obj->x1C0;
    Vec__c3 *p120 = &obj->x120;
    Vec__c3 *p80 = &obj->x80;
    Vec__c3 *pD0 = &obj->xD0;

    if (D_0029D358) {
        VSub__c3(&tmp, p210, p300);
        MakePlane(&plane, &tmp, p210);
        func_0023AF18__c3(obj->x20, &plane, p80, 0, 1);
        VSub__c3(&tmp, p300, p210);
        MakePlane(&plane, &tmp, p300);
        func_0023AF18__c3(obj->x20, &plane, p170, 0, 1);
    }
    if (D_0029D35C) {
        VSub__c3(&tmp, p210, p300);
        NormalizeTo__c3(&plane, &tmp);
        VAdd__c3(&tmp, p210, p300);
        VScale(&mid, &tmp, 0.5f);
        plane.w = Dot(&mid, &plane);
        func_0023AF18__c3(obj->x20, &plane, p260, 0, 1);
        func_0023AF18__c3(obj->x20, &plane, pD0, 0, 1);
        plane.w = Dot(p30, &plane);
        func_0023AF18__c3(obj->x20, &plane, p80, 0, 1);
        VSub__c3(&tmp, p300, p210);
        NormalizeTo__c3(&plane, &tmp);
        VAdd__c3(&tmp, p210, p300);
        VScale(&mid, &tmp, 0.5f);
        plane.w = Dot(&mid, &plane);
        func_0023AF18__c3(obj->x20, &plane, p350, 0, 1);
        func_0023AF18__c3(obj->x20, &plane, p1C0, 0, 1);
        plane.w = Dot(p120, &plane);
        func_0023AF18__c3(obj->x20, &plane, p170, 0, 1);
        VSub__c3(&tmp, p120, p30);
        VSub__c3(&mid, p210, p30);
        VCross__c3(&cross, &mid, &tmp);
        MakePlane(&plane, &cross, p210);
        func_0023AF18__c3(obj->x20, &plane, p260, 0, 1);
        func_0023AF18__c3(obj->x20, &plane, p350, 0, 1);
    }
}

typedef union Flags__c4 {
    unsigned long value;
    struct {
        unsigned long low : 57;
        unsigned long x39 : 1;
        unsigned long high : 6;
    } bits;
} Flags__c4;

typedef struct Entity__c4 {
    Flags__c4 x0;
    char pad8[0x8];
    Vec x10;
    char pad20[0x20];
    Vec x40;
    char pad50[0x60];
    float xB0;
} Entity__c4;

typedef struct Hit__c4 {
    unsigned long x0;
    float x8;
    Entity__c4 *xC;
    Vec x10;
    Vec x20;
} Hit__c4;

typedef struct Query__c4 {
    Vec *x0;
    Vec *x4;
    int x8;
    float xC;
    float x10;
    int x14;
    int x18;
    int x1C;
    Hit__c4 *x20;
    int x24;
} Query__c4;

typedef struct Point__c4 {
    Vec x0;
    Vec x10;
    char pad20[0x30];
} Point__c4;

typedef struct Aux__c4 {
    char pad0[0x281C];
    void *x281C;
} Aux__c4;

typedef struct Driver__c4 {
    char pad0[0x7908];
    int x7908;
    char pad790C[0x60];
    int x796C;
    char pad7970[0xD4];
    Aux__c4 *x7A44;
} Driver__c4;

typedef struct Parent__c4 {
    char pad0[0x16D0];
    Driver__c4 *x16D0;
    Entity__c4 *x16D4;
    char pad16D8[0xA90];
    char x2168[4];
} Parent__c4;

typedef struct Obj__c4 {
    char pad0[0x10];
    Vec x10;
    Vec x20;
    Point__c4 points[12];
    char pad3F0[0x9B4];
    Parent__c4 *xDA4;
    char padDA8[0x28];
    int xDD0;
    unsigned char xDD4;
    char padDD5[0x23];
    float xDF8;
} Obj__c4;

extern Hit__c4 *func_001A6A30(Hit__c4 *, int);
extern void func_0023AF18(void *, Vec *, Vec *, int, int);
extern void func_002093C8(Entity__c4 *, Vec *, Vec *);
extern void func_001B5DE8(Vec *, Vec *, unsigned short, Vec *, float,
                          Vec *, void *, Driver__c4 *);

#if defined(__mips__)
extern void func_001D00F0(void *, int, float);
#else

#endif


/* The vector, accumulator and square-root operations use the game's SDK wrappers. */
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Add__c4(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Sub__c4(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\t"
                      "vsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Scale__c4(Vec *out, Vec *v, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void AddScaled__c4(Vec *out, Vec *a, Vec *b, float s) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline void Cross__c4(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\t"
                      "vopmula.xyz ACC, $vf11, $vf12\n\t"
                      "vopmsub.xyz $vf11, $vf12, $vf11\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Set__c4(Vec *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

static inline float Dot__c4(Vec *a, Vec *b) {
    float value;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(value) : "f"(a->x), "f"(b->x), "f"(a->y),
                        "f"(b->y), "f"(a->z), "f"(b->z));
    return value;
}

static inline float Length__c4(Vec *v) {
    float value;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\t"
                      "sqrt.s %0, %0" : "=&f"(value) : "f"(v->x), "f"(v->y), "f"(v->z));
    return value;
}

static inline void Normalize__c4(Vec *out, Vec *v) {
    float x = v->x, y = v->y, z = v->z, length, inverse;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\t"
                      "sqrt.s %1, %1\n\tlui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\t"
                      "div.s %0, %0, %1"
                      : "=&f"(inverse), "=&f"(length), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inverse;
    out->y = y * inverse;
    out->z = z * inverse;
}

static inline void Copy__c4(Vec *dst, Vec *src) {
    COPY_VECTOR(*dst, src);
}

extern void func_001A5E00__c4(Obj__c4 *obj) asm("func_001A5E00");
void func_001A5E00__c4(Obj__c4 *obj) {
    Vec scratch;
    Hit__c4 hits[12][10];
    Query__c4 queries[12];
    Vec end[12];
    Vec start[12];
    Vec plane;
    Vec motion;
    Vec displacement;
    Vec projected;
    Vec lateral;
    Vec impulse;
    Vec up;
    Vec effectPosition;
    Vec effectNormal;
    int hitCount = 0;
    short surface = 0;
    int count = 0;
    unsigned int savedFlag;
    int setupIndex;
    int i;
    Hit__c4 *hit;
    Vec *normal;
    Vec *a;
    Vec *b;
    float speed;
    float strength;
    float fraction;

    for (setupIndex = 0; setupIndex < 12; setupIndex++) {
        Add__c4(&end[setupIndex], &obj->points[setupIndex].x0, &obj->x10);
        Add__c4(&start[setupIndex], &obj->points[setupIndex].x10, &obj->x10);
    }

    for (setupIndex = 0; setupIndex < 12; setupIndex++) {
        queries[setupIndex].x0 = &start[setupIndex];
        count++;
        queries[setupIndex].x4 = &end[setupIndex];
        queries[setupIndex].x8 = 0;
        queries[setupIndex].xC = 1.0f;
        queries[setupIndex].x20 = hits[setupIndex];
        queries[setupIndex].x1C = 10;
        queries[setupIndex].x10 = D_0029D338;
        queries[setupIndex].x14 = 0;
        queries[setupIndex].x18 = 0;
    }

    savedFlag = obj->xDA4->x16D4->x0.bits.x39 ? 1 : 0;
    obj->xDA4->x16D4->x0.value &= ~0x0200000000000000UL;
    QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(queries), count);

    for (i = 0; i < count; i++) {
        hit = func_001A6A30(hits[i], queries[i].x24);
        a = queries[i].x0;
        normal = &hit->x20;
        b = queries[i].x4;
        if (hit != 0) {
            hitCount++;
            normal->w = Dot__c4(&hit->x10, normal);
            fraction = hit->x10.w;
            if (fraction < 0.0f) {
                Scale__c4(&scratch, normal, hit->x8);
                Copy__c4(&plane, normal);
                Sub__c4(&scratch, &hit->x10, &obj->x10);
                plane.w = Dot__c4(&scratch, &plane) + D_0029D334;
                func_0023AF18(&obj->x20, &plane, &obj->points[i].x0, 1, 1);
                Copy__c4(&obj->points[i].x10, &obj->points[i].x0);
            } else {
                Copy__c4(&plane, normal);
                plane.w = hit->x20.w;
                Sub__c4(&scratch, &obj->points[i].x0, &obj->points[i].x10);
                Scale__c4(&scratch, &scratch, fraction);
                Add__c4(&obj->points[i].x0, &obj->points[i].x10, &scratch);
                AddScaled__c4(&scratch, &obj->points[i].x0, &plane, -D_0029D338);
                plane.w = Dot__c4(&scratch, &plane) + D_0029D334;
                func_0023AF18(&obj->x20, &plane, &obj->points[i].x0, 1, 1);

                if (hit->xC != 0 && (hit->xC->x0.value & 0x0400000000000000UL)) {
                    func_002093C8(hit->xC, &hit->x10, &motion);
                    Scale__c4(&motion, &motion, hit->xC->xB0);
                    Scale__c4(&motion, &motion, D_0029D39C);
                }

                Sub__c4(&scratch, a, b);
                strength = Dot__c4(&scratch, &hit->x20);
                Scale__c4(&impulse, &hit->x20, -strength);
                Add__c4(&projected, a, &impulse);
                if (hit->xC != 0 && (hit->xC->x0.value & 0x0400000000000000UL)) {
                    Scale__c4(&impulse, &motion, -Dot__c4(&motion, &hit->x20));
                }

                Sub__c4(&lateral, b, &projected);
                Scale__c4(&lateral, &lateral, D_0029D34C);
                Set__c4(&up, 0.0f, 0.0f, 1.0f);
                if (Dot__c4((Vec *)&up, &hit->x20) < D_0029D3A0) {
                    Scale__c4(&lateral, &lateral, 0.0f);
                }
                strength *= D_0029D348;
                if (i != 0 && i != 3 && i != 6 && i != 9) {
                    strength += strength;
                }
                Scale__c4(&impulse, &impulse, strength);

                if (impulse.x > D_0029D38C) impulse.x = D_0029D38C;
                if (impulse.y > D_0029D38C) impulse.y = D_0029D38C;
                if (impulse.z > D_0029D38C) impulse.z = D_0029D38C;
                if (impulse.x < -D_0029D38C) impulse.x = -D_0029D38C;
                if (impulse.y < -D_0029D38C) impulse.y = -D_0029D38C;
                if (impulse.z < -D_0029D38C) impulse.z = -D_0029D38C;

                Add__c4(&projected, &projected, &lateral);
                Add__c4(&projected, &projected, &impulse);
                Sub__c4(&displacement, &projected, b);
                Add__c4(&obj->points[i].x10, &obj->points[i].x0, &displacement);

                if (i == 0 || i == 3 || i == 6 || i == 9) {
                    speed = Length__c4(&impulse);
                    if (speed > D_0029D384) {
                        if (obj->xDD0 >= D_0029D388) {
                            obj->xDD0 = 0;
                            Add__c4(&effectPosition, &obj->points[i].x0, &obj->x10);
                            if (((unsigned char *)&obj->xDA4->x16D4->x0)[5] != 0) {
                                effectNormal = hit->x20;
                                CallShared001B5DE8_0(&effectNormal, &effectPosition,
                                              (unsigned short)(hit->x0 >> 53) & 0x3F,
                                              &scratch, speed * 100.0f,
                                              &obj->xDA4->x16D4->x40,
                                              obj->xDA4->x16D0->x7A44->x281C,
                                              obj->xDA4->x16D0);
                            }
                        } else if (obj->xDD0 >= (D_0029D388 >> 1)) {
                            if (obj->xDA4->x16D0->x796C == 0x200) {
                                func_0022CFB8((Vector4 *)(&obj->xDA4->x16D4->x10), &obj->xDA4->x16D4->x40, 0x49, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
                            } else if (obj->xDA4->x16D0->x7908 < 0x23) {
                                SpawnParticleScaleBeforeDirection((Vector4 *)(&obj->xDA4->x16D4->x10), &obj->xDA4->x16D4->x40, 7, 1.0f, (Vector4 *)(0), (Matrix4 *)(0), (int)(0), (int)(0));
                                SpawnParticleScaleBeforeDirection((Vector4 *)(&obj->xDA4->x16D4->x10), &obj->xDA4->x16D4->x40, 1, 1.0f, (Vector4 *)(0), (Matrix4 *)(0), (int)(0), (int)(0));
                            }
                        }
                    }
                }
                surface = (unsigned short)(hit->x0 >> 59) & 0x1F;
            }
        }
    }

    obj->xDA4->x16D4->x0.bits.x39 = (savedFlag & 1) ? 1 : 0;
    if (hitCount >= 2) {
        CallScalarBridge001D00F0_22_func_001D00F0(obj->xDA4->x2168, surface, obj->xDF8);
        obj->xDD4 = 1;
    } else {
        obj->xDD4 = 0;
    }

    if (hitCount >= 4) {
        Vec *adjustment;
        Sub__c4(&impulse, &obj->points[3].x0, &obj->points[0].x0);
        adjustment = &lateral;
        lateral.x = 0.0f;
        lateral.y = 0.0f;
        adjustment->z = 0.15f;
        Scale__c4(&impulse, &impulse, D_0029D3A4);
        Sub__c4(&plane, &obj->points[3].x0, &obj->points[0].x0);
        Sub__c4(&motion, &obj->points[6].x0, &obj->points[0].x0);
        Cross__c4(&displacement, &motion, &plane);
        Normalize__c4(&projected, &displacement);
        if (projected.z < 0.0f) {
            if (impulse.z <= 0.0f) {
                Add__c4(&lateral, &lateral, &impulse);
                Add__c4(&obj->points[0].x0, &obj->points[0].x0, &lateral);
                Add__c4(&obj->points[1].x0, &obj->points[1].x0, &lateral);
                Add__c4(&obj->points[2].x0, &obj->points[2].x0, &lateral);
                Add__c4(&obj->points[6].x0, &obj->points[6].x0, &lateral);
                Add__c4(&obj->points[7].x0, &obj->points[7].x0, &lateral);
                Add__c4(&obj->points[8].x0, &obj->points[8].x0, &lateral);
                Scale__c4(&lateral, &lateral, -1.0f);
                Add__c4(&obj->points[3].x0, &obj->points[3].x0, &lateral);
                Add__c4(&obj->points[4].x0, &obj->points[4].x0, &lateral);
                Add__c4(&obj->points[5].x0, &obj->points[5].x0, &lateral);
                Add__c4(&obj->points[9].x0, &obj->points[9].x0, &lateral);
                Add__c4(&obj->points[10].x0, &obj->points[10].x0, &lateral);
                Add__c4(&obj->points[11].x0, &obj->points[11].x0, &lateral);
            } else {
                Scale__c4(&impulse, &impulse, -1.0f);
                Add__c4(&lateral, &lateral, &impulse);
                Add__c4(&obj->points[3].x0, &obj->points[3].x0, &lateral);
                Add__c4(&obj->points[4].x0, &obj->points[4].x0, &lateral);
                Add__c4(&obj->points[5].x0, &obj->points[5].x0, &lateral);
                Add__c4(&obj->points[9].x0, &obj->points[9].x0, &lateral);
                Add__c4(&obj->points[10].x0, &obj->points[10].x0, &lateral);
                Add__c4(&obj->points[11].x0, &obj->points[11].x0, &lateral);
                Scale__c4(&lateral, &lateral, -1.0f);
                Add__c4(&obj->points[0].x0, &obj->points[0].x0, &lateral);
                Add__c4(&obj->points[1].x0, &obj->points[1].x0, &lateral);
                Add__c4(&obj->points[2].x0, &obj->points[2].x0, &lateral);
                Add__c4(&obj->points[6].x0, &obj->points[6].x0, &lateral);
                Add__c4(&obj->points[7].x0, &obj->points[7].x0, &lateral);
                Add__c4(&obj->points[8].x0, &obj->points[8].x0, &lateral);
            }
        }
    }
}

typedef struct Entry__c5 {
    unsigned short x0;
    char pad2[0x1A];
    float x1C;
    char pad20[0x10];
} Entry__c5;

extern Entry__c5 *func_001A6A30__c5(Entry__c5 *entry, int count) asm("func_001A6A30");
Entry__c5 *func_001A6A30__c5(Entry__c5 *entry, int count)
{
    float best = 2.0f;
    Entry__c5 *found = 0;
    int i;

    for (i = 0; i < count; i++) {
        if (entry->x0 != 0 && entry->x1C < best) {
            best = entry->x1C;
            found = entry;
        }
        entry++;
    }
    return found;
}

typedef struct Vec4__c6 { float x, y, z, w; } __attribute__((aligned(16))) Vec4__c6;

typedef struct Item__c6 {
    Vec4__c6 pos;
    char pad10[0x50 - 0x10];
} Item__c6;

typedef struct Entry__c6 {
    short index;
    short x2;
    float dist;
} Entry__c6;

typedef struct Obj__c6 {
    char pad0[0x20];
    char x20[0x10];
    Item__c6 items[(0xBCC - 0x30) / 0x50];
    char padBBC[0xBCC - 0x30 - ((0xBCC - 0x30) / 0x50) * 0x50];
    Entry__c6 entries[1];
} Obj__c6;

typedef struct Source__c6 {
    Vec4__c6 pos;
    char pad10[0x38 - 0x10];
    signed char slots[1];
} Source__c6;

extern signed char func_0023B050(void *p);

extern void func_001A6A78__c6(Obj__c6 *obj, Source__c6 *src, int index, int *count) asm("func_001A6A78");
void func_001A6A78__c6(Obj__c6 *obj, Source__c6 *src, int index, int *count)
{
    Vec4__c6 *target;
    Entry__c6 *entry;
    float d[4] __attribute__((aligned(16)));

    target = &obj->items[index].pos;
    src->slots[*count] = func_0023B050(obj->x20);
    entry = &obj->entries[src->slots[*count]];
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(d), "r"(&src->pos), "r"(target) : "memory");
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(entry->dist) : "f"(d[0]), "f"(d[1]), "f"(d[2]));
    entry->index = index;
    entry->x2 = 0;
    (*count)++;
}

typedef struct Entry__c7 { short x0, x2; float x4; } Entry__c7;
extern signed char func_0023B050(void *);
extern void func_001A6B48__c7(char *arg0, signed char *arg1, int arg2, int *arg3, float value) asm("func_001A6B48");
void func_001A6B48__c7(char *arg0, signed char *arg1, int arg2, int *arg3, float value) {
    signed char *p;
    Entry__c7 *entry;
    int index = func_0023B050(arg0 + 0x20);
    p = arg1 + 0x38;
    p[*arg3] = index;
    entry = (Entry__c7 *)(arg0 + (p[*arg3] * 8 + 0xBCC));
    entry->x4 = value;
    entry->x0 = arg2;
    entry->x2 = 2;
    ++*arg3;
}

typedef struct Entry__c8 { short x0, x2; float x4; } Entry__c8;
extern signed char func_0023B050(void *);
extern void func_001A6BE8__c8(char *arg0, signed char *arg1, int arg2, int *arg3, float value) asm("func_001A6BE8");
void func_001A6BE8__c8(char *arg0, signed char *arg1, int arg2, int *arg3, float value) {
    signed char *p;
    Entry__c8 *entry;
    int index = func_0023B050(arg0 + 0x20);
    p = arg1 + 0x38;
    p[*arg3] = index;
    entry = (Entry__c8 *)(arg0 + (p[*arg3] * 8 + 0xBCC));
    entry->x4 = value;
    entry->x0 = arg2;
    entry->x2 = 1;
    ++*arg3;
}

struct Entry__c9 { int x0; float x4; };
extern int func_0023B0B0(void *);
extern void func_001A6C88__c9(char *object, char *dest, int value, int offset, float amount) asm("func_001A6C88");
void func_001A6C88__c9(char *object, char *dest, int value, int offset, float amount) {
    int index;
    struct Entry__c9 *entry;
    index = func_0023B0B0(object + 0x20);
    (offset + dest)[0x49] = index;
    entry = (struct Entry__c9 *)(object + 0xD5C + (signed char)index * 8);
    entry->x0 = value;
    entry->x4 = amount;
}

typedef struct Vec__c10 { float x, y, z, w; } __attribute__((aligned(16))) Vec__c10;

typedef struct Item__c10 {
    Vec__c10 pos;
    Vec__c10 x10;
    Vec__c10 x20;
    int x30;
    int x34;
    signed char slots[6];
    signed char links[4];
    signed char x42[6];
    signed char x48;
    char pad49[0x50 - 0x49];
} Item__c10;

typedef struct Entry__c10 {
    short index;
    short x2;
    float dist;
} Entry__c10;

typedef struct Obj__c10 {
    char pad0[0x10];
    Vec__c10 x10;
    int count;
    char pad24[0x30 - 0x24];
    Item__c10 items[14];
    char pad490[0xBCC - 0x490];
    Entry__c10 entries[50];
    char padD5C[0xDA4 - 0xD5C];
    char *xDA4;
    char padDA8[0xDB0 - 0xDA8];
    Vec__c10 xDB0;
    Vec__c10 xDC0;
} Obj__c10;

#undef COPY_VECTOR
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Add__c10(Vec__c10 *out, Vec__c10 *a, Vec__c10 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Sub__c10(Vec__c10 *out, Vec__c10 *a, Vec__c10 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Scale__c10(Vec__c10 *out, Vec__c10 *v, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v), "f"(scale) : "$2");
}

static inline void ScaleAdd__c10(Vec__c10 *out, Vec__c10 *a, Vec__c10 *b, float scale)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}

extern float D_0029D380;

extern void func_001B1DD8(void *, void *);
extern void func_001B1FE0(void *, Vec__c10 *);


extern void func_001A6A78__c10(Obj__c10 *, Item__c10 *, int, int *) asm("func_001A6A78");
extern void func_001A6B48__c10(Obj__c10 *, Item__c10 *, int, int *, float) asm("func_001A6B48");
extern void func_001A6BE8__c10(Obj__c10 *, Item__c10 *, int, int *, float) asm("func_001A6BE8");
extern void func_001A6C88__c10(Obj__c10 *, Item__c10 *, int, int, float) asm("func_001A6C88");
extern signed char func_0023AFA8(void *, int, int, int, int, float);
extern signed char func_0023AFF8(void *, int, int, int, int, int, int);

#define ENTRY__c10(obj, item) ((Entry__c10 *)((char *)(obj) + ((item)->slots[0] * 8 + 0xBCC)))

void func_001A6D00(Obj__c10 *obj)
{
    Vec__c10 zero;
    Vec__c10 center;
    Vec__c10 offset;
    Vec__c10 mid;
    Vec__c10 v40;
    Vec__c10 v50;
    int n0, n6, n9, n3, n12, n13, n2, n5, n8, n11, n7;
    Entry__c10 *e1, *e0, *e4, *e7, *e10, *e3, *e6, *e9;
    Item__c10 *p0, *p3, *p6, *p9, *p1, *p4, *p7, *p10, *p5, *p2, *p11, *p8;
    Item__c10 *p12, *p13;
    signed char r;
    int i;
    int j;
    int k;
    Entry__c10 *entry;

    func_001B1DD8(obj->xDA4 + 0x1CB0, &obj->count);
    for (i = 0; i < 4; i++) {
        obj->entries[obj->items[i * 3].slots[0]].index = i * 3 + 1;
        obj->entries[obj->items[i * 3 + 1].slots[0]].index = i * 3 + 2;
        obj->entries[obj->items[i * 3 + 2].slots[0]].index = -1;
    }
    p0 = &obj->items[0];
    p3 = &obj->items[3];
    p6 = &obj->items[6];
    p9 = &obj->items[9];
    p1 = &obj->items[1];
    p4 = &obj->items[4];
    p7 = &obj->items[7];
    p10 = &obj->items[10];
    p5 = &obj->items[5];
    p2 = &obj->items[2];
    p11 = &obj->items[11];
    p8 = &obj->items[8];
    p12 = &obj->items[12];
    n7 = 1;
    n9 = 1;
    n6 = 1;
    n3 = 1;
    n0 = 1;
    n13 = 0;
    n12 = 0;
    n11 = 0;
    n8 = 0;
    n5 = 0;
    n2 = 0;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;

    COPY_VECTOR(obj->items[12].pos, &obj->x10);
    COPY_VECTOR(obj->items[12].x10, &obj->x10);
    p12->x34 = -1;
    Add__c10(&center, &p0->pos, &p3->pos);
    Add__c10(&center, &center, &p6->pos);
    Add__c10(&center, &center, &p9->pos);
    Scale__c10(&center, &center, 0.25f);
    Sub__c10(&offset, &p12->pos, &center);
    func_00221888((Matrix4 *)(*(char **)(obj->xDA4 + 0x16D4) + 0x40), (Vector4 *)(&offset), (Vector4 *)(&obj->xDB0));
    COPY_VECTOR(obj->items[12].pos, &center);
    COPY_VECTOR(obj->items[12].x10, &center);
    p13 = &obj->items[13];
    Add__c10(&mid, &p3->pos, &p0->pos);
    Scale__c10(&mid, &mid, 0.5f);
    COPY_VECTOR(obj->items[13].pos, &mid);
    COPY_VECTOR(obj->items[13].x10, &mid);
    COPY_VECTOR(obj->items[13].x20, &zero);
    p13->x34 = -1;
    for (i = 0; i < 6; i++) {
        p12->x42[i] = -1;
        p13->x42[i] = -1;
    }
    for (j = 0; j < obj->count; j++) {
        Sub__c10(&obj->items[j].pos, &obj->items[j].pos, &obj->x10);
        Sub__c10(&obj->items[j].x10, &obj->items[j].x10, &obj->x10);
    }
    for (k = 0; k < 12; k++) {
        if (k == 2 || k == 5 || k == 8 || k == 11) {
            entry = (Entry__c10 *)((obj->items[k - 1].slots[0] * 8 + 0xBCC) + (char *)obj);
            entry->dist *= 1.2f;
        }
    }

    func_001A6A78__c10(obj, p0, 6, &n0);
    func_001A6A78__c10(obj, p6, 9, &n6);
    func_001A6A78__c10(obj, p9, 3, &n9);
    func_001A6A78__c10(obj, p3, 0, &n3);
    func_001A6A78__c10(obj, p0, 9, &n0);
    func_001A6A78__c10(obj, p3, 6, &n3);
    func_001A6A78__c10(obj, p12, 0, &n12);
    func_001A6A78__c10(obj, p12, 3, &n12);
    func_001A6A78__c10(obj, p12, 6, &n12);
    func_001A6A78__c10(obj, p12, 9, &n12);
    func_001A6A78__c10(obj, p13, 3, &n13);
    func_001A6A78__c10(obj, p13, 0, &n13);
    func_001A6A78__c10(obj, p13, 9, &n13);
    func_001A6A78__c10(obj, p13, 6, &n13);

    e1 = ENTRY__c10(obj, p1);
    e0 = ENTRY__c10(obj, p0);
    e4 = ENTRY__c10(obj, p4);
    e7 = ENTRY__c10(obj, p7);
    e10 = ENTRY__c10(obj, p10);
    e3 = ENTRY__c10(obj, p3);
    e6 = ENTRY__c10(obj, p6);
    e9 = ENTRY__c10(obj, p9);

    func_001A6B48__c10(obj, p2, 0, &n2, (e1->dist + e0->dist) * 0.8f);
    func_001A6B48__c10(obj, p5, 3, &n5, (e4->dist + e3->dist) * 0.8f);
    func_001A6B48__c10(obj, p8, 6, &n8, (e7->dist + e9->dist) * 0.92f);
    func_001A6B48__c10(obj, p11, 9, &n11, (e10->dist + e6->dist) * 0.92f);
    func_001A6BE8__c10(obj, p2, 0, &n2, (e1->dist + e0->dist) * 0.3f);
    func_001A6BE8__c10(obj, p5, 3, &n5, (e4->dist + e3->dist) * 0.3f);
    func_001A6BE8__c10(obj, p8, 6, &n8, (e7->dist + e9->dist) * 0.6f);
    func_001A6BE8__c10(obj, p11, 9, &n11, (e10->dist + e6->dist) * 0.6f);

    func_001A6C88__c10(obj, p1, 0, 0, D_0029D380);
    func_001A6C88__c10(obj, p2, 1, 0, D_0029D380);
    func_001A6C88__c10(obj, p4, 3, 0, D_0029D380);
    func_001A6C88__c10(obj, p5, 4, 0, D_0029D380);
    func_001A6C88__c10(obj, p7, 6, 0, D_0029D380);
    func_001A6C88__c10(obj, p8, 7, 0, D_0029D380);
    func_001A6C88__c10(obj, p10, 9, 0, D_0029D380);
    func_001A6C88__c10(obj, p11, 10, 0, D_0029D380);

    func_001A6B48__c10(obj, p7, 10, &n7, e7->dist * 1.5f);
    func_001A6B48__c10(obj, p11, 8, &n11, e10->dist * 1.5f);
    func_001A6BE8__c10(obj, p0, 7, &n0, e0->dist * 2.0f);
    func_001A6BE8__c10(obj, p0, 10, &n0, e0->dist * 2.0f);
    func_001A6BE8__c10(obj, p3, 10, &n3, e0->dist * 2.0f);
    func_001A6BE8__c10(obj, p3, 7, &n3, e0->dist * 2.0f);
    func_001A6BE8__c10(obj, p0, 8, &n0, e0->dist * 3.75f);
    func_001A6BE8__c10(obj, p8, 3, &n8, e0->dist * 3.75f);
    func_001A6BE8__c10(obj, p3, 11, &n3, e0->dist * 3.75f);
    func_001A6BE8__c10(obj, p11, 0, &n11, e0->dist * 3.75f);

    ScaleAdd__c10(&v40, &p3->pos, &p0->pos, 0.5f);
    ScaleAdd__c10(&v50, &p9->pos, &p6->pos, 0.5f);

    r = func_0023AFA8(&obj->count, 3, 9, 0, 6, 0.8f);
    p5->links[0] = r;
    p2->links[0] = r;
    p4->links[0] = r;
    p1->links[0] = r;
    p11->links[0] = r;
    p8->links[0] = r;
    p10->links[0] = r;
    p7->links[0] = r;
    r = func_0023AFA8(&obj->count, 9, 10, -1, -1, 0.4f);
    p8->links[1] = r;
    p7->links[1] = r;
    r = func_0023AFA8(&obj->count, 10, 11, -1, -1, 0.4f);
    p8->links[2] = r;
    p7->links[2] = r;
    r = func_0023AFA8(&obj->count, 6, 7, -1, -1, 0.4f);
    p11->links[2] = r;
    p10->links[1] = r;
    r = func_0023AFA8(&obj->count, 7, 8, -1, -1, 0.4f);
    p11->links[3] = r;
    p10->links[2] = r;

    func_001B1FE0(obj->xDA4 + 0x1CB0, &v40);
    func_002224E0((Matrix4 *)(*(char **)(obj->xDA4 + 0x16D4) + 0x40), (Vector3 *)(&v50));
    Sub__c10(&obj->xDC0, &v40, &v50);
    p7->x48 = func_0023AFF8(&obj->count, 3, 0, 6, 6, 8, 0);
    p10->x48 = func_0023AFF8(&obj->count, 0, 3, 9, 9, 11, 1);
}

typedef struct Node__c12 {
    Vec pos;
    Vec x10;
    char pad20[0x30];
} Node__c12;

typedef struct Obj__c12 {
    char pad0[0x30];
    Node__c12 nodes[13];     /* 0x30 */
} Obj__c12;

float D_0029D3A8 = 0.0002f;

/* dst.xyz = a.xyz - b.xyz (VU0 macro mode) */
static inline void VecSub__c12(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*dst) : "r"(a), "r"(b) : "memory");
}

void func_001A77E0(Obj__c12 *self, Vec *v)
{
    Vec scaled;
    int tmp;
    int i;

    /* sceVu0ScaleVector(scaled, v, D_0029D3A8) */
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%2)\n"
        "mfc1 %0, %3\n"
        "qmtc2 %0, $vf12\n"
        "vmulx.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, 0x0(%1)\n"
        : "=&r"(tmp) : "r"(&scaled), "r"(v), "f"(D_0029D3A8) : "memory");
    for (i = 0; i < 13; i++) {
        VecSub__c12(&self->nodes[i].x10, &self->nodes[i].x10, &scaled);
    }
}

float D_0029D3AC = 0.0002f;

void func_001A7830(Obj__c12 *self, Vec *v)
{
    Vec scaled;
    int tmp;
    int i;

    /* sceVu0ScaleVector(scaled, v, D_0029D3AC) */
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%2)\n"
        "mfc1 %0, %3\n"
        "qmtc2 %0, $vf12\n"
        "vmulx.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, 0x0(%1)\n"
        : "=&r"(tmp) : "r"(&scaled), "r"(v), "f"(D_0029D3AC) : "memory");
    for (i = 6; i < 12; i++) {
        VecSub__c12(&self->nodes[i].x10, &self->nodes[i].x10, &scaled);
    }
}

typedef struct Node__c13 {
    Vec pos;
    Vec vel;
    char pad20[0x30];
} Node__c13;

typedef struct Obj__c13 {
    char pad0[0x10];
    Vec x10;
    Vec x20;
    Node__c13 nodes[13];     /* 0x30 */
    char pad440[0xD9D - 0x440];
    unsigned char xD9D;
    char padD9E[6];
    Rider__c11 *xDA4;
    char padDA8[0xDC0 - 0xDA8];
    Vec xDC0;
} Obj__c13;

extern void func_001B1C20(void *, Vec *);

static inline void VScale__c13(Vec *dst, Vec *src, float f)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(src), "f"(f) : "$2");
}

void func_001A7880(Obj__c11 *obj)
{
    Obj__c13 *self = (Obj__c13 *)obj;
    Vec diff;
    Vec sum;
    Vec a;
    Vec b;
    char *data;
    Vec *center;
    Vec *pos0, *pos3, *pos6, *pos9;
    Vec *vel0, *vel3, *vel6, *vel9;
    float scale;
    int i;
    int j;

    self->xD9D = 1;
    func_001B1C20(self->xDA4->x1CB0, &self->x20);
    for (i = 0; i < 12; i++) {
        VSub(&self->nodes[i].pos, &self->nodes[i].pos, &self->x10);
        VSub(&self->nodes[i].vel, &self->nodes[i].vel, &self->x10);
    }
    data = self->xDA4->x16D0;
    if (*(int *)(data + 0x407C) == 5) {
        scale = 0.75f;
        if (*(int *)(data + 0x7984) == 0) {
            scale = 1.25f;
        }
        for (j = 0; j < 12; j++) {
            VSub(&diff, &self->nodes[j].pos, &self->nodes[j].vel);
            VScale__c13(&diff, &diff, scale);
            VSub(&self->nodes[j].vel, &self->nodes[j].pos, &diff);
        }
    }
    center = &self->nodes[12].pos;
    pos0 = &self->nodes[0].pos;
    pos3 = &self->nodes[3].pos;
    pos6 = &self->nodes[6].pos;
    pos9 = &self->nodes[9].pos;
    vel0 = &self->nodes[0].vel;
    vel3 = &self->nodes[3].vel;
    vel6 = &self->nodes[6].vel;
    vel9 = &self->nodes[9].vel;
    center->x = 0.0f;
    center->y = 0.0f;
    center->z = 0.0f;
    VSub(&sum, pos0, vel0);
    VSub(&diff, pos3, vel3);
    VAdd(&sum, &sum, &diff);
    VSub(&diff, pos6, vel6);
    VAdd(&sum, &sum, &diff);
    VSub(&diff, pos9, vel9);
    VAdd(&sum, &sum, &diff);
    VScale__c13(&sum, &sum, 0.25f);
    VSub(&self->nodes[12].vel, center, &sum);
    func_001B1FE0(self->xDA4->x1CB0, (Vec__c10 *)&a);
    func_002224E0((Matrix4 *)((char *)self->xDA4->x16D4 + 0x40), (Vector3 *)((Vec__c10 *)&b));
    VSub(&self->xDC0, &a, &b);
}

typedef struct AnimN23 { char pad0[12]; float *values; unsigned char *bits; } AnimN23;
typedef struct CountN23 { unsigned short x0, count; } CountN23;
typedef struct ModelN23 { char pad0[32]; CountN23 *info; } ModelN23;
typedef struct BoneN23 { char pad0[18]; short x12,x14,x16; char pad18[40]; Mat x40,x80; } BoneN23;
typedef struct NodeN23 { Vec pos, velocity; char pad20[16]; BoneN23 *bone; char pad34[4]; signed char link; char pad39[23]; } NodeN23;
typedef struct LinkN23 { short index, x2; float length; } LinkN23;
typedef struct RootN23 { char pad0[48]; NodeN23 nodes[13]; char pad440[0xBCC-0x440]; LinkN23 links[59]; void *owner; } RootN23;
extern int func_001B1C08(void *);
extern void func_001A8200(void *, Vec *, int);
extern void func_00221AB8(Mat *, Mat *, Mat *);
extern void func_00221D58(Mat *, Mat *);
extern Mat D_00769830[];
static inline void SUBN(void *d, void *a, void *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(a),"r"(b)); }
static inline void ADDN(void *d, void *a, void *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(a),"r"(b)); }
static inline void CROSSN(void *d, void *a, void *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(a),"r"(b)); }
static inline void SCALEN(void *d, void *a, float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*(Vec *)(d)) : "r"(a),"f"(f) : "$2"); }
static inline void COPYN(void *d, void *a) { __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(*(Vec *)(d)) : "r"(a) : "$2"); }
/* sceVu0CopyMatrix */
static inline void CopyN(Mat *d, Mat *src) {
 Mat *s = src;
 __asm__ volatile("lq $8,0(%4)\n\tlq $9,0x10(%4)\n\tlq $10,0x20(%4)\n\tlq $11,0x30(%4)\n\tsq $8,%0\n\tsq $9,%1\n\tsq $10,%2\n\tsq $11,%3" : "=m"(d->q[0]),"=m"(d->q[1]),"=m"(d->q[2]),"=m"(d->q[3]) : "r"(s) : "$8","$9","$10","$11");
}
/* Scalar normalization sequence from the game's libvu0 wrapper. */
static inline void NormalizeN(Vec *d, Vec *s) {
 float x=s->x,y=s->y,z=s->z,len,inv; int bits;
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 d->x=x*inv; d->y=y*inv; d->z=z*inv;
}
#define STORE_N(field,component) \
 anim->values[bone->field]=rot.component; \
 { unsigned int bit = bone->field; unsigned char *flags = anim->bits + (bit>>3); *flags |= 1<<(bit&7); }
void func_001A7AA0(RootN23 *obj, AnimN23 *anim, ModelN23 *model, int group) {
 Vec rot;
 Mat current,saved,previous,temp,result,old;
 Vec normal,cross,a,b;
 Mat world,intermediate,local,inverse;
 Vec scaled,direction,original,delta;
 int i; unsigned int j;
 float length;
 BoneN23 *bone;
 LinkN23 *link;
 for(j=0;(int)j<model->info->count;j++) anim->bits[j>>3] &= ~(1<<(j&7));
 CopyN(&previous,&D_00769830[group*3+func_001B1C08(obj->owner)]);
 for(i=group*3;i<group*3+2;i++) {
  NodeN23 *point=&obj->nodes[i];
  if(i==group*3) {
   SUBN(&a,(char *)obj+i*0x50+0x30,(char *)obj+i*0x50+0x80);
   SUBN(&b,(char *)obj+i*0x50+0xD0,(char *)obj+i*0x50+0x80);
   if(i==0 || i==9) {CROSSN(&cross,&b,&a);} else {CROSSN(&cross,&a,&b);}
   NormalizeN(&normal,&cross);
  }
  link=&obj->links[obj->nodes[i].link];
  func_001B16C8(&current,&obj->nodes[i].pos,&obj->nodes[link->index].pos,&normal);
  CopyN(&saved,&current);
  func_00221AB8(&result,&current,&previous);
  CopyN(&old,&previous);
  CopyN(&previous,&current);
  bone=point->bone;
  func_00221D58(&temp,&bone->x40);
  func_00221930((Matrix4 *)(&current), (Matrix4 *)(&temp), (Matrix4 *)(&result));
  func_00221D58(&temp,&bone->x80);
  func_00221930((Matrix4 *)(&result), (Matrix4 *)(&current), (Matrix4 *)(&temp));
  func_002224E0((Matrix4 *)(&result), (Vector3 *)((Vec__c10 *)&rot));
  {
  int active=i<6;
  if(active) func_001A8200(obj,&rot,i);
  STORE_N(x12,x); STORE_N(x14,y); STORE_N(x16,z);
  length=obj->links[point->link].length;
  if(active) {
   rot.x=-rot.x; rot.y=-rot.y; rot.z=-rot.z;
   func_00222040(&inverse,&rot);
   func_00221930((Matrix4 *)(&local), (Matrix4 *)(&inverse), (Matrix4 *)(&bone->x80));
   func_00221930((Matrix4 *)(&intermediate), (Matrix4 *)(&local), (Matrix4 *)(&bone->x40));
   func_00221930((Matrix4 *)(&world), (Matrix4 *)(&intermediate), (Matrix4 *)(&old));
   CopyN(&previous,&world);
   NormalizeN(&direction,(Vec *)&world);
   SCALEN(&scaled,&direction,length);
   COPYN(&original,&obj->nodes[link->index].pos);
   ADDN(&obj->nodes[link->index].pos,&obj->nodes[i].pos,&scaled);
   SUBN(&delta,&obj->nodes[link->index].pos,&original);
   SCALEN(&delta,&delta,D_0029D370);
   ADDN(&obj->nodes[link->index].velocity,&obj->nodes[link->index].velocity,&delta);
  }
  }
 }
 func_00221AB8(&result,&saved,&previous);
 bone=obj->nodes[group*3+2].bone;
 func_00221D58(&temp,&bone->x40);
 func_00221930((Matrix4 *)(&current), (Matrix4 *)(&temp), (Matrix4 *)(&result));
 func_00221D58(&temp,&bone->x80);
 func_00221930((Matrix4 *)(&result), (Matrix4 *)(&current), (Matrix4 *)(&temp));
 func_002224E0((Matrix4 *)(&result), (Vector3 *)((Vec__c10 *)&rot));
 STORE_N(x12,x); STORE_N(x14,y); STORE_N(x16,z);
}
