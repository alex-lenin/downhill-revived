/* UNIT 0x00194248 0x00194290 */
void func_00194248(void *obj, int bit) {
    int *words = (int *)((char *)obj + 0x1217C);
    int mask = (int)1 << bit;
    int remaining = 4;
    do {
        int value = *words;
        --remaining;
        if ((value & mask)) {
            *words = value ^ mask;
            return;
        }
        words--;
    } while (remaining >= 0);
}
