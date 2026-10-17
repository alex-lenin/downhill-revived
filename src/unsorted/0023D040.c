/* UNIT 0x0023D040 0x0023D060 */
typedef struct Entry {
    void *x0;
    int x4;
    char pad8[0xC];
} Entry;

extern Entry D_0075A9D0[];

void *func_0023D040(int i)
{
    return D_0075A9D0[i].x0;
}
