/* UNIT 0x0013D6F8 0x0013D750 */
struct Entry { int x0, x4, x8; };
extern struct Entry D_0026DEA0[];
int func_0013D6F8(void *unused, struct Entry *key) {
    struct Entry *entry = D_0026DEA0;
    int index = 0;
    do {
        if (entry->x0 == key->x0 && entry->x4 == key->x4 && entry->x8 == key->x8) return index;
        index++;
        entry++;
    } while (index < 41);
    return -1;
}
