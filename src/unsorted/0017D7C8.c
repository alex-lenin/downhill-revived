/* UNIT 0x0017D7C8 0x0017DC10 */
struct Game {
    char pad0[0x7978];
    int x7978;
    char pad797C[0x7A5C - 0x797C];
    int x7A5C;
};

struct Obj {
    struct Game *game;
};

struct GameData {
    char pad0[0x11A6C];
    unsigned char x11A6C[1];
};

extern struct GameData D_004225A0;
extern void func_0017DC10(struct Obj *obj, int id, int a, int b, int c, int d);

void func_0017D7C8(struct Obj *obj)
{
    int mode = 0;

    if (obj->game != 0 && obj->game->x7A5C == 1) {
        mode = D_004225A0.x11A6C[obj->game->x7978];
    }

    switch (mode) {
    case 1:
        func_0017DC10(obj, 4, 0x8000, 0, 0, 0);
        func_0017DC10(obj, 0xB, 0x2000, 0, 0, 0);
        func_0017DC10(obj, 0xC, 0x2000, 0, 0, 0);
        func_0017DC10(obj, 0xD, 0x200, 0, 0, 0);
        func_0017DC10(obj, 0xF, 0x80, 0x80, 0, 0);
        func_0017DC10(obj, 0x10, 0x20, 0x20, 0, 0);
        break;
    case 2:
        func_0017DC10(obj, 4, 0x1000, 0, 0, 0);
        func_0017DC10(obj, 0xB, 0x2000, 0, 0, 0);
        func_0017DC10(obj, 0xC, 0x2000, 0, 0, 0);
        func_0017DC10(obj, 0xD, 0x800, 0, 0, 0);
        func_0017DC10(obj, 0xF, 0x8000, 0, 0, 0);
        func_0017DC10(obj, 0x10, 0x8000, 0, 0, 0);
        break;
    case 0:
    default:
        func_0017DC10(obj, 4, 0x1000, 0, 0, 0);
        func_0017DC10(obj, 0xB, 0x8000, 0, 0, 0);
        func_0017DC10(obj, 0xC, 0x2000, 0, 0, 0);
        func_0017DC10(obj, 0xD, 0x200, 0, 0, 0);
        func_0017DC10(obj, 0xF, 0x80, 0x80, 0, 0);
        func_0017DC10(obj, 0x10, 0x20, 0x20, 0, 0);
        break;
    }
    func_0017DC10(obj, 1, 8, 0, 0, 0);
    func_0017DC10(obj, 2, 1, 0, 0, 0);
    func_0017DC10(obj, 3, 0x4000, 0, 0, 0);
    func_0017DC10(obj, 7, 0x20, 0, 0, 0);
    func_0017DC10(obj, 8, 0x80, 0, 0, 0);
    func_0017DC10(obj, 9, 0x40, 0, 0, 0);
    func_0017DC10(obj, 0xA, 0x10, 0, 0, 0);
    func_0017DC10(obj, 5, 0x4000, 0x4000, 0, 0);
    func_0017DC10(obj, 6, 4, 0, 0, 0);
    func_0017DC10(obj, 0x11, 2, 0, 0, 0);
    func_0017DC10(obj, 0x12, 0x100, 0, 0, 0);
    func_0017DC10(obj, 0x13, 0x1000, 0, 0, 0);
    func_0017DC10(obj, 0x14, 1, 1, 0, 0);
    func_0017DC10(obj, 0x15, 0x40, 0x40, 0, 0);
    func_0017DC10(obj, 0x16, 0x1000, 0, 0, 0);
    func_0017DC10(obj, 0x17, 0x10, 0x10, 0, 0);
    func_0017DC10(obj, 0x18, 0x200, 0, 0, 0);
}
