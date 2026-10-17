/* UNIT 0x0020E7B0 0x0020E800 */
typedef struct { char pad[6]; unsigned short count; char pad8[0x18]; int *items[1]; } List;
extern List D_004DA280;
int *func_0020E7B0(int id) {
    int i;
    for (i = 0; i < D_004DA280.count; i++) {
        int *item = D_004DA280.items[i];
        if (*item == id) return item;
    }
    return 0;
}
