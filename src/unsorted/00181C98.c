/* UNIT 0x00181C98 0x00181CA8 */
struct Obj {
    char pad0[0x8];
    int x8;
    int xC;
};

void func_00181C98(struct Obj *obj)
{
    obj->x8 = 0;
    obj->xC = 0;
}
