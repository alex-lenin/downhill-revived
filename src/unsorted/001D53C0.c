/* UNIT 0x001D53C0 0x001D58F0 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

/* names packed little-endian into 128-bit values */
#define H2OFALL_BIG (((u128)0x4C5F4749425F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define H2OFALL_SML (((u128)0x4C5F4C4D535F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define H2OFALL_TDH (((u128)0x4C5F4844545F4CLL << 64) | (u128)0x4C41464F32485F45LL)
#define WIND_LCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x4C5F444E49575F45LL)
#define WIND_SCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x535F444E49575F45LL)
#define WIND_LGT (((u128)0x4C5F5447LL << 64) | (u128)0x4C5F444E49575F45LL)
#define WIND_HVY (((u128)0x4C5F5956LL << 64) | (u128)0x485F444E49575F45LL)
#define WIND_BRDG (((u128)0x4C5F474452LL << 64) | (u128)0x425F444E49575F45LL)
#define WIND_WCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x575F444E49575F45LL)
#define BIRDS_LGT (((u128)0x4C5F54474CLL << 64) | (u128)0x5F53445249425F45LL)
#define BIRDS_HVY (((u128)0x4C5F595648LL << 64) | (u128)0x5F53445249425F45LL)
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

void func_001D53C0(char *obj)
{
    char *state;
    char *x2C0;
    char *x330;
    char *x3A0;

    func_001D4C48(obj);
    if (D_004B2840[0] == 1) {
        func_001DB4C0((SoundEmitter *)(obj + 0x10), H2OFALL_BIG, 300, 2000, -0xAA3, -0x1074, -0xD48, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x50), H2OFALL_SML, 300, 2000, -0x788, -0xF6D, -0xA8B, 0);
        func_001DB4C0((SoundEmitter *)(obj + 0x90), H2OFALL_BIG, 360, 2000, -0xB45, -0x1205, -0x1078, 0);
        func_001C0B40(obj + 0x410, 18, 13, 150, 400, -1286.0f, -1621.0f, -15083.0f);
    } else if (D_004B2840[0] == 13) {
        func_001DB4C0((SoundEmitter *)(obj + 0x50), H2OFALL_TDH, 150, 600, -0x48A, 0x303, -0x7DE, 0);
        func_001C0B40(obj + 0x410, 18, 13, 150, 400, -1290.0f, -1624.0f, -15068.0f);
    }

    x2C0 = obj + 0x2C0;
    x330 = obj + 0x330;
    x3A0 = obj + 0x3A0;

    func_001D2C70(obj + 0x1A0, 0x645, WIND_LCAVE);
    func_001D2C70(obj + 0x1D0, 0x64A, WIND_SCAVE);
    func_001D2C70(obj + 0x110, 0x64B, WIND_LGT);
    func_001D2C70(obj + 0x140, 0x64C, WIND_HVY);
    func_001D2C70(obj + 0x170, 0x653, WIND_BRDG);
    func_001D2C70(obj + 0x260, 0x650, WIND_WCAVE);
    func_001D2C70(obj + 0x200, 0x647, BIRDS_LGT);
    func_001D2C70(obj + 0x230, 0x648, BIRDS_HVY);

    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 27);
    func_001D4818(state, 0x65E, 10);
    func_001D4818(state, 0x65F, 10);
    func_001D2C70(obj + 0x290, 0x64D, FOLIAGE);
    func_001D1C08(x2C0, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x330, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x3A0, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
}
