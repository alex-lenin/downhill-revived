/* UNIT 0x00170E70 0x00170E80 */
struct Inner {
    char pad0[0x4094];
    int x4094;
};

struct Obj {
    struct Inner *x0;
};

int func_00170E70(struct Obj *obj)
{
    return obj->x0->x4094 != 0;
}
