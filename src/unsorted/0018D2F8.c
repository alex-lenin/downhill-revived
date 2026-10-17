/* UNIT 0x0018D2F8 0x0018D338 */
typedef struct Game {
    char pad0[0x40C0];
    char x40C0[0x4C0];
    char x4580[0x330];
    int x48B0;
} Game;

typedef struct Obj {
    Game *x0;
} Obj;

extern void func_001ACFA0(void *p, int a);
extern void func_00136030(void *p, int a);

void func_0018D2F8(Obj *obj)
{
    Game *game = obj->x0;

    func_001ACFA0(game->x4580, 1);
    game->x48B0 = 1;
    func_00136030(game->x40C0, 0);
}
