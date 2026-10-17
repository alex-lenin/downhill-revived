/* UNIT 0x00194D28 0x00194D58 */
typedef struct Entry {
    char x0[0x50];
} Entry;

extern int D_004B26AC[];
extern void func_00195368(Entry *entry, short *a, short *b, int *c, int *d);

void func_00194D28(Entry *entries, short *a, short *b, int *c, int *d)
{
    func_00195368(&entries[D_004B26AC[0]], a, b, c, d);
}
