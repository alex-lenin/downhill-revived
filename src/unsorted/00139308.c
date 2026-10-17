/* UNIT 0x00139308 0x00139660 .sdata=0x29C3BC:0x29C3C4 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00139940_3_func_00139940 func_00139940
#else
#define CallScalarBridge00139940_3_func_00139940 ScalarBridge00139940_3
#endif
typedef unsigned int Vec __attribute__((mode(TI)));
typedef union Quad { Vec v; float f[4]; } Quad;
typedef struct Entry { signed char x; signed char y; signed char z; signed char flags; } Entry;
typedef struct Packet { int x0; char *x4; char pad8[0x18]; Entry e[1]; } Packet;
typedef struct Body { char pad0[0x18]; float x18; } Body;
typedef struct Rider { char pad0[0x792C]; Body *x792C; } Rider;
typedef struct State {
    char pad0[0x316E4];
    Packet *x316E4;
    int x316E8, x316EC, x316F0, x316F4, x316F8, x316FC, x31700, x31704;
    Rider *x31708;
    int x3170C, x31710;
    char pad31714[0x2C];
    Quad x31740;
    Quad x31750;
    int x31760;
    char x31764;
    char x31765;
} State;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

int D_0029C3BC = 250;
int D_0029C3C0 = 25;

extern void func_001398F8(State *);
extern int func_001396B8(State *, int, int, float *);
extern void func_00138828(State *, Quad *);

#if defined(__mips__)
extern void func_00139940(State *, Quad *, int, int, float);
#else

#endif

extern void func_00139A48(State *);

void func_00139308(State *s)
{
    Quad pos;
    float w;
    int count;
    int i;
    int j;
    Quad *dst;
    Quad *cur;
    Quad *saved;
    int next;
    int type;
    int value;
    Packet *p;

    s->x316FC = s->x316F8++;
    func_001398F8(s);
    count = s->x31704;
    for (j = 0; j < s->x31704; j++) {
        s->x316E4->x4 -= 0x10;
    }
    dst = &s->x31740;
    COPY_VECTOR(*dst, &s->x31750);
    for (i = s->x31700; i < s->x316E4->x0; i = next) {
        type = s->x316E4->e[i].flags & 0x7F;
        s->x31710 = type;
        value = func_001396B8(s, type, i, &w);
        cur = &s->x31740;
        saved = &s->x31750;
        if (i == 0) {
            COPY_VECTOR(pos, s->x316E4->x4);
            COPY_VECTOR(*cur, &pos);
            count++;
            s->x316E4->x4 -= 0x10;
        } else {
            p = s->x316E4;
            if ((p->e[i].flags & 0x7F) == 0x7F) {
                __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                                  : "=m"(pos) : "r"(cur), "r"(p->x4));
                count++;
                p->x4 -= 0x10;
            } else {
                pos.f[0] = s->x31740.f[0] + p->e[i].x * 0.25f;
                pos.f[1] = s->x31740.f[1] + p->e[i].y * 0.25f;
                pos.f[2] = s->x31740.f[2] + p->e[i].z * 0.25f;
            }
            COPY_VECTOR(*cur, &pos);
        }
        if (i == s->x316F8) {
            if (pos.f[2] < s->x31708->x792C->x18) {
                s->x31764 = 1;
            } else {
                s->x31764 = 0;
            }
        }
        if (i == s->x316F8 + 1) {
            func_00138828(s, &pos);
        }
        if (s->x31708->x792C->x18 + (float)D_0029C3C0 < pos.f[2]) {
            s->x31700 = i + 1;
            COPY_VECTOR(*saved, &pos);
            s->x31704 = count;
            next = i + 1;
        } else {
            if (pos.f[2] < s->x31708->x792C->x18 - (float)D_0029C3BC) break;
            next = i + 1;
            if (s->x316E4->e[i].flags & 0x80) {
                CallScalarBridge00139940_3_func_00139940(s, &pos, value, i, w);
            }
        }
    }
    if (i < s->x316F8) {
        s->x31765 = 0;
    }
    func_00139A48(s);
}
