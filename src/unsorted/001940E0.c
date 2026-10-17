/* UNIT 0x001940E0 0x00194128 */
void func_001940E0(void *obj, int bit) {
    long *words = (long *)((char *)obj + 0x12130);
    long mask = (long)1 << bit;
    int remaining = 0;
    do {
        long value = *words;
        ++remaining;
        if (!(value & mask)) {
            *words = value | mask;
            return;
        }
        words++;
    } while (remaining < 5);
}
