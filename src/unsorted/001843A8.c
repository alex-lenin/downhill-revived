/* UNIT 0x001843A8 0x001843B8 */
struct Obj {
    char pad0[0x8];
    int x8;
    int xC;
};

void func_001843A8(struct Obj *obj)
{
    obj->x8 = 0;
    obj->xC = 1;
}
