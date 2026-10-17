/* UNIT 0x0020D220 0x0020D238 */
struct Entry {
    char pad0[0x38];
    int x38;
    short x3C;
    char pad3E[0x48 - 0x3E];
};

void func_0020D220(struct Entry *entries, int index, short value)
{
    entries[index].x3C = value;
}
