/* UNIT 0x0023E288 0x0023E7C8 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: per-group table setup (0x23E3FC-0x23E488) only: D_0027F6C0/D0/E0/700 address registers permuted and two loads/adds reordered; best diff 20/335 words */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vec {
    u128 q;
    float f[4];
    int i[4];
} Vec __attribute__((aligned(16)));

/* Particle spray emitter (same object as ParticleSprayEmitter in particle_spray.h). */
typedef struct Obj {
    char pad0[0x2610];
    char x2610[0x10];          /* bounds min */
    char x2620[0x100];         /* bounds max */
    int x2720;
    char pad2724[0x48];
    unsigned short x276C;
    unsigned short x276E;
    int x2770;
    float x2774;
    float x2778;
    int x277C;
    char pad2780[0x5A];
    short x27DA;
    char pad27DC[0x10];
    float x27EC;
    char pad27F0[0x18];
    float x2808;
    char pad280C[0x0C];
    unsigned int x2818;
    int x281C;
    char pad2820[0x10];
    Vec *x2830;                /* positions (2 per particle) / per-particle flag bytes */
    Vec *x2834;                /* velocities */
    short *x2838;              /* remaining life */
    int *x283C;                /* RGBA colours */
    char pad2840[0x10];
    int x2850;
    int x2854;
    char pad2858[2];
    short x285A;
    signed char x285C;         /* particle count per group (3 groups) */
    signed char x285D;
    signed char x285E;
} Obj;

extern int D_002694D0[];
extern int D_0027F6C0[]; /* per-group offsets: positions */
extern int D_0027F6D0[]; /* velocities */
extern int D_0027F6E0[]; /* colours */
extern int D_0027F6F0[]; /* flag bytes */
extern int D_0027F700[]; /* lives */
extern float D_0029DCC0; /* gravity, -0.017f */

extern int func_0023E188(int life, int *colors, float start, float step);
extern void func_00223628(void *boundsMin, void *boundsMax);

#define VMADD_POS(dst, src, scale) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyzw ACC, $vf11, $vf0x\n\tvmaddx.xyzw $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0" \
                      : "=m"(*(dst)) : "r"(dst), "r"(src), "f"(scale) : "$2")

#define BBOX_INIT(p) \
    __asm__ volatile ("lqc2 $vf30, 0x0(%0)\n\tlqc2 $vf31, 0x0(%0)" : : "r"(p))

#define BBOX_GROW(p) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tvmini.xyzw $vf30, $vf30, $vf11\n\tvmax.xyzw $vf31, $vf31, $vf11" : : "r"(p))

#define VSCALE_INPLACE(p, scale) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" \
                      : "=m"(*(p)) : "r"(p), "f"(scale) : "$2")

#define VADD_INPLACE(p, add) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" \
                      : "=m"(*(p)) : "r"(p), "r"(add))

#define FMIN(dst, a, b) \
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(dst) : "f"(a), "f"(b))

static inline int mode_is_one(short mode)
{
    return (mode ^ 1) == 0;
}

/* Ages and moves the emitter's particles by dt, drops dead ones (compacting the arrays)
   and grows the bounding box. The do { } while (0) blocks only steer scheduling. */
void func_0023E288(Obj *obj, Vec *delta, int dt)
{
    Vec deltaCopy;
    int dtLocal;
    int useScale;
    int useDelta;
    int useZ;
    int firstBounds;
    int initialLife;
    int * colors;
    int lastColor;
    int nextGroup;
    void * p2610;
    void * p2620;
    int colorMode;
    unsigned int colorCount;
    float dtf;
    float baseScale;
    float colorStart;
    float colorStep;
    float lifeScale;
    int group;
    int removed;
    signed char *counts;

    initialLife = obj->x27DA;
    deltaCopy = *delta;
    delta = &deltaCopy;
    dtLocal = dt;
    firstBounds = 1;
    useScale = (obj->x2770 >> 4) & 1;
    useDelta = (obj->x2770 >> 10) & 1;
    useZ = (obj->x2770 >> 8) & 1;
    colorMode = obj->x276C == 0x37;

    colorCount = obj->x2818;
    colorStart = (float)colorCount - 1.0f;
    colors = &D_002694D0[obj->x281C];
    colorStep = -colorStart / (float)obj->x27DA;
    baseScale = obj->x2774;
    lifeScale = (obj->x2778 - baseScale) / (float)obj->x277C;
    lastColor = colors[obj->x2818 - 1];
    dtf = (float)dtLocal;
    p2610 = obj->x2610;
    p2620 = obj->x2620;

    group = 0;
    do {
        int count;
        int i;
        char *active;
        Vec *pos;
        Vec *vel;
        int *color;
        short *life;
        int posBack;
        int velBack;
        int colorBack;
        int lifeBack;
        int posOffset;
        float alphaMax;

        {
            int activeOffset = D_0027F6F0[group];
            int velOffset = D_0027F6D0[group];
            int colorOffset = D_0027F6E0[group];
            int *posTable = &D_0027F6C0[group];
            int lifeOffset = D_0027F700[group];
            Vec *base = obj->x2830;
            posOffset = *posTable;
            counts = &obj->x285C;
            count = counts[group];
            posOffset <<= 4;
            posOffset += 0x10;
            active = (char *)base + activeOffset;
            pos = (Vec *)(posOffset + (int)base);
            vel = obj->x2834 + velOffset;
            color = obj->x283C + colorOffset;
            life = obj->x2838 + lifeOffset;
        }
        *active = 0;
        if (count >= obj->x2854 && mode_is_one(obj->x285A)) {
            active[0x20] = 0;
        }

        i = 0;
        removed = 0;
        nextGroup = group + 1;
        if (count > 0) {
        posBack = 0;
        lifeBack = 0;
        colorBack = 0;
        velBack = 0;
        alphaMax = 255.0f;

        do {
            unsigned char nextActive;
            unsigned char curActive;
            if (i + 1 < count) {
                nextActive = (unsigned char)active[0x20];
            } else {
                nextActive = 4;
            }

            *life = (short)(*life - dtLocal);
            if (*life <= 0) {
                curActive = 0;
                if (colorMode) {
                    *color = lastColor;
                }
                ((unsigned char *)color)[3] = (unsigned char)(int)obj->x27EC;
            } else {
                float alpha;
                float capped;
                float remaining;
                float alphaStep;
                float alphaBase;
                curActive = (unsigned char)*active;
                if (colorMode) {
                    *color = func_0023E188(*life, colors, colorStart, colorStep);
                }
                remaining = (float)*life;
                alphaStep = obj->x2808;
                alphaBase = obj->x27EC;
                alpha = alphaBase + remaining * alphaStep;
                FMIN(capped, alphaMax, alpha);
                ((unsigned char *)color)[3] = (unsigned char)(int)capped;
            }

            if (curActive == 0 && nextActive == 0) {
                do {
                int flags = obj->x2720;
                int flagSet;
                lifeBack -= 2;
                colorBack -= 4;
                flagSet = flags & 0x20000000;
                velBack -= 0x10;
                do { removed++; } while (0);
                posBack -= 0x20;
                if (flagSet) {
                    obj->x2720 = (flags & 0xDFFFFFFF) | 0x01000000;
                }
                } while (0);
            } else {
                float scale;
                VMADD_POS(pos, vel, dtf);
                if (firstBounds) {
                    BBOX_INIT(pos);
                    firstBounds = 0;
                } else {
                    BBOX_GROW(pos);
                }
                *active = (char)curActive;

                if (useScale) {
                    int age;
                    age = initialLife - *life;
                    scale = obj->x2778;
                    if (age < obj->x277C) {
                        scale = baseScale + (float)age * lifeScale;
                    }
                    VSCALE_INPLACE(vel, scale);
                    if (useDelta) {
                        VADD_INPLACE(pos, delta);
                    }
                }
                if (useZ) {
                    vel->f[2] += D_0029DCC0 * dtf;
                }

                do { if (removed != 0) {
                    *(Vec *)(posBack + (int)pos - 0x10) = *(Vec *)((char *)pos - 0x10);
                    *(Vec *)(posBack + (int)pos) = *pos;
                    *(Vec *)(velBack + (int)vel) = *vel;
                    *(int *)(colorBack + (int)color) = *color;
                    *(short *)(lifeBack + (int)life) = *life;
                } } while (0);
            }

            pos += 2;
            vel++;
            color++;
            life++;
            active += 0x20;
            i++;
        } while (i < count);
        }

        counts = &obj->x285C;
        counts[group] = (signed char)(count - removed);
        group = nextGroup;
    } while (group < 3);

    {
        int a;
        int b;
        int c;
        int m;
        a = obj->x285D;
        b = obj->x285E;
        c = obj->x285C;
        m = b < a ? a : b;
        if (m >= c) {
            c = m;
        }
        obj->x2850 = c;
        obj->x276E = (unsigned short)(obj->x276E + dtLocal);
        func_00223628(p2610, p2620);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0023E288);
#endif
