/* UNIT 0x001A40B8 0x001A42D8 */
typedef struct Rider {
    char pad0[0x7908];
    int x7908;
    char pad790c[0x60];
    int x796c;
    char pad7970[8];
    int x7978;
    char pad797c[0x1c];
    int x7998;
    char pad799c[0xc0];
    int x7a5c;
} Rider;

typedef struct Result {
    Rider *rider;
    int time;
    int pad8[3];
} Result;

typedef struct Output {
    Result entries[15];
    int pad12c[3];
    int x138;
    int x13c;
} Output;

typedef struct State {
    char pad0[0x288];
    int count;
    char pad28c[0x14];
    int x2a0;
} State;

typedef struct Clock {
    char pad0[0x10];
    int x10;
    char pad14[8];
    int x1c;
} Clock;

typedef struct Sub {
    char pad0[0x320];
    int x320[1];
    char pad324[0x694 - 0x324];
} Sub;

typedef struct Record {
    char pad0[0x48];
    long x48;
    char pad50[8];
    int x58;
    int x5c;
    char pad60[0x78];
} Record;

typedef struct Table {
    Sub subs[18];
    Record records[1];
} Table;

typedef struct Inputs {
    char pad0[0x1c];
    int x1c[4];
} Inputs;

typedef struct Riders {
    char pad0[0x50130];
    Rider *x50130[4];
} Riders;

extern State D_004B25A0;
extern Clock D_0038DB60;
extern Table D_004225A0;
extern Riders D_002AF2C0;
extern Inputs D_0043EEC0;
extern void func_00194FA8(Sub *, int, int, int);
extern void func_001956C0(Record *, int, int, int, int, int, int);


/* &t->subs[n] and &t->records[n] through an int offset: retail recomputes the
   multiply for every use instead of sharing it */
static inline Sub *GetSub(Table *t, int n)
{
    int offset = n * sizeof(Sub);
    offset += (int)t;
    return (Sub *)offset;
}

static inline Record *GetRecord(Table *t, int n)
{
    int offset = n * sizeof(Record);
    offset += (int)t;
    return (Record *)(offset + 0x7668);
}

void func_001A40B8(Output *out) {
    int times[4];
    Inputs *in = &D_0043EEC0;
    int *src;
    int i;
    Rider *rider;
    int value, index, extra, flag, diff;
    Table *table;

    i = 0;
    out->x13c = 0;
    out->x138 = 0;
    src = in->x1c;
    for (; i < D_004B25A0.count; i++) {
        diff = (D_0038DB60.x10 - D_0038DB60.x1c) * 60;
        table = &D_004225A0;
        rider = D_002AF2C0.x50130[i];
        value = src[i] + diff;
        index = (rider->x796c >> 5) - 1;
        times[i] = value;
        GetRecord(table, D_004B25A0.x2a0)->x48 += value;
        GetRecord(table, D_004B25A0.x2a0)->x58++;
        if (times[i] < GetSub(table, index)->x320[D_004B25A0.x2a0]) {
            out->x13c |= 1;
            func_00194FA8(&table->subs[index], D_004B25A0.x2a0, times[i], rider->x7998);
        }
        if (times[i] < table->records[D_004B25A0.x2a0].x5c) {
            out->x138 |= 1;
            flag = rider->x7a5c == 1;
            extra = 0;
            if (flag) {
                extra = rider->x7978 + 1;
            }
            func_001956C0(&table->records[D_004B25A0.x2a0], 0, times[i], rider->x796c,
                          rider->x7908, extra, rider->x7998);
        }
        out->entries[i].rider = rider;
        out->entries[i].time = src[i];
    }
}
