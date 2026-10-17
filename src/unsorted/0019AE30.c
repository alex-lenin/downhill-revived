/* UNIT 0x0019AE30 0x0019AEE0 */
typedef struct Obj {
    char pad0[0x50C];
    char rows[4][13];
    unsigned char counts[4];
} Obj;

extern char D_004225A0[];

void func_0019AE30(Obj *obj) {
    int i, j;
    char *table, *src;

    table = D_004225A0;
    for (i = 0; i < 4; i++) {
        src = (char *)(i * 12 + 0x121D7);
        src += (int)table;
        obj->counts[i] = 0;
        for (j = 0; j < 12; j++) {
            obj->rows[i][j] = *src;
            if (*src++ != 0x28) obj->counts[i]++;
        }
        obj->rows[i][12] = 0x28;
    }
}
