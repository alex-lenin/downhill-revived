/* UNIT 0x00119FC8 0x0011A050 */
typedef struct Player {
    char pad0[0x7A5C];
    int x7A5C;
    char pad7A60[0x170];
} Player; /* 0x7BD0 bytes */

typedef struct Game {
    Player players[10];
    char pad4D620[0x2B9C];
    int x501BC;
} Game;

extern void func_0012BF88(Player *player);
extern int func_00222BD8(int min, int max);
extern void func_0011A050(Game *game);

void func_00119FC8(Game *game)
{
    Player *player;
    int i;

    for (i = 0; i < 10; i++) {
        player = &game->players[i];
        if (player->x7A5C != 0) {
            func_0012BF88(player);
        }
    }
    game->x501BC = func_00222BD8(0, 9);
    func_0011A050(game);
}
