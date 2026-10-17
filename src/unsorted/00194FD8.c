/* UNIT 0x00194FD8 0x00195000 */
void func_00194FD8(char *base, int index, int value) {
    int *entry;
    index *= 4;
    base += 0x3A4;
    entry = (int *)(base + index);
    if (*entry < value) {
        *entry = value;
    }
}
