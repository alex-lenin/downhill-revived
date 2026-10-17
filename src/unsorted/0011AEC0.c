/* UNIT 0x0011AEC0 0x0011B030 */
typedef struct Racer {
    char pad0[0x7BD0];
} Racer;

typedef struct World {
    Racer racers[10];
    char pad4D620[0x50130 - 0x4D620];
    Racer *x50130;
    char pad50134[0x50140 - 0x50134];
    Racer *x50140[9];
    Racer *x50164[17];
    int x501A8;
} World;

typedef struct Config {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x358 - 0x28C];
    int x358;
} Config;

extern Config D_004B25A0;
extern int func_00222BD8(int min, int max);
extern void func_0011B030(World *world, Racer *a, Racer *b);

void func_0011AEC0(World *world)
{
    int i;
    int j;
    int total;
    int first;
    int back;
    int r;
    int pos;
    int k;

    if (D_004B25A0.x358 <= 0) {
        return;
    }
    first = 0;
    back = 1;
    total = D_004B25A0.x288 + D_004B25A0.x358;
    for (k = 0; k < total; k++) {
        pos = k + 1;
        j = total / 2 + pos;
        if (k == 0) {
            first = j;
        }
        if (j > total) {
            j = first - back;
            back++;
        }
        if (k == 0) {
            world->x50164[j - 1] = world->x50130;
        } else {
            world->x50164[j - 1] = world->x50140[k - 1];
        }
    }
    if (D_004B25A0.x10C == 0x10) {
        return;
    }
    for (i = 0; i < D_004B25A0.x288; i++) {
        r = func_00222BD8(0, world->x501A8 - 1);
        if (i != r) {
            func_0011B030(world, &world->racers[i], &world->racers[r]);
        }
    }
}
