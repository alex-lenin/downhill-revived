/* UNIT 0x00179A38 0x00179A50 */
struct Obj {
    char pad0[0x132C];
    int x132C;
    char x1330;
};

void func_00179A38(struct Obj *obj)
{
    obj->x132C = 120;
    obj->x1330 = 1;
}
