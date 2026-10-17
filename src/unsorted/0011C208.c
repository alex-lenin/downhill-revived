/* UNIT 0x0011C208 0x0011C2E0 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec x10;
} Body;

typedef struct Rider {
    char pad0[0x792C];
    Body *x792C;
    char pad7930[0x12C];
    int x7A5C;
    char pad7A60[0x170];
} Rider;

typedef struct World {
    Rider riders[10];
    char pad4D620[0x2B88];
    int count;
} World;

extern float D_0029C158[];

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline float VecDot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile (
        "mula.s %1, %2\n"
        "madda.s %3, %4\n"
        "madd.s %0, %5, %6\n"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

/* Finds the nearest rider of type 3 and of type 1 to pos; *dist = the smaller distance squared. */
void func_0011C208(World *world, Vec *pos, Rider **nearest3, Rider **nearest1, float *dist)
{
    Vec d;
    Rider *rider;
    float best1;
    float best3;
    float len;
    int i;

    best1 = D_0029C158[0];
    best3 = best1;
    rider = world->riders;
    for (i = 0; i < world->count; i++) {
        VecSub(&d, pos, &rider->x792C->x10);
        len = VecDot(&d, &d);
        if (len < best1 && rider->x7A5C == 1) {
            *nearest1 = rider;
            best1 = len;
        }
        if (len < best3 && rider->x7A5C == 3) {
            *nearest3 = rider;
            best3 = len;
        }
        rider++;
    }
    if (best3 < best1) {
        *dist = best3;
    } else {
        *dist = best1;
    }
}
