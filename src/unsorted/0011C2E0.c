/* UNIT 0x0011C2E0 0x0011C378 */
typedef struct Item { char pad[0x7BD0]; } Item;

typedef struct Game {
    Item items[10];
    char pad4D620[0x501A8 - 0x4D620];
    int count;
} Game;

typedef struct GlobalState {
    char pad0[0x488];
    int mode;
} GlobalState;

extern GlobalState D_0043EEC0;
extern char D_0032FC60[];
extern void func_00138E08(void *, Item *);

void func_0011C2E0(Game *game)
{
    int i;

    for (i = 0; i < game->count; i++) {
        if (D_0043EEC0.mode >= 2 && D_0043EEC0.mode <= 4) {
            func_00138E08(D_0032FC60, &game->items[i]);
        }
    }
}
