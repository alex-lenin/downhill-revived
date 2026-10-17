/* UNIT 0x0020C0A0 0x0020C530 */
#include "particle_emission.h"
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Mtx { Vec r[4]; } Mtx __attribute__((aligned(16)));

typedef struct Node {
    int used;
    int prev;
    int next;
} Node;

typedef struct Point {
    float x, y, z;
    int flag;
} Point __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x2600];
    float x2600, x2604, x2608, x260C;
    char pad2610[0x100];
    int x2710;
    char pad2714[0xC];
    int x2720;
    short pad2724;
    short x2726;
    char pad2728[0x24];
    float x274C;
    char pad2750[0xC];
    unsigned int x275C;
    char pad2760[0x10];
    int x2770;
    char pad2774[0x4C];
    int x27C0;
    int x27C4;
    float x27C8;
    char pad27CC[8];
    int x27D4;
    char pad27D8[0x18];
    float x27F0, x27F4;
    char pad27F8[0x18];
    int x2810, x2814, x2818;
    int *x281C;
    float x2820;
    int x2824;
    char pad2828[4];
    int x282C;
    Point *x2830;
    char pad2834[0x10];
    int x2844;
    char pad2848[4];
    int x284C;
    int x2850;
    int x2854;
    char pad2858[8];
} Obj;

extern int D_0029E30C;
extern int D_0029E308;
extern Node D_0071F980[];
extern Obj D_0067E180[];
extern void func_002226C8(Mtx *, Vec *);
extern void func_00222C38(Vec *, float);
extern void func_002269E8(Obj *);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* levels above max saturate at 6 */
static inline int ClampLevel(int v, int max)
{
    return (v > max) ? 6 : v;
}

static inline int ClampMin(int v, int min)
{
    return (v > min) ? v : min;
}

static inline int Log2(unsigned int v)
{
    int r = -1;
    while (v) {
        v >>= 1;
        r++;
    }
    return r;
}

static inline float Normalize(Vec *v)
{
    float length, inv, x, y, z;
    x = v->x;
    y = v->y;
    z = v->z;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
    return length;
}

#if defined(__mips__)
int func_0020C0A0(Vec *from, Vec *to, int color0, int color1, int count, unsigned int alpha, int kind,
                  float radius, float scale, float spread, float speed)
#else
int SharedCall0020C0A0(struct Vector4 * from_view, struct Vector4 * to_view, int color0, int color1, int count, unsigned int alpha, int kind, float radius, float scale, float spread, float speed)
#endif
{
#if !defined(__mips__)
    Vec * from = (Vec *)from_view;
    Vec * to = (Vec *)to_view;
#endif

    int idx;
    Obj *obj;
    Vec dir;
    Vec rnd;
    Mtx mtx;
    int colors[2];
    float length, ratio;
    int level;
    int i;
    Point *out;
    int *flag;

    idx = D_0029E30C;
    if (idx != -1) {
        D_0029E30C = D_0071F980[idx].next;
        if (D_0071F980[idx].next != -1) {
            D_0071F980[D_0071F980[idx].next].prev = -1;
        }
        if (D_0029E308 != -1) {
            D_0071F980[D_0029E308].prev = idx;
        }
        D_0071F980[idx].next = D_0029E308;
        D_0071F980[idx].prev = -1;
        D_0071F980[idx].used = 1;
        D_0029E308 = idx;
    } else {
        return -1;
    }
    obj = &D_0067E180[idx];
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dir) : "r"(to), "r"(from));
    length = Normalize(&dir);
    count *= 2;
    func_002226C8(&mtx, &dir);

    ratio = speed * (length - radius) / scale;
    level = (int)(ratio * 1.414f);
    level = Log2(ClampMin(level, 0));
    level = ClampLevel(level, 5);
    obj->x2810 = 0x8005;
    obj->x2814 = 6;
    obj->x2824 = 0x48;
    obj->x281C = colors;
    obj->x2820 = 5000.0f;
    obj->x2818 = 4;
    obj->x2770 = 0x1000;
    obj->x2720 = 0x100004;
    obj->x274C = speed;
    obj->x2726 = kind;
    obj->x27C8 = scale;
    obj->x282C = 0;
    obj->x275C = alpha;
    obj->x27C0 = 0;
    obj->x27F0 = (float)(alpha & 0xFF) * (1.0f / 256.0f);
    obj->x27C4 = -50;
    obj->x27F4 = ratio * 0.25f;
    colors[0] = color0;
    colors[1] = color1;
    obj->x27D4 = 0;
    obj->x2844 = 0;
    obj->x2710 = 0;
    obj->x284C = 0;
    if (spread < 0.067f) {
        obj->x2600 = (from->x + to->x) * 0.5f;
        obj->x2604 = (from->y + to->y) * 0.5f;
        obj->x2608 = (from->z + to->z) * 0.5f;
        obj->x260C = length * 0.5f;
    } else {
        obj->x2600 = from->x;
        obj->x2604 = from->y;
        obj->x2608 = from->z;
        obj->x260C = length;
    }
    func_002269E8(obj);
    obj->x2850 = count + 1;
    if (obj->x2854 < obj->x2850) {
        obj->x2850 = obj->x2854;
    }
    count = obj->x2850 >> 1;
    out = obj->x2830;
    for (i = 0; i < count; i++) {
        {
            func_00222C38(&rnd, spread);
            __asm__ volatile ("lqc2 $vf11,0x0(%1)\n\tlqc2 $vf12,0x0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(dir) : "r"(&rnd), "r"(&mtx));
            if (radius > 0.0f) {
                __asm__ volatile ("lqc2 $vf11,%1\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(out[0]) : "m"(dir), "f"(radius) : "$2");
                __asm__ volatile ("lqc2 $vf11,%1\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out[0]) : "m"(out[0]), "r"(from));
            } else {
                COPY_VECTOR(out[0], from);
            }
            flag = &out->flag;
            *flag = 2;
            out++;
            __asm__ volatile ("lqc2 $vf11,%1\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(out[0]) : "m"(dir), "f"(length) : "$2");
            __asm__ volatile ("lqc2 $vf11,%1\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out[0]) : "m"(out[0]), "r"(from));
            flag[4] = level ? 5 << (level - 1) : 0;
            out++;
        }
    }
    out->flag = 0;
    return idx;
}
