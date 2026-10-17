/* UNIT 0x001E7B78 0x001E7BA0 */
typedef struct {
    char pad0[0x3C];
    int x3C;
    int x40;
    char pad44[0x10];
} Entry;

extern Entry D_00272888[];

int func_001E7B78(int unused, int i)
{
    Entry *e = &D_00272888[i];

    if (e->x40 > -1) {
        return 1;
    }
    return 0;
}
