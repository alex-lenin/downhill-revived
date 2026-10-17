/* UNIT 0x0023CF78 0x0023CF98 */
typedef struct Entry {
    char *x0;
    int x4;
    char pad8[0xC];
} Entry;

extern Entry D_0075A9D0[];

char *func_0023CF78(int i)
{
    return D_0075A9D0[i].x0 + 0x10;
}
