/* UNIT 0x00169808 0x00169920 */
typedef struct Rider {
    char pad0[0x7908];
    short x7908;
    char pad790A[0x796C - 0x790A];
    int x796C;
    char pad7970[0x7998 - 0x7970];
    int x7998;
} Rider;

typedef struct Game {
    char pad0[0x50130];
    Rider *rider;
} Game;

typedef struct Global {
    char pad0[0x10C];
    int x10C;
    char pad110[0x2A0 - 0x110];
    int x2A0;
} Global;

typedef struct Lap {
    char pad0[0x48];
    int x48;
    int x4C;
} Lap;

typedef struct Track {
    Lap laps[1];
    char pad50[0x694 - 0x50];
} Track;

typedef struct Rec {
    char pad0[0xD8];
} Rec;


typedef struct Timer {
    char pad0[0x524];
    int x524;
    int x528;
    int x52C;
    char pad530[0x53C - 0x530];
    int x53C;
    char pad540[0x553 - 0x540];
    unsigned char x553;
    unsigned char x554;
    unsigned char x555;
} Timer;

extern Game D_002AF2C0;
typedef struct World {
    Track tracks[18];
    Rec recs[1];
} World;

extern World D_004225A0;
extern int D_0043EEC8[];
extern Global D_004B25A0;
extern void func_00195640(Rec *rec, int time, int a, int b, int c);
extern unsigned char func_00194EF8(Track *track);
extern void func_00169920(Timer *timer);

static inline int GetTrackIndex(Rider *rider)
{
    return (rider->x796C >> 5) - 1;
}

void func_00169808(Timer *timer)
{
    Rider *rider = D_002AF2C0.rider;
    int time;
    int track;
    Lap *lap;
    char *p;
    int ofs;

    time = D_0043EEC8[0] + timer->x52C;
    track = GetTrackIndex(rider);
    timer->x555 = 0;
    timer->x524 = time;
    /* The product goes into p and p is then overwritten, so the
       track * sizeof(Track) below is recomputed after the calls. */
    p = (char *)(track * sizeof(Track));
    p += (int)&D_004225A0;
    lap = (Lap *)(D_004B25A0.x10C * sizeof(Lap) + (int)p);
    lap->x48 = timer->x53C;
    lap->x4C = time;
    if (timer->x553) {
        if (timer->x524 < timer->x528) {
            ofs = D_004B25A0.x2A0 * sizeof(Rec) + sizeof(D_004225A0.tracks);
            func_00195640((Rec *)((char *)&D_004225A0 + ofs), timer->x524, rider->x796C, rider->x7908, rider->x7998);
        }
        timer->x554 = func_00194EF8(&D_004225A0.tracks[track]);
        if (timer->x554) {
            timer->x555 = 1;
            func_00169920(timer);
        }
    }
}
