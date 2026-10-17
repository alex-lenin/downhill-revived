/* UNIT 0x001E7BA0 0x001E7BD8 */
typedef struct IndexEntry { int x0; int x4; } IndexEntry;
typedef struct Entry { char pad[0x40]; int x40; char pad44[0x10]; } Entry;
extern IndexEntry D_002752B0[];
extern Entry D_00273D60[];
int func_001E7BA0(void *obj, int index) {
    int entryIndex;
    Entry *entry;
    entryIndex = D_002752B0[index].x4;
    entry = &D_00273D60[entryIndex];
    if (entry->x40 >= 0) return 1;
    return 0;
}
