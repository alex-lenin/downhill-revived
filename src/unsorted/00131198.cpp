/* UNIT 0x00131198 0x00131548 */
#include "object_registry.h"
typedef float Vector[4] __attribute__((aligned(16)));

typedef struct Item {
    int x0;
    unsigned short x4;
    short x6;
    float x8;
    char padC[0x10];
    float x1C;
    Vector x20;
} Item;

typedef struct Source {
    char pad0[0x20];
    Item *items;
    int count;
} Source;

class Handler {
public:
    virtual void Apply(int, Item *);
};

typedef struct Pair {
    Handler *handler;
    Item *item;
} Pair;

typedef struct Obj {
    char pad0[0x158];
    int x158;
} Obj;

extern "C" void func_00211610(Item *, int, Item **, int *);


#define SCALE(dst, a, s) __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "f"(s) : "$2")
#define ADD(dst, a, b) __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b))
#define MADD(dst, a, b, s) __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b), "f"(s) : "$2")

static inline float Dot(float *a, float *b) {
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    return r;
}

extern "C" void func_00131198(Obj *obj, Source *src, int count, float *out) {
    Item *list[32];
    Vector dir;
    int n;
    int zeros;
    int neg;
    int i;
    int j;
    int k;
    int m;
    int a;
    int b;
    int npairs;
    Item *item;
    Handler *handler;
    float d;
    float x, y, z;
    float len;
    float inv;
    float scale;
    float t;

    n = 0;
    for (k = 0; k < count; k++) {
        func_00211610(src[k].items, src[k].count, list, &n);
    }
    zeros = 0;
    neg = 0;
    for (m = 0; m < n; m++) {
        d = list[m]->x1C;
        if (d < 0.0f) {
            neg++;
        } else if (d == 0.0f) {
            zeros++;
        } else {
            break;
        }
    }
    {
        Pair pairs[neg];

        npairs = 0;
        for (i = 0; i < neg; i++) {
            item = list[i];
            handler = (Handler *)func_0017D3D0(item->x4);
            if (handler) {
                for (j = 0; j < npairs; j++) {
                    if (pairs[j].handler == handler) {
                        if (pairs[j].item->x8 < item->x8) {
                            pairs[j].item = item;
                        }
                        break;
                    }
                }
                if (j == npairs) {
                    pairs[npairs].handler = handler;
                    pairs[npairs].item = item;
                    npairs++;
                }
            }
        }
        for (k = 0; k < npairs; k++) {
            pairs[k].handler->Apply(obj->x158, pairs[k].item);
        }
    }
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    for (a = 0; a < neg; a++) {
        SCALE(dir, list[a]->x20, list[a]->x8);
        for (b = 0; b < neg + zeros; b++) {
            if (a == b) {
                continue;
            }
            d = Dot(dir, list[b]->x20);
            if (d >= 0.0f) {
                continue;
            }
            MADD(dir, dir, list[b]->x20, -d);
        }
        ADD(*(Vector *)out, out, dir);
    }
    scale = 0.0f;
    x = out[0];
    y = out[1];
    z = out[2];
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    out[0] = x * inv;
    out[1] = y * inv;
    out[2] = z * inv;
    for (m = 0; m < neg; m++) {
        t = list[m]->x8 / Dot(out, list[m]->x20);
        __asm__ volatile ("max.s %0, %1, %2" : "=&f"(d) : "f"(scale), "f"(t));
        scale = d;
    }
    SCALE(*(Vector *)out, out, scale);
}
