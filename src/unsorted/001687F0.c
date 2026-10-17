/* UNIT 0x001687F0 0x00168800 */
struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
};

void func_001687F0(struct Obj *obj)
{
    obj->x500 = 0;
    obj->x504 = 0;
}
