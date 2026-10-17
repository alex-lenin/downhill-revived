/* UNIT 0x001B3050 0x001B3068 */
struct Entry {
    char pad0[0x50];
};

struct Obj {
    char pad0[0x120];
    struct Entry entries[1];
};

struct Entry *func_001B3050(struct Obj *obj, int index)
{
    return &obj->entries[index];
}
