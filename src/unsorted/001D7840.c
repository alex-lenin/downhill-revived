/* UNIT 0x001D7840 0x001D7968 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0xC];
    unsigned int xC;
} Obj;

typedef struct Game {
    char pad0[0x2A0];
    int x2A0;
} Game;

extern Game D_004B25A0;
extern int D_004BD8A0[];

/* "E_H2OFALL_SML_L" and "E_H2OFALL_BG_L" packed little-endian into 128-bit values */
#define NAME_E_H2OFALL_SML_L (((u128)0x4C5F4C4D535F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define NAME_E_H2OFALL_BG_L (((u128)0x4C5F47425F4CLL << 64) | (u128)0x4C41464F32485F45LL)

void func_001D7840(Obj *obj)
{
    Game *game = &D_004B25A0;

    if (game->x2A0 != 30) {
        if (game->x2A0 != 29) {
            return;
        }
    }
    if (obj->xC == 0) {
        obj->xC = 0xFFFFFFFF;
        if (game->x2A0 == 30) {
            func_00201BD0(D_004BD8A0[0], 0, NAME_E_H2OFALL_SML_L, 0x399, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->xC));
        } else {
            func_00201BD0(D_004BD8A0[0], 0, NAME_E_H2OFALL_BG_L, 0x399, 0, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->xC));
        }
    }
}
