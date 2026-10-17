/* UNIT 0x00181CB0 0x00181D60 */
#include "tween.h"
#include "animation.h"
struct Owner { char pad0[0x48B0]; int x48B0; };
struct State { struct Owner *owner; int x4; int x8; int xC; int x10; };
extern void func_001ACFA0(void *, int);
extern void func_00136030(void *, int);
extern void func_00181F70(struct State *);



void func_00181CB0(struct State *state) {
    struct Owner *owner;
    char *base;
    int index;
    if (state->xC) {
        owner = state->owner;
        base = (char *)owner + 0x1E40;
        func_001ACFA0((char *)owner + 0x4580, 0);
        owner->x48B0 = 0;
        func_00136030((char *)owner + 0x40C0, 1);
        func_00181F70(state);
        index = state->x8;
        if (index) func_00202E38((AnimationHandle *)(base + (index * 16 + 0x14)), 1, 7, (float)state->x10);
        SetSecondaryBlendFloatFirst(0.25f, (RiderSecondaryBlend *)((char *)owner + 0x3530), state->x10);
        func_001AE630((RiderBodyBlend *)((char *)owner + 0x47F0), state->x10, 0.0f);
    }
}
