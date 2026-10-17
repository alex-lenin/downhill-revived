/* UNIT 0x00172580 0x00172638 */
typedef struct Entry {
    char pad0[0x4];
    int type;
    char pad8[0x130 - 0x8];
} Entry;

typedef struct Group {
    Entry entries[7];
    int values[14];
    int count;
} Group;

extern char D_003B01E0[];
extern int func_001743F0(void *);
extern int func_001746A8(void *);
extern void func_001728A8(Entry *, Group *, int, int);

void func_00172580(Group *group) {
    int i;
    int r;
    Entry *entry;

    for (i = 0; i < group->count; i++) {
        entry = &group->entries[i];
        if (entry->type == 2) {
            r = func_001743F0(D_003B01E0);
        } else {
            r = func_001746A8(D_003B01E0);
        }
        func_001728A8(entry, group, group->values[i], r);
    }
}
