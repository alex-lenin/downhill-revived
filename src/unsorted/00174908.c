/* UNIT 0x00174908 0x00174920 */
struct Obj {
    char pad0[0xC];
    int xC;
    char pad10[0x10];
    char x20;
    char x21;
};

void func_00174908(struct Obj *obj)
{
    obj->x21 = 1;
    obj->x20 = 0;
    obj->xC = 0;
}
