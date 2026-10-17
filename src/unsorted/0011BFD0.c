/* UNIT 0x0011BFD0 0x0011C048 */
typedef struct Item {
    char pad0[0x4ED0];
    char x4ED0[0x7BD0 - 0x4ED0];
} Item;

typedef struct Game {
    Item items[10];
    char pad4D620[0x501A8 - 0x4D620];
    int count;
} Game;

extern void func_0012FE38(Item *item, void *sub);

void func_0011BFD0(Game *game)
{
    int i;

    for (i = 0; i < game->count; i++) {
        func_0012FE38(&game->items[i], game->items[i].x4ED0);
    }
}
