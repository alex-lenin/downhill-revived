/* UNIT 0x00194128 0x00194170 */
void func_00194128(void *obj, int bit) {
    int *words = (int *)((char *)obj + 0x12158);
    int mask = (int)1 << bit;
    int remaining = 0;
    do {
        int value = *words;
        ++remaining;
        if (!(value & mask)) {
            *words = value | mask;
            return;
        }
        words++;
    } while (remaining < 5);
}
