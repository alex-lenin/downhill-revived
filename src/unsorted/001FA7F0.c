/* UNIT 0x001FA7F0 0x001FA858 */
struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
};

extern void func_001FAAA0(struct Obj *);

void func_001FA7F0(struct Obj *obj)
{
    obj->x30 = 11;
    obj->x34 = 12;
    obj->x38 = 2;
    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    obj->x0 = 0;
    obj->x10 = 0;
    obj->x14 = 0;
    obj->x18 = 0;
    obj->x1C = 0;
    obj->x20 = 0;
    obj->x24 = 0;
    obj->x28 = 0;
    obj->x2C = 0;
    func_001FAAA0(obj);
}
