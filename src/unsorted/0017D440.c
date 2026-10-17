/* UNIT 0x0017D440 0x0017D4C0 */
struct List { int count; int x4; int entries[16]; };
int func_0017D440(struct List *list, int value) {
    int i;
    int count;
    if (value == 0) {
        list->x4++;
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        if (value == list->entries[i]) return 0;
    }
    count = list->count;
    if (count < 16) {
        list->entries[count] = value;
        list->count = count + 1;
        return 1;
    }
    return 0;
}
