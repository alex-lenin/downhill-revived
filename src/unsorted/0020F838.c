/* UNIT 0x0020F838 0x0020F860 */
struct Entry {
    char pad0[0x8];
    unsigned short x8;
    char padA[0x4];
    unsigned short xE;
    char pad10[0x10];
};

extern struct Entry D_004F3570[];

void func_0020F838(int i, int offset)
{
    struct Entry *e = &D_004F3570[i];
    e->xE = e->x8 + offset * 16;
}
