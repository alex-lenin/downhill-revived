/* UNIT 0x0010F0D8 0x0010F710 */
/* func_0010F0D8: rider input/state update.
 * Inline asm: vector copy, VU0 add, mula/madda/madd, rsqrt.s, min.s/max.s,
 * and abs.s (Abs, same helper as promoted src/unsorted/0010FDA0.c). The
 * volatile abs.s is a scheduling barrier: retail keeps the constant loads and
 * the x796C load below both abs.s, which builtin fabsf does not reproduce.
 */
#include "vector.h"
typedef Vector4 Vec;

typedef struct Rider {
    char pad0[0x4180];
    Vec x4180;
    char pad4190[0x4308 - 0x4190];
    float x4308;
    char pad430C[0x4564 - 0x430C];
    short x4564;
    char pad4566[0x792C - 0x4566];
    char *x792C;
    char pad7930[0x796C - 0x7930];
    int x796C;
    char pad7970[0x797C - 0x7970];
    int x797C;
} Rider;

typedef struct Item { char pad0[0x10]; unsigned char x10; } Item;

typedef struct Obj {
    Rider *x0;
    char pad4[0x30 - 4];
    Vec x30;
    char pad40[0x74 - 0x40];
    Item *x74;
    char pad78[0x220 - 0x78];
    Vec x220;
    Vec x230;
    char pad240[0x27C - 0x240];
    float x27C;
    float x280;
    char pad284[0x2B0 - 0x284];
    float x2B0;
    char pad2B4[0x334 - 0x2B4];
    float x334;
    char pad338[0x418 - 0x338];
    int x418;
    char pad41C[0x440 - 0x41C];
    unsigned long x440;
} Obj;

typedef struct Input {
    char p0[0x1A4];
    int x1A4;
    char p1A8[8];
    int x1B0, x1B4;
    char p1B8[0x20];
    int x1D8;
    char p1DC[0x38];
    int x214, x218;
    char p21C[0x50];
    int x26C;
    char p270[8];
    int x278, x27C;
    char p280[0x20];
    int x2A0;
    char p2A4[0x150];
    unsigned char x3F4;
    char p3F5[2];
    unsigned char x3F7, x3F8;
    char p3F9[8];
    unsigned char x401;
} Input;

/* 0x501B4 into this bss block is only reached with the base split off, so the
   access is modelled as a struct field. */
typedef struct Unk2AF2C0 { char pad0[0x50000]; char pad1[0x1B4]; int x501B4; } Unk2AF2C0;

#define MAKE_SET(a,b,c) static __inline__ void set_##a(Input *in,int value) {if(in->a) in->b=value; else {in->c=value;in->b=0;}}
#define SET(a,b,c) set_##a(in,1);

MAKE_SET(x3F7,x278,x1B0)
MAKE_SET(x3F8,x27C,x1B4)
MAKE_SET(x401,x2A0,x1D8)

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline float maxf(float a, float b) { float r; __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
static inline float minf(float a, float b) { float r; __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}
extern Unk2AF2C0 D_002AF2C0;
extern char D_00383A10[];

extern int func_0010F710(Obj *, float *);
extern int func_00158158(void *, char *, float *, float, float);
extern void func_0010B998(Obj *, Item *, Vec *);
extern float func_00222640(Vec *);
extern void func_002087C0(float *);
extern float func_00235EA8(float, float, float, float);
extern float func_0012E070(Rider *);



void func_0010F0D8(Obj *obj, Input *in)
{
    Vec v;
    struct { float height; float angle; } scratch;
    float limit;
    float f20;
    float f13;
    float sum;
    float inv;
    int pressed;
    float heading;

    if (obj->x440 & ((unsigned long)0xC000 << 37))
        return;

    scratch.height = 0.0f;
    if (func_0010F710(obj, &scratch.height)) {
        obj->x440 |= (unsigned long)0x8000 << 24;
    } else if ((obj->x440 & ((unsigned long)0x8000 << 24)) ||
               obj->x0->x797C == D_002AF2C0.x501B4) {
        obj->x440 = (obj->x440 & ~((unsigned long)0x8000 << 24)) |
            ((unsigned long)(func_00158158(&D_00383A10, obj->x0->x792C, &scratch.height, -300.0f, 20.0f) & 1) << 39);
    }

    if (obj->x440 & ((unsigned long)0x8000 << 24)) {
        if (scratch.height < 0.0f) {
            SET(x3F7,x278,x1B0)
            set_x3F8(in,0);
            limit = obj->x74->x10;
            if (!(obj->x220.x < limit))
                return;
        } else {
            SET(x3F8,x27C,x1B4)
            set_x3F7(in,0);
            limit = -obj->x74->x10;
            if (!(limit < obj->x220.x))
                return;
        }
        obj->x230.x = limit;
        obj->x230.y = 0.0f;
        obj->x230.z = 0.0f;
        COPY_VECTOR(obj->x220, &obj->x230);
        func_0010B998(obj, obj->x74, &v);
        __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                          : "=m"(obj->x30) : "r"(&obj->x30), "r"(&v) : "memory");
        return;
    } else {
        if (obj->x0->x4564 >= 0x3D) {
            COPY_VECTOR(v, &obj->x0->x4180);
            v.z *= 0.01f;
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(sum) : "f"(v.x), "f"(v.y), "f"(v.z));
            __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(sum));
            v.x *= inv;
            v.y *= inv;
            v.z *= inv;
            heading = func_00222640(&v);
            scratch.angle = obj->x27C - (heading - obj->x280);
            func_002087C0(&scratch.angle);
            if (Abs(scratch.angle) > 0.5235988f) {
                float target = heading;
                if (scratch.angle > 0.0f) target += 0.5235988f;
                else target -= 0.5235988f;
                obj->x27C = target - obj->x280;
                func_002087C0(&obj->x27C);
            }
        }
        f20 = obj->x27C - 2.0f * obj->x0->x4308;
        f13 = Abs(f20) * 2.864789f;
        if (obj->x0->x796C == 0xA0) {
            f13 = minf(f13, 0.2f);
        } else if (obj->x440 & ((unsigned long)0x8000 << 35)) {
            f13 = maxf(f13, 0.5f);
        } else {
            f13 = minf(f13, 1.0f);
        }
        if (f13 < obj->x2B0) {
            obj->x2B0 = f13;
        } else {
            obj->x2B0 = func_00235EA8(obj->x2B0, f13, 0.07f, 0.001f);
        }
        pressed = in->x3F7 ? in->x278 : in->x214;
        if (pressed) {
            if (f20 > 0.0f) {
                if (obj->x418 == 0 && (func_0012E070(obj->x0) != 0.0f || obj->x334 < f20)) {
                    SET(x3F7,x278,x1B0)
                    SET(x401,x2A0,x1D8)
                } else {
                    SET(x3F7,x278,x1B0)
                    set_x3F8(in,0);
                }
            }
        } else {
            pressed = in->x3F8 ? in->x27C : in->x218;
            if (pressed) {
                if (f20 < 0.0f) {
                    if (obj->x418 == 0 && (func_0012E070(obj->x0) != 0.0f || f20 < -obj->x334)) {
                        SET(x3F8,x27C,x1B4)
                        SET(x401,x2A0,x1D8)
                    } else {
                        SET(x3F8,x27C,x1B4)
                        set_x3F7(in,0);
                    }
                }
            } else {
                if (f20 > 0.05235988f) {
                    SET(x3F7,x278,x1B0)
                    set_x3F8(in,0);
                } else if (f20 < -0.05235988f) {
                    SET(x3F8,x27C,x1B4)
                    set_x3F7(in,0);
                }
            }
        }
    }
}
