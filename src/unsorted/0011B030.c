/* UNIT 0x0011B030 0x0011B180 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Mtx {
    u128 row[4];
} Mtx;

typedef struct Pose {
    char pad0[0x10];
    u128 pos;
    char pad20[0x20];
    Mtx rot;
} Pose;

/* the part of a Pose that gets swapped */
typedef struct Saved {
    u128 pos;
    Mtx rot;
} Saved;

typedef struct Rider {
    char pad0[0x40C0];
    char body[0x386C];
    Pose *x792C;
} Rider;

typedef struct Game {
    char pad0[0x50164];
    Rider *riders[1];
} Game;

typedef struct Global {
    char pad0[0x288];
    int x288;
    char pad28C[0xCC];
    int x358;
} Global;

extern Global D_004B25A0;
extern void func_00231A08(void *body, u128 *pos);
extern void func_00231A78(void *body, Mtx *rot);
extern void func_0012DDC0(Rider *rider);

/* Swaps the poses of two riders and their slots in game->riders. */
void func_0011B030(Game *game, Rider *a, Rider *b)
{
    Saved saved;
    Pose *pose;
    int i;
    int ia;
    int ib;
    int count;

    if (a == b) {
        return;
    }
    pose = a->x792C;
    /* vector copy (as sceVu0CopyVector) */
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(saved.pos) : "r"(&pose->pos) : "$2");
    /* 64-byte matrix copy (as sceVu0CopyMatrix) */
    __asm__ volatile (
        "lq $8, 0x0(%4)\n\t"
        "lq $9, 0x10(%4)\n\t"
        "lq $10, 0x20(%4)\n\t"
        "lq $11, 0x30(%4)\n\t"
        "sq $8, %0\n\t"
        "sq $9, %1\n\t"
        "sq $10, %2\n\t"
        "sq $11, %3"
        : "=m"(saved.rot.row[0]), "=m"(saved.rot.row[1]), "=m"(saved.rot.row[2]), "=m"(saved.rot.row[3])
        : "r"(&pose->rot)
        : "$8", "$9", "$10", "$11");
    func_00231A08(a->body, &b->x792C->pos);
    func_00231A78(a->body, &b->x792C->rot);
    func_0012DDC0(a);
    func_00231A08(b->body, &saved.pos);
    func_00231A78(b->body, &saved.rot);
    func_0012DDC0(b);

    ia = -1;
    ib = -1;
    count = D_004B25A0.x288 + D_004B25A0.x358;
    for (i = 0; i < count; i++) {
        if (game->riders[i] == a) {
            ia = i;
        } else if (game->riders[i] == b) {
            ib = i;
        }
    }
    game->riders[ia] = b;
    game->riders[ib] = a;
}
