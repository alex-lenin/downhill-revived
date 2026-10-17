/* UNIT 0x00163C58 0x00163D40 */
#include "animation_track_search.h"
struct Entry {
    char pad0[0x8];
    float x8;
    char padC[0x8];
    short flags;
};

struct Table {
    char pad0[0x5C];
    int count;
};

extern struct Entry *func_00163D40(struct Table *, int);

#if defined(__mips__)
int func_00163C58(struct Table *table, int after, int flags, float limit)
#else
int SharedCall00163C58(struct Shared00163C58_struct_Table * table_view, int after, int flags, float limit)
#endif
{
#if !defined(__mips__)
    struct Table * table = (struct Table *)table_view;
#endif

    int i;
    int found = -1;
    struct Entry *entry;

    i = 0;
    after++;
    if (after > found) { /* found is still -1 here; retail compares against it */
        i = after;
    }
    if (i < table->count) {
        do {
            if (func_00163D40(table, i)->flags & 1) {
                entry = func_00163D40(table, i);
                if (flags == 0 || entry->flags == (flags |= 1)) {
                    if (entry != 0 && entry->x8 < limit) {
                        found = i;
                        break;
                    }
                }
            }
            i++;
        } while (i < table->count);
    }
    return found;
}
