/* UNIT 0x00208AE0 0x00208B40 */
void func_00208AE0(int *entries, int value) {
    int slot = 20;
    int absent = 1;
    int i = 0;
    int *p = entries;
    do {
        int current = *p;
        if (current == 0) {
            if (slot == 20) slot = i;
        }
        if (value == current) absent = 0;
        i++;
        p++;
    } while (i < 20);
    if (absent) {
        *(int *)((int)entries + (slot << 2)) = value;
        entries[20]++;
    }
}
