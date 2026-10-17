/* UNIT 0x001BE328 0x001BE3D8 */
typedef struct Global {
    char pad0[0x288];
    int x288;
} Global;

#include "rider.h"
typedef struct World {
    char pad0[0x50130];
    Rider *riders[1];
} World;

extern Global D_004B25A0;
extern World D_002AF2C0;

extern float func_00222330(void *pos, void *arg);

int func_001BE328(void *self, void *pos)
{
    int i;
    int dist;
    int best = 0x7FFF;

    for (i = 0; i < D_004B25A0.x288; i++) {
        dist = func_00222330(&D_002AF2C0.riders[i]->bodyNode->position, pos);
        if (dist < best) {
            best = dist;
        }
    }
    return best;
}
