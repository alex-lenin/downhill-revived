/* UNIT 0x002202D0 0x002202F8 */
struct Entry { char pad0[4]; unsigned char x4, x5, x6, x7; char pad8[24]; };
extern struct Entry D_00660B80[];
void func_002202D0(int index, int x4, int x5, int x6, int x7) {
    struct Entry *entry = (struct Entry *)(index * 32 + (char *)D_00660B80);
    entry->x4 = x4;
    entry->x5 = x5;
    entry->x6 = x6;
    entry->x7 = x7;
}
