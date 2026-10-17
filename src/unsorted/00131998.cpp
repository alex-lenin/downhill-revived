/* UNIT 0x00131998 0x001326F8 .sdata=0x29C2BC:0x29C2C0 */
#include "particle_emission.h"
#if defined(__mips__)
#define CallShared001B5DE8_0 func_001B5DE8
#else
#define CallShared001B5DE8_0 Forward001B5DE8_1
#endif
#include "spatial_batch.h"
#include "impulse.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef union Header { u128 q; unsigned long flags; float f[4]; } Header;
typedef struct Hit { Header h; Vec position; Vec normal; } Hit;
typedef struct Group {
    Vec *x0;
    Vec *x4;
    float x8, xC, x10;
    Vec *x14;
    Vec *x18;
    int x1C;
    Hit *x20;
    int x24;
} Group;
typedef struct Body {
    Vec x0, x10;
    char pad20[0x10];
    u128 x30, x40, x50;
    char pad60[0x14];
    unsigned char x74;
    unsigned char state() const { return x74; }
    char pad75[0xB];
} Body;
typedef struct State {
    unsigned long x0;
    char pad8[0x58];
    Vec x60;
} State;
typedef struct Ref { char pad0[0x281C]; void *x281C; } Ref;
typedef struct Parent {
    char pad0[0x4098];
    unsigned long x4098;
    char pad40A0[0xE0];
    Vec x4180;
    char pad4190[0x70];
    Vec x4200;
    char pad4210[0xCBC];
    void *x4ECC;
    char pad4ED0[0x2A00];
    void *x78D0;
    char pad78D4[0x58];
    State *x792C;
    char pad7930[0x114];
    Ref *x7A44;
    char pad7A48[0x58];
    Vec x7AA0;
    char pad7AB0[0x10];
    float x7AC0;
    char pad7AC4[0xEB];
    unsigned char x7BAF;
} Parent;
typedef struct Actor {
    char pad0[0x34];
    Vec *x34;
    Vec *x38;
    char pad3C[0x24];
    Vec x60;
    char pad70[0x50];
    Vec xC0, xD0;
    char padE0[0x50];
    Vec x130;
    char pad140[0x18];
    Parent *x158;
    char pad15C[0xA4];
    u128 x200, x210, x220;
    char pad230[0x20];
    Vec x250, x260;
    char pad270[0x70];
    Body x2E0, x360;
    char pad3E0[0x50];
    float x430;
    char pad434[0x58];
    float x48C;
    char pad490[0x14];
    short x4A4, x4A6;
    char pad4A8[8];
    unsigned long x4B0;
} Actor;
typedef struct Saved { Vec v[15]; } Saved;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define BIT57 ((unsigned long)1 << 57)

float D_0029C2BC = 2000.0f;
extern float D_0029C2B8, D_0029C300, D_0029C330;
extern int D_0029C304;
extern void func_00131548(Actor *);
extern int func_00238F30(void);
extern Body *func_00134F58(Actor *, int);
extern void func_00134D70(Body *, Vec *, Actor *);
extern int func_00138560(void *);
extern int func_00138570(void *);
extern void func_00133260(Actor *, Saved *);
extern void func_001332A8(Actor *, Saved *);
extern void func_00133518(Actor *, float);
extern void func_001336F8(Actor *);
extern Hit *func_002116E8(Group *, int, Hit *);
extern void func_00131198(Actor *, Group *, int, Vec *);
extern void func_00231A48(Actor *, Vec *);

extern void func_00155E78(void *, int);
extern void func_00130FE0(Actor *, Vec *, Vec *, int, float);
extern int func_001326F8(Actor *, Hit *, int, int, int, int);
extern void func_001CCFB8(void *, float);
extern void func_001554C0(void *, Vec *);
extern void func_001B0320(void *, Vec *);
extern void func_001AFC90(void *, Vec *, short);
extern void func_00199AD8(void *, float);
extern void func_001B5DE8(Vec *, Vec *, int, Vec *, float, void *, void *, Parent *);
extern void func_001C99B0(void *, int, float);
extern void func_001C9A00(void *, int, float);

static inline float Length(Vec *v)
{
    float r;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(r) : "f"(v->x), "f"(v->y), "f"(v->z));
    return r;
}

static inline float Min(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline void Scale(Vec *dst, Vec *src, float s)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(src), "f"(s) : "$2");
}

static inline void AddScaled(Vec *dst, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b), "f"(s) : "$2");
}

static inline int FindGroup(Group *groups, Hit *hit)
{
    int i;
    for (i = 0; ; i++) {
        Group *group = &groups[i];
        int k = hit - group->x20;
        if (k >= 0 && k < group->x1C)
            break;
    }
    return i;
}

/* EE vector operations and FPU multiply-accumulate/sqrt use inline assembly. */
void func_00131998(Actor *a)
{
    Group groups[4];
    Hit hits[4][10];
    Vec before[4];
    Vec after[4];
    Vec correction;
    Vec scales[3];
    Saved saved;
    Vec position;
    int first = 1;
    /* These must stay variables: their RTL sets (removed only after GCSE) size GCSE's
       hash table, whose bucket order decides the spill-slot layout. */
    int numWheels = 3;
    int maxPasses = 10;
    int numProbes = 3;
    float remaining = 1.0f;
    float inv;
    bool savedBit;
    short surface;

    func_00131548(a);
    inv = 1.0f / (float)func_00238F30();
    for (int i = 0; i < 3; i++) {
        groups[i].x0 = &before[i];
        groups[i].x4 = &after[i];
        groups[i].x20 = hits[i];
        groups[i].x14 = 0;
        groups[i].x18 = &scales[i];
        groups[i].x1C = 10;
        groups[i].x10 = func_00134F58(a, i)->x0.w;
    }
    groups[3].x8 = 1.0f;
    groups[3].xC = 1.0f;
    groups[3].x0 = &before[3];
    groups[3].x4 = &after[3];
    groups[3].x10 = 0.0f;
    groups[3].x14 = 0;
    groups[3].x18 = 0;
    groups[3].x20 = hits[3];
    groups[3].x1C = 10;
    COPY_VECTOR(before[3], a->x34);
    COPY_VECTOR(after[3], &before[3]);
    after[3].z -= 1000.0f;
    for (int i = 0; i < 3; i++) {
        func_00134D70(func_00134F58(a, i), &position, a);
        if (D_0029C304) {
            Body *body = func_00134F58(a, i);
            COPY_VECTOR(*groups[i].x18, &body->x10);
        } else {
            Vec *scale = groups[i].x18;
            scale->x = 1.0f;
            scale->y = 1.0f;
            scale->z = 1.0f;
        }
        if (D_0029C304)
            groups[i].x14 = a->x38;
    }
    COPY_VECTOR(a->x158->x4200, &a->xC0);
    a->x4B0 = (a->x4B0 & ~1L) | (func_00138560(a) & 1);
    a->x4A6 = a->x4A4;
    COPY_VECTOR(a->x130, &a->x60);
    surface = 0;
    savedBit = (int)(a->x158->x792C->x0 >> 57) & 1;
    a->x158->x792C->x0 &= ~BIT57;
    if (remaining > 0.0f) {
    int pass = 1;
    do {
        float step;
        float speed;
        Hit *hit;
        func_00133260(a, &saved);
        for (int i = 0; i < 3; i++) {
            groups[i].x8 = 1.0f - remaining;
            func_00134D70(func_00134F58(a, i), &before[i], a);
        }
        speed = Length(&a->xD0);
        step = Min(remaining, (float)func_00238F30() * 1.5707963705062866f / speed);
        func_00133518(a, step * inv);
        for (int i = 0; i < 3; i++) {
            groups[i].xC = groups[i].x8 + step;
            func_00134D70(func_00134F58(a, i), &after[i], a);
        }
        QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(groups), pass <= 1 ? 4 : 3);
        hit = func_002116E8(groups, numWheels, 0);
        if (hit) {
            func_001332A8(a, &saved);
            if (hit->position.w < 0.0f) {
                func_00131198(a, groups, numWheels, &correction);
                if (Length(&correction) > 1.0f) {
                    func_00155E78((char *)a->x158 + 0x3FD0, 2);
                    return;
                }
                func_00231A48(a, &correction);
            } else {
                int index;
                do {
                    float time = hit->position.w;
                    float advance;
                    index = FindGroup(groups, hit);
                    advance = step * time;
                    func_00133518(a, advance * inv);
                    func_00130FE0(a, before, after, index, time);
                    if (func_001326F8(a, hit, index, first, 1, 1)) {
                        remaining -= advance;
                        func_001336F8(a);
                        first = 0;
                        surface = (unsigned short)(hit->h.flags >> 53) & 0x3F;
                        COPY_VECTOR(position, &hit->position);
                        Hit *other = func_002116E8(groups, numWheels, hit);
                        while (other && other->position.w == time) {
                            func_001326F8(a, other, index, 0, 1, 1);
                            other = func_002116E8(groups, numWheels, other);
                        }
                        break;
                    }
                    func_001332A8(a, &saved);
                    hit = func_002116E8(groups, numWheels, hit);
                } while (hit);
                if (!hit) {
                    remaining -= step;
                    func_00133518(a, step * inv);
                }
            }
        } else {
            remaining -= step;
        }
        if (remaining < 0.01f)
            remaining = 0.0f;
    } while (remaining > 0.0f && ++pass < maxPasses);
    }
    if (remaining == 1.0f) {
        if (a->x158->x4ECC != (char *)a->x158 + 0x62B4) {
            func_00155E78((char *)a->x158 + 0x3FD0, 1);
            return;
        }
    }
    a->x158->x792C->x0 = (a->x158->x792C->x0 & ~BIT57) | ((unsigned long)savedBit << 57);
    if (a->x430 != 0.0f) {
        if (a->x158->x7BAF) {
            if (Length(&a->x250) > D_0029C2BC) {
                Vec scaled;
                Scale(&scaled, &a->x250, D_0029C2B8);
                func_00231C88((ImpulseBody *)(a), (Vector4 *)(&scaled));
            }
        }
        func_001CCFB8((char *)a->x158 + 0x7720, a->x430);
        func_001554C0((char *)a->x158 + 0x3FD0, &a->x250);
        func_001B0320((char *)a->x158 + 0x4D40, &a->x260);
        func_001AFC90((char *)a->x158 + 0x4D58, &a->x250, a->x4A6);
    }
    if (a->x158->x78D0)
        func_00199AD8(a->x158->x78D0, a->x430);
    float height = 0.0f;
    Hit *hit = func_002116E8(&groups[3], 1, 0);
    if (hit) {
        if (hit->position.w < 0.0f)
            height = -hit->h.f[2];
        else
            height = before[3].z - hit->position.z;
        COPY_VECTOR(a->x158->x7AA0, &hit->normal);
        if (a->x430 != 0.0f && (a->x48C < 1.0f ||
            (func_00138570((char *)a->x158 + 0x40C0) && !(unsigned char)func_00138560((char *)a->x158 + 0x40C0)))) {
            Parent *p = a->x158;
            if (((unsigned char)(p->x4098 >> 2) & 1) == 0) {
              if (((unsigned char *)&p->x792C->x0)[5]) {
                Vec normal;
                Vec old;
                *(u128 *)&normal = *(u128 *)&hit->normal;
                *(u128 *)&old = *(u128 *)&p->x4180;
                CallShared001B5DE8_0(&normal, &position, surface, &old, a->x430 * 0.0004f,
                             (char *)p->x792C + 0x40, p->x7A44->x281C, p);
              }
            }
        }
    }
    a->x158->x7AC0 = height;
    Body *body = &a->x2E0;
    unsigned short activeState = 2;
    Body *right = &a->x360;
    if (body->state() == activeState || right->state() == activeState) {
        Group probes[3];
        Hit probeHits[3][10];
        Vec probeBefore[3];
        Vec probeAfter[3];
        for (int i = 0; i < 3; i++) {
            probes[i].x0 = &probeBefore[i];
            probes[i].x4 = &probeAfter[i];
            probes[i].x20 = probeHits[i];
            probes[i].x14 = 0;
            probes[i].x18 = 0;
            probes[i].x1C = 10;
            probes[i].x10 = 0.0f;
            probes[i].x8 = 1.0f;
            probes[i].xC = 1.0f;
        }
        Body *left = &a->x2E0;
        float distance = func_00134F58(a, 0)->x0.w + D_0029C300 + D_0029C330;
        func_00134D70(left, &probeBefore[0], a);
        distance = -distance;
        AddScaled(&probeAfter[0], &probeBefore[0], (Vec *)&left->x50, distance);
        func_00134D70(right, &probeBefore[1], a);
        AddScaled(&probeAfter[1], &probeBefore[1], (Vec *)&right->x50, distance);
        Vec *axis = &a->x158->x792C->x60;
        AddScaled(&probeBefore[2], &probeBefore[0], axis, 0.9f);
        AddScaled(&probeAfter[2], &probeBefore[1], axis, 0.9f);
        QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(probes), numProbes);
        hit = func_002116E8(&probes[0], 1, 0);
        if (hit && left->x74 == 2) {
            left->x30 = ((u128 *)hit)[0];
            left->x40 = ((u128 *)hit)[1];
            left->x50 = ((u128 *)hit)[2];
            a->x200 = ((u128 *)hit)[0];
            a->x210 = ((u128 *)hit)[1];
            a->x220 = ((u128 *)hit)[2];
            func_001C99B0((char *)a->x158 + 0x7720, (unsigned short)(hit->h.flags >> 59), 0.0f);
            float time = hit->position.w;
            float radius = func_00134F58(a, 0)->x0.w;
            if (time < (radius + D_0029C300) /
                       (func_00134F58(a, 0)->x0.w + D_0029C300 + D_0029C330) || (a->x4B0 & 8))
                left->x74 = 1;
        }
        hit = func_002116E8(&probes[1], 1, 0);
        if (hit && right->x74 == 2) {
            right->x30 = ((u128 *)hit)[0];
            right->x40 = ((u128 *)hit)[1];
            right->x50 = ((u128 *)hit)[2];
            a->x200 = ((u128 *)hit)[0];
            a->x210 = ((u128 *)hit)[1];
            a->x220 = ((u128 *)hit)[2];
            func_001C9A00((char *)a->x158 + 0x7720, (unsigned short)(hit->h.flags >> 59), 0.0f);
            float time = hit->position.w;
            float radius = func_00134F58(a, 1)->x0.w;
            if (time < (radius + D_0029C300) /
                       (func_00134F58(a, 1)->x0.w + D_0029C300 + D_0029C330) || (a->x4B0 & 8))
                right->x74 = 1;
        }
        if (func_002116E8(&probes[2], 1, 0))
            func_00155E78((char *)a->x158 + 0x3FD0, 1);
    }
}
