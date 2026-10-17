/* UNIT 0x00218A30 0x00218A70 */
typedef struct Entry { int x0; unsigned int flags; int x8, xC; } Entry;
extern Entry D_00621310[];
void func_00218A30(void) {
    int index;
    Entry *entry = D_00621310;
    for (index = 0; index < 1000; ++index) {
        entry->flags &= 0x7FFFFFFF;
        ++entry;
    }
}
