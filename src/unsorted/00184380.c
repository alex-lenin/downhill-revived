/* UNIT 0x00184380 0x001843A8 */
struct Obj {
    char pad0[0x10];
    int x10;
};

extern void func_001843A8(struct Obj *obj);

void func_00184380(struct Obj *obj)
{
    func_001843A8(obj);
    obj->x10 = 0;
}
