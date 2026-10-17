/* UNIT 0x001DC4B8 0x001DC4E8 */
struct Entry {
    char pad_00[0x30];
    int x30;
    int x34;
    char pad_38[0x28];
};

static __inline__ int *fld_x34(struct Entry *e)
{
    return &e->x34;
}

void func_001DC4B8(struct Entry *entries, int a, int b)
{
    int va;
    int vb;

    va = *fld_x34(entries + b);
    vb = *fld_x34(entries + a);
    *fld_x34(entries + a) = va;
    *fld_x34(entries + b) = vb;
}
