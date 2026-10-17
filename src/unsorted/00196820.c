/* UNIT 0x00196820 0x00196840 */
struct Inner {
    int x0;
    char x4;
};

struct Obj {
    char pad0[0x8];
    struct Inner *x8;
    int xC;
    int x10;
};

void func_00196820(struct Obj *obj)
{
    struct Inner *inner = obj->x8;

    if (inner->x0 != 0) {
        inner->x4 = 1;
    }
    obj->x10 = 0;
    obj->xC = 0;
}
