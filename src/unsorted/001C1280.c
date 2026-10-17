/* UNIT 0x001C1280 0x001C1298 */
struct Obj {
    int x0;
    int x4;
    int x8;
};

void func_001C1280(struct Obj *obj)
{
    obj->x8 = -1;
    obj->x4 = 14;
    obj->x0 = -1;
}
