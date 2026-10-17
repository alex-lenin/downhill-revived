/* UNIT 0x001E64C8 0x001E64E0 */
struct Inner {
    char pad0[0x4ECC];
    void *x4ECC;
    char pad4ED0[0x5FF0 - 0x4ED0];
    char x5FF0[1];
};

struct Obj {
    char pad0[0x40];
    struct Inner *x40;
};

int func_001E64C8(struct Obj *obj)
{
    return (void *)obj->x40->x5FF0 == obj->x40->x4ECC;
}
