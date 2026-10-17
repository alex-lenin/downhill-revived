/* UNIT 0x0018A868 0x0018A9C0 */
#include "menu_items.h"
/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry {
    short delta;
    short pad2;
    void (*func)();
} VEntry;

typedef struct Player {
    char pad0[0x4F8];
    VEntry *vtbl;
    char pad4FC[0x610 - 0x4FC];
} Player;

typedef struct PlayerHolder {
    int x0;
    void *x4;
    char pad8[0x8];
    Player player;
} PlayerHolder;

typedef struct Item {
    char pad0[0x1C];
    short x1C;
} Item;

typedef struct Pair {
    int x0;
    char *x4;
} Pair;

extern char D_003E3BD0[];
extern char D_00286038[];
extern char D_003EB760[];
extern unsigned char D_003E6E40[];
extern char D_003E5BF0[];

extern PlayerHolder *func_00184940(PlayerHolder *holder);
extern int func_0021F818(void);
extern void func_00187048(void *p, void *msg);
extern void func_001870E0(void *p, void *state);
extern void func_0018A5A8(void *menu, int value);
extern void func_00187438(Player *p);

void func_0018A868(Player *self, int index)
{
    Item *item = (Item *)(index * 16 + (unsigned int)self);
    int matches;
    PlayerHolder holder;
    Pair pair;
    Player *player;

    matches = item->x1C == 0x1F;
    if (matches) {
        self->vtbl[8].func((char *)self + self->vtbl[8].delta, item);
        return;
    }
    func_00184940(&holder);
    player = &holder.player;
    player->vtbl[14].func((char *)player + player->vtbl[14].delta);
    func_001873C0((struct SharedMenu *)(void *)(player), 0x20, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0xC, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), func_0021F818() + 0x10, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0xE, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0xC, 0);
    func_001873C0((struct SharedMenu *)(void *)(player), 0xF, 0);
    func_00187048(D_003E3BD0, &holder);
    pair.x0 = 0;
    pair.x4 = D_00286038;
    func_00187048(D_003E3BD0, &pair);
    func_001870E0(D_003E3BD0, D_003EB760);
    D_003E6E40[0] = 0;
    func_0018A5A8(D_003E5BF0, 0);
    func_00187438(self);
}
