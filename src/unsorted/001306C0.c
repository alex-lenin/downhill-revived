/* UNIT 0x001306C0 0x001308A8 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001C2458_1_func_001C2458 func_001C2458
#else
#define CallScalarBridge001C2458_1_func_001C2458 ScalarBridge001C2458_1
#endif
typedef unsigned int u128 __attribute__((mode(TI)));

typedef union Vec {
    u128 q;
    float f[4];
} Vec;

typedef struct Item {
    unsigned long bits;
    char pad8[0x20 - 0x8];
    Vec v;
} Item;

typedef struct Part {
    char pad0[0x34];
    int x34;
    float *x38;
} Part;

typedef struct State {
    char pad0[5];
    unsigned char x5;
    char pad6[0x10 - 0x6];
    Vec x10;
    char pad20[0x90 - 0x20];
    Vec x90;
} State;

typedef struct Other {
    char pad0[0x281C];
    int x281C;
} Other;

typedef struct Owner {
    char pad0[0x40C0];
    Part part;
    char pad40FC[0x4280 - 0x40FC];
    Vec x4280;
    char pad4290[0x4ECC - 0x4290];
    void *x4ECC;
    char pad4ED0[0x62B4 - 0x4ED0];
    char x62B4[0x7720 - 0x62B4];
    char x7720[0x792C - 0x7720];
    State *state;
    char pad7930[0x7A44 - 0x7930];
    Other *other;
    char pad7A48[0x7A54 - 0x7A48];
    int x7A54;
    int x7A58;
    char pad7A5C[0x7AC4 - 0x7A5C];
    float x7AC4;
    char pad7AC8[0x7B10 - 0x7AC8];
    Vec x7B10;
    char pad7B20[0x7B50 - 0x7B20];
    Vec x7B50;
} Owner;

extern unsigned char func_00138560(Part *);
extern Item *func_00138558(Part *);
extern float func_001385A8(Part *);
extern int func_001304F8(Vec *, Vec *, Vec *, Vec *, Vec *, Vec *, Vec *);

#if defined(__mips__)
extern void func_001C2458(int *, int *, int, Vec *, Vec *, Vec *, unsigned long, float, float, unsigned short, int);
#else

#endif

extern void func_001C7FC8(void *);
extern void func_001C8188(void *);
extern float func_00236088(float, float, float, float);

static inline float Dot(float *a, float *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    return r;
}

static inline int StateFlag(State *state)
{
    return state->x5 != 0;
}

static inline Part *GetPart(Owner *owner)
{
    return &owner->part;
}

static inline void UpdateTrail(Owner *owner, Part *part, int field, Vec *from, Vec *a, Vec *b,
                               unsigned char flag, Vec *out)
{
    CallScalarBridge001C2458_1_func_001C2458(&owner->x7A54, &owner->x7A58, field, from, a, b, flag, out->f[3],
                  func_001385A8(part), (unsigned short)(func_00138558(part)->bits >> 53) & 0x3F,
                  owner->other->x281C);
}

void func_001306C0(Owner *owner)
{
    Vec out;
    Vec pos;
    Vec velocity;
    Vec extra;
    Part *part = &owner->part;
    Part *self;
    Vec *from;
    Vec *a;
    Vec *b;
    int active;
    int field;
    unsigned long flag;
    unsigned long trailFlag;

    active = (func_00138560(part) ^ 1) & 0xFF;
    if (active) {
        pos.q = owner->state->x10.q;
        velocity.q = owner->state->x90.q;
        active = func_001304F8(&owner->x7B10, &owner->x7B50, &owner->x4280, &pos, &velocity,
                               (extra.q = func_00138558(part)->v.q, &extra), &out);
    }
    field = part->x34;
    from = &owner->x4280;
    a = &owner->x7B10;
    b = &owner->x7B50;
    do {
        flag = 0;
        if (active && owner->x4ECC != owner->x62B4)
            flag = StateFlag(owner->state);
    } while (0);
    trailFlag = flag % 256;
    CallScalarBridge001C2458_1_func_001C2458(&owner->x7A54, &owner->x7A58, field, from, a, b, trailFlag, out.f[3],
                  func_001385A8(GetPart(owner)),
                  (unsigned short)(func_00138558(GetPart(owner))->bits >> 53) & 0x3F,
                  owner->other->x281C);
    if (active) {
        func_001C7FC8(owner->x7720);
    } else {
        func_001C8188(owner->x7720);
    }
    owner->x7AC4 = func_00236088(owner->x7AC4, Dot(out.f, GetPart(owner)->x38), 0.4f, 0.2f);
}
