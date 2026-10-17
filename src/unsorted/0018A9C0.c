/* UNIT 0x0018A9C0 0x0018A9E0 */
struct Obj {
    int x0;
};

extern void func_00187728(struct Obj *obj);

void func_0018A9C0(struct Obj *obj)
{
    obj->x0 = 4;
    func_00187728(obj);
}
