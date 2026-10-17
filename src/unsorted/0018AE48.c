/* UNIT 0x0018AE48 0x0018AE70 */
/* gcc 2.95 vtable entry: this-adjust delta, then function */
struct VEntry {
    short delta;
    short pad;
    void (*fn)(void *self);
};

struct Obj {
    char pad0[0x4F8];
    struct VEntry *vtbl;
};

void func_0018AE48(struct Obj *obj)
{
    obj->vtbl[8].fn((char *)obj + obj->vtbl[8].delta);
}
