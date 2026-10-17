/* UNIT 0x00194770 0x001947B0 */
int func_00194770(int rows, int index) {
    int count = 0;
    int end;
    int v;
    do {
        end = rows + 0x7668;
    } while (0);
    do {
        v = index * 0x50;
        v = v + rows;
        if (*(signed char *)(v + 0x33)) count++;
        rows += 0x694;
    } while (rows < end);
    return count;
}
