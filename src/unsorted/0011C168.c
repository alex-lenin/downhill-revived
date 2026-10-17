/* UNIT 0x0011C168 0x0011C208 */
#include "rider_queries.h"
#if defined(__mips__)
#define ReturnShared0011C168(value) (value)
#else
#define ReturnShared0011C168(value) ((struct Rider *)(value))
#endif
typedef struct Body {
    char pad0[0x10];
    float pos[4];
} Body;

typedef struct Player {
    char pad0[0x792C];
    Body *x792C;
    char pad7930[0x12C];
    int x7A5C;
    char pad7A60[0x170];
} Player;

typedef struct Game {
    Player players[10];
    char pad4D620[0x2B88];
    int count;
} Game;

/* Returns the last active player within maxDist of pos (distance written to outDist). */
#if defined(__mips__)
Player *func_0011C168(Game *game, float *pos, float *outDist, float maxDist)
#else
struct Rider * SharedCall0011C168(struct Shared0011C168_Game * game_view, float * pos, float * outDist, float maxDist)
#endif
{
#if !defined(__mips__)
    Game * game = (Game *)game_view;
#endif

    Player *found = 0;
    Player *player;
    float v[4] __attribute__((aligned(16)));
    float dist;
    int i;

    for (i = 0; i < game->count; i++) {
        player = &game->players[i];
        if (player->x7A5C == 1) {
            __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v), "r"(pos), "r"(player->x792C->pos) : "memory");
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(dist) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
            if (dist < maxDist) {
                found = player;
                if (outDist != 0) {
                    *outDist = dist;
                }
            }
        }
    }
    return ReturnShared0011C168(found);
}
