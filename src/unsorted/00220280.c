/* UNIT 0x00220280 0x002202B8 */
typedef struct { char pad0[4]; unsigned char x4, x5, x6, x7; char pad8[0x18]; } Entry;
extern Entry D_00660B80[];
void func_00220280(int index, unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *d) {
    Entry *entry = &D_00660B80[index];
    *a = entry->x4;
    *b = entry->x5;
    *c = entry->x6;
    *d = entry->x7;
}
