/* UNIT 0x00194C48 0x00194C80 */
struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x50 - 0x33];
};

extern int D_004B26AC[];
extern int func_001821E8(int, int);

int func_00194C48(struct Entry *entries)
{
    int n = D_004B26AC[0];
    return func_001821E8(n, entries[n].x32);
}
