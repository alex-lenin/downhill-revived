/* UNIT 0x00183770 0x001838D0 */
typedef struct Obj {
    char pad0[0x20];
    int x20;
    char pad24[0x2C - 0x24];
    int x2C;
    char pad30[0x34 - 0x30];
    int x34;
    char pad38[0x290 - 0x38];
    int x290[4][8];
    int x310[4][8];
} Obj;

typedef struct Global {
    char pad0[0x288];
    int count;
    char pad28C[0x2A0 - 0x28C];
    int x2A0;
} Global;

typedef struct Rider {
    char pad0[0x7908];
    short x7908;
    char pad790A[0x796C - 0x790A];
    int x796C;
} Rider;

typedef struct Game {
    char pad0[0x50130];
    Rider *riders[1];
} Game;

typedef struct Entry {
    int x0;
    char pad4[0xD8 - 0x4];
} Entry;

extern Global D_004B25A0;
extern Obj D_003C6930;
extern Game D_002AF2C0;
extern char D_004225A0[];
extern void func_00195828(Entry *, int, int, int, int, int);

static inline int GetCount(void)
{
    return D_003C6930.x2C;
}

void func_00183770(Obj *obj)
{
    int i, j, sum, offset;
    int *a, *b;
    Entry *entry;
    Rider *rider;

    if (obj->x20 == 1) {
        obj->x34 = 0;
        offset = D_004B25A0.x2A0 * sizeof(Entry) + 0x7668;
        entry = (Entry *)(D_004225A0 + offset);
        for (i = 0; i < D_004B25A0.count; i++) {
            rider = D_002AF2C0.riders[i];
            b = obj->x290[i];
            a = obj->x310[i];
            sum = 0;
            for (j = 1; j <= GetCount(); j++) {
                sum += b[j] * a[j];
            }
            if (entry->x0 < sum) {
                obj->x34 = 1;
                func_00195828(entry, sum, rider->x796C, rider->x7908, 1, 0);
            }
        }
    }
}
