/* UNIT 0x001AAB18 0x001AAC00 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00231E00_2_func_00231E00 func_00231E00
#else
#define CallScalarBridge00231E00_2_func_00231E00 ScalarBridge00231E00_2
#endif
struct Model {
    char pad0[0x4];
    float x4;
    float x8;
    float xC;
    char pad10[0x30];
    char x40[1];
};

struct Flags {
    long x0;
    char pad8[0x8];
    char x10[1];
};

struct Sub {
    char pad0[0x4B2];
    short x4B2;
};

struct Player {
    char pad0[0x40C0];
    Sub x40C0;
};

struct Target {
    char pad0[0x10];
    char x10[0x10];
    char x20[1];
};


#if defined(__mips__)
extern void func_00231E00(Sub *sub, char *model, char *a, char *b, float x, float y, float z);
#else

#endif

extern float func_00138580(Sub *sub);
extern void func_001CF478(void *obj, int value, char *flags, int id);

struct A {
    char pad0[0x20];
    Model *x20;
    Flags *x24;
    short x28;
    char pad2A[2];
    char x2C[1];
    int func_001AA318();
    void func_001AAB18(Player *player, Target *target);
};

void A::func_001AAB18(Player *player, Target *target)
{
    if (x20 || func_001AA318()) {
        Sub *sub = &player->x40C0;
        sub->x4B2 = 1;
        x24->x0 &= ~(1L << 57);
        if (player && target) {
            CallScalarBridge00231E00_2_func_00231E00(sub, x20->x40, target->x10, target->x20, x20->x4, x20->x8, x20->xC);
            func_001CF478(x2C, (int)func_00138580(sub), x24->x10, x28);
        }
    }
}
