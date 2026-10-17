/* UNIT 0x00202DB0 0x00202E00 */
typedef struct Inner {
    char pad0[0xC];
    int xC;
} Inner;

typedef struct Table {
    int x0;
    int *x4;
    char pad8[0x18];
    Inner *x20;
} Table;

extern int func_002046F8(void *arg0, int key);

int *func_00202DB0(void *arg0, Table **arg1)
{
    Table *table = *arg1;
    int index;

    if (table == 0) {
        return 0;
    }
    index = func_002046F8(arg0, table->x20->xC);
    if (index < 0) {
        return 0;
    }
    return &table->x4[index];
}
