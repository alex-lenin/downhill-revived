/* UNIT 0x00194EF8 0x00194F88 */
typedef struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x44 - 0x33];
    int x44;
    int x48;
    int x4C;
} Entry;

typedef struct Global {
    char pad0[0x10C];
    int current;
} Global;

extern Global D_004B25A0;

extern int func_001821E8(int, signed char);

int func_00194EF8(Entry *entries)
{
    int result;
    int index;
    Entry *entry;

    index = D_004B25A0.current;
    result = func_001821E8(index, ++entries[index].x32) == 0x21;
    entries[D_004B25A0.current].x44 = 0;
    entry = &entries[D_004B25A0.current];
    entry->x48 = 0;
    entry->x4C = 0;
    return result;
}
