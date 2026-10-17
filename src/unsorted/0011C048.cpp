/* UNIT 0x0011C048 0x0011C0D0 */
extern "C" void func_00111D98(void *p, void *other);

struct Item {
    char pad0[0x78D4];
    void *x78D4;
    char pad78D8[0x7BD0 - 0x78D8];

    void Update(void *other)
    {
        if (x78D4)
            func_00111D98(x78D4, other);
    }
};

struct Game {
    Item items[10];
    char pad4D620[0x501A8 - 0x4D620];
    int count;
};

extern "C" void func_0011C048(Game *game, void *other)
{
    int i;

    if (other) {
        for (i = 0; i < game->count; i++) {
            game->items[i].Update(other);
        }
    }
}
