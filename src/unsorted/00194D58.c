/* UNIT 0x00194D58 0x00194D88 */
typedef struct Entry {
    char x0[0x50];
} Entry;

extern int D_004B26AC[];
extern void func_00195390(Entry *entry, unsigned char *a, unsigned char *b,
                          unsigned char *c, unsigned char *d);

void func_00194D58(Entry *entries, unsigned char *a, unsigned char *b,
                   unsigned char *c, unsigned char *d)
{
    func_00195390(&entries[D_004B26AC[0]], a, b, c, d);
}
