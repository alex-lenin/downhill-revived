/* UNIT 0x001D9038 0x001D9750 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

#define WIND_LGT_L (((u128)0x4C5F5447LL << 64) | (u128)0x4C5F444E49575F45LL)
#define WIND_HVY_L (((u128)0x4C5F5956LL << 64) | (u128)0x485F444E49575F45LL)
#define WIND_SCAVE_L (((u128)0x4C5F45564143LL << 64) | (u128)0x535F444E49575F45LL)
#define FOREST_LEAF_L (((u128)0x4C5F4641454C5FLL << 64) | (u128)0x545345524F465F45LL)
#define BIRDS_LGT_L (((u128)0x4C5F54474CLL << 64) | (u128)0x5F53445249425F45LL)
#define BIRDS_HVY_L (((u128)0x4C5F595648LL << 64) | (u128)0x5F53445249425F45LL)
#define WIND_BRDG_L (((u128)0x4C5F474452LL << 64) | (u128)0x425F444E49575F45LL)
#define STONE_STRC_L (((u128)0x4C5F43525453LL << 64) | (u128)0x5F454E4F54535F45LL)
#define WIND_CANYON_L (((u128)0x4C5F4E4F594E41LL << 64) | (u128)0x435F444E49575F45LL)
#define FOREST_PINE_L (((u128)0x4C5F454E49505FLL << 64) | (u128)0x545345524F465F45LL)
#define FOREST_DEAD_L (((u128)0x4C5F444145445FLL << 64) | (u128)0x545345524F465F45LL)
#define CITY_L (((u128)0x0LL << 64) | (u128)0x4C5F595449435F45LL)
#define FOLIAGE (((u128)0x45LL << 64) | (u128)0x4741494C4F465F45LL)
#define TREE_WHOOSH (((u128)0x48534F4F48LL << 64) | (u128)0x575F454552545F45LL)
#define CHURCH_BELLS (((u128)0x534C4C45425FLL << 64) | (u128)0x4843525548435F45LL)
#define LAKE_L (((u128)0x0LL << 64) | (u128)0x4C5F454B414C5F45LL)
#define WATERWHEEL_L (((u128)0x4C5F4C454548LL << 64) | (u128)0x5752455441575F45LL)
#define H2OFALL_MX_L (((u128)0x4C5F584D5F4CLL << 64) | (u128)0x4C41464F32485F45LL)

extern void func_001C0B40(void *, int, int, int, int, float, float, float);
extern void func_001D1C08(void *, int, int, u128, u128);
extern void func_001D2C70(void *, int, u128);
extern void func_001D4818(void *, int, int);
extern int func_001D4850(void *, int);
extern void func_001D4C48(void *);
extern char D_00398C40[];
extern int D_004B2840[];

void func_001D9038(char *obj)
{
    char *state;
    char *x280 = obj + 0x280;
    char *x2F0 = obj + 0x2F0;
    char *x360 = obj + 0x360;

    func_001D4C48(obj);
    func_001D2C70(obj + 0x70, 0x64B, WIND_LGT_L);
    func_001D2C70(obj + 0xA0, 0x64C, WIND_HVY_L);
    func_001D2C70(obj + 0xD0, 0x64A, WIND_SCAVE_L);
    func_001D2C70(obj + 0x100, 0x658, FOREST_LEAF_L);
    func_001D2C70(obj + 0x10, 0x647, BIRDS_LGT_L);
    func_001D2C70(obj + 0x40, 0x648, BIRDS_HVY_L);
    func_001D2C70(obj + 0x130, 0x653, WIND_BRDG_L);
    func_001D2C70(obj + 0x160, 0x652, STONE_STRC_L);
    func_001D2C70(obj + 0x190, 0x659, WIND_CANYON_L);
    func_001D2C70(obj + 0x1C0, 0x65A, FOREST_PINE_L);
    func_001D2C70(obj + 0x1F0, 0x65B, FOREST_DEAD_L);
    func_001D2C70(obj + 0x250, 0x655, CITY_L);
    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 30);
    func_001D4818(state, 0x65E, 15);
    func_001D4818(state, 0x65F, 15);
    func_001D2C70(obj + 0x220, 0x64D, FOLIAGE);
    func_001D1C08(x280, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x2F0, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x360, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
    if (D_004B2840[0] == 11) {
        func_001DB4C0((SoundEmitter *)(obj + 0x3D0), CHURCH_BELLS, 500, 4000, 0x16F9, -0x2F50, -0x3096, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4D0), LAKE_L, 900, 1400, 0xE3D, -0x210C, -0x27D3, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x510), LAKE_L, 900, 1400, 0xBE0, -0x22D6, -0x28FD, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x550), LAKE_L, 900, 1400, 0xE64, -0x23F4, -0x2AE9, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x590), WATERWHEEL_L, 100, 400, 0x14E5, -0x2A2F, -0x2F7D, 0);
        func_001C0B40(obj + 0x5D0, 18, 13, 150, 400, 5825.0f, -12398.0f, -12653.0f);
    } else if (D_004B2840[0] == 15) {
        func_001DB4C0((SoundEmitter *)(obj + 0x410), H2OFALL_MX_L, 10, 350, 0x344, -0x32, -0x54A, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x450), H2OFALL_MX_L, 10, 350, 0x2F8, 0x54, -0x550, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x490), H2OFALL_MX_L, 10, 350, 0x2B4, 0xC4, -0x551, 0);
    } else if (D_004B2840[0] == 16) {
        func_001C0B40(obj + 0x5D0, 18, 13, 150, 400, -2065.0f, 3377.0f, -6604.0f);
    }
}
