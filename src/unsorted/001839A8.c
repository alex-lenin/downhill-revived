/* UNIT 0x001839A8 0x001839B8 */
struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
};

void func_001839A8(struct Obj *obj)
{
    obj->x500 = 0;
    obj->x504 = 0;
}
