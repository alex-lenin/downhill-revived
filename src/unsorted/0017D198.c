/* UNIT 0x0017D198 0x0017D1D8 */
struct Game {
    char pad0[0x40C0];
    char x40C0[0x4580 - 0x40C0];
    char x4580[0x48B0 - 0x4580];
    int x48B0;
};

struct Obj {
    struct Game *game;
};

extern void func_001ACFA0(void *p, int arg);
extern void func_00136030(void *p, int arg);

void func_0017D198(struct Obj *obj)
{
    struct Game *game = obj->game;
    func_001ACFA0(game->x4580, 0);
    game->x48B0 = 0;
    func_00136030(game->x40C0, 1);
}
