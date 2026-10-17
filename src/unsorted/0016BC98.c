/* UNIT 0x0016BC98 0x0016C0C8 */
typedef struct {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x360 - 0x28C];
    int x360;
    char pad364[0x36C - 0x364];
    int x36C;
} Game;

typedef struct {
    char pad0[0x2B0];
    int x2B0;
    char pad2B4[0x380 - 0x2B4];
} Rider;

typedef struct {
    Rider riders[4];
    int count;
} Riders;

typedef struct {
    char pad0[0x38];
    int x38;
} Mode;

typedef struct {
    int x0;
    int x4;
    int x8;
} Arena;

typedef struct {
    float x0, x4, x8, xC, x10, x14;
} Camera;

typedef struct {
    short delta;
    short pad;
    void (*func)(void *);
} VtblEntry;

typedef struct {
    int x0;
    VtblEntry *vtbl;
} VObject;

typedef struct {
    char pad0[0x240];
    char x240[0x5BBC - 0x240];
    float x5BBC;
    float x5BC0;
    float x5BC4;
    char pad5BC8[0x5BE8 - 0x5BC8];
    int x5BE8;
    char pad5BEC[0x5BF8 - 0x5BEC];
    VObject *x5BF8;
} World;

static inline void ArenaPush(Arena *arena) {
    int start = (arena->x4 + 3) & ~3;
    int prev = arena->x8;
    arena->x8 = start;
    arena->x4 = start + 4;
    *(int *)start = prev;
}

extern int D_0029C138;
extern int D_0029DD70;
extern Game D_004B25A0;
extern Riders D_0037EA60;
extern Mode D_004C3740;
extern Arena D_00663B08;
extern Camera D_004F3480;
extern char D_0039E840[], D_0029EA20[], D_0036CFC0[], D_003705C0[], D_003041B0[];
extern char D_003AD8B0[], D_003B01E0[], D_003BD580[], D_004BD8C0[], D_003C2230[];
extern char D_003C42C0[], D_004A6390[], D_00438750[], D_00437850[], D_003985B0[];
extern char D_004C5640[], D_002AF2C0[], D_00372650[], D_003B71D0[], D_004BF950[];
extern char D_004C5C50[], D_0036B870[], D_00308D40[], D_003B2270[];
extern char D_0043EEC0[], D_004348F0[], D_0038DB60[], D_003C6930[];

extern int func_0023D040(int);
extern void func_00148260(void *);
extern void func_00230DA8(void *);
extern void func_0020EC78(float *, float *, float *, float *, float *, float *, float *);
extern void func_002109A0(void);
extern void func_00229050(void);
extern void func_0020C918(void);
extern void func_002396C8(void);
extern int func_00239028(void);
extern void func_00118380(void *);
extern void func_00144720(void *);
extern void func_00145FE0(void *);
extern void func_00122368(void *);
extern void func_00172408(void *);
extern void func_00174398(void *);
extern void func_001818C0(void *);
extern void func_001DBBB8(void *);
extern void func_00182070(void *);
extern void func_001AB950(void *);
extern void func_001990C0(void *);
extern void func_00198680(void *);
extern void func_00168DB0(void *);
extern void func_001E4150(void *);
extern void func_00119FC8(void *);
extern void func_00147170(void *);
extern void func_0017C9F8(void *);
extern void func_001DC328(void *);
extern void func_001EACF0(void *);
extern void func_0016BC30();
extern void func_002182C0(void (*)());
extern void func_00143CB0(void *);
extern void func_00192ED8(void);
extern void func_001275B0(void *);
extern void func_00163678(void *);
extern void func_001751C8(void *);
extern void func_0019E9A0(void *);
extern void func_001960F0(void *);
extern void func_001D40F8(void *);
extern void func_00161408(void *);
extern void func_001833A8(void *);
extern void func_001E1BE8(void *);
extern float func_00118458(void *);
extern float func_001184F0(void *);
extern float func_00118588(void *);

void func_0016BC98(World *world) {
    float v[7];
    int count;
    int mode;
    VObject *obj;
    VtblEntry *entry;

    count = D_004B25A0.x288;
    if (count == 1) {
        D_0037EA60.count = count;
        D_0037EA60.riders[0].x2B0 = func_0023D040(0);
        D_0029C138 = 0;
    } else if (count == 2) {
        D_0037EA60.count = count;
        D_0037EA60.riders[0].x2B0 = func_0023D040(0);
        D_0037EA60.riders[1].x2B0 = func_0023D040(1);
        D_0029C138 = -1;
    } else if (count >= 3) {
        D_0037EA60.count = count;
        D_0037EA60.riders[0].x2B0 = func_0023D040(0);
        D_0037EA60.riders[1].x2B0 = func_0023D040(1);
        D_0037EA60.riders[2].x2B0 = func_0023D040(2);
        D_0037EA60.riders[3].x2B0 = func_0023D040(3);
        D_0029C138 = -1;
    }
    func_00148260(&D_0037EA60);
    func_00230DA8(D_0039E840);
    if (D_004B25A0.x288 >= 3) {
        func_0020EC78(&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6]);
        D_004F3480.x4 = v[0];
        D_004F3480.xC = v[1];
        D_004F3480.x14 = v[3];
        D_004F3480.x0 = 160.0f;
        D_004F3480.x8 = 400.0f;
        D_004F3480.x10 = 400.0f;
    }
    func_002109A0();
    func_00229050();
    func_0020C918();
    func_002396C8();
    *(int *)world = func_00239028();
    ArenaPush(&D_00663B08);
    if (D_004B25A0.x36C) {
        func_00118380(D_0029EA20);
        func_00144720(D_0036CFC0);
        func_00145FE0(D_003705C0);
        func_00122368(D_003041B0);
        func_00172408(D_003AD8B0);
        func_00174398(D_003B01E0);
        func_001818C0(D_003BD580);
        func_001DBBB8(D_004BD8C0);
        func_00182070(D_003C2230);
        func_00182070(D_003C42C0);
        func_001AB950(D_004A6390);
    }
    mode = D_004B25A0.x10C;
    if ((mode == 18 && D_004C3740.x38 == 1) || mode == 15 || mode == 11) {
        func_001990C0(D_00438750);
    } else if (mode == 14 || D_004B25A0.x360) {
        func_00198680(D_00437850);
    } else {
        func_001990C0(D_00438750);
    }
    if (D_004B25A0.x10C == 5) {
        func_00168DB0(D_003985B0);
    }
    if (D_004B25A0.x10C == 11) {
        func_001E4150(D_004C5640);
    }
    func_00119FC8(D_002AF2C0);
    func_00147170(D_00372650);
    func_0017C9F8(D_003B71D0);
    func_001DC328(D_004BF950);
    func_001EACF0(D_004C5C50);
    func_002182C0(func_0016BC30);
    func_00143CB0(D_0036B870);
    func_00192ED8();
    func_001275B0(D_00308D40);
    func_00163678(&D_0029DD70);
    func_001751C8(D_003B2270);
    func_0019E9A0(D_0043EEC0);
    func_001960F0(D_004348F0);
    func_001D40F8(world->x240);
    obj = world->x5BF8;
    if (obj) {
        entry = &obj->vtbl[2];
        entry->func((char *)obj + entry->delta);
    }
    func_00161408(D_0038DB60);
    func_001833A8(D_003C6930);
    func_001E1BE8(&D_004C3740);
    world->x5BBC = func_00118458(D_0029EA20);
    world->x5BC0 = func_001184F0(D_0029EA20);
    world->x5BC4 = func_00118588(D_0029EA20);
    world->x5BE8 = 0;
}
