/* UNIT 0x0023DF20 0x0023DF40 */
typedef struct Entry {
    void *x0;
    int x4;
    char pad8[0xC];
} Entry;

extern Entry D_0075A9D0[];

int func_0023DF20(int i)
{
    return D_0075A9D0[i].x4;
}
