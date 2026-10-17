/* UNIT 0x001941B8 0x00194200 */
void func_001941B8(void *obj, int bit) {
    long *words = (long *)((char *)obj + 0x12150);
    long mask = (long)1 << bit;
    int remaining = 4;
    do {
        long value = *words;
        --remaining;
        if ((value & mask)) {
            *words = value ^ mask;
            return;
        }
        words--;
    } while (remaining >= 0);
}
