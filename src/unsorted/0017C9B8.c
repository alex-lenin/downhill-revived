/* UNIT 0x0017C9B8 0x0017C9F8 */
struct Entry {
    char pad0[0x2C];
};

struct Obj {
    struct Entry entries[32];
    int count;
};

extern void func_0017CB40(struct Entry *entry, void *target, int value, void *pos);

void func_0017C9B8(struct Obj *obj, void *target, int value, void *pos)
{
    func_0017CB40(&obj->entries[obj->count], target, value, pos);
    obj->count++;
}
