/* UNIT 0x0011A438 0x0011A548 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: retail recomputes &riders[i] with mult for the x7A5C==1 block and keeps an index counter; this walks one pointer (smaller, same loads/stores/compares);
   best diff 65/67 words */
#include "rider.h"

/* Race manager: 10 riders followed by per-frame height bounds (see 0011A100). */
typedef struct Race {
    Rider riders[10];
    char pad4D620[0x50190 - 0x4D620];
    float minHeight;           /* 0x50190: lowest z of all active riders */
    float maxHeight;           /* 0x50194: highest z of all active riders */
    float minPlayerHeight;     /* 0x50198: lowest z among riders with x7A5C == 1 */
    float maxPlayerHeight;     /* 0x5019C: highest z among riders with x7A5C == 1 */
    Rider *lowestPlayer;       /* 0x501A0 */
    Rider *highestPlayer;      /* 0x501A4 */
} Race;

extern float D_0029C140[]; /* FLT_MAX */
extern float D_0029C144[]; /* -FLT_MAX */

/* Recomputes the race's min/max rider heights (bodyNode->position.z). */
void func_0011A438(Race *race)
{
    float lo = D_0029C140[0];
    float hi = D_0029C144[0];
    int i;

    race->maxPlayerHeight = hi;
    race->minPlayerHeight = lo;
    race->minHeight = lo;
    race->maxHeight = hi;

    for (i = 0; i < 10; i++) {
        Rider *rider = &race->riders[i];

        if (rider->x7A5C != 0) {
            if (rider->bodyNode->position.z < race->minHeight) {
                race->minHeight = rider->bodyNode->position.z;
            }
            if (race->maxHeight < rider->bodyNode->position.z) {
                race->maxHeight = rider->bodyNode->position.z;
            }
        }

        if (rider->x7A5C == 1) {
            if (rider->bodyNode->position.z < race->minPlayerHeight) {
                race->minPlayerHeight = rider->bodyNode->position.z;
                race->lowestPlayer = rider;
            }
            if (race->maxPlayerHeight < rider->bodyNode->position.z) {
                race->maxPlayerHeight = rider->bodyNode->position.z;
                race->highestPlayer = rider;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0011A438);
#endif
