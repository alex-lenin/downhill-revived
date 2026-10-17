/* UNIT 0x0010C108 0x0010C278 */
#include "rider.h"
typedef struct LocalRider {
    char pad0[0x7978];
    int x7978;
} LocalRider;

typedef struct Obj {
    LocalRider *x0;
    char pad4[4];
    LocalRider *x8;
    char padC[0x110 - 0xC];
    float x110[4][4];
    float x150[20][4];
    char pad290[4];
    float x294;
    char pad298[0x420 - 0x298];
    int x420;
    int x424;
    char pad428[0x440 - 0x428];
    long x440;
} Obj;

struct Global {
    char pad[0x50140];
    LocalRider *riders[26];
    int count;
    int n;
};

extern struct Global D_002AF2C0;
extern int D_004B2828[];

extern int func_00238F60(void);
extern void func_00113650(Obj *);


void func_0010C108(Obj *obj)
{
    long flags;
    long bits;
    int time;
    LocalRider *self;
    struct Global *global;
    LocalRider **riders;
    float *pos;
    int count;
    int n;
    float length;

    time = obj->x420 - func_00238F60();
    flags = obj->x440;
    if (time < 0) {
        time = 0;
    }
    obj->x420 = time;
    if (flags & (1L << 43)) {
        func_00113650(obj);
        return;
    }
    self = obj->x8;
    if (self == 0 || time != 0) {
        return;
    }
    obj->x420 = obj->x424;
    bits = flags | (1L << 33);
    obj->x440 = bits | (1L << 50);
    func_00113398((RiderPhysics *)(obj), (Rider *)(self), (const Vector4 *)(obj->x110[self->x7978]), obj->x294);
    if (obj->x440 & (1L << 42)) {
        return;
    }
    global = &D_002AF2C0;
    n = global->count - D_004B2828[0];
    if (n > 0) {
        count = n;
        pos = obj->x150[0];
        riders = global->riders;
        do {
            if (*riders != obj->x0) {
                __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(pos[0]), "f"(pos[1]), "f"(pos[2]));
                func_00113398((RiderPhysics *)(obj), (Rider *)(*riders), (const Vector4 *)(pos), length);
            }
            pos += 4;
            count--;
            riders++;
        } while (count != 0);
    }
}
