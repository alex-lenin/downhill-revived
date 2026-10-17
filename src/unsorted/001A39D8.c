/* UNIT 0x001A39D8 0x001A3C18 */
typedef struct Motion { char pad0[0xC]; float xC; char pad10[0x14]; float x24; } Motion;
typedef struct Actor { char pad0[0x6610]; Motion motion; char pad6638[0x12D0]; int x7908; char pad790C[0x60]; int x796C; char pad7970[8]; int x7978; char pad797C[0x10]; int x798C; char pad7990[8]; int x7998; char pad799C[0xC0]; int x7A5C; } Actor;
typedef struct Entry { Actor *actor; int x4, x8; int score; int x10; } Entry;
typedef struct Obj { Entry entries[14]; char pad118[0x10]; int x128[4]; int x138; int x13C; char pad140[0x18]; int x158; } Obj;
typedef struct Race { char pad0[0x3A4]; int score[(0x694 - 0x3A4) / 4]; } Race;
typedef struct Stats { int x0; char pad4[0x4C]; long x50; char pad58[0x80]; } Stats;
typedef struct Root { Race races[18]; Stats stats[203]; char pad121B0[0x5C]; int x1220C; } Root;
typedef struct Game { char pad0[0x288]; int count; char pad28C[0x14]; int index; } Game;
typedef struct World { char pad0[0x50130]; Actor *actors[16]; } World;

extern Root D_004225A0;
extern Game D_004B25A0;
extern World D_002AF2C0;
extern float func_001301B8(Actor *);
extern void func_00194FD8(Race *, int, int);
extern void func_00195828(Stats *, int, int, int, int, int);

/*
 * The race/stats lookups were written as integer address arithmetic
 * (offset, then base added in place); the plain array forms let CSE share
 * the products and do not match.
 */
void func_001A39D8(Obj *o)
{
    int i;
    int raceAddr, statsAddr, bestAddr;
    Stats *stats;
    Actor *actor;
    Motion *m;
    int race, a, b, number, score, slot;

    o->x13C = 0;
    o->x138 = 0;
    o->x158 = D_004225A0.x1220C / 20;
    for (i = 0; i < D_004B25A0.count; i++) {
        actor = D_002AF2C0.actors[i];
        m = &actor->motion;
        race = (actor->x796C >> 5) - 1;
        a = m->xC;
        b = m->x24;
        number = actor->x798C;
        score = (float)(a - b) - func_001301B8(actor);
        o->x128[actor->x7978] = number;
        statsAddr = D_004B25A0.index * sizeof(Stats);
        statsAddr += (int)&D_004225A0;
        stats = ((Root *)statsAddr)->stats;
        stats->x50 += score;
        slot = number - 1;
        raceAddr = race * sizeof(Race);
        raceAddr += (int)D_004225A0.races;
        if (((Race *)raceAddr)->score[D_004B25A0.index] < score) {
            o->x13C = 1;
            func_00194FD8(&D_004225A0.races[race], D_004B25A0.index, score);
        }
        o->entries[slot].actor = actor;
        o->entries[slot].score = score;
        bestAddr = D_004B25A0.index * sizeof(Stats);
        bestAddr += (int)&D_004225A0;
        if (((Root *)bestAddr)->stats[0].x0 < score) {
            Root *root = &D_004225A0;

            func_00195828(&root->stats[D_004B25A0.index], score,
                          o->entries[slot].actor->x796C, o->entries[slot].actor->x7908,
                          o->entries[slot].actor->x7A5C == 1, o->entries[slot].actor->x7998);
            o->x138 |= 8;
        }
    }
}
