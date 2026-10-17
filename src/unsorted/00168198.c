/* UNIT 0x00168198 0x001681A8 */
struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
};

void func_00168198(struct Obj *obj)
{
    obj->x500 = 0;
    obj->x504 = 0;
}
