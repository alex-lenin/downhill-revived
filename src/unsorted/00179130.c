/* UNIT 0x00179130 0x00179140 */
struct Obj {
    char pad0[0x12C4];
    int x12C4;
    int x12C8;
    int x12CC;
};

void func_00179130(struct Obj *obj, int a, int b)
{
    obj->x12CC = b;
    obj->x12C8 = a;
    obj->x12C4 = 0;
}
