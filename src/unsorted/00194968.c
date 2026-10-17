/* UNIT 0x00194968 0x001949A8 */
typedef struct Entry { char pad0[0x32]; signed char x32; } Entry;
extern int func_001821E8(int, int);
int func_00194968(int obj, int index) {
    int offset;
    Entry *entry;
    offset = (index - 4) * 0x50;
    entry = (Entry *)(offset + obj + 0x11A88);
    return func_001821E8(index, entry->x32);
}

