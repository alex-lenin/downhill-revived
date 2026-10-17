/* UNIT 0x00194538 0x001945D8 */
typedef struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x44 - 0x33];
    int x44;
} Entry;

typedef struct Rider {
    char pad0[0x694];
} Rider;

typedef struct Game {
    Rider riders[18];
} Game;

extern void func_001952A0(Entry *);
extern void func_00194C00(Rider *, int);

void func_00194538(Game *game, int index)
{
    int i;
    int a, b, c;

    i = index - 4;
    a = i * 0x50;
    a += (int)game;
    a += 0x11A88;
    b = i * 0x50;
    b += (int)game;
    b += 0x11A88;
    c = i * 0x50;
    c += 0x11A88;
    ((Entry *)a)->x32 = 0;
    ((Entry *)b)->x44 = 0;
    func_001952A0((Entry *)((int)game + c));
    for (i = 0; i < 18; i++) {
        func_00194C00(&game->riders[i], index);
    }
}
