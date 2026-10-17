/* UNIT 0x0011A050 0x0011A100 */
typedef struct Progress {
    float x0;
    float x4;
} Progress;

typedef struct Rider {
    char pad0[0x2E94];
    Progress x2E94;
    char pad2E9C[0x4BCC];
    unsigned char x7A68;
} Rider;

typedef struct Game {
    char pad0[0x50140];
    Rider *riders[26];
    int count;
    char pad501AC[0x10];
    int leader;
    int x501C0;
} Game;

extern int D_004B2828[];

void func_0011A050(Game *game)
{
    Rider *rider;
    Progress *progress;
    float best;
    int count;
    int i;

    game->leader++;
    count = game->count - D_004B2828[0];
    if (game->leader >= count) {
        game->leader = 0;
    }
    best = 0.0f;
    for (i = 0; i < count; i++) {
        rider = game->riders[i];
        if (rider != 0 && rider->x7A68 == 0) {
            progress = &rider->x2E94;
            if (best < progress->x4) {
                game->leader = i;
                best = progress->x4;
            }
        }
    }
    game->x501C0 = 0x1A4;
}
