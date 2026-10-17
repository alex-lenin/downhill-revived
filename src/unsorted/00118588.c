/* UNIT 0x00118588 0x00118640 */
typedef struct Player {
    char pad0[0x78D4];
    void *x78D4;
} Player;

typedef struct Global {
    char pad0[0x50140];
    Player *x50140;
} Global;

extern Global D_002AF2C0;

extern int func_00112158(void *obj);

/* Returns a frame count as minutes + seconds / 100 (e.g. 1:23 -> 1.23). */
float func_00118588(unsigned char *game)
{
    Player *player;
    int seconds;
    float time = 0.0f;

    if (game[0x1089F] != 0) {
        player = D_002AF2C0.x50140;
        if (player != 0 && player->x78D4 != 0) {
            seconds = func_00112158(player->x78D4) / 60;
            time = (float)(seconds / 60 % 60) + (float)(seconds % 60) * 0.01f;
        }
    }
    return time;
}
