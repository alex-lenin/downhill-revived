/* UNIT 0x001D8618 0x001D8CA0 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

/* names packed little-endian into 128-bit values */
#define HOT_STEAM (((u128)0x4C5F4D4145LL << 64) | (u128)0x54535F544F485F45LL)
#define OCEAN (((u128)0x4CLL << 64) | (u128)0x5F4E4145434F5F45LL)
#define H2OFALL_BIG (((u128)0x4C5F4749425F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define WIND_LGT (((u128)0x4C5F5447LL << 64) | (u128)0x4C5F444E49575F45LL)
#define WIND_HVY (((u128)0x4C5F5956LL << 64) | (u128)0x485F444E49575F45LL)
#define WIND_STRUC (((u128)0x4C5F43555254LL << 64) | (u128)0x535F444E49575F45LL)
#define WIND_LCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x4C5F444E49575F45LL)
#define FOREST_LEAF (((u128)0x4C5F4641454C5FLL << 64) | (u128)0x545345524F465F45LL)
#define FOREST_PINE (((u128)0x4C5F454E49505FLL << 64) | (u128)0x545345524F465F45LL)
#define FOREST_DNSE (((u128)0x4C5F45534E445FLL << 64) | (u128)0x545345524F465F45LL)
#define PARKING_LOT (((u128)0x4C5F544F4C5F47LL << 64) | (u128)0x4E494B5241505F45LL)
#define HOT_SPRING (((u128)0x4C5F474E4952LL << 64) | (u128)0x50535F544F485F45LL)
#define LOBBY (((u128)0x4CLL << 64) | (u128)0x5F5942424F4C5F45LL)
#define FOLIAGE (((u128)0x45LL << 64) | (u128)0x4741494C4F465F45LL)
#define TREE_WHOOSH (((u128)0x48534F4F48LL << 64) | (u128)0x575F454552545F45LL)

extern void func_001C0B40(void *, int, int, int, int, float, float, float);
extern void func_001D1C08(void *, int, int, u128, u128);
extern void func_001D2C70(void *, int, u128);
extern void func_001D4818(void *, int, int);
extern int func_001D4850(void *, int);
extern void func_001D4C48(void *);
extern char D_00398C40[];
extern int D_004B2840[];

void func_001D8618(char *obj)
{
    char *state;
    char *x4F0;
    char *x560;
    char *x5D0;

    func_001D4C48(obj);
    *(int *)(obj + 0x4B4) = 0;
    if (D_004B2840[0] == 12) {
        func_001DB4C0((SoundEmitter *)(obj + 0x1F0), HOT_STEAM, 100, 350, 0x1F8C, -0x1F4F, 0xDEF, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x230), HOT_STEAM, 100, 350, 0x1FE9, -0x1F4C, 0xDB2, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x270), HOT_STEAM, 100, 350, 0x20C8, -0x1ED0, 0xD55, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x2B0), HOT_STEAM, 100, 350, 0x211B, -0x1EDC, 0xD24, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x2F0), HOT_STEAM, 100, 350, 0x2214, -0x1F1A, 0xCAB, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x330), HOT_STEAM, 80, 250, 0x227A, -0x1F1F, 0xC70, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x370), HOT_STEAM, 80, 250, 0x234E, -0x1ED8, 0xC16, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x3B0), HOT_STEAM, 80, 250, 0x239B, -0x1ED8, 0xBDB, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x3F0), OCEAN, 1200, 1800, 0x33EC, -0x246C, 0, 0);
        *(int *)(obj + 0x4B0) = 9;
        func_001C0B40(obj + 0x640, 18, 13, 150, 400, 12420.0f, -9327.0f, 8.0f);
    } else if (D_004B2840[0] == 21) {
        func_001DB4C0((SoundEmitter *)(obj + 0x1F0), H2OFALL_BIG, 200, 500, -0xB3C, 0x97F, -0x7FE, 0);
        *(int *)(obj + 0x4B0) = 1;
        func_001C0B40(obj + 0x640, 18, 13, 150, 400, -3316.0f, 2959.0f, -2705.0f);
    }

    x4F0 = obj + 0x4F0;
    x560 = obj + 0x560;
    x5D0 = obj + 0x5D0;

    func_001D2C70(obj + 0x10, 0x64B, WIND_LGT);
    func_001D2C70(obj + 0x40, 0x64C, WIND_HVY);
    func_001D2C70(obj + 0x70, 0x660, WIND_STRUC);
    func_001D2C70(obj + 0xA0, 0x645, WIND_LCAVE);
    func_001D2C70(obj + 0xD0, 0x658, FOREST_LEAF);
    func_001D2C70(obj + 0x100, 0x65A, FOREST_PINE);
    func_001D2C70(obj + 0x130, 0x65C, FOREST_DNSE);
    func_001D2C70(obj + 0x160, 0x66A, PARKING_LOT);
    func_001D2C70(obj + 0x190, 0x66E, HOT_SPRING);
    func_001D2C70(obj + 0x1C0, 0x652, LOBBY);

    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 35);
    func_001D4818(state, 0x65E, 30);
    func_001D4818(state, 0x65F, 15);
    func_001D2C70(obj + 0x4C0, 0x64D, FOLIAGE);
    func_001D1C08(x4F0, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x560, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x5D0, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
}
