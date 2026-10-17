/* UNIT 0x00194050 0x00194098 */
int func_00194050(void *obj, int bit) {
    int *words = (int *)((char *)obj + 0x12158);
    int mask = 1 << bit;
    int count = 0;
    int remaining = 4;
    do {
        count += (*words++ & mask) != 0;
    } while (--remaining >= 0);
    return count;
}
