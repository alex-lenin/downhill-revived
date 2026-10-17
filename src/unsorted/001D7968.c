/* UNIT 0x001D7968 0x001D8190 */
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

void func_001D7968(char *obj)
{
    char *state;
    char *x700;
    char *x770;
    char *x7E0;

    func_001D4C48(obj);
    if (D_004B2840[0] == 4) {
        func_001DB4C0((SoundEmitter *)(obj + 0x10), H2OFALL_BG, 360, 1150, 0x586, 0x48E, -0xA72, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x50), H2OFALL_BG, 200, 500, 0x56F, 0x4FC, -0xB28, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x90), H2OFALL_BG, 200, 500, 0x4B2, 0x561, -0xBCB, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0xD0), H2OFALL_BG, 250, 700, 0x42D, 0x516, -0xC67, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x110), H2OFALL_BG, 250, 1000, 0x3CE, 0x55E, -0xCD2, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x150), H2OFALL_BG, 360, 1000, 0x371, 0x566, -0xD58, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x190), H2OFALL_BG, 150, 500, 0x6F, 0x6FD, -0x1170, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x1D0), H2OFALL_BG, 360, 1950, -0x771, 0x8A8, -0x17BF, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x210), H2OFALL_BG, 360, 2000, -0x7B6, 0x799, -0x17C1, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x250), H2OFALL_BG, 360, 2050, -0x6A1, 0x94A, -0x17C0, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x290), H2OFALL_BG, 360, 1900, -0x599, 0x928, -0x17BE, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x2D0), H2OFALL_BG, 200, 800, 0x4F, 0x4FB, -0x10EA, 0);
        func_001C0B40(obj + 0x8C0, 18, 13, 150, 400, -1699.0f, 2095.0f, -6033.0f);
    } else if (D_004B2840[0] == 3) {
        func_001DB4C0((SoundEmitter *)(obj + 0x310), H2OFALL_BG, 360, 2000, 0x608, 0x1232, -0x533B, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x350), H2OFALL_BG, 360, 2000, 0x6BA, 0x12F9, -0x5339, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x390), H2OFALL_BG, 360, 2000, 0x7AA, 0x1334, -0x5339, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x3D0), H2OFALL_BG, 360, 2000, 0x86F, 0x12A8, -0x533E, 0);
        func_001C0DE0(obj + 0x6B0, 0x655, 5);
        func_001C0B40(obj + 0x8C0, 18, 13, 150, 400, 1864.0f, 4641.0f, -21240.0f);
    } else if (D_004B2840[0] == 9) {
        func_001DB4C0((SoundEmitter *)(obj + 0x310), H2OFALL_BG, 360, 2000, 0x455, 0x871, -0x8F3, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x350), H2OFALL_BG, 360, 2000, 0x4F5, 0x78B, -0x8F0, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x390), H2OFALL_BG, 360, 2000, 0x4E7, 0x684, -0x8F2, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x3D0), H2OFALL_BG, 360, 2000, 0x421, 0x5D2, -0x8F0, 0);
    }

    x700 = obj + 0x700;
    x770 = obj + 0x770;
    x7E0 = obj + 0x7E0;

    func_001D2C70(obj + 0x410, 0x64B, WIND_LGT);
    func_001D2C70(obj + 0x440, 0x64C, WIND_HVY);
    func_001D2C70(obj + 0x470, 0x647, BIRDS_LGT);
    func_001D2C70(obj + 0x4A0, 0x648, BIRDS_HVY);
    func_001D2C70(obj + 0x560, 0x645, WIND_LCAVE);
    func_001D2C70(obj + 0x650, 0x655, CITY);
    func_001D2C70(obj + 0x590, 0x658, FOREST_LEAF);
    func_001D2C70(obj + 0x5C0, 0x650, WIND_WCAVE);
    func_001D2C70(obj + 0x5F0, 0x65C, FOREST_DNSE);
    func_001D2C70(obj + 0x4D0, 0x653, WIND_BRDG);
    func_001D2C70(obj + 0x530, 0x652, STONE_STRC);

    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 25);
    func_001D4818(state, 0x65E, 25);
    func_001D4818(state, 0x65F, 10);
    func_001D2C70(obj + 0x6D0, 0x64D, FOLIAGE);
    func_001D1C08(x700, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x770, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x7E0, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
}
