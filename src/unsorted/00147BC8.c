/* UNIT 0x00147BC8 0x00147BD8 */
struct Obj {
    char pad0[0x20];
    int x20;
};

void func_00147BC8(struct Obj *obj)
{
    obj->x20 = 600;
}
