/* UNIT 0x00194200 0x00194248 */
void func_00194200(void *obj, int bit) {
    int *words = (int *)((char *)obj + 0x12168);
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
