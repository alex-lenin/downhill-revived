/* UNIT 0x001703A0 0x001703B0 */
struct Obj {
    char pad0[0xC];
    int xC;
    char pad10[0x50 - 0x10];
    int x50;
};

void func_001703A0(struct Obj *obj)
{
    obj->xC = 0;
    obj->x50 = 0;
}
