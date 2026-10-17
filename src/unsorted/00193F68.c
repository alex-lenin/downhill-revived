/* UNIT 0x00193F68 0x00193FC8 */
extern int D_004B2828[];
extern int func_00193F38(void *obj);

int func_00193F68(void *obj, int bit) {
    int result;
    if (D_004B2828[0] < 3) {
        int mask = (1 << bit) & func_00193F38(obj);
        result = mask != 0;
    } else {
        result = (0x020001c2 >> bit) & 1;
    }
    return result;
}
