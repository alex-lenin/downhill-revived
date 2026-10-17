/* UNIT 0x00195F90 0x00196038 */
#include "motion_sample.h"
typedef struct Vec { float x, y, z, w; } __attribute__((aligned(16))) Vec;
typedef struct Rider { char pad0[0x793c]; int x793c; } Rider;
typedef struct Slot { char pad0[0x2b4]; Rider *x2b4; char pad1[0x380 - 0x2b8]; } Slot;

extern Slot D_0037EA60[];
extern Vec *func_0023CF78(int);
extern char *func_0023D180(int);
extern int func_0023D608(void);

extern void func_0021D758(int, float);

void func_00195F90(char *obj)
{
    int i;
    Slot *slot;
    Rider *rider;
    char *p;
    Vec pos;
    float a;
    float b;

    slot = D_0037EA60;
    for (i = 0; i < func_0023D608(); i++) {
        rider = slot->x2b4;
        slot++;
        pos = *func_0023CF78(i);
        p = func_0023D180(i);
        SampleMotionDiscard(obj, i, &a, p + 0xb8, &b, pos.z);
        func_0021D758(rider->x793c, b);
    }
}
