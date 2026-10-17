/* UNIT 0x0010B2B8 0x0010B770 */
#include "vector.h"
typedef Vector4 Vec;

typedef struct Node {
    Vec x0;
    unsigned int x10;
    unsigned short x14;
    unsigned char x16;
    unsigned char x17;
    short links[4];
} Node;

typedef struct World {
    char pad0[0x792C];
    char *x792C;
    char pad7930[0x7958 - 0x7930];
    Node *x7958;
} World;

typedef union Flags3A0 {
    int word;
    struct {
        unsigned char low;
        unsigned char b1, b2, b3;
    } bytes;
} Flags3A0;

typedef struct Sub2D4 {
    char pad0[0xCC];
    Flags3A0 flags;
} Sub2D4;

typedef struct Obj {
    World *x0;
    char pad4[4];
    World *x8;
    char padC[4];
    Vec x10;
    Vec x20;
    Vec x30;
    Vec x40;
    Vec x50;
    Vec x60;
    Node *x70;
    Node *x74;
    Node *x78;
    char pad7C[0xF0 - 0x7C];
    Vec xF0;
    Vec x100;
    char pad110[0x240 - 0x110];
    float x240;
    float x244;
    char pad248[0x250 - 0x248];
    float x250;
    float x254;
    char pad258[0x260 - 0x258];
    float x260;
    float x264;
    float x268;
    float x26C;
    float x270;
    float x274;
    char pad278[0x2D4 - 0x278];
    Sub2D4 x2D4;
    char pad3A4[0x3BC - 0x3A4];
    int x3BC;
    char pad3C0[0x440 - 0x3C0];
    long x440;
} Obj;

extern Node D_0029EA20[];
extern float D_0029C104[];

extern void func_0010B770(Obj *);
extern Node *func_0010B8E8(Obj *);
extern void func_0010B920(Obj *);
extern void func_0010B998(Obj *, Node *, Vec *);
extern int func_0010D778(Obj *);
extern void func_0010F8E8(Obj *);
extern void func_001122D8(Obj *, void *);
extern int func_00118640(Node *, Node *);
extern Node *func_00118650(Node *, Node *, int, int);
extern Node *func_00118898(Node *, Node *, Node *, int, int);
extern Node *func_00118BB8(Node *, Node *, Node *);
extern float func_00222610(void *, void *);

static inline void add_xyz(Vec *dst, Vec *delta)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "vadd.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*dst) : "r"(dst), "r"(delta));
}

static inline void sub_xyz(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "vsub.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*dst) : "r"(a), "r"(b));
}

static inline float normalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile (
        "mula.s %2, %2\n\t"
        "madda.s %3, %3\n\t"
        "madd.s %1, %4, %4\n\t"
        "sqrt.s %1, %1\n\t"
        "lui $3, 0x3f80\n\t"
        "mtc1 $3, %0\n\t"
        "nop\n\t"
        "div.s %0, %0, %1"
        : "=&f"(inv), "=f"(len) : "f"(x), "f"(y), "f"(z) : "$3");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline float normalize_last(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile (
        "mula.s %2, %2\n\t"
        "madda.s %3, %3\n\t"
        "madd.s %1, %4, %5\n\t"
        "sqrt.s %1, %1\n\t"
        "lui $3, 0x3f80\n\t"
        "mtc1 $3, %0\n\t"
        "nop\n\t"
        "div.s %0, %0, %1"
        : "=&f"(inv), "=f"(len) : "f"(x), "f"(y), "f"(z), "f"(z) : "$3");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

static inline float abs_s(float v)
{
    return __builtin_fabsf(v);
}

static inline Sub2D4 *get_sub(Obj *obj)
{
    return &obj->x2D4;
}

void func_0010B2B8(Obj *obj, Node *newPoint)
{
    Vec delta;
    Vec *current;

    obj->x440 &= (long)0xFFDFFFFFFFFFFFFF;

    if (obj->x74 != 0 && obj->x70 != obj->x74) {
        obj->x70 = obj->x74;
        obj->x10 = obj->x30;
        obj->x20 = obj->x40;
    }

    if (newPoint != 0) {
        obj->x74 = newPoint;
        obj->x30 = *(Vec *)newPoint;
        obj->x40 = *(Vec *)((char *)newPoint + 0x10);
        func_0010B998(obj, newPoint, &delta);
        add_xyz(&obj->x30, &delta);
        current = &obj->x30;
        obj->x78 = 0;
    } else {
        current = &obj->x30;
        if (obj->x78 != 0) {
            obj->x30 = obj->x50;
            obj->x40 = obj->x60;
            obj->x74 = obj->x78;
            obj->x268 = obj->x26C;
            obj->x78 = 0;
        } else if (obj->x74 != 0) {
            obj->x74 = func_00118650(D_0029EA20, obj->x74,
                                     get_sub(obj)->flags.bytes.low & 1,
                                     func_0010D778(obj));
            obj->x74 = func_0010B8E8(obj);
            func_0010B998(obj, obj->x74, &delta);
            add_xyz(&obj->x30, &delta);
        }
    }

    if (obj->x74 != 0) {
        if (obj->x78 == 0) {
            Node *candidate;
            int stateFlags;
            candidate = 0;
            stateFlags = get_sub(obj)->flags.word;

            if ((stateFlags >> 2) & 1) {
                if (obj->x8 != 0) {
                    candidate = obj->x8->x7958;
                    if (candidate != 0) {
                        if (func_00118640(D_0029EA20, candidate) <
                            func_00118640(D_0029EA20, obj->x74)) {
                            candidate = func_00118BB8(D_0029EA20, obj->x74, candidate);
                            if (candidate != 0) {
                                float difference;
                                float absolute;
                                difference = *(float *)(obj->x8->x792C + 0x18) -
                                             *(float *)(obj->x0->x792C + 0x18);
                                absolute = abs_s(difference);
                                __asm__ volatile ("L_web6_abs_hold:" : : "f"(difference));
                                if (20.0f > absolute) {
                                    obj->x440 |= ((long)0x8000 << 38);
                                }
                            }
                        }
                    }
                }
            }

            if (candidate == 0) {
                if (get_sub(obj)->flags.bytes.low & 1) {
                    candidate = func_00118650(D_0029EA20, obj->x74, 1, func_0010D778(obj));
                }

                if (candidate == 0) {
                    if (obj->x8 != 0) {
                        candidate = obj->x8->x7958;
                        if (candidate != 0) {
                            if (obj->x74 != 0) {
                                if (func_00118640(D_0029EA20, candidate) <
                                    func_00118640(D_0029EA20, obj->x74)) {
                                    candidate = func_00118BB8(D_0029EA20, obj->x74, candidate);
                                }
                            }
                        }
                    }
                }
            }

            if (candidate != 0) {
                obj->x78 = func_00118898(D_0029EA20, obj->x74, candidate,
                                         get_sub(obj)->flags.bytes.low & 1, func_0010D778(obj));
            } else {
                obj->x78 = func_00118650(D_0029EA20, obj->x74,
                                         get_sub(obj)->flags.bytes.low & 1, func_0010D778(obj));
            }

            obj->x50 = *(Vec *)obj->x78;
            obj->x60 = *(Vec *)((char *)obj->x78 + 0x10);
            func_0010B998(obj, obj->x78, &delta);
            add_xyz(&obj->x50, &delta);
            obj->x26C = func_00222610(current, &obj->x50);
            func_0010B920(obj);
        }
    }

    func_0010B770(obj);
    obj->x240 = D_0029C104[0];
    func_0010F8E8(obj);
    obj->x244 = obj->x240;
    obj->x254 = obj->x250;
    obj->x264 = obj->x260;
    obj->x3BC = 0;

    {
        float len;
        sub_xyz(&delta, current, &obj->x10);
        len = normalize(&obj->xF0, &delta);
        obj->x270 = len;
    }

    {
        float len;
        sub_xyz(&delta, &obj->x50, current);
        len = normalize_last(&obj->x100, &delta);
        obj->x274 = len;
    }

    func_001122D8(obj, obj->x0->x792C + 0x10);
}
