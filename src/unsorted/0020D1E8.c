/* UNIT 0x0020D1E8 0x0020D200 */
struct Entry {
    char pad0[0x38];
    int x38;
    short x3C;
    char pad3E[0x48 - 0x3E];
};

void func_0020D1E8(struct Entry *entries, int index, int value)
{
    entries[index].x38 = value;
}
