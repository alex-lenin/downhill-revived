/* UNIT 0x0020D238 0x0020D250 */
struct Entry {
    char pad0[0xC];
    char xC[0x48 - 0xC];
};

char *func_0020D238(struct Entry *entries, int index)
{
    return entries[index].xC;
}
