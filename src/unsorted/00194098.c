/* UNIT 0x00194098 0x001940E0 */
int func_00194098(void *obj, int bit) {
    int *words = (int *)((char *)obj + 0x1216C);
    int mask = 1 << bit;
    int count = 0;
    int remaining = 4;
    do {
        count += (*words++ & mask) != 0;
    } while (--remaining >= 0);
    return count;
}
