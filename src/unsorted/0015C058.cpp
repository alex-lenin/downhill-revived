/* UNIT 0x0015C058 0x0015C118 .rodata=0x28A370:0x28A3A0 */
class Item {
public:
    int x0;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual int v7();
};

struct Game {
    char pad0[0x4ECC];
    Item *x4ECC;
};

class Screen {
public:
    Game *game;
    int func_0015C058();
};

int Screen::func_0015C058()
{
    int ids[11] = { 1, 5, 6, 9, 10, 11, 12, 13, 14, 15, 16 };
    int i;

    for (i = 10; i >= 0; i--) {
        Item *item = game->x4ECC;
        item->v7();
    }
    return 0;
}
