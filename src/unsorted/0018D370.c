/* UNIT 0x0018D370 0x0018D468 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Node {
    char pad0[0x10];
    Vec x10;
} Node;

typedef struct World {
    char pad0[0x3FF0];
    Vec x3FF0;
    char pad4000[0x40C0 - 0x4000];
    char x40C0[0x4580 - 0x40C0];
    char x4580[0x48B0 - 0x4580];
    int x48B0;
    char pad48B4[0x6300 - 0x48B4];
    int x6300;
    char pad6304[0x792C - 0x6304];
    Node *x792C;
    char pad7930[0x7984 - 0x7930];
    int x7984;
    int x7988;
} World;

typedef struct Obj {
    World *world;
    int x4;
    int x8;
} Obj;

typedef struct Player {
    char pad0[0x2B8];
    int x2B8;
    char pad2BC[0x380 - 0x2BC];
} Player;

typedef struct Racer {
    char pad0[0x1380];
} Racer;

typedef struct RacerSet {
    char pad0[0x150];
    Racer racers[4];
} RacerSet;

/* 128-bit vector copy (libvu0-style inline asm) */
static inline void VecCopy(Vec *out, Vec *in)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(in) : "$2");
}

extern Player D_0037EA60[];
extern RacerSet D_003B2270;
extern unsigned int D_0029CE68;

extern void func_001ACFA0(void *p, int a);
extern void func_00136030(void *p, int a);
extern void func_0017B338(void *p, int a, int b, int c, int d, int e);
extern int func_00148B68(Player *players, World *world);
extern void func_0020CC40(int a, float d, float e, unsigned int b, int c);

static inline void StartRacer(RacerSet *set, unsigned int index)
{
    if (index < 4) {
        func_0017B338(&set->racers[index], 5, 1, 1, 0, 0);
    }
}

static inline int GetState(Player *players, int index)
{
    return players[index].x2B8;
}

static inline void PlaySound(int sound, float a, float b)
{
    func_0020CC40(sound, a, b, D_0029CE68, 0);
}

void func_0018D370(Obj *obj)
{
    World *world = obj->world;
    int index;

    func_001ACFA0(world->x4580, 0);
    world->x48B0 = 0;
    func_00136030(world->x40C0, 1);
    obj->x8 = 60;
    VecCopy(&world->x3FF0, &world->x792C->x10);
    if (world->x7984 != 0) {
        index = world->x7988;
        if (GetState(D_0037EA60, index) != 4) {
            StartRacer(&D_003B2270, index);
            PlaySound(func_00148B68(D_0037EA60, world), 0.0f, 1.0f);
        }
    }
    world->x6300 = 60;
}
