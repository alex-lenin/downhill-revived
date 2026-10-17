/* UNIT 0x00227310 0x00227320 */
struct Obj {
    char pad0[0x27C0];
    int flags;
};

void func_00227310(struct Obj *obj)
{
    obj->flags |= 0x800;
}
