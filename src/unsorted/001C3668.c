/* UNIT 0x001C3668 0x001C3740 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[4];
    int x4;
    int x8;
    char padC[0x28];
    unsigned int x34;
} Obj;

extern int D_004BD8B4[];
extern int func_0023CF78(int);
extern int func_0023CF58(int);
extern float func_00222640(int);
#if defined(__mips__)
extern int func_00208C60(float, int, int);
#else
#include "sound_math.h"
#define func_00208C60 SoundHeadingFirst
#endif

/* "B_AI_DIRT_RIDE" packed little-endian into a 128-bit value */
#define NAME_B_AI_DIRT_RIDE (((u128)0x454449525F54LL << 64) | (u128)0x5249445F49415F42LL)

void func_001C3668(Obj *obj)
{
    int a;
    int handle;

    a = func_0023CF78(0);
    handle = func_00208C60(func_00222640(func_0023CF58(0) + 0x10), a, obj->x4);
    if (obj->x34 == 0) {
        obj->x34 = 0xFFFFFFFF;
        func_00201BD0(D_004BD8B4[0], 0, NAME_B_AI_DIRT_RIDE, 0x400, handle, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x34));
        obj->x8 = 0;
    }
}
