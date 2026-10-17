/* UNIT 0x001DF920 0x001E1BE8 .sdata=0x29D690:0x29D6F0 */
#include "ui_region.h"
#if defined(__mips__)
#define CallShared0020ED30_0 func_0020ED30
#else
#define CallShared0020ED30_0 Forward0020ED30_2
#endif
#include "particle_emission.h"
#if defined(__mips__)
#define CallShared0020C0A0_0 func_0020C0A0
#else
#define CallShared0020C0A0_0 Forward0020C0A0_4
#endif
#if defined(__mips__)
#define CallShared0020C0A0_1 func_0020C0A0
#else
#define CallShared0020C0A0_1 Forward0020C0A0_4
#endif
#include "particle.h"
/* Retains retail call scheduling; guarded PC permutations remain where needed. */
#include "timer.h"
#include "include_asm.h"
typedef unsigned int u128 __attribute__((mode(TI)));

float D_0029D690=-1600.0f;
int D_0029D694=60;
int D_0029D698=64;
int D_0029D69C=60;
float D_0029D6A0=200.0f;
float D_0029D6A4=180.0f;
float D_0029D6A8=2000.0f;
float D_0029D6AC=1800.0f;
float D_0029D6B0=1.0f;
int D_0029D6B4=0,D_0029D6B8=0,D_0029D6BC=0,D_0029D6C0=0,D_0029D6C4=0;
int D_0029D6C8=0,D_0029D6CC=0;

unsigned int D_0029D6D0 = 0x061CC052;
unsigned int D_0029D6D4 = 0;
unsigned int D_0029D6D8 = 0;
unsigned int D_0029D6DC = 0;
unsigned int D_0029D6E0 = 0;
unsigned int D_0029D6E4 = 4;
u128 *D_0029D6E8 = 0;
struct Orb *D_0029D6EC = 0;

typedef struct State {
    float x0[4][4];
    float x40[4][4];
    union {
        u128 q;
        float f[4];
    } x80;
    union {
        u128 q;
        float f[4];
    } x90;
    float xA0[4];
    float xB0[4];
    int xC0[4];
    int xD0[4];
    int xE0[4];
    int xF0;
    int xF4;
    int xF8;
    char xFC;
    float x100;
    char x104;
    float x108;
} State;

extern State D_004C26C0;
extern void *func_0022DD88(int);

extern u128 D_00271600;
extern int func_0023D608(void);

void func_001DF920(int arg0)
{
    int n;

    n = func_0023D608();
    while (n != 0) {
        n--;
        D_004C26C0.xA0[n] = 1000000.0f;
        D_004C26C0.xB0[n] = 1000000.0f;
        D_004C26C0.xD0[n] = 0;
        D_004C26C0.xE0[n] = arg0;
    }
    D_004C26C0.x90.q = D_00271600;
    D_004C26C0.xF0 = 5;
    D_0029D6D0 = 0x061CC052;
    D_0029D6E4 = 4;
    D_004C26C0.xF4 = -1;
    D_004C26C0.xFC = 0;
    D_004C26C0.x100 = 0;
    D_004C26C0.x104 = 0;
    D_004C26C0.x108 = 0;
    D_0029D6D4 = 0;
    D_0029D6D8 = 0;
    D_0029D6E0 = 0;
    D_004C26C0.xF8 = -1;
}


extern float *func_0023CF58(int);
extern float *func_0023CF78(int);

static inline void SubVector(float *out, float *a, float *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(a), "r"(b) : "memory");
}

static inline void ApplyMatrix(float *out, float *m, float *in)
{
    __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)"
                      : : "r"(out), "r"(m), "r"(in) : "memory");
}

static inline float Length(float *v)
{
    float len;

    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(len) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
    return len;
}

void func_001DF9E8(int i)
{
    float diff[4] __attribute__((aligned(16)));

    SubVector(diff, D_004C26C0.x80.f, func_0023CF78(i));
    ApplyMatrix(D_004C26C0.x0[i], func_0023CF58(i), diff);
    D_004C26C0.xA0[i] = Length(D_004C26C0.x0[i]);
    SubVector(diff, D_004C26C0.x90.f, func_0023CF78(i));
    ApplyMatrix(D_004C26C0.x40[i], func_0023CF58(i), diff);
    D_004C26C0.xB0[i] = Length(D_004C26C0.x40[i]);
}

typedef struct Effect {
    char pad0[0x2726];
    short x2726;
} Effect;

typedef struct Rider1DFB08 {
    char pad0[0x3FD0];
    char x3FD0[0x792C - 0x3FD0];
    char *x792C;
} Rider1DFB08;

extern void func_00155E78(void *p, int n);

void func_001DFB08(Rider1DFB08 *rider)
{
    char *matrix = rider->x792C;
    int handle1;
    int handle2;
    Effect *fx1;
    Effect *fx2;

    func_0022CFB8((Vector4 *)(matrix + 0x10), matrix + 0x40, 4, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
    handle1 = SpawnParticleScaleBeforeDirection((Vector4 *)(matrix + 0x10), matrix + 0x40, 0x41, 1.0f, (Vector4 *)(0), (Matrix4 *)(0), (int)(0), (int)(0));
    handle2 = func_0022CFB8((Vector4 *)(matrix + 0x10), matrix + 0x40, 0x42, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
    fx1 = func_0022DD88(handle1);
    fx2 = func_0022DD88(handle2);
    if (fx1 != 0) {
        fx1->x2726 = 20;
    }
    if (fx2 != 0) {
        fx2->x2726 = 20;
    }
    func_00155E78(rider->x3FD0, 10);
}

typedef struct GameDFC08 {
    char pad0[0x50130];
    Rider1DFB08 *players[4];
    Rider1DFB08 *riders[26];
    int count;
} GameDFC08;

typedef struct Slot {
    char pad0[0x18];
    int x18;
    char pad1C[4];
} Slot;

typedef struct SlotList {
    char pad0[0x910];
    Slot slots[25];
} SlotList;

typedef struct World {
    char pad0[0x240];
    SlotList x240;
} World;

#define VEC_SUB(out, a, b) \
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" \
                      : "=m"(out) : "r"(a), "r"(b))

extern GameDFC08 D_002AF2C0;
extern World D_00398C40;
extern int D_004B2828[];
extern u128 D_004C22C0[];
extern float func_00227D80(int);
extern void func_001D3F20(Slot *, float *);
extern void func_001D3ED8(Slot *, float *, float);



void func_001DFC08(void)
{
    int limits[3];
    float diff[4] __attribute__((aligned(16)));
    SlotList *list;
    Slot *slot;
    Slot *slots;
    World *world;
    Rider1DFB08 *rider;
    Rider1DFB08 *current;
    int numPlayers;
    int cpuCount;
    int value;
    int choice;
    int i;
    int n;
    int off;
    int j;

    numPlayers = D_004B2828[0];
    choice = 0;
    cpuCount = D_002AF2C0.count - numPlayers;
    limits[0] = 100;
    limits[1] = 1024 - cpuCount * 20;
    limits[2] = 1024;
    D_0029D6D0 = D_0029D6D0 * 0x41C64E6D + 0x3039;
    value = D_0029D6D0 & 0x3FF;
    while (choice < 3 && value >= limits[choice])
        choice++;

    switch (choice) {
    default:
    case 0:
        *(u128 *)D_004C26C0.x80.f = D_00271600;
        D_004C26C0.x80.f[2] += 600.0f;
        break;
    case 1:
    pick_slot:
        if (D_0029D6BC == 0)
            break;
        D_0029D6D0 = D_0029D6D0 * 0x41C64E6D + 0x3039;
        *(u128 *)D_004C26C0.x80.f = D_004C22C0[D_0029D6D0 % D_0029D6BC];
        SpawnParticleScaleBeforeMatrix((Vector4 *)D_004C26C0.x80.f, 4, 1.0f, 0, 0, 0);
        SpawnParticleScaleBeforeMatrix((Vector4 *)D_004C26C0.x80.f, 5, 1.0f, 0, 0, 0);
        world = &D_00398C40;
        list = &world->x240;
        slots = list->slots;
        for (i = 0; i < 25; i++, slots++) {
            off = i * 32;
            if (slots->x18 == 0)
                goto use_slot;
        }
        break;
    case 2:
        rider = 0;
        for (j = 0; j < cpuCount; j++) {
            current = D_002AF2C0.riders[j];
            VEC_SUB(*(u128 *)diff, current->x792C + 0x10, D_004C26C0.x90.f);
            if (Length(diff) < 1700.0f)
                rider = current;
        }
        if (rider == 0) {
            choice = 1;
            goto pick_slot;
        }
        func_001DFB08(rider);
        break;
    use_slot:
        off += 0x910;
        slot = (Slot *)(off + (int)list);
        func_001D3F20(slot, D_004C26C0.x80.f);
        func_001D3ED8(slot, D_004C26C0.x80.f, func_00227D80(5));
        break;
    case 3:
        do {
        rider = 0;
        for (j = 0; j < numPlayers; j++) {
            current = D_002AF2C0.players[j];
            VEC_SUB(*(u128 *)diff, current->x792C + 0x10, D_004C26C0.x90.f);
            if (Length(diff) < 1700.0f)
                rider = current;
        }
        if (rider == 0) {
            choice = 1;
            goto pick_slot;
        }
        func_001DFB08(rider);
        break;
        } while (0);
        break;
    }
    D_004C26C0.xF0 = choice;
    n = func_0023D608();
    while (n != 0) {
        n--;
        func_001DF9E8(n);
    }
}

extern u128 D_00271630[],D_004C27D0[];
extern char D_00759780[];
extern int func_001E1530(void);
extern void func_001DF920(int);
extern void func_00238BC8(int);
extern void func_0020EC78(float*,float*,float*,float*,int*,int*,int*);
extern u128 D_004C24C0[];

void func_001DFF98(u128 *value, int which)
{
    if (which) {
        D_004C22C0[D_0029D6C0++] = *value;
    } else {
        D_004C24C0[D_0029D6B8++] = *value;
    }
}
typedef struct Block {
    u128 q[33];
} Block;
extern Block D_004C27E0;
extern Block D_004C29F0[];
extern char D_00271640[];
extern unsigned char D_002694D0[];

typedef struct Light {
    char pad0[0x260C];
    float x260C;
    char pad2610[0x2630 - 0x2610];
    u128 x2630;
    char pad2640[0x272C - 0x2640];
    float x272C;
    char pad2730[0x27DA - 0x2730];
    short x27DA;
    char pad27DC[0x2804 - 0x27DC];
    float x2804;
    char pad2808[0x2818 - 0x2808];
    int x2818;
    char pad281C[0x2834 - 0x281C];
    int *x2834;
    char pad2838[0x283C - 0x2838];
    unsigned int x283C;
    char pad2840[0x2854 - 0x2840];
    int x2854;
} Light;

/* this file's callers pass the scale before the pointer argument */


#define FIRST ((Light *)D_0029D6C4)
#define SECOND ((Light *)D_0029D6C8)

void func_001DFFF0(void)
{
    int handle;
    int n;
    int i;
    unsigned int address;
    u128 *src;
    u128 *copy;
    u128 *dst;
    u128 value;
    u128 last;
    u128 end;
    unsigned char *colors;
    float *out;
    Light *light;

    D_0029D6B4 = D_0029D6B8;
    D_0029D6BC = D_0029D6C0;
    D_0029D6B8 = 0;
    D_0029D6C0 = 0;
    func_0020EC78(&D_0029D6A4, &D_0029D6A8, &D_0029D6A0, &D_0029D6AC, &D_0029D694, &D_0029D698, &D_0029D69C);
    D_0029D6A4 = 200.0f;
    D_0029D6A8 = 2000.0f;
    D_0029D6AC = 8000.0f;
    D_0029D694 = 80;
    D_0029D698 = 80;
    D_0029D69C = 80;
    D_0029D6A0 = 180.0f;
    D_0029D6B0 = 3.0f;
    D_0029D6D4 = 0;
    D_0029D6D8 = 0;
    D_0029D6EC = 0;
    D_0029D6C8 = 0;
    func_001DF920(1);
    D_0029D6C4 = 0;
    handle = (int)SpawnParticleScaleBeforeMatrix((Vector4 *)&D_00271600, 3, 1.0f, 0, 0, 0);
    if (handle != -1) {
        D_0029D6C4 = (int)func_0022DD88(handle);
        FIRST->x2804 = (float)-FIRST->x27DA / D_0029D690;
        FIRST->x260C = ((float *)&D_00271600)[3];
        D_0029D6C8 = (int)func_0022DD88((int)SpawnParticleScaleBeforeMatrix((Vector4 *)D_00271640, 20, 1.0f, 0, 0, 0));
        SECOND->x260C = ((float *)D_00271640)[3];
        SECOND->x2854 = SECOND->x272C;
        __asm__ volatile ("sqc2 $vf0,%0" : "=m"(SECOND->x2630));
        address = FIRST->x283C;
        D_0029D6E8 = (u128 *)(address & ~15);
        copy = D_0029D6E8;
        dst = D_004C27E0.q;
        i = 32;
        do {
            *dst++ = *copy++;
            i--;
        } while (i >= 0);
        src = D_0029D6E8;
        value = src[0];
        D_004C29F0[3].q[0] = value;
        D_004C29F0[2].q[0] = value;
        D_004C29F0[1].q[0] = value;
        D_004C29F0[0].q[0] = value;
        last = src[31];
        D_004C29F0[3].q[31] = last;
        D_004C29F0[2].q[31] = last;
        D_004C29F0[1].q[31] = last;
        D_004C29F0[0].q[31] = last;
        end = src[32];
        D_004C29F0[3].q[32] = end;
        D_004C29F0[2].q[32] = end;
        D_004C29F0[1].q[32] = end;
        D_004C29F0[0].q[32] = end;
        for (i = 0; i < 4; i++) {
            n = FIRST->x2818;
            colors = D_002694D0 + i * 128;
            out = (float *)((char *)D_004C29F0[i].q + (address & 12));
            while (n != 0) {
                out[0] = colors[0];
                out[1] = colors[1];
                out[2] = colors[2];
                out[3] = colors[3];
                colors += 4;
                out += 4;
                n--;
            }
        }
        light = SECOND;
        i = 0;
        do {
            light->x2834[i * 4 + 3] = 0;
            i++;
        } while (i < 4);
    } else {
        D_0029D6C4 = 0;
    }
}
typedef struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

typedef struct Obj1E02D0 {
    char pad0[0x2630];
    Vec4 x2630;
    char pad2640[0x2780 - 0x2640];
    Vec4 x2780;
    char pad2790[0x27C8 - 0x2790];
    float x27C8;
} Obj1E02D0;

extern Vec4 D_0027A4A0;

void func_001E02D0(Obj1E02D0 *obj, Vec4 *step)
{
    /* step.xyz = x2780 + D_0027A4A0 * x2780.w */
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)
	"
                      "lqc2 $vf12, 0x0(%1)
	"
                      "mfc1 $2, %2
	"
                      "qmtc2.ni $2, $vf13
	"
                      "vaddax.xyz ACC, $vf11, $vf0x
	"
                      "vmaddx.xyz $vf11, $vf12, $vf13x
	"
                      "sqc2 $vf11, 0x0(%3)"
                      : : "r"(&obj->x2780), "r"(&D_0027A4A0), "f"(obj->x2780.w), "r"(step)
                      : "$2", "memory");
    /* x2630.xyz += step.xyz */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)
	"
                      "lqc2 $vf12, 0x0(%2)
	"
                      "vadd.xyz $vf11, $vf11, $vf12
	"
                      "sqc2 $vf11, %0"
                      : "=m"(obj->x2630) : "r"(&obj->x2630), "r"(step));
    /* wrap each coordinate into [0, x27C8) */
    if (obj->x2630.x < 0.0f) {
        obj->x2630.x += obj->x27C8;
    }
    if (obj->x2630.y < 0.0f) {
        obj->x2630.y += obj->x27C8;
    }
    if (obj->x2630.z < 0.0f) {
        obj->x2630.z += obj->x27C8;
    }
    if (obj->x2630.x >= obj->x27C8) {
        obj->x2630.x -= obj->x27C8;
    }
    if (obj->x2630.y >= obj->x27C8) {
        obj->x2630.y -= obj->x27C8;
    }
    if (obj->x2630.z >= obj->x27C8) {
        obj->x2630.z -= obj->x27C8;
    }
}

typedef struct Obj1E03E8 {
    char pad0[0x2788];
    float x2788;
} Obj1E03E8;

void func_001E03E8(Obj1E03E8 *obj, Vec4 *base, Vec4 *out, Vec4 *prev, Vec4 *cur)
{
    Vec4 v;
    Vec4 delta;
    Vec4 sum;
    float limit;
    float len;
    float scale;

    v = *base;
    __asm__ volatile ("lqc2 $vf11,0x0(%1)
	lqc2 $vf12,0x0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(delta) : "r"(cur), "r"(prev));
    __asm__ volatile ("lqc2 $vf11,0x0(%1)
	lqc2 $vf12,0x0(%2)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(sum) : "r"(&delta), "r"(&v));
    limit = -obj->x2788;
    __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0" : "=&f"(len) : "f"(sum.x), "f"(sum.y), "f"(sum.z));
    *out = v;
    out->w = 1.0f;
    if (limit < len) {
        __asm__ volatile ("sqrt.s %0,%1" : "=&f"(scale) : "f"(limit / len));
        out->w = scale;
    }
    *prev = *cur;
}
typedef struct Obj1E0480 {
    char pad0[0x2600];
    Vec4 x2600;
    char pad2610[0x20];
    Vec4 x2630;
    char pad2640[0x272C-0x2640];
    float x272C;
    char pad2730[0x27C0-0x2730];
    int x27C0;
    int x27C4;
    float x27C8;
    char pad27CC[0x2830-0x27CC];
    Vec4 *x2830;
    Vec4 *x2834;
} Obj1E0480;
extern unsigned int D_0029DBAC;
typedef struct State0480 { char pad[0xC0]; float distance[4]; float weight[4]; int id[4]; } State0480;
#define ST0480 (*(State0480 *)&D_004C26C0)
extern int func_001DF8C0(void *);

static inline float Wave(float pos, float frequency, float speed)
{
    float value = pos * frequency + (D_0029DBAC & 0xFFFFFF) * speed;
    value -= (int)value;
    if (value < 0.0f) value += 1.0f;
    return value + (value - 1.0f);
}

void func_001E0480(int h, int mode)
{
    Obj1E0480 *obj = (Obj1E0480 *)h;
    Vec4 step, offset, pos, delta, cell;
    int i;
    float invCount, distance, weight;
    float a, b, c, d, e, f;
    int count, ix, iy, iz;
    float *matrix, *rider;
    unsigned int phase;

    func_001E02D0((Obj1E02D0 *)obj, &step);
    i = func_0023D608();
    invCount = 1.0f / i;
    while (i != 0) {
        i--;
        matrix = func_0023CF58(i);
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
            : "=m"(offset) : "r"(matrix + 4), "f"(obj->x27C8 * 0.5f) : "$2");
        rider = func_0023CF78(i);
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
            : "=m"(pos) : "r"(&offset), "r"(rider));
        func_001E03E8((Obj1E03E8 *)obj, &step, &obj->x2834[i+4], &obj->x2830[i+4], &pos);
        distance = 0.0f;
        if (obj->x27C0 & 0x200000) {
            __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                : "=m"(delta) : "r"(&pos), "r"(&obj->x2600));
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                : "=&f"(distance) : "f"(delta.x), "f"(delta.y), "f"(delta.z));
            weight = (obj->x2600.w - distance) / obj->x2600.w;
            if (weight > 0.0f) {
                weight *= 2.0f - weight;
                weight *= weight;
            } else weight = 0.0f;
        } else {
            a = pos.x * (1.0f/23.0f) + (D_0029DBAC & 0xFFFFFF) * 0.001f;
            a -= (int)a;
            if (a < 0.0f) a += 1.0f;
            a = a + (a - 1.0f);
            b = pos.x * (1.0f/41.0f) + (D_0029DBAC & 0xFFFFFF) * 0.005f;
            b -= (int)b;
            if (b < 0.0f) b += 1.0f;
            b = b + (b - 1.0f);
            c = pos.x * (1.0f/61.0f) + (D_0029DBAC & 0xFFFFFF) * 0.008f;
            c -= (int)c;
            if (c < 0.0f) c += 1.0f;
            c = c + (c - 1.0f);
            d = pos.y * (1.0f/31.0f) + (D_0029DBAC & 0xFFFFFF) * 0.003f;
            d -= (int)d;
            if (d < 0.0f) d += 1.0f;
            d = d + (d - 1.0f);
            e = pos.y * (1.0f/53.0f) + (D_0029DBAC & 0xFFFFFF) * 0.007f;
            e -= (int)e;
            if (e < 0.0f) e += 1.0f;
            e = e + (e - 1.0f);
            f = pos.y * (1.0f/83.0f) + (D_0029DBAC & 0xFFFFFF) * 0.01f;
            f -= (int)f;
            if (f < 0.0f) f += 1.0f;
            f = f + (f - 1.0f);
            weight = (a-a*a*a+b-b*b*b+c-c*c*c+d-d*d*d+e-e*e*e+f-f*f*f)*0.2165f+0.5f;
        }
        switch (mode) {
        case 0:
            ST0480.distance[i] = distance;
            ST0480.weight[i] = weight;
            break;
        case 1:
            ST0480.distance[i] = distance;
            ST0480.weight[i] = weight;
            ST0480.id[i] = func_001DF8C0(obj);
            break;
        case 2:
            if (distance < ST0480.distance[i]) {
                ST0480.distance[i] = distance;
                ST0480.weight[i] = weight;
                ST0480.id[i] = func_001DF8C0(obj);
            }
            break;
        }
        count = obj->x272C * weight * invCount;
        if (count != 0) {
            float x, y, z, coordinate;
            x = obj->x2630.x + obj->x27C8*0.5f;
            y = obj->x2630.y + obj->x27C8*0.5f;
            z = obj->x2630.z + obj->x27C8*0.5f;
            coordinate = (pos.x - x) / obj->x27C8;
            cell.x = coordinate;
            cell.y = (pos.y - y) / obj->x27C8;
            cell.z = (pos.z - z) / obj->x27C8;
            ix = cell.x < 0.0f;
            ix = (int)cell.x - ix;
            iy = cell.y < 0.0f;
            iy = (int)cell.y - iy;
            iz = cell.z < 0.0f;
            iz = (int)cell.z - iz;
            phase = D_0029DBAC & 0x3F;
            obj->x2830[i].x = ix * obj->x27C8 + obj->x2630.x;
            coordinate = iy;
            coordinate *= obj->x27C8;
            coordinate += obj->x2630.y;
            obj->x2830[i].y = coordinate;
            obj->x2830[i].z = iz * obj->x27C8 + obj->x2630.z;
            *(int *)&obj->x2830[i].w = count;
            obj->x2834[i].x = pos.x - obj->x2830[i].x - obj->x27C8*0.5f;
            obj->x2834[i].y = pos.y - obj->x2830[i].y - obj->x27C8*0.5f;
            obj->x2834[i].z = pos.z - obj->x2830[i].z - obj->x27C8*0.5f;
            obj->x2834[i].w = phase * (1.0f/64.0f);
        }
    }
}


extern void func_001E0480(int h, int n);

typedef struct Spawn {
    char pad0[0x2690];
    Vec4 x2690;
} Spawn;

typedef struct Orb {
    char pad0[0x27C0];
    int x27C0;
    char pad27C4[0x2820 - 0x27C4];
    float x2820;
    char pad2824[0x282C - 0x2824];
    unsigned int x282C;
    char pad2830[0x2848 - 0x2830];
    struct Orb **x2848;
} Orb;

extern int D_0029DE8C;
extern int D_004C27B0[];
extern float D_004C2740[];
extern Block D_004C27E0;
extern Block D_004C29F0[];
extern char D_00271610[];
extern char D_002716D0[];
extern char D_00271710[];
extern char D_00271750[];
extern char D_00271790[];
extern void func_0011C208(void *, Vec4 *, void *, void *, float *);
extern void func_001DFC08(void);
extern int func_0020C0A0(void *, float *, unsigned int, unsigned int, float, float, float, float, int, int, int);
extern void func_00226C10(Orb *, void *, int, float, float, float, float, float);

/* LCG step: D_0029D6D0 is the random seed */
static inline unsigned int rnd(void)
{
    D_0029D6D0 = D_0029D6D0 * 0x41C64E6D + 0x3039;
    return D_0029D6D0;
}

int func_001E0D20(void)
{
    Vec4 *pos;
    void *nearest3;
    void *nearest1;
    float dist;
    int k;
    u128 *src;
    u128 *dst;
    int i;
    int first;
    int handle;
    Orb *orb;
    void *model;

    if (D_0029D6C4 == 0) {
        return 1;
    }
    pos = &((Spawn *)D_0029D6C4)->x2690;
    func_0011C208(&D_002AF2C0, pos, &nearest3, &nearest1, &dist);
    if (dist >= 9000000.0f) {
        return 1;
    }
    if (D_0029D6D8 == 0) {
        D_0029D6E4 = 4;
        D_004C26C0.xF0 = 5;
        D_004C26C0.xFC = 1;
        D_0029DE8C = rnd();
        D_0029D6EC = 0;
        rnd();
        D_0029D6E0 = ((D_0029D6D0 >> 8) & 0xF) + 2;
        D_0029D6D4 = ((D_0029D6D0 >> 16) & 0x1F) + 3;
        D_0029D6D8 = D_0029D6D4 + 15 + D_0029D6E0 + ((D_0029D6D0 >> 24) & 0x1F);
        D_004C26C0.x100 = D_0029D6E0 / 60;
        D_004C26C0.x104 = 0;
        D_004C26C0.x108 = D_0029D6D4 / 60;
    } else {
        D_0029D6D8--;
        if (D_0029D6E0 != 0) {
            if (--D_0029D6E0 == 0) {
                func_001DFC08();
                D_004C26C0.x104 = 1;
            }
            D_004C26C0.xFC = 0;
            if (D_0029D6E0 != 0) {
                k = (rnd() >> 16) % 5;
            } else {
                k = 4;
            }
            if (k != D_0029D6E4) {
                if (k < 4) {
                    src = D_004C29F0[k].q;
                } else {
                    src = D_004C27E0.q;
                }
                dst = D_0029D6E8;
                for (i = 31; i >= 0; i--) {
                    *dst++ = *src++;
                }
            }
            D_0029D6E4 = k;
        } else if (D_0029D6D4 != 0) {
            if (--D_0029D6D4 == 0) {
                D_004C27B0[0] = 5;
            } else {
                D_004C26C0.x104 = 0;
                if (D_0029D6EC == 0) {
                    first = D_004C26C0.xF0 == 0;
                    handle = CallShared0020C0A0_0(first ? (void *)pos : (void *)D_00271610, D_004C2740, 0xFFFFFFFF, 0x20802080,
                                           first ? 100.0f : 200.0f, 15.0f, first ? 1.0f : 0.0f, 0.3f,
                                           first ? 3 : 1, D_0029DE8C, D_0029D6D4 + 1);
                    if (handle != -1) {
                        orb = func_0022DD88(handle);
                        D_0029D6EC = orb;
                        orb->x27C0 |= 0x200;
                        orb->x2848 = &D_0029D6EC;
                        orb->x2820 = pos->w;
                        D_0029D6DC = rnd() & 0xC;
                        model = 0;
                        switch ((D_0029D6D0 >> 8) & 3) {
                        case 0:
                            model = D_002716D0;
                            break;
                        case 1:
                            model = D_00271710;
                            break;
                        case 2:
                            model = D_00271750;
                            break;
                        case 3:
                            model = D_00271790;
                            break;
                        }
                        func_00226C10(D_0029D6EC, model, 0, 0.0f, 0.0f, 0.0f, 0.0f, D_0029D6DC);
                    }
                } else {
                    D_0029D6DC = (rnd() >> 16) & 0xC;
                    func_00226C10(D_0029D6EC, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, D_0029D6DC);
                    if (D_0029D6C8 != 0) {
                        ((Orb *)D_0029D6C8)->x282C = D_0029D6DC >> 2;
                    }
                }
            }
        } else if (D_0029D6C8 != 0) {
            ((Orb *)D_0029D6C8)->x282C = 0;
        }
    }
    func_001E0480(D_0029D6C8, 0);
    return 1;
}

typedef struct Obj1E1228 {
    char pad0[0x260C];
    float x260C;
    char pad2610[0x20];
    Vec4 x2630;
    char pad2640[0xEC];
    float x272C;
    char pad2730[0x104];
    Vec4 *x2834;
    char pad2838[0x1C];
    int x2854;
} Obj1E1228;

extern float D_00271650[];
extern float D_00271660[];
extern float D_00271680[];
extern float D_00271690[];
extern float D_002716A0[];
extern float D_002716B0[];
extern float D_002716C0[];

void func_001E1228(int mode)
{
    Obj1E1228 *obj;
    int h1;
    int h2;
    float f20;
    float f21;
    int i;

    D_0029D6C8 = 0;
    h1 = -1;
    h2 = -1;
    f21 = 0.0f;
    D_0029D6CC = 0;
    func_001DF920(0);
    f20 = f21;
    switch (mode) {
    case 19:
        h1 = (int)func_0022CDC0((Vector4 *)(D_002716C0), 0x34, (Matrix4 *)(0), (int)(0), 1.0f, (Vector4 *)(0));
        f20 = D_002716C0[3];
        break;
    case 5:
        f20 = 1.0f;
        h1 = (int)func_0022CDC0((Vector4 *)(D_00271680), 0x13, (Matrix4 *)(0), (int)(0), f20, (Vector4 *)(0));
        h2 = (int)func_0022CDC0((Vector4 *)(D_00271690), 0x13, (Matrix4 *)(0), (int)(0), f20, (Vector4 *)(0));
        f20 = D_00271680[3];
        f21 = D_00271690[3];
        obj = func_0022DD88(h2);
        D_0029D6CC = (int)obj;
        if (obj != 0) {
            obj->x272C *= 0.5f;
        }
        break;
    case 12:
        f20 = 1.0f;
        h1 = (int)func_0022CDC0((Vector4 *)(D_002716A0), 0x30, (Matrix4 *)(0), (int)(0), f20, (Vector4 *)(0));
        h2 = (int)func_0022CDC0((Vector4 *)(D_002716B0), 0x33, (Matrix4 *)(0), (int)(0), f20, (Vector4 *)(0));
        f20 = D_002716A0[3];
        f21 = D_002716B0[3];
        break;
    case 3:
        f20 = 1.0f;
        h1 = (int)func_0022CDC0((Vector4 *)(D_00271650), 0x16, (Matrix4 *)(0), (int)(0), f20, (Vector4 *)(0));
        h2 = (int)func_0022CDC0((Vector4 *)(D_00271660), 0x17, (Matrix4 *)(0), (int)(0), f20, (Vector4 *)(0));
        f20 = D_00271650[3];
        f21 = D_00271660[3];
        break;
    }
    if (h1 != -1) {
        obj = func_0022DD88(h1);
        D_0029D6C8 = (int)obj;
        obj->x260C = f20;
        obj->x2854 = obj->x272C;
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x2630));
        for (i = 0; i < 4; i++) {
            ((Obj1E1228 *)D_0029D6C8)->x2834[i].w = 0.0f;
        }
    }
    if (h2 != -1) {
        obj = func_0022DD88(h2);
        D_0029D6CC = (int)obj;
        obj->x260C = f21;
        obj->x2854 = obj->x272C;
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x2630));
        for (i = 0; i < 4; i++) {
            ((Obj1E1228 *)D_0029D6CC)->x2834[i].w = 0.0f;
        }
    }
}
extern void func_001E0480(int h, int n);

int func_001E14E8(void)
{
    if (D_0029D6C8 == 0) {
        return 0;
    }
    func_001E0480(D_0029D6C8, 1);
    if (D_0029D6CC != 0) {
        func_001E0480(D_0029D6CC, 2);
    }
    return 1;
}
extern int D_002717D0[][3];
extern void func_0020ED30(int, float, float, float, float, float, int, int, int);

int func_001E1530(void)
{
    Orb *orb;
    void *model;
    int handle;
    int i;
    int n;
    int s3;
    int s2;
    int s1;
    float f20;

    s3 = D_0029D694;
    s2 = D_0029D698;
    s1 = D_0029D69C;
    if (D_0029D6D8 == 0) {
        D_004C26C0.xF0 = 5;
        D_004C26C0.xFC = 1;
        D_0029DE8C = rnd();
        D_0029D6EC = 0;
        rnd();
        D_0029D6E0 = ((D_0029D6D0 >> 8) & 0xF) + 2;
        D_0029D6D4 = ((D_0029D6D0 >> 12) & 0xF) + 3;
        D_0029D6D8 = D_0029D6D4 + 2 + D_0029D6E0 + ((D_0029D6D0 >> 16) & 0x7F);
        D_004C26C0.x100 = D_0029D6E0 / 60;
        D_004C26C0.x104 = 0;
        D_004C26C0.x108 = D_0029D6D4 / 60;
    } else {
        D_0029D6D8--;
        if (D_0029D6E0 != 0) {
            if (--D_0029D6E0 == 0) {
                D_004C26C0.x80.q = D_004C27D0[0];
                D_004C26C0.x104 = 1;
                D_004C26C0.x80.f[2] -= D_0029D6C8 ? 2200.0f : 3000.0f;
                n = func_0023D608();
                while (n != 0) {
                    n--;
                    func_001DF9E8(n);
                }
            }
            D_004C26C0.xFC = 0;
            f20 = 0.0f;
            if (D_0029D6E0 != 0) {
                rnd();
                i = (D_0029D6D0 >> 8) & 0xFF;
                f20 = i * (1.0f / 255.0f);
                s3 = D_002717D0[i & 3][0];
                s2 = D_002717D0[i & 3][1];
                s1 = D_002717D0[i & 3][2];
            }
            n = func_0023D608();
            while (n != 0) {
                n--;
                CallShared0020ED30_0(n, f20, D_0029D6A4, D_0029D6A8, D_0029D6A0, D_0029D6AC, s3, s2, s1);
            }
        } else if (D_0029D6D4 != 0) {
            if (--D_0029D6D4 == 0) {
                D_004C27B0[0] = 5;
            } else {
                D_004C26C0.x104 = 0;
                if (D_0029D6EC == 0) {
                    handle = CallShared0020C0A0_1(D_004C27D0, D_004C26C0.x80.f, 0xFFFFFFFF, 0x20802080,
                                           300.0f, D_0029D6C8 ? 15.0f : 30.0f, 0.25f, D_0029D6C8 ? 0.4f : 0.3f,
                                           (D_0029DE8C & 3) + 1, D_0029DE8C, D_0029D6D4 + 1);
                    if (handle != -1) {
                        orb = func_0022DD88(handle);
                        D_0029D6EC = orb;
                        orb->x27C0 |= 0x200;
                        orb->x2848 = &D_0029D6EC;
                        orb->x2820 = ((float *)D_004C27D0)[3];
                        D_0029D6DC = rnd() & 0xC;
                        model = 0;
                        switch ((D_0029D6D0 >> 8) & 3) {
                        case 0:
                            model = D_002716D0;
                            break;
                        case 1:
                            model = D_00271710;
                            break;
                        case 2:
                            model = D_00271750;
                            break;
                        case 3:
                            model = D_00271790;
                            break;
                        }
                        func_00226C10(D_0029D6EC, model, 0, 0.0f, 0.0f, 0.0f, 0.0f, D_0029D6DC);
                    }
                } else {
                    D_0029D6DC = (rnd() >> 16) & 0xC;
                    func_00226C10(D_0029D6EC, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, D_0029D6DC);
                    if (D_0029D6C8 != 0) {
                        ((Orb *)D_0029D6C8)->x282C = D_0029D6DC >> 2;
                    }
                }
            }
        } else if (D_0029D6C8 != 0) {
            ((Orb *)D_0029D6C8)->x282C = 0;
        }
    }
    if (D_0029D6C8 != 0) {
        func_001E0480(D_0029D6C8, 0);
    }
    return 1;
}
typedef struct Obj1E1A90 {
    char pad0[0x260C];
    float x260C;
    char pad2610[0x20];
    Vec4 x2630;
    char pad2640[0xEC];
    float x272C;
    char pad2730[0x124];
    int x2854;
} Obj1E1A90;

extern float D_00271670[];
extern u128 D_00271620[];

void func_001E1A90(void)
{
    Obj1E1A90 *obj;

    func_001DF920(1);
    obj = func_0022DD88((int)func_0022CDC0((Vector4 *)(D_00271670), 0x2E, (Matrix4 *)(0), (int)(0), 1.0f, (Vector4 *)(0)));
    D_0029D6C8 = (int)obj;
    if (obj != 0) {
        obj->x260C = D_00271670[3];
        obj->x2854 = obj->x272C;
        D_004C27D0[0] = D_00271620[0];
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x2630));
        func_00238BC8(0);
        func_00236B98((TimerPool *)(D_00759780), (TimerCallbackNoArg)(func_001E1530), 1);
    }
    func_0020EC78(&D_0029D6A4, &D_0029D6A8, &D_0029D6A0, &D_0029D6AC, &D_0029D694, &D_0029D698, &D_0029D69C);
}
void func_001E1B68(void) {
 func_001DF920(1);
 D_0029D6C8=0;
 D_004C27D0[0]=D_00271630[0];
 func_00238BC8(0);
 func_00236B98((TimerPool *)(D_00759780), (TimerCallbackNoArg)(func_001E1530), 1);
 func_0020EC78(&D_0029D6A4,&D_0029D6A8,&D_0029D6A0,&D_0029D6AC,&D_0029D694,&D_0029D698,&D_0029D69C);
}
