/* UNIT 0x001B03D8 0x001B03F8 */
struct Obj {
    char pad0[0x10];
    int x10;
};

extern void func_001B03F8(struct Obj *obj);

void func_001B03D8(struct Obj *obj, int value)
{
    obj->x10 = value;
    func_001B03F8(obj);
}
