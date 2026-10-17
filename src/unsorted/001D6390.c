/* UNIT 0x001D6390 0x001D6A38 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

/* names packed little-endian into 128-bit values */
#define H2OFALL_BG (((u128)0x4C5F47425F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define WIND_LGT (((u128)0x4C5F5447LL << 64) | (u128)0x4C5F444E49575F45LL)
#define WIND_HVY (((u128)0x4C5F5956LL << 64) | (u128)0x485F444E49575F45LL)
#define BIRDS_LGT (((u128)0x4C5F54474CLL << 64) | (u128)0x5F53445249425F45LL)
#define BIRDS_HVY (((u128)0x4C5F595648LL << 64) | (u128)0x5F53445249425F45LL)
#define WIND_LCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x4C5F444E49575F45LL)
#define CITY ((u128)0x4C5F595449435F45LL)
#define FOREST_LEAF (((u128)0x4C5F4641454C5FLL << 64) | (u128)0x545345524F465F45LL)
#define WIND_WCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x575F444E49575F45LL)
#define FOREST_DNSE (((u128)0x4C5F45534E445FLL << 64) | (u128)0x545345524F465F45LL)
#define WIND_BRDG (((u128)0x4C5F474452LL << 64) | (u128)0x425F444E49575F45LL)
#define STONE_STRC (((u128)0x4C5F43525453LL << 64) | (u128)0x5F454E4F54535F45LL)
#define FOLIAGE (((u128)0x45LL << 64) | (u128)0x4741494C4F465F45LL)
#define TREE_WHOOSH (((u128)0x48534F4F48LL << 64) | (u128)0x575F454552545F45LL)

extern void func_001C0B40(void *, int, int, int, int, float, float, float);
extern void func_001C0DE0(void *, int, int);
extern void func_001D1C08(void *, int, int, u128, u128);
extern void func_001D2C70(void *, int, u128);
extern void func_001D4818(void *, int, int);
extern int func_001D4850(void *, int);
extern void func_001D4C48(void *);
extern char D_00398C40[];
extern int D_004B2840[];

#define STEAM_BURST (((u128)0x4C5F5453525542LL << 64) | (u128)0x5F4D414554535F45LL)
#define FALL (((u128)0x4C5F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define OCEAN (((u128)0x4CLL << 64) | (u128)0x5F4E4145434F5F45LL)
#define OCEANX (((u128)0x4C5F58LL << 64) | (u128)0x4D4E4145434F5F45LL)
#define WIND_SCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x535F444E49575F45LL)
#define WORLD (((u128)0x4C5F444C524FLL << 64) | (u128)0x575F4156414C5F45LL)

#define ICE (((u128)0x4C5F56LL << 64) | (u128)0x4E455F4543495F45LL)
#define REFINERY (((u128)0x4C5F5952LL << 64) | (u128)0x454E494645525F45LL)
#define OIL ((u128)0x4C5F4C494F5F45LL)
#define ALARM (((u128)0x4CLL << 64) | (u128)0x5F4D52414C415F45LL)
#define FOREST_PINE (((u128)0x4C5F454E49505FLL << 64) | (u128)0x545345524F465F45LL)
#define WIND_STRUCT (((u128)0x4C5F43555254LL << 64) | (u128)0x535F444E49575F45LL)

void func_001D6390(char *obj)
{
    char *state;
    char *x220;
    char *x290;
    char *x300;

    x220 = obj + 0x220;
    x290 = obj + 0x290;
    x300 = obj + 0x300;
    func_001D4C48(obj);
    *(int *)(obj + 0x564) = 0;
    func_001D2C70(obj + 0xD0, 0x646, ICE);
    func_001D2C70(obj + 0x10, 0x64B, WIND_LGT);
    func_001D2C70(obj + 0x40, 0x64C, WIND_HVY);
    func_001D2C70(obj + 0x70, 0x645, WIND_LCAVE);
    func_001D2C70(obj + 0x130, 0x65A, FOREST_PINE);
    func_001D2C70(obj + 0x160, 0x653, WIND_BRDG);
    func_001D2C70(obj + 0x190, 0x660, WIND_STRUCT);
    func_001D2C70(obj + 0x100, 0x647, BIRDS_LGT);
    func_001D2C70(obj + 0x1C0, 0x66B, REFINERY);
    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 40);
    func_001D4818(state, 0x65E, 30);
    func_001D4818(state, 0x65F, 15);
    func_001D2C70(obj + 0x1F0, 0x64D, FOLIAGE);
    func_001D1C08(x220, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x290, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x300, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
    func_001C0B40(obj + 0x5B0, 31, 4, 200, 1300, -2039.0f, -4404.0f, -6967.0f);
    if (D_004B2840[0] == 22) {
        func_001DB4C0((SoundEmitter *)(obj + 0x3E0), OIL, 20, 200, 0x1B1E, 0x24D, -0x1758, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x420), OIL, 20, 200, 0x1BD5, 0x2CA, -0x1819, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x460), OIL, 20, 200, 0x1C42, 0x2E8, -0x18E4, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4A0), OIL, 20, 200, 0x1D77, 0x3B4, -0x1A0B, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4E0), OIL, 20, 200, 0x1E91, 0x2AD, -0x1B19, 0);
        *(int *)(obj + 0x560) = 5;
        func_001C0DE0(obj + 0x5E0, 0x670, 5);
        func_001C0B40(obj + 0x600, 18, 13, 150, 400, 8165.0f, 705.0f, -7123.0f);
    } else if (D_004B2840[0] == 5) {
        func_001DB4C0((SoundEmitter *)(obj + 0x3E0), OCEAN, 2000, 3000, -0xE83, -0x2600, -0x2B81, 0);
        *(int *)(obj + 0x560) = 1;
        func_001DB4C0((SoundEmitter *)(obj + 0x570), ALARM, 300, 1100, -0xAFA, -0xF82, -0x18C6, 0);
        func_001C0B40(obj + 0x600, 18, 13, 150, 400, -3860.0f, -8081.0f, -11207.0f);
        *(int *)(obj + 0x568) = 0;
    } else if (D_004B2840[0] == 14) {
        func_001DB4C0((SoundEmitter *)(obj + 0x3E0), OCEAN, 1100, 1900, 0x39E, 0x866, -0x730, 0);
        *(int *)(obj + 0x560) = 1;
    }
}
