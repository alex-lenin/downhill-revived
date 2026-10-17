/* UNIT 0x001B0F28 0x001B0F80 */
typedef struct Entry {
    char data[0x110];
} Entry;

extern void func_00254204(void *dst, int c, int n);
extern void func_001B1188(Entry *entry);

void func_001B0F28(Entry *entries)
{
    int i;

    for (i = 0; i < 4; i++) {
        func_00254204(&entries[i], 0, sizeof(Entry));
        func_001B1188(&entries[i]);
    }
}
