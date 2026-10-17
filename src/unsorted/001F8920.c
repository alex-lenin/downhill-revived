/* UNIT 0x001F8920 0x001F8A08 */
struct Game {
    char pad0[0x10C];
    int x10C;
};

struct Race {
    char pad0[0x4A8];
    int x4A8;
    char pad4AC[0x54];
    int x500;
    int x504;
    int x508;
    int x50C;
};

extern struct Game D_003F52D0;

extern void func_001F84A8(struct Race *race);
extern void func_0018E468(struct Race *race);
extern void func_00190358(struct Game *game);
extern void func_0018E098(struct Race *race);
extern void func_001F8AC8(struct Race *race);
extern void func_0018DCE8(struct Race *race);
extern void func_0018E1C0(struct Race *race);

void func_001F8920(struct Race *race)
{
    float ratio;

    race->x4A8 = 5;
    ratio = (float)race->x500 / (float)race->x504;
    func_001F84A8(race);
    if (race->x508 < 0 && ratio == 0.0f && race->x50C == 0) {
        func_0018E468(race);
        return;
    }
    if (race->x508 > 0 && ratio >= 1.0f) {
        D_003F52D0.x10C = 15;
        func_00190358(&D_003F52D0);
        func_0018E098(race);
        func_001F8AC8(race);
        func_0018DCE8(race);
        func_0018E1C0(race);
    }
}
