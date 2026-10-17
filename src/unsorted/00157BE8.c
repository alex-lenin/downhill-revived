/* UNIT 0x00157BE8 0x00157E90 */
#include "spatial_batch.h"
typedef float Vector[4] __attribute__((aligned(16)));

/* the rigid body state at obj+0x10; func_00232B60/func_00232BF8 save and restore it */
typedef struct Body { char bytes[0x10C]; } Body;
typedef struct BodyBackup { char bytes[0xE0]; } BodyBackup __attribute__((aligned(16)));

typedef struct Mover {
    char pad0[0x10];
    Body body;
    char *x11C; /* its position vector is at +0x10 */
    int x120;
    float radius;
} Mover;

typedef struct Hit { char pad0[0x1C]; float fraction; } Hit;
typedef struct HitBuffer { char bytes[0x180]; } HitBuffer __attribute__((aligned(16)));

/* one swept-sphere query for func_00214BA0 / func_002116E8 */
typedef struct Sweep {
    Vector *start;
    Vector *end;
    int x8;
    float xC;
    float radius;
    int x14;
    int x18;
    int maxHits;
    HitBuffer *hits;
    int x24;
} Sweep;

extern int func_00238F30(void);
extern void func_00232B60(Body *, BodyBackup *);
extern void func_00232BF8(Body *, BodyBackup *);
extern void func_002327B0(Body *, float);
extern void func_00231C10(Body *);
extern Hit *func_002116E8(Sweep *, int, Hit *);
extern void func_00157E90(Mover *, Sweep *);
extern int func_00157F60(Mover *, Hit *, int);

#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* Advances the body one frame, sweeping it against the world and backing it up
   to the first hit it accepts (at most two passes). */
void func_00157BE8(Mover *obj)
{
    BodyBackup backup;
    Sweep sweeps[1];
    HitBuffer hits[1];
    Vector start[1], end[1];
    float dt, remaining, step, value, zero, outerZero;
    int j, pass;
    Hit *hit;

    dt = 1.0f / (float)func_00238F30();
    for (j = 0; j < 1; j++) {
        sweeps[j].x8 = 0;
        sweeps[j].xC = 1.0f;
        sweeps[j].start = &start[j];
        sweeps[j].end = &end[j];
        sweeps[j].hits = &hits[j];
        sweeps[j].maxHits = 8;
        sweeps[j].radius = obj->radius;
        sweeps[j].x14 = 0;
        sweeps[j].x18 = 0;
    }
    remaining = 1.0f;
    outerZero = 0.0f;
    pass = 0;
    do {
        func_00232B60(&obj->body, &backup);
        COPY_VECTOR(start[0], obj->x11C + 0x10);
        step = dt * remaining;
        func_002327B0(&obj->body, step);
        func_00231C10(&obj->body);
        COPY_VECTOR(end[0], obj->x11C + 0x10);
        QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(sweeps), 1);
        hit = func_002116E8(sweeps, 1, 0);
        if (hit) {
            if (pass + 1 == 2)
                hit->fraction = -1.0f;
            else
                func_00232BF8(&obj->body, &backup);
            if (hit->fraction < outerZero) {
                func_00157E90(obj, sweeps);
            } else {
                zero = 0.0f;
                for (;;) {
                    if (hit) {
                        func_002327B0(&obj->body, step * hit->fraction);
                        if (func_00157F60(obj, hit, 0)) {
                            value = remaining - remaining * hit->fraction;
                            __asm__("max.s %0,%1,%2" : "=f"(remaining) : "f"(zero), "f"(value));
                            break;
                        }
                        func_00232BF8(&obj->body, &backup);
                    } else {
                        /* no acceptable hit: take the whole step */
                        func_002327B0(&obj->body, step);
                        goto next;
                    }
                    hit = func_002116E8(sweeps, 1, hit);
                }
            next:;
            }
            pass++;
        } else
            break;
    } while (remaining > outerZero && pass < 2);
}
