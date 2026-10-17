/* UNIT 0x00194008 0x00194050 */
int func_00194008(char *obj, int bit) {
    unsigned long mask = 1UL << bit;
    unsigned long *flags = (unsigned long *)(obj + 0x12130);
    int count = 0;
    int remaining = 4;
    do { count += (*flags & mask) != 0; --remaining; ++flags; } while (remaining >= 0);
    return count;
}
