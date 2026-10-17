/* UNIT 0x001D9A40 0x001DAC50 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

/* names packed little-endian into 128-bit values */
#define WIND_LGT_L (((u128)0x4C5F5447LL << 64) | (u128)0x4C5F444E49575F45LL)
#define NEIBORHOOD_L (((u128)0x4C5F444F4F48LL << 64) | (u128)0x524F4249454E5F45LL)
#define CITY_SND_L (((u128)0x4C5F444ELL << 64) | (u128)0x535F595449435F45LL)
#define MALL_L ((u128)0x4C5F4C4C414D5F45LL)
#define PARK_GARAGE_L (((u128)0x4C5F4547415241LL << 64) | (u128)0x475F4B5241505F45LL)
#define FOREST_LEAF_L (((u128)0x4C5F4641454C5FLL << 64) | (u128)0x545345524F465F45LL)
#define SUBWAYSTA_L (((u128)0x4C5F415453LL << 64) | (u128)0x5941574255535F45LL)
#define TUNNEL_L (((u128)0x4C5FLL << 64) | (u128)0x4C454E4E55545F45LL)
#define QUARRY_L (((u128)0x4C5FLL << 64) | (u128)0x5952524155515F45LL)
#define STRUC_UNDER_L (((u128)0x4C5F5245444E55LL << 64) | (u128)0x5F43555254535F45LL)
#define ESCALATOR_L (((u128)0x4C5F524F54LL << 64) | (u128)0x414C414353455F45LL)
#define EXHAUSTFAN_L (((u128)0x4C5F4E414654LL << 64) | (u128)0x5355414858455F45LL)
#define HOUSE ((u128)0x4553554F485F45LL)
#define STORE ((u128)0x45524F54535F45LL)
#define CARALARM_L (((u128)0x4C5F4D52LL << 64) | (u128)0x414C415241435F45LL)
/* "E_CAR_L " (with a trailing space) */
#define CAR_L ((u128)0x204C5F5241435F45LL)
#define COPCAR_L (((u128)0x4C5FLL << 64) | (u128)0x524143504F435F45LL)
#define SEWER_DRIP_L (((u128)0x4C5F50495244LL << 64) | (u128)0x5F52455745535F45LL)
#define TRACTOR_L (((u128)0x4C5F52LL << 64) | (u128)0x4F54434152545F45LL)
#define CONVEYOR_L (((u128)0x4C5F524FLL << 64) | (u128)0x5945564E4F435F45LL)
#define CAPITAL_L (((u128)0x4C5F4CLL << 64) | (u128)0x4154495041435F45LL)
#define TRUCK_L (((u128)0x4CLL << 64) | (u128)0x5F4B435552545F45LL)
#define TRANFORMER_L (((u128)0x4C5F52454D52LL << 64) | (u128)0x4F464E4152545F45LL)
#define FOUNTAIN_L (((u128)0x4C5F4E49LL << 64) | (u128)0x41544E554F465F45LL)
#define BIRDS_LGT_L (((u128)0x4C5F54474CLL << 64) | (u128)0x5F53445249425F45LL)
#define STONE_STRC_L (((u128)0x4C5F43525453LL << 64) | (u128)0x5F454E4F54535F45LL)
#define CAMPUS_L (((u128)0x4C5FLL << 64) | (u128)0x5355504D41435F45LL)
#define FURNACE_L (((u128)0x4C5F45LL << 64) | (u128)0x43414E5255465F45LL)
#define SCHOOL_BELLS (((u128)0x534C4C45425FLL << 64) | (u128)0x4C4F4F4843535F45LL)
#define FOLIAGE (((u128)0x45LL << 64) | (u128)0x4741494C4F465F45LL)
#define TREE_WHOOSH (((u128)0x48534F4F48LL << 64) | (u128)0x575F454552545F45LL)

extern void func_001BE9D0(void *, int, int, int, int, int, int);
extern void func_001C0B40(void *, int, int, int, int, float, float, float);
extern void func_001C0DE0(void *, int, int);
extern void func_001D1C08(void *, int, int, u128, u128);
extern void func_001D2C70(void *, int, u128);
extern void func_001D4818(void *, int, int);
extern int func_001D4850(void *, int);
extern void func_001D4C48(void *);
extern char D_00398C40[];
extern int D_004B2840[];

void func_001D9A40(char *obj)
{
    char *state;
    char *x2E0;
    char *x350;
    char *x3C0;

    func_001D4C48(obj);
    *(int *)(obj + 0xAA4) = 0;
    *(int *)(obj + 0xAA0) = 0;
    if (D_004B2840[0] == 20) {
        func_001D2C70(obj + 0x40, 0x64b, WIND_LGT_L);
        func_001D2C70(obj + 0x70, 0x66d, NEIBORHOOD_L);
        func_001D2C70(obj + 0xa0, 0x655, CITY_SND_L);
        func_001D2C70(obj + 0xd0, 0x66a, MALL_L);
        func_001D2C70(obj + 0x100, 0x667, PARK_GARAGE_L);
        func_001D2C70(obj + 0x130, 0x658, FOREST_LEAF_L);
        func_001D2C70(obj + 0x160, 0x652, SUBWAYSTA_L);
        func_001D2C70(obj + 0x190, 0x669, TUNNEL_L);
        func_001D2C70(obj + 0x1c0, 0x66b, QUARRY_L);
        func_001D2C70(obj + 0x1f0, 0x660, STRUC_UNDER_L);
        func_001D2C70(obj + 0x280, 0x671, ESCALATOR_L);
        func_001DB4C0((SoundEmitter *)(obj + 0x430), EXHAUSTFAN_L, 0x78, 0x384, 0x104, -0xd1f, -0x8c8, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x470), EXHAUSTFAN_L, 0x96, 0x3b6, 0xc0, -0x1535, -0xe1a, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4b0), EXHAUSTFAN_L, 0x96, 0x3b6, 0x99, -0x1634, -0xf5d, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4f0), HOUSE, 0x3c, 0xfa, 0x1be, -0x34e, -0x293, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x530), HOUSE, 0x3c, 0xfa, -0x25, -0x42c, -0x369, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x570), HOUSE, 0x3c, 0xfa, -0xb, -0x8eb, -0x6b1, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x5b0), HOUSE, 0x3c, 0xfa, -0x9c, -0x64c, -0x52c, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x5f0), HOUSE, 0x3c, 0xfa, 0x97, -0x71e, -0x520, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x630), HOUSE, 0x3c, 0xfa, -0x73, -0x7b4, -0x5e4, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x670), HOUSE, 0x3c, 0xfa, 0x59, -0xa2c, 0x764, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x6b0), STORE, 0x3c, 0xfa, 0x1e, -0xfd6, -0xac2, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x6f0), STORE, 0x3c, 0xfa, -0xba, -0x1105, -0xb10, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x730), STORE, 0x3c, 0xfa, -3, -0x12b5, -0xc03, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x770), CARALARM_L, 0x3c, 0xfa, -0x3c, -0x1a91, -0x1279, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x7b0), CAR_L, 0x3c, 0xfa, -0x3c, -0x1a91, -0x1279, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x7f0), COPCAR_L, 0x3c, 0xfa, -0x1dc, -0x1a27, -0x12d5, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x830), COPCAR_L, 0x3c, 0xfa, -0x2cc, -0x1b81, -0x1392, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x870), SEWER_DRIP_L, 0x3c, 0xfa, -0xd69, -0x1b2b, -0x1a20, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x8b0), TRACTOR_L, 0x3c, 0xfa, -0xf87, -0x1a9b, -0x1c32, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x8f0), TRACTOR_L, 0x3c, 0xfa, -0xfb6, -0x1ada, -0x1c61, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x930), TRACTOR_L, 0x3c, 0xfa, -0x1106, -0x1b41, -0x1da9, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x970), CONVEYOR_L, 0x3c, 0xfa, -0x1119, -0x1ad8, -0x1d86, 0);
        *(int *)(obj + 0xA98) = 22;
        func_001C0B40(obj + 0x9b0, 0x2b, 5, 0x3c, 0xfa, 446.0f, -846.0f, -659.0f);
        func_001C0B40(obj + 0x9e0, 0x2b, 5, 0x3c, 0xfa, -156.0f, -1612.0f, -1324.0f);
        func_001C0B40(obj + 0xa10, 0x12, 0xd, 0x96, 0x190, -5300.0f, -7117.0f, -8390.0f);
        *(int *)(obj + 0xA9C) = 3;
        func_001C0DE0(obj + 0xa70, 0x652, 5);
        func_001C0DE0(obj + 0xa84, 0x669, 5);
        func_001BE9D0(obj + 0xab0, 0x29, 1, 0x3c, 0x78, 7, 0xa);
    } else if (D_004B2840[0] == 17) {
        func_001D2C70(obj + 0x40, 0x64b, WIND_LGT_L);
        func_001D2C70(obj + 0x70, 0x66d, NEIBORHOOD_L);
        func_001D2C70(obj + 0xa0, 0x655, CITY_SND_L);
        func_001D2C70(obj + 0x130, 0x658, FOREST_LEAF_L);
        func_001D2C70(obj + 0x160, 0x652, CAPITAL_L);
        func_001D2C70(obj + 0x1f0, 0x660, QUARRY_L);
        func_001DB4C0((SoundEmitter *)(obj + 0x430), EXHAUSTFAN_L, 0x78, 0x2bc, 0x1d7, 0xc85, -0x748, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x470), TRUCK_L, 0x3c, 0x15e, 0xbb, 0x87a, -0x532, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4b0), TRUCK_L, 0x3c, 0x15e, 0xbb, 0x895, -0x532, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4f0), TRANFORMER_L, 0x14, 0x64, 0x1ee, 0x5e1, -0x346, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x530), TRANFORMER_L, 0x14, 0x64, 0x290, 0x5df, -0x365, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x570), TRANFORMER_L, 0x14, 0x64, 0x242, 0x660, -0x380, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x5b0), FOUNTAIN_L, 0x14, 0xfa, 0x231, 0xd84, -0x7f8, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x5f0), FOUNTAIN_L, 0x14, 0xfa, 0x1bb, 0xdb9, -0x7d8, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x630), FOUNTAIN_L, 0x14, 0x28a, 0x532, 0xdc3, -0x8b9, 0);
        *(int *)(obj + 0xA98) = 9;
        func_001C0B40(obj + 0x9B0, 18, 13, 150, 400, 1083.0f, 3519.0f, -2237.0f);
        *(int *)(obj + 0xA9C) = 1;
    } else if (D_004B2840[0] == 18) {
        func_001D2C70(obj + 0x10, 0x647, BIRDS_LGT_L);
        func_001D2C70(obj + 0x40, 0x64b, WIND_LGT_L);
        func_001D2C70(obj + 0x70, 0x66d, NEIBORHOOD_L);
        func_001D2C70(obj + 0xa0, 0x655, CITY_SND_L);
        func_001D2C70(obj + 0xd0, 0x66a, MALL_L);
        func_001D2C70(obj + 0x100, 0x667, PARK_GARAGE_L);
        func_001D2C70(obj + 0x130, 0x658, FOREST_LEAF_L);
        func_001D2C70(obj + 0x190, 0x669, TUNNEL_L);
        func_001D2C70(obj + 0x1c0, 0x66b, QUARRY_L);
        func_001D2C70(obj + 0x1f0, 0x660, QUARRY_L);
        func_001D2C70(obj + 0x220, 0x652, STONE_STRC_L);
        func_001D2C70(obj + 0x250, 0x668, CAMPUS_L);
        func_001DB4C0((SoundEmitter *)(obj + 0x430), EXHAUSTFAN_L, 0x78, 0x384, 0x374, 0x2a, -0x136, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x470), EXHAUSTFAN_L, 0x96, 0x3b6, 0x960, 0x318, -0x4f9, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4b0), EXHAUSTFAN_L, 0x96, 0x3b6, 0xe49, 0xd1, -0x755, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x4f0), FURNACE_L, 0x32, 0x320, 0x132a, 0x1f4, -0xa05, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x530), SCHOOL_BELLS, 0x32, 0x320, 0x784, 0xcc, -0x3ed, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x570), EXHAUSTFAN_L, 0xf, 0x50, 0x1308, 0x1fc, -0xa29, 0);
        *(int *)(obj + 0xA98) = 6;
        func_001C0B40(obj + 0x9B0, 18, 13, 150, 400, 5913.0f, 490.0f, -3127.0f);
        *(int *)(obj + 0xA9C) = 1;
    }

    x2E0 = obj + 0x2E0;
    x350 = obj + 0x350;
    x3C0 = obj + 0x3C0;

    func_001D2C70(obj + 0x2B0, 0x64D, FOLIAGE);

    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 80);
    func_001D4818(state, 0x65E, 80);
    func_001D4818(state, 0x65F, 10);
    func_001D1C08(x2E0, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x350, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x3C0, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
}
