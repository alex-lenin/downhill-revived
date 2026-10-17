/* UNIT 0x00194170 0x001941B8 */
void func_00194170(void *obj, int bit) {
    int *words = (int *)((char *)obj + 0x1216C);
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
