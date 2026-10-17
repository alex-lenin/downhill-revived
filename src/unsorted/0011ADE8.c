/* UNIT 0x0011ADE8 0x0011AEC0 */
typedef struct Sub7930 {
    char pad0[0x4];
    unsigned char x4;
} Sub7930;

typedef struct Racer {
    char pad0[0x7930];
    Sub7930 *x7930;
    char pad7934[0x7BD0 - 0x7934];
} Racer;

typedef struct World {
    Racer racers[10];
    char pad4D620[0x50140 - 0x4D620];
    Racer *x50140[26];
    int x501A8;
} World;

typedef struct Config {
    char pad0[0x288];
    int x288;
} Config;

extern Config D_004B25A0;
extern void func_0012C450(Racer *);
extern void func_0011AEC0(World *);
extern void func_001B6668(Racer *);

static inline void remove_racer(Racer *racer, int flag) {
    if (racer->x7930) {
        racer->x7930->x4 = flag;
    }
    func_001B6668(racer);
}

void func_0011ADE8(World *world) {
    int i;
    int n;
    Racer *racer;

    for (i = 0; i < world->x501A8; i++) {
        func_0012C450(&world->racers[i]);
    }
    func_0011AEC0(world);
    n = world->x501A8 - D_004B25A0.x288;
    for (i = 0; i < n; i++) {
        racer = world->x50140[i];
        if (racer) {
            remove_racer(racer, 1);
        }
    }
}
