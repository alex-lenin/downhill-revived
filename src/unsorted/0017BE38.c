/* UNIT 0x0017BE38 0x0017BE88 */
struct Global { char pad[0x288]; int count; };
struct Entry { char pad[0x130C]; int x130C; char pad1310[0x70]; };
extern struct Global D_004B25A0;
void func_0017BE38(void *obj) {
    int i = 0;
    struct Entry *entry;
    if (D_004B25A0.count > 0) {
        entry = (struct Entry *)((char *)obj + 0x150);
        do {
            entry->x130C = 1;
            i++;
            entry++;
        } while (i < D_004B25A0.count);
    }
}
