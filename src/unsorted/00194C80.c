/* UNIT 0x00194C80 0x00194C98 */
typedef struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x50 - 0x33];
} Entry;

int func_00194C80(Entry *entries, int index)
{
    return entries[index].x32;
}
