/* UNIT 0x001ABB70 0x001ABBA0 */
typedef struct Entry {
    char pad0[0x30];
} Entry;

typedef struct Obj {
    Entry entries[1];
    char pad30[0x16CC - 0x30];
    int count;
} Obj;

Entry *func_001ABB70(Obj *obj, int index)
{
    if (index < 0 || index >= obj->count) {
        index = 0;
    }
    return &obj->entries[index];
}
