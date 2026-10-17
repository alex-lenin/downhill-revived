/* UNIT 0x001A3220 0x001A3598 */
/* EE min.s uses inline asm; all scalar operations are C. */
#include "rider.h"
typedef struct Game {
    Rider riders[10];
    char pad0[0x501A8 - 10 * 0x7BD0];
    int x501A8;
} Game;

typedef struct Clock {
    char pad0[8];
    int x8;
    char padC[0x1C - 0xC];
    int times[1];
} Clock;

typedef struct Mode {
    char pad0[0x110];
    int x110;
    char pad114[0x2A0 - 0x114];
    int x2A0;
} Mode;

typedef struct Track {
    char pad0[0x694];
} Track;

typedef struct EntryD8 {
    char pad0[0x48];
    long x48;
    long x50;
    int x58;
    char pad5C[0xD8 - 0x5C];
} EntryD8;

typedef struct Root {
    Track races[18];
    EntryD8 stats[203];
} Root;

typedef struct Entry14 {
    Rider *r;
    int x4;
    float x8;
    int xC;
    int x10;
} Entry14;

typedef struct Entry18 {
    Rider *r;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
} Entry18;

typedef struct Obj {
    Entry14 e14[10];
    Entry18 e18[4];
    int arr128[4];
    int x138;
    int x13C;
} Obj;

extern Game D_002AF2C0;
extern Clock D_0043EEC0;
extern Mode D_004B25A0;
extern Root D_004225A0;
extern char D_004C8508[];

extern void func_0019FE28(void *);
extern float func_001301B8(void *);
extern int func_00195000(void *, int, int, int, int);
extern void func_00194F88(void *, int, int);
extern void func_00194FA8(void *, int, int, int);
extern void func_00194FD8(void *, int, int);
extern int func_001FA168(void *, int, int);
extern void func_00194CF0(void *, int, int, int, int);
extern int func_00194430(void *, int, int);
extern void func_001A3598(void *);

static inline Mode *GetMode(void) { Mode *m = &D_004B25A0; return m; }
static inline int GetCourse(void) { Mode *m = &D_004B25A0; return m->x2A0; }
static inline EntryD8 *GetStats(char *table, int course) {
    int addr = course * 0xD8;
    addr += (int)table;
    return ((Root *)addr)->stats;
}
void func_001A3220(Obj *obj) {
    int count;
    int idx;
    int type;
    int *timeValues;
    int i;
    Rider *r;
    Rider *riders;
    int pos;
    int delta;
    RiderSub6610 *sub;
    int a;
    int b;
    float fret;
    Game *game;
    Clock *clock;

    game = &D_002AF2C0;
    count = game->x501A8;
    clock = &D_0043EEC0;
    timeValues = clock->times;
    i = 0;
    func_0019FE28(clock);
    obj->x13C = 0;
    for (; i < count;) {
        riders = D_002AF2C0.riders;
        r = &riders[i];
        pos = r->x798C;
        sub = &r->sub6610;
        idx = pos - 1;
        a = (int)sub->xC;
        b = (int)sub->x24;
        fret = func_001301B8(r);
        delta = (int)((float)(a - b) - fret);
        type = r->x7A5C;
        if (type == 1) {
            Track *t;
            char *table;
            Mode *mode;
            int res;
            int prev;
            EntryD8 *pe;
            int addr;
            int trackOffset;

            mode = GetMode();
            table = (char *)&D_004225A0;
            trackOffset = ((r->x796C >> 5) - 1) * 0x694;
            obj->arr128[r->x7978] = pos;
            addr = mode->x2A0;
            addr *= 0xD8;
            addr += (int)table;
            pe = ((Root *)addr)->stats;
            t = (Track *)(table + trackOffset);
            pe->x48 += timeValues[i];
            GetStats(table, mode->x2A0)->x58++;
            GetStats(table, mode->x2A0)->x50 += delta;
            res = func_00195000(t, mode->x2A0, timeValues[i], delta, pos);
            obj->x13C = res;
            if (res != 0) {
                func_00194F88(t, mode->x2A0, pos);
                func_00194FA8(t, mode->x2A0, timeValues[i], r->x7998);
                func_00194FD8(t, mode->x2A0, delta);
            }
            if (mode->x110 == type) {
                prev = func_001FA168(D_004C8508, pos, (r->x796C >> 5) - 1);
                func_00194CF0(t, delta, r->u7998, pos, prev + (int)sub->xC);
            }
            obj->e18[i].r = r;
            addr = (int)obj + i * 0x18;
            ((Obj *)addr)->e18[0].x4 = delta;
            obj->e18[i].x8 = func_00194430(table, pos, mode->x2A0);
            fret = func_001301B8(r);
            obj->e18[i].xC = (int)fret;
            obj->e18[i].x10 = (int)sub->x24;
            obj->e18[i].x14 = pos;
        }
        {
            float div;
            float m;

            obj->e14[idx].r = r;
            obj->e14[idx].x4 = timeValues[i];
            if (r->x7994 == 0) div = 0.0f;
            else div = r->x7A3C / (float)r->x7994;
            m = 200.0f;
            __asm__("min.s %0,%0,%1" : "+f"(m) : "f"(div));
            {
                obj->e14[idx].x8 = m;
                obj->e14[idx].xC = delta;
                i++;
                obj->e14[idx].x10 = func_00194430(&D_004225A0, pos, GetCourse());
            }
        }
    }
    func_001A3598(obj);
}
