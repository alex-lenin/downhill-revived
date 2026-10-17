/* UNIT 0x001960F0 0x00196138 */
extern void func_00196880(void *);
void func_001960F0(void *obj) {
    char *entry = (char *)obj + 0x18;
    int remaining = 0x95;
    do {
        --remaining;
        func_00196880(entry);
        entry += 0x14;
    } while (remaining >= 0);
}
