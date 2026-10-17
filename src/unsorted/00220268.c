/* UNIT 0x00220268 0x00220280 */
struct Entry {
    short x0;
    short x2;
    char pad4[0x1C];
};

extern struct Entry D_00660B80[];

short func_00220268(int i)
{
    return D_00660B80[i].x2;
}
