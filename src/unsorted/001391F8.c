/* UNIT 0x001391F8 0x00139308 */
struct State {
    char pad0[0x4ECC];
    void *x4ECC;
    char x4ED0[0x10];
    char x4EE0[0x5FF0 - 0x4EE0];
    char x5FF0[0x6210 - 0x5FF0];
    char x6210[0xC];
    char x621C[0x7A78 - 0x621C];
    float x7A78;
};
struct Game {
    char pad0[0x31708];
    struct State *x31708;
};

extern float func_0012DF48(struct State *);
extern int func_00138560(void *);

void func_001391F8(struct Game *game, int *mode)
{
    struct State *state = game->x31708;
    void *cur = state->x4ECC;

    if (cur == state->x4ED0) *mode = 3;
    else if (cur == state->x621C) *mode = 0;
    else if (cur == state->x6210) *mode = 2;
    else if (cur == state->x5FF0) *mode = 1;
    else if (cur == state->x4EE0) *mode = 6;

    if (*mode == 3) {
        if (game->x31708->x7A78 != 0.0f) *mode = 4;
        else if (func_0012DF48(game->x31708) != 0.0f) *mode = 5;
    }
    if ((unsigned char)func_00138560((char *)game->x31708 + 0x40C0)) *mode = 7;
}
