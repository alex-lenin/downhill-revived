/* UNIT 0x001BF758 0x001BF770 */
struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
};

void func_001BF758(struct Obj *obj, int a, int b)
{
    obj->x8 = a;
    obj->x4 = b;
    obj->xC = -1;
    obj->x10 = -1;
}
