/* UNIT 0x00112360 0x001126A8 */
#include "rider.h"
typedef struct Vec {
    float x, y, z, w;
} Vec;

struct Obj;

struct Sub2D4 {
    char pad0[0xCC];
    int flags; /* 0xCC (= Obj 0x3A0) */
};

struct LocalRider {
    char pad0[0x6390];
    int difficulty; /* 0x6390 */
    char pad6394[0x1540];
    struct Obj *x78D4;
    char pad78D8[0x54];
    Vec *x792C;
    char pad7930[0x48];
    int x7978;
    int x797C;
};

struct Obj {
    struct LocalRider *x0;
    int x4;
    struct LocalRider *x8;
    char padC[0x104];
    Vec x110[4];
    Vec x150[16];
    char pad250[0x44];
    float x294;
    char pad298[0x108];
    int x3A0;
    char pad3A4[0x20];
    int x3C4[4];
    int x3D4[4];
    char pad3E4[0x18];
    int x3FC;
    char pad400[0x18];
    int x418;
    int x41C;
    int timer;      /* 0x420 */
    int timerReset; /* 0x424 */
    char pad428[0x18];
    unsigned long flags; /* 0x440 */
};

struct Game {
    char pad0[0x50140];
    struct LocalRider *riders[26]; /* 0x50140 */
    int x501A8;
    char pad501AC[8];
    int x501B4;
    char pad501B8[12];
    int x501C4;
};

struct Mode {
    char pad0[0x10C];
    int x10C;
    char pad110[0x178];
    int x288;
};

struct Other {
    char pad0[0x5BE0];
    int x5BE0;
};

struct Clock {
    char pad0[8];
    int x8;
};

extern struct Game D_002AF2C0;
extern struct Mode D_004B25A0;
extern struct Other D_00398C40;
extern struct Clock D_0043EEC0;

extern int func_00238F60(void);
extern int func_00222BD8(int min, int max);
extern void func_00113650(struct Obj *o);
extern void func_00113330(struct Obj *o, struct LocalRider *r, Vec *v);


void func_00112360(struct Obj *o)
{
    int timer, prob, count, index;
    Vec *v;
    struct Sub2D4 *sub;
    unsigned long flags;
    struct LocalRider *r;

    timer = o->timer - func_00238F60();
    timer = timer < 0 ? 0 : timer;
    o->timer = timer;

    if (o->flags & (0x8000UL << 28)) {
        func_00113650(o);
        return;
    }

    if (o->x8 == 0 || timer != 0 || o->x0->x797C != D_002AF2C0.x501B4) {
        return;
    }

    o->timer = o->timerReset;
    prob = o->x3C4[o->x8->x7978];
    if (prob && o->x0->difficulty >= 2) {
        prob /= 2;
    }
    if (func_00222BD8(0, 100) < prob) {
        r = o->x8;
        func_00113398((RiderPhysics *)(o), (Rider *)(r), (const Vector4 *)(&o->x110[r->x7978]), o->x294);
    } else if (((o->x3A0 >> 3) & 1) && !o->x418) {
        r = o->x8;
        func_00113330(o, r, &o->x110[r->x7978]);
    }

    flags = o->flags;
    if (flags & (0x8000UL << 27)) {
        return;
    }
    if (!((flags & (0x8000UL << 35)) && o->x8->x792C[1].z > o->x0->x792C[1].z)
        && D_004B25A0.x10C != 10) {
        return;
    }

    count = D_002AF2C0.x501A8 - D_004B25A0.x288;
    index = 0;
    if (count > 0) {
        sub = (struct Sub2D4 *)((char *)o + 0x2D4);
        v = o->x150;
        do {
            struct LocalRider *other = D_002AF2C0.riders[index];

            if (other != o->x0) {
                struct Obj *state = other->x78D4;
                int chance = o->x3D4[index];

                if (D_00398C40.x5BE0 && D_002AF2C0.x501C4 >= 4) {
                    chance = chance >= state->x3FC ? 100 : chance;
                }

                if (chance && o->x0->difficulty == 0) {
                    chance += 10;
                } else if (chance && o->x0->difficulty == 1) {
                    chance += 5;
                } else if (o->x0->difficulty == 0) {
                    chance += 3;
                } else if (o->x0->difficulty == 1) {
                    chance += 1;
                }

                if (D_0043EEC0.x8 < 420) {
                    chance /= 2;
                }

                if (chance >= state->x3FC) {
                    float dist;
                    /* EE FPU multiply-accumulate + sqrt.s */
                    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                                      : "=&f"(dist) : "f"(v->x), "f"(v->y), "f"(v->z));
                    func_00113398((RiderPhysics *)(o), (Rider *)(other), (const Vector4 *)(v), dist);
                } else if (((sub->flags >> 3) & 1) && !o->x418) {
                    func_00113330(o, other, v);
                }
            }
            index++;
            v++;
        } while (index < count);
    }
}
