/* UNIT 0x0011C0D0 0x0011C168 */
#include "rider_queries.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec4 pos;
} Body;

typedef struct Rider {
    char pad0[0x792C];
    Body *x792C;
    char pad7930[0x7A5C - 0x7930];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Rider;

typedef struct World {
    Rider riders[10];
    char pad[0x501A8 - 10 * 0x7BD0];
    int count;
} World;

/* Nonzero if any rider (skipping state-3 riders when skip is set) is within radius of pos. */
#if defined(__mips__)
int func_0011C0D0(World *world, Vec4 *pos, int skip, float radius)
#else
int SharedCall0011C0D0(struct Shared0011C0D0_World * world_view, struct Vector4 * pos_view, int skip, float radius)
#endif
{
#if !defined(__mips__)
    World * world = (World *)world_view;
    Vec4 * pos = (Vec4 *)pos_view;
#endif

    Vec4 diff;
    float distSq;
    Rider *rider;
    int i;

    radius = radius * radius;
    rider = world->riders;
    for (i = 0; i < world->count; i++, rider++) {
        if (skip == 0 || rider->x7A5C != 3) {
            __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                              : "=m"(diff) : "r"(pos), "r"(&rider->x792C->pos));
            __asm__ ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3"
                     : "=f"(distSq) : "f"(diff.x), "f"(diff.y), "f"(diff.z));
            if (distSq < radius) {
                return 1;
            }
        }
    }
    return 0;
}
