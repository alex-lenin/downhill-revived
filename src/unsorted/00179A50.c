/* UNIT 0x00179A50 0x00179A60 */
struct Obj {
    char pad0[0x132C];
    int x132C;
    char x1330;
};

void func_00179A50(struct Obj *obj)
{
    obj->x1330 = 0;
    obj->x132C = 0x78;
}
