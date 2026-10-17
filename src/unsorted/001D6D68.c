/* UNIT 0x001D6D68 0x001D7488 */
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

void func_001D6D68(char *obj)
{
    char *state;
    char *x420;
    char *x490;
    char *x500;

    x420 = obj + 0x420;
    x490 = obj + 0x490;
    x500 = obj + 0x500;
    func_001D4C48(obj);
    func_001DB4C0((SoundEmitter *)(obj + 0x10), STEAM_BURST, 100, 130, -978, -9378, 6810, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0x50), STEAM_BURST, 100, 130, -1035, -9446, 6700, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0x90), STEAM_BURST, 100, 130, -1021, -9644, 6486, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0xD0), STEAM_BURST, 100, 130, -1112, -9729, 6355, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0x110), STEAM_BURST, 100, 130, -878, -9775, 6342, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0x150), STEAM_BURST, 100, 130, -1117, -9591, 6568, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0x190), STEAM_BURST, 100, 130, -957, -9869, 6227, 0);
    func_001DB4C0((SoundEmitter *)(obj + 0x1D0), STEAM_BURST, 100, 130, -829, -10541, 6375, 0);
    func_001D2C70(obj + 0x240, 0x64B, WIND_LGT);
    func_001D2C70(obj + 0x270, 0x64C, WIND_HVY);
    func_001D2C70(obj + 0x330, 0x658, FOREST_LEAF);
    func_001D2C70(obj + 0x360, 0x65C, FOREST_DNSE);
    func_001D2C70(obj + 0x300, 0x645, WIND_LCAVE);
    func_001D2C70(obj + 0x390, 0x655, CITY);
    func_001D2C70(obj + 0x3F0, 0x64A, WIND_SCAVE);
    func_001D2C70(obj + 0x2A0, 0x647, BIRDS_LGT);
    func_001D2C70(obj + 0x2D0, 0x648, BIRDS_HVY);
    func_001D2C70(obj + 0x210, 0x649, WORLD);

    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 30);
    func_001D4818(state, 0x65E, 30);
    func_001D4818(state, 0x65F, 0);
    func_001D2C70(obj + 0x3C0, 0x64D, FOLIAGE);
    func_001D1C08(x420, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x490, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x500, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(obj + 0x570, 0x657, 0x7D00, TREE_WHOOSH, FOLIAGE);
    if (D_004B2840[0] == 10) {
        func_001DB4C0((SoundEmitter *)(obj + 0x5E0), FALL, 50, 300, 0x4D2, -0x1F42, 0x2526, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x620), FALL, 50, 300, 0x6D5, -0x1FB0, 0x238C, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x660), OCEAN, 250, 3000, 0x623, -0x1FB0, 0x2363, 0);
        func_001C0B40(obj + 0x9D0, 18, 13, 150, 400, -923.0f, 5971.0f, -5076.0f);
    } else if (D_004B2840[0] == 2) {
        func_001DB4C0((SoundEmitter *)(obj + 0x660), OCEAN, 1800, 5000, -0xC22, -0x31FB, -0x1618, 0);
        func_001C0B40(obj + 0x9D0, 18, 13, 150, 400, -1758.0f, -10606.0f, -5369.0f);
    } else if (D_004B2840[0] == 8) {
        func_001DB4C0((SoundEmitter *)(obj + 0x660), OCEANX, 1800, 6000, 0x107A, -0x125F, -0x630, 0);
    }
}
