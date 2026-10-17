/* UNIT 0x0014F868 0x0014FC08 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct SmallCopy { int bytes[6]; } SmallCopy;
typedef struct BigCopy { int bytes[36]; } BigCopy;
typedef struct Scratch {
    int limits[6];
    char pad0[8];
    int items[6][6];
} Scratch;

typedef struct Rider {
    char pad0[0x796C];
    int x796C;
} Rider;

typedef struct Game {
    char pad0[0x50130];
    Rider *x50130;
} Game;

typedef struct World {
    char pad0[0x11C68];
    char x11C68[0x520];
    u32 x12188;
    char pad1[0x1A];
    u8 x121A6;
} World;

typedef struct LevelEntry {
    int x0;
    int x4;
    int x8;
} LevelEntry;

typedef struct Level {
    char pad0[0x30];
    LevelEntry entries[7];
    int x84;
    char pad1[0x18];
} Level;

typedef struct Config {
    char pad0[0x10C];
    int x10C;
} Config;

typedef struct Actor {
    char pad0[0x510];
    u32 x510;
    u32 x514;
} Actor;

extern const SmallCopy D_00289F08;
extern const BigCopy D_00289F20;
extern Game D_002AF2C0;
extern World D_004225A0;
extern Config D_004B25A0;
extern Level D_004C8508[];

extern void func_00194D88(void *, int, u8 *, u8 *, u8 *, u8 *);
extern int func_00195420(void *, LevelEntry *);
extern void func_001953B8(void *, LevelEntry *);

static inline World *GetWorld(void) { return &D_004225A0; }
static inline void GetStats(void *world, int mode, u8 *a, u8 *b, u8 *c, u8 *d) {
    func_00194D88(world, mode, a, b, c, d);
}

void func_0014F868(Actor *actor) {
    Scratch scratch;
    u8 a,b,c,d;
    int added;
    int mode;
    int riderIndex;
    int score;
    int limit;
    int i;
    int id;
    int *item;
    u32 flags;
    LevelEntry *entry;
    Level *level;
    World *world;
    char *base;
    int count;
    int next;
    int j;

    added = 0;
    *(SmallCopy *)scratch.limits = D_00289F08;
    *(BigCopy *)scratch.items = D_00289F20;

    {
    int (*table)[6] = scratch.items;
    riderIndex = (D_002AF2C0.x50130->x796C >> 5) - 1;
    world = GetWorld();
    flags = world->x12188;
    mode = D_004B25A0.x10C;
    GetStats((char *)world + riderIndex * 0x694, mode, &a, &b, &c, &d);
    score = a + (b >> 1) + (c / 3U);
    mode = D_004B25A0.x10C;

    if (mode < 6) {
        item = table[mode];
        limit = scratch.limits[mode];
        for (j = 0; j < 6; limit--, j++) {
            int insufficient = score < limit;
            id = table[mode][j];
            if (id == 0 || insufficient) goto next_item;
            {
                u32 bit = 1U << id;
                if (flags & bit) goto next_item;
                added++;
                GetWorld()->x12188 |= bit;
                actor->x510 |= bit;
                if (id == 10) GetWorld()->x121A6 = 1;
            }
next_item:
        }
    }

    if (added) {
        level = &D_004C8508[mode];
        count = level->x84;
        entry = &level->entries[0];
        base = GetWorld()->x11C68;
        for (i = 0; i < count; i = next) {
            next = i + 1;
            if (added > 0) {
                LevelEntry *cur;
                int offset = i * 12;
                cur = (LevelEntry *)((char *)entry + offset);
                if (func_00195420(base, cur) != 0) goto advance;
                actor->x514 |= 1 << i;
                added--;
                func_001953B8(base, cur);
                if (D_004B25A0.x10C == 3 && cur->x0 == 0x12 && next < count) {
                    cur = (LevelEntry *)((char *)entry + (offset + 12));
                    if (func_00195420(base, cur) == 0) {
                        actor->x514 |= 1 << next;
                        func_001953B8(base, cur);
                    }
                }
            }
advance:
            ;
        }
    }
    }
}
