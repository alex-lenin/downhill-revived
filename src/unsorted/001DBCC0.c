/* UNIT 0x001DBCC0 0x001DBCD8 */
struct Obj {
    char pad0[0xC];
    int xC;
    int x10;
    int x14;
    char pad18[0x4];
    int x1C;
};

void func_001DBCC0(struct Obj *obj)
{
    obj->xC = 0;
    obj->x10 = 0;
    obj->x14 = 0;
    obj->x1C = 0;
}
