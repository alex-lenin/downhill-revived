/* UNIT 0x00174270 0x001742C0 */
struct Entry { void *x0; char pad[0x12C]; };
extern struct Entry D_003AF940[];
extern char D_00285B58[];
void func_00174270(int enabled,int mode) {
    int count;
    struct Entry *entry;
    if (mode == 0xFFFF && enabled != 0) {
        entry = D_003AF940;
        count = 6;
        do {
            entry->x0 = D_00285B58;
            count--;
            entry++;
        } while (count != -1);
    }
}
