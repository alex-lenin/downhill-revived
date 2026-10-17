/* UNIT 0x00151EE0 0x00151F38 */
typedef struct Target {
    unsigned int lo : 7;
    unsigned int x0_7 : 11;
    unsigned int hi : 14;
} Target;

typedef struct Entry {
    int x0;
    int x4;
    Target *x8;
    char padC[0x24];
} Entry;

extern Entry D_00381DF0[100];

void func_00151EE0(void)
{
    int i;
    Entry *e;

    for (i = 0; i < 100; i++) {
        e = &D_00381DF0[i];
        if (e->x8 != 0) {
            e->x8->x0_7 = e->x4;
        }
    }
}
