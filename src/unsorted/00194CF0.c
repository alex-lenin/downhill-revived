/* UNIT 0x00194CF0 0x00194D28 */
struct Entry {
    char pad0[0x50];
};

extern int D_004B26AC[];
extern void func_001952D8(struct Entry *, int, unsigned short, int, int);

void func_00194CF0(struct Entry *entries, int a, unsigned short b, int c, int d)
{
    func_001952D8(&entries[D_004B26AC[0]], a, b, c, d);
}
