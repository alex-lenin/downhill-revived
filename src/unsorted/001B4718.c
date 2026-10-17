/* UNIT 0x001B4718 0x001B4900 */
typedef struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x178];
    int playerCount; /* 0x288 */
    char pad28C[0xD8];
    int x364;
} State;
typedef struct Racer { char pad0[0x79AC]; int x79AC; char pad79B0[0x220]; } Racer;
typedef struct Race { Racer racers[10]; char pad4D620[0x2B88]; int racerCount; /* 0x501A8 */ } Race;
typedef struct Scores { char pad0[0x12210]; int x12210; } Scores;
typedef struct Obj { int timer; int x4; } Obj;
typedef struct Player { char pad0[0x150]; char x150[0x1230]; } Player;

extern State D_004B25A0;
extern Race D_002AF2C0;
extern Scores D_004225A0;
extern Player D_003B2270[];
extern int D_0038B1E0[];
extern int D_0043EEC8[];
extern int D_0043F348[];
extern void func_00175708(void *, int);
extern void func_001B45F8(Obj *, int, int);

static inline Race *GetRace(void) { return &D_002AF2C0; }
static inline State *GetState(void) { return &D_004B25A0; }
static inline Player *GetPlayers(void) { return D_003B2270; }
static inline int Greater(int a, int b) { return a > b; }
static inline int CountActive(Race *race, int total) {
    int count = 0;
    int j;
    for (j = 0; j < total; j++) count += Greater(race->racers[j].x79AC, 1);
    return count;
}
static inline int IsMode15(void) {
    State *state = GetState();
    int result = 0;
    if (state->x10C == 15) result = state->x364 == 1;
    return result;
}

void func_001B4718(Obj *obj) {
    unsigned int i;
    char *p;
    
    if (D_004B25A0.x364 == 0) {
        if (obj->timer > 0 && (unsigned int)(D_0043F348[0] - 2) < 3) {
            if (--obj->timer <= 0) {
                i = 0;
                if (D_004B25A0.playerCount > 0) {
                    p = GetPlayers()[0].x150;
                    do {
                        if (i < 4) func_00175708(p, 0xAA);
                        ++i;
                        p += 0x1380;
                    } while ((int)i < D_004B25A0.playerCount);
                }
                func_001B45F8(obj, 1, 120);
            }
        }
    } else {
        Race *race = GetRace();
        int total = race->racerCount;
        if (total - CountActive(race, total) < 2) {
            i = 0;
            if (D_004B25A0.playerCount > 0) {
                p = GetPlayers()[0].x150;
                do {
                    if (i < 4) func_00175708(p, 0xAA);
                    ++i;
                    p += 0x1380;
                } while ((int)i < D_004B25A0.playerCount);
            }
            if (IsMode15() && obj->x4 == 0) {
                D_0038B1E0[0] = D_0043EEC8[0];
                if (D_0043EEC8[0] < D_004225A0.x12210) D_004225A0.x12210 = D_0043EEC8[0];
            }
            func_001B45F8(obj, 1, 300);
        }
    }
}
