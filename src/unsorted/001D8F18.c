/* UNIT 0x001D8F18 0x001D9000 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

/* names packed little-endian into 128-bit values */
#define NAME_E_REFINERY_L (((u128)0x4C5F5952LL << 64) | (u128)0x454E494645525F45LL)
#define NAME_E_WIND_HVY_L (((u128)0x4C5F5956LL << 64) | (u128)0x485F444E49575F45LL)
#define NAME_E_RANDOM_THUNDE (((u128)0x45444E5548545FLL << 64) | (u128)0x4D4F444E41525F45LL)

typedef struct Ambient {
    char pad0[0x10];
    char x10[0x30];
    char x40[0x30];
    char x70[0x10];
} Ambient;

extern void func_001D2C70(void *obj, int value, u128 name);

void func_001D8F18(Ambient *amb)
{
    func_001D2C70(amb->x40, 0x66B, NAME_E_REFINERY_L);
    func_001D2C70(amb->x10, 0x64C, NAME_E_WIND_HVY_L);
    func_001DB4C0((SoundEmitter *)(amb->x70), NAME_E_RANDOM_THUNDE, 8000, 9000, 0x71, -0x1A, 0x7B, 0);
}
