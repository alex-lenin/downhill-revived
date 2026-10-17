/* UNIT 0x0017BD80 0x0017BE38 */
typedef struct Player {
    char pad0[0x1380];
} Player;

typedef struct Race {
    char pad0[0x144];
    int seconds;
    char pad148[0x150 - 0x148];
    Player players[1];
} Race;

typedef struct Game {
    char pad0[0x288];
    int playerCount;
} Game;

extern Game D_004B25A0;
extern void func_0017BEF0(Player *, int);
extern void func_001C3428(void);

void func_0017BD80(Race *race, int time) {
    int i;
    int seconds;

    for (i = 0; i < D_004B25A0.playerCount; i++) {
        func_0017BEF0(&race->players[i], time);
    }
    seconds = time / 60;
    if (seconds < 4 && seconds != race->seconds) {
        func_001C3428();
    }
    race->seconds = seconds;
}
