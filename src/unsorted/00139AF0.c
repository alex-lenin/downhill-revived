/* UNIT 0x00139AF0 0x00139B90 */
typedef struct Big { int x0; char pad4[0x2BE0 - 4]; } __attribute__((aligned(16))) Big;
typedef struct Small { int x0; char pad4[0x1600 - 4]; } __attribute__((aligned(16))) Small;

struct Game {
    char pad0[0x10];
    union {
        Big big[1];
        struct { char pad[0xAF00]; Small small[1]; } s;
    } u;
    char padC510[0x316E0 - 0xC510];
    Big *x316E0;
    char pad316E4[0x31760 - 0x316E4];
    int x31760;
};

void func_00139AF0(struct Game *game)
{
    Big *src = game->x316E0;
    int idx;

    if (src->x0 == 0) return;
    idx = game->x31760;
    if (idx < 9) game->u.big[idx] = *src;
    else game->u.s.small[idx] = *(Small *)src;
}
