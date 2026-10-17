/* UNIT 0x002202B8 0x002202D0 */
struct Entry {
    short x0;
    short x2;
    char pad4[0x1C];
};

extern struct Entry D_00660B80[];

void func_002202B8(int i, short v)
{
    D_00660B80[i].x2 = v;
}
