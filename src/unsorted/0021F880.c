/* UNIT 0x0021F880 0x0021F8E0 */
typedef struct { unsigned char x0, x1; char pad[30]; } Entry;
extern Entry D_00660B80[];
extern void func_00254204(void *, int, int);
void func_0021F880(int index) {
    Entry *entry = &D_00660B80[index];
    unsigned char a = entry->x0;
    unsigned char b = entry->x1;
    func_00254204(entry, 0, 32);
    entry->x0 = a;
    entry->x1 = b;
}
