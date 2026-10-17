/* UNIT 0x001907D0 0x00190890 */
/* gcc 2.95 vtable entry: this-adjustment, then function */
typedef struct VEntry {
    short delta;
    short pad2;
    void (*func)(void *);
} VEntry;

typedef struct Player {
    char pad0[0x4AC];
    int x4AC;
    char pad4B0[0x4B8 - 0x4B0];
    unsigned char x4B8;
    char pad4B9[0x4F8 - 0x4B9];
    VEntry *vtbl;
} Player;

typedef struct StackEntry {
    Player *x0;
    Player *x4;
} StackEntry;

typedef struct Menu {
    char pad0[0x150];
    Player *x150;
    StackEntry *stack[13];
    int depth;
} Menu;

extern void func_00190410(Menu *menu, int arg);
extern void func_001904B8(Menu *menu, int arg, Player *player);

void func_001907D0(Menu *menu, int force)
{
    Player *player;
    VEntry *entry;

    player = menu->x150;
    entry = &player->vtbl[2];
    entry->func((char *)player + entry->delta);
    do {
        if (--menu->depth < 0 || !menu->stack[menu->depth] || !menu->stack[menu->depth]->x0) {
            func_00190410(menu, 1);
            return;
        }
    } while (menu->stack[menu->depth]->x0->x4B8 && !force);
    func_001904B8(menu, menu->stack[menu->depth]->x4->x4AC, menu->stack[menu->depth]->x4);
}
