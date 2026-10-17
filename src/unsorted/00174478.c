/* UNIT 0x00174478 0x00174590 */
typedef struct Entry {
    char pad0[0x8];
    float x8;
    char padC[0x8];
    unsigned short flags;
} Entry;

typedef struct Table {
    char pad0[0x5C];
    int count;
} Table;

extern float D_0029C8F0[]; /* { FLT_MAX } */
extern Entry *func_00163D40(Table *table, int index);
extern Entry *func_00163F98(Table *table, int id, float pos);

/* Finds the active entry closest to pos (preferring ones behind it), else falls back. */
Entry *func_00174478(Table *table, int id, float pos)
{
    Entry *best;
    Entry *entry;
    float bestDist;
    float dist;
    int i;

    best = 0;
    bestDist = D_0029C8F0[0];
    for (i = 0; i < table->count; i++) {
        entry = func_00163D40(table, i);
        if ((entry->flags >> 1) & 1) {
            dist = pos - entry->x8;
            if (entry->x8 < pos) {
                if (dist < bestDist || bestDist < 0.0f) {
                    best = entry;
                    bestDist = dist;
                }
            } else if (__builtin_fabsf(dist) < bestDist) {
                best = entry;
                bestDist = dist;
            }
        }
    }
    if (best == 0) {
        best = func_00163F98(table, id, pos);
    }
    return best;
}
