/* UNIT 0x001DAEE8 0x001DB280 */
typedef unsigned int u128 __attribute__((mode(TI)));

#define WIND_LGT (((u128)0x4C5F5447LL << 64) | (u128)0x4C5F444E49575F45LL)
#define WIND_HVY (((u128)0x4C5F5956LL << 64) | (u128)0x485F444E49575F45LL)
#define BIRDS_LGT (((u128)0x4C5F54474CLL << 64) | (u128)0x5F53445249425F45LL)
#define BIRDS_HVY (((u128)0x4C5F595648LL << 64) | (u128)0x5F53445249425F45LL)
#define WIND_SCAVE (((u128)0x4C5F45564143LL << 64) | (u128)0x535F444E49575F45LL)
#define FOREST_PINE (((u128)0x4C5F454E49505FLL << 64) | (u128)0x545345524F465F45LL)
#define FOREST_DENS (((u128)0x4C5F534E45445FLL << 64) | (u128)0x545345524F465F45LL)
#define GOLF_L ((u128)0x4C5F464C4F475F45LL)
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

void func_001DAEE8(char *obj)
{
    char *state;
    char *x1C0;
    char *x230;
    char *x2A0;

    func_001D4C48(obj);
    if (D_004B2840[0] == 19) {
        func_001C0B40(obj + 0x310, 18, 13, 150, 400, 88.0f, -11081.0f, -11930.0f);
    }

    x1C0 = obj + 0x1C0;
    x230 = obj + 0x230;
    x2A0 = obj + 0x2A0;

    func_001D2C70(obj + 0x10, 0x64B, WIND_LGT);
    func_001D2C70(obj + 0x40, 0x64C, WIND_HVY);
    func_001D2C70(obj + 0x70, 0x647, BIRDS_LGT);
    func_001D2C70(obj + 0xA0, 0x648, BIRDS_HVY);
    func_001D2C70(obj + 0xD0, 0x64A, WIND_SCAVE);
    func_001D2C70(obj + 0x100, 0x65A, FOREST_PINE);
    func_001D2C70(obj + 0x130, 0x65C, FOREST_DENS);
    func_001D2C70(obj + 0x160, 0x66C, GOLF_L);
    func_001D2C70(obj + 0x190, 0x64D, FOLIAGE);

    state = D_00398C40;
    state += 0x240;
    func_001D4818(state, 0x65D, 40);
    func_001D4818(state, 0x65E, 30);
    func_001D4818(state, 0x65F, 15);
    func_001D1C08(x1C0, 0x65D, func_001D4850(state, 0x65D), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x230, 0x65E, func_001D4850(state, 0x65E), TREE_WHOOSH, FOLIAGE);
    func_001D1C08(x2A0, 0x65F, func_001D4850(state, 0x65F), TREE_WHOOSH, FOLIAGE);
}
