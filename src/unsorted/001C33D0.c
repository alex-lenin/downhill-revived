/* UNIT 0x001C33D0 0x001C3428 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

/* "E_GUNSHOT" packed into a 128-bit name */
#define NAME_E_GUNSHOT (((u128)0x54LL << 64) | (u128)0x4F48534E55475F45LL)

extern int D_004BD8A0[];


void func_001C33D0(void)
{
    func_00201B68(D_004BD8A0[0], 0, NAME_E_GUNSHOT, 0x400, 0, 0, 0);
}
