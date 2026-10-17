/* UNIT 0x0023D020 0x0023D040 */
typedef struct Entry {
    void *x0;
    int x4;
    char pad8[0xC];
} Entry;

extern Entry D_0075A9D0[];

void func_0023D020(int i, void *p)
{
    D_0075A9D0[i].x0 = p;
}
