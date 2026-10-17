/* UNIT 0x001AC2E8 0x001AC300 */
struct Inner {
    char pad0[0x8];
    char x8;
};

struct Obj {
    char pad0[0x8];
    struct Inner *x8;
};

void func_001AC2E8(struct Obj *obj, int value)
{
    if (obj->x8 != 0) {
        obj->x8->x8 = value;
    }
}
