/* UNIT 0x00156580 0x00156A70 */
#include "motion_interpolation.h"
#if defined(__mips__)
#define CallShared0012D660_0 func_0012D660
#else
#define CallShared0012D660_0 Forward0012D660_1
#endif
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001554B8_6_func_001554B8 func_001554B8
#else
#define CallScalarBridge001554B8_6_func_001554B8 ScalarBridge001554B8_6
#endif
#include "camera.h"
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Global {
    char pad0[0x10C];
    int x10C;
    char pad110[0x364 - 0x110];
    int x364;
} Global;

typedef struct Frame {
    char pad0[0x10];
    Vec x10;
} Frame;

typedef struct Sub {
    char pad0[0x38];
    Frame *x38;
} Sub;

typedef struct Rider {
    char pad0[0xFE0];
    char xFE0[0x1184 - 0xFE0];
    int x1184;
    char pad1188[0x124C - 0x1188];
    int x124C;
    char pad1250[0x13D4 - 0x1250];
    unsigned char x13D4;
    char pad13D5[0x3530 - 0x13D5];
    char x3530[0x40C0 - 0x3530];
    struct Sub x40C0;
    char pad40FC[0x4200 - 0x40FC];
    float x4200[3];
    char pad420C[0x43D0 - 0x420C];
    unsigned long x43D0;
    char pad43D8[0x4450 - 0x43D8];
    unsigned long x4450;
    char pad4458[0x4ECC - 0x4458];
    void *x4ECC;
    char pad4ED0[0x6320 - 0x4ED0];
    char x6320[0x78D0 - 0x6320];
    void *x78D0;
    struct Other *x78D4;
    char pad78D8[0x792C - 0x78D8];
    Frame *x792C;
    char pad7930[0x797C - 0x7930];
    int x797C;
    char pad7980[0x7988 - 0x7980];
    int x7988;
    char pad798C[0x79A0 - 0x798C];
    int x79A0;
    char pad79A4[0x79AC - 0x79A4];
    int x79AC;
    char pad79B0[0x7A5C - 0x79B0];
    int x7A5C;
    void *x7A60;
    void *x7A64;
    char pad7A68[0x7B00 - 0x7A68];
    int x7B00;
} Rider;

typedef struct Other {
    char pad0[8];
    Rider *x8;
} Other;

typedef struct Entry {
    char pad0[0x2BC];
    int x2BC;
    char pad2C0[0x380 - 0x2C0];
} Entry;

typedef struct Obj {
    Rider *x0;
    char pad4[0x3C];
    Vec x40;
    char pad50[0xB0 - 0x50];
    int xB0;
    int xB4;
    int xB8;
    int xBC;
    char padC0[8];
    unsigned long xC8;
} Obj;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern Global D_004B25A0;
extern Entry D_0037EA60[];
extern char D_004D0318[];
extern int func_00238F60(void);
extern float func_001385A8(void *p);
extern int func_00138560(void *p);
extern int func_00148BA8(void *table, Rider *r);
extern int func_00148B68(void *table, Rider *r);

#if defined(__mips__)
extern void func_001554B8(Obj *obj, float a);
#else

#endif

extern void func_0012DE68(Rider *r, int a);
extern void func_00155208(Obj *obj);
extern void func_00155380(Obj *obj);
extern void func_00155148(Obj *obj);
extern void func_00156A70(Obj *obj);
extern void func_00162D98(int type, int b);
extern void func_0017D740(void *p, Rider *r);
extern void func_0012DDC0(Rider *r);


extern void func_00199A48(void *p);
extern void func_00199AD8(void *p, float f);
extern void func_002210C0(void *p);
extern void func_0012D660(Rider *r, float f, int a);
extern void func_00232AD0(void *p, Vec *v);
extern void func_0020CD78(int id, float a, float b);
extern void func_00162920(Rider *r);
extern void func_0022DDD8(void *p);
extern void func_0012D838(Rider *r, int a, int b);
extern void func_0017DC78(void *p);
extern void func_0017DCA0(void *p);
extern void func_00157400(Obj *obj, int a);

static inline int IsCoop(void)
{
    Global *g = &D_004B25A0;
    int ok = 0;
    if (g->x10C == 15) {
        ok = g->x364 == 1;
    }
    return ok;
}

static inline void Reset(Rider *r)
{
    int on = 1;

    if (r->x13D4) {
        r->x124C = on;
    } else {
        r->x1184 = on;
        r->x124C = 0;
    }
}

static inline int GreaterThan(int count, int limit)
{
    return count > limit;
}

void func_00156580(Obj *obj)
{
    Vec vec;
    Rider *other;
    Rider *r;
    Sub *sub;
    float *v;
    int id;
    int dec;
    float diff;
    float len;

    dec = 0;
    if (obj->xC8 & 4) {
        obj->xB0 -= func_00238F60();
        if (obj->xB0 >= 0) {
            goto done;
        }
        if (IsCoop() && GreaterThan(obj->x0->x79AC, 1)) {
            obj->xB0 = 10;
        } else if (obj->x0->x7A5C == 3 && obj->x0->x78D4 != 0
                   && (other = obj->x0->x78D4->x8) != 0
                   && func_001385A8(&other->x40C0) > 0.05f) {
            diff = other->x792C->x10.z - obj->x0->x792C->x10.z;
            if (diff > -30.0f && diff < 30.0f && D_004B25A0.x10C != 15) {
                obj->xB0 = 10;
            }
        }
        if (obj->xB0 < 0) {
            obj->xC8 |= 8;
            if (obj->x0->x79A0 != 0) {
                dec = 1;
                obj->x0->x79A0--;
            }
            if (func_00148BA8(D_0037EA60, obj->x0) == 0) {
                CallScalarBridge001554B8_6_func_001554B8(obj, 10.0f);
                obj->xB8 = 240;
            } else if (D_0037EA60[obj->x0->x7988].x2BC == 0) {
                func_0012DE68(obj->x0, 1);
            }
        }
    } else {
        obj->xBC += func_00238F60();
        if (!func_00138560(&obj->x0->x40C0)) {
            COPY_VECTOR(obj->x40, &obj->x0->x792C->x10);
        }
        if (D_004B25A0.x10C != 17) {
            func_00155208(obj);
        }
        func_00155380(obj);
    }
done:
    if (IsCoop() && GreaterThan(obj->x0->x79AC, 1)) {
        return;
    }
    if (!((unsigned char)(obj->xC8 >> 2) & 1)) {
        return;
    }
    if ((unsigned char)(obj->xC8 >> 3) & 1 && obj->x0->x4ECC != obj->x0->x6320) {
        func_00155148(obj);
        func_00156A70(obj);
        obj->x0->x43D0 &= 0xFFE0FFFFFFFFFFFFUL;
        obj->x0->x4450 &= 0xFFE0FFFFFFFFFFFFUL;
        func_00162D98(0, obj->x0->x797C);
        func_0017D740(obj->x0->xFE0, obj->x0);
        func_0012DDC0(obj->x0);
        func_001B33A8(obj->x0->x3530);
        func_001B0E00((RiderCamera *)(obj->x0->x3530));
        if (obj->x0->x78D0 != 0) {
            func_00199A48(obj->x0->x78D0);
        }
        func_002210C0(D_004D0318);
        CallShared0012D660_0(obj->x0, 1.0f, -1);
        id = func_00148B68(D_0037EA60, obj->x0);
        if (dec) {
            r = obj->x0;
            v = r->x4200;
            sub = &r->x40C0;
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
            len *= 0.56f;
            __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                              : "=m"(vec) : "r"(&sub->x38->x10), "f"(len) : "$2");
            func_00232AD0(sub, &vec);
            if (id >= 0) {
                func_0020CD78(id, 0.0625f, 0.0625f);
            }
        } else if (id >= 0) {
            func_0020CD78(id, 0.0625f, 0.125f);
        }
        obj->x0->x7B00 = 0;
        func_00162920(obj->x0);
        if (obj->x0->x7A60 != 0) {
            func_0022DDD8(obj->x0->x7A60);
        }
        if (obj->x0->x7A64 != 0) {
            func_0022DDD8(obj->x0->x7A64);
        }
        func_0012D838(obj->x0, 0, 0);
    } else {
        func_0017DC78(obj->x0->xFE0);
        func_0017DCA0(obj->x0->xFE0);
        Reset(obj->x0);
        func_00157400(obj, 0);
        if (obj->x0->x78D0 != 0) {
            func_00199AD8(obj->x0->x78D0, 0.0f);
        }
    }
}
