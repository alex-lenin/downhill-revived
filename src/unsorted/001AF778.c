/* UNIT 0x001AF778 0x001AF790 */
struct Obj {
    int x0;
    int x4;
    int x8;
};

void func_001AF778(struct Obj *obj, int value)
{
    if (value != 0) {
        obj->x8 = value;
    }
    obj->x0 = 0;
    obj->x4 = 0;
}
