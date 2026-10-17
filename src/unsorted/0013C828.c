/* UNIT 0x0013C828 0x0013C838 */
struct Obj {
    char pad0[0x46C];
    int x46C;
    char pad470[0x500 - 0x470];
    struct Obj *x500;
};

void func_0013C828(struct Obj *obj, struct Obj *other)
{
    obj->x500 = other;
    other->x46C = 0;
}
