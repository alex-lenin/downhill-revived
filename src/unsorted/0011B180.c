/* UNIT 0x0011B180 0x0011B268 .sdata=0x29C14C:0x29C150 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec x10;
} Body;

typedef struct Rider {
    char pad0[0x4180];
    Vec x4180;
    char pad4190[0x379C];
    Body *x792C;
    char pad7930[0x2A0];
} Rider;

typedef struct World {
    Rider riders[10];
    char pad4D620[0x2B88];
    int count;
} World;

extern float D_0029C150[];
float D_0029C14C = 0.2f;

/* Nearest rider (by a point slightly ahead of it) within maxZ height of pos. */
Rider *func_0011B180(World *world, Vec *pos, float *dist, Vec *outPos, float maxZ)
{
    Vec d;
    Vec p;
    Rider *best;
    float len;
    int n;
    int i;

    best = 0;
    n = world->count;
    if (dist != 0) {
        *dist = D_0029C150[0];
    }
    for (i = 0; i < n; i++) {
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "lqc2 $vf12, 0x0(%2)\n\t"
                          "mfc1 $2, %3\n\t"
                          "qmtc2.ni $2, $vf13\n\t"
                          "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                          "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(p) : "r"(&world->riders[i].x792C->x10), "r"(&world->riders[i].x4180), "f"(D_0029C14C) : "$2");
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "lqc2 $vf12, 0x0(%2)\n\t"
                          "vsub.xyz $vf11, $vf11, $vf12\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(d) : "r"(pos), "r"(&p));
        if (__builtin_fabsf(d.z) < maxZ) {
            __asm__ volatile ("mula.s %1, %2\n\t"
                              "madda.s %3, %4\n\t"
                              "madd.s %0, %5, %6"
                              : "=f"(len) : "f"(d.x), "f"(d.x), "f"(d.y), "f"(d.y), "f"(d.z), "f"(d.z));
            if (len < *dist) {
                best = &world->riders[i];
                if (dist != 0) {
                    *dist = len;
                }
                if (outPos != 0) {
                    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, 0x0(%0)" : : "r"(outPos), "r"(&p) : "$2", "memory");
                }
            }
        }
    }
    return best;
}
