/* UNIT 0x00207560 0x00207630 */
typedef struct Key {
    unsigned char key;
    unsigned char x1;
} Key;

typedef struct Table {
    char pad0[4];
    short count;
    Key keys[1];
} Table;

typedef struct Curve {
    float base;
    float scale;
    char pad8[8];
    Table table;
} Curve;

typedef struct Pos {
    int idx;
    int x4;
    int x8;
} Pos;

int func_00207560(Table *table, short *out, float x)
{
    Curve *curve;
    Key *keys;
    Pos pos;
    int last;
    int lo;
    int hi;
    int mid;

    curve = table ? (Curve *)((char *)table - 0x10) : 0;
    keys = table->keys;
    last = table->count - 1;
    mid = last >> 1;
    pos.idx = (x - curve->base) / curve->scale;
    if (pos.idx >= keys[mid + 1].key) {
        lo = mid + 1;
        hi = last;
        mid = mid + 2;
    } else if (pos.idx >= keys[mid].key) {
        return mid;
    } else {
        lo = 0;
        hi = mid;
        mid = 1;
    }
    while (hi - lo >= 2) {
        if (pos.idx < keys[mid].key) {
            hi = mid;
        } else {
            lo = mid;
        }
        mid = (lo + hi) >> 1;
    }
    *out = lo;
    return lo;
}
