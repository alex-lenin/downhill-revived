/* UNIT 0x00184AA0 0x00184AB0 */
struct Obj {
    char pad0[0x8];
    int x8;
    int xC;
    int x10;
};

void func_00184AA0(struct Obj *obj, int a, int b, int c)
{
    obj->x8 = a;
    obj->xC = b;
    obj->x10 = c;
}
